#include <XpsImport/XpsFileTranslator.h>

namespace XpsImport
{
	void* XpsFileTranslator::creator()
	{
		return new XpsFileTranslator();
	}

	MStatus XpsFileTranslator::reader(const MFileObject& file, const MString& options, FileAccessMode mode)
	{
		MString path = file.resolvedFullName();

		std::ifstream inFile(path.asChar(), std::ios::binary);

		if (!inFile.is_open())
		{
			MGlobal::displayError("Cannot open file.");
			return MS::kFailure;
		}

		MGlobal::displayInfo("File " + path + " opened!");

		std::optional<XpsHeader> header = XpsBinUtils::findHeader(inFile);

		if (!header.has_value())
		{
			MGlobal::displayError("Cannot read header! This may indicate corrupted or incompatible file.");
			return MS::kFailure;
		}

		std::vector<XpsBone> bones = XpsBinUtils::readBones(inFile, header.value());
		std::vector<XpsMesh> meshes = XpsBinUtils::readMeshes(inFile, header, !bones.empty());

		for (auto& xpsMesh : meshes)
		{
			createMesh(xpsMesh);
		}

		MGlobal::displayInfo("Import successful!");
		return MS::kSuccess;
	}

	MPxFileTranslator::MFileKind XpsFileTranslator::identifyFile(const MFileObject& file, const char* buffer, short size)
	{
		MString name = file.name();

		std::filesystem::path p(name.asChar());

		std::string ext = p.extension().string();

		if (strcmp(ext.c_str(), ".xps") == 0)
			return kIsMyFileType;
		
		return kNotMyFileType;
	}

	MIntArray XpsFileTranslator::flipFaces(const MIntArray& polygonCounts, const MIntArray& polygonConnects)
	{
		MIntArray out;
		uint32_t pos = 0;
		for (size_t i = 0; i < polygonCounts.length(); i++)
		{
			int c = polygonCounts[i];
			for (int j = c - 1; j >= 0; j--)
			{
				out.append(polygonConnects[pos + j]);
			}

			pos += c;
		}

		return out;
	}

	MStatus XpsFileTranslator::createMesh(const XpsMesh& mesh)
	{
#ifdef _DEBUG
		if (mesh.vertices.size() == 0 || mesh.faces.size() == 0)
		{
			MGlobal::displayWarning("Ghost mesh encountered!");
			return MStatus::kFailure;
		}
#else
		if (mesh.vertices.size() == 0 || mesh.faces.size() == 0)
			return MStatus::kFailure;
#endif

		MString meshName(mesh.name.c_str());
		MString vertCountStr(std::to_string(mesh.vertices.size()).c_str());
		MString faceCountStr(std::to_string(mesh.faces.size()).c_str());

		MGlobal::displayInfo("Creating mesh " + meshName);
		MGlobal::displayInfo("Vertex count = " + vertCountStr);
		MGlobal::displayInfo("Face count = " + faceCountStr);

		MeshVertexData mvd = parseVertices(mesh.vertices);

		MFnMesh meshFn = {};

		MeshIndexData mid = parseIndices(mesh.faces);

		MIntArray flipedPolygons = flipFaces(mid.polygonCounts, mid.polygonConnects);
		MIntArray flipedUvs = flipFaces(mid.uvFaceCounts, mid.uvFaceConnects);

		MStatus createStatus;

		MObject meshObj = meshFn.create(mvd.positionArray.length(), mid.polygonCounts.length(), mvd.positionArray, mid.polygonCounts, flipedPolygons, MObject::kNullObj, &createStatus);

		if (!createStatus)
		{
			MGlobal::displayError("Failed to create mesh " + meshName + " EC = " + createStatus.errorString());
			return createStatus;
		}

		createStatus = meshFn.setVertexNormals(mvd.normalArray, mvd.vertexIdArray, MSpace::kObject);

		if (!createStatus)
		{
			MGlobal::displayError("Failed to apply normals to " + meshName + " EC = " + createStatus.errorString());
			return createStatus;
		}

		for (auto& uvSet : mvd.uvDataVec)
		{
			if(uvSet.setName != "map1")
				createStatus = meshFn.createUVSet(uvSet.setName);

			if (!createStatus)
			{
				MGlobal::displayError("Failed to create UV set " + uvSet.setName + " for mesh " + meshName + " EC = " + createStatus.errorString());
				return createStatus;
			}

			createStatus = meshFn.setUVs(uvSet.uArray, uvSet.vArray, &uvSet.setName);
			if (!createStatus)
			{
				MGlobal::displayError("Failed to append UVs to " + uvSet.setName + " for mesh " + meshName + " EC = " + createStatus.errorString());
				return createStatus;
			}

			meshFn.assignUVs(mid.uvFaceCounts, flipedUvs, &uvSet.setName);
		}

		return createStatus;
	}

	MeshVertexData XpsFileTranslator::parseVertices(const std::vector<XpsVertex>& verts)
	{
		MeshVertexData result = {};

		size_t currentVertId = 0;

		result.uvDataVec.resize(verts[0].uv.size());

		for (size_t i = 0; i < result.uvDataVec.size(); i++)
		{
			std::string rawSetName = "map" + std::to_string(i+1);

			MString setName = MString(rawSetName.c_str());

			result.uvDataVec[i].setName = setName;
		}

		for (auto& vertex : verts)
		{
			result.positionArray.append(vertex.coordDelta.x, vertex.coordDelta.y, vertex.coordDelta.z);

			MVector n(vertex.normal.x, vertex.normal.y, vertex.normal.z);
			result.normalArray.append(n);

			size_t currentUvSet = 0;
			for (auto& uv : vertex.uv)
			{
				result.uvDataVec[currentUvSet].uArray.append(uv.x);
				result.uvDataVec[currentUvSet].vArray.append(1-uv.y);
				currentUvSet++;
			}

			result.vertexIdArray.append(currentVertId);
			currentVertId++;
		}

		return result;
	}

	MeshIndexData XpsFileTranslator::parseIndices(const std::vector<XpsIuVector3>& inds)
	{
		MeshIndexData result = {};

		for (auto& index : inds)
		{
#ifdef _DEBUG
			if (index.x == 0 && index.y == 0 && index.z == 0)
			{
				MGlobal::displayWarning("Ghost face encountered!");
				continue;
			}
#else
			if (index.x == 0 && index.y == 0 && index.z == 0)
				continue;
#endif

			result.polygonCounts.append(3);

			result.polygonConnects.append(index.x);
			result.polygonConnects.append(index.y);
			result.polygonConnects.append(index.z);
		}

#ifdef _DEBUG
		if (result.polygonCounts.length() * 3 != result.polygonConnects.length())
			MGlobal::displayError("polygonCounts / polygonConnects mismatch detected!");
#endif

		int faceIndex = 0;

		for (size_t i = 0; i < result.polygonCounts.length(); i++)
		{
			result.uvFaceCounts.append(result.polygonCounts[i]);

			for (size_t j = 0; j < result.polygonCounts[i]; j++)
			{
				result.uvFaceConnects.append(result.polygonConnects[faceIndex]);
				faceIndex++;
			}
		}

		return result;
	}
}