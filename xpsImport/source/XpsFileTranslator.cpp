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

		MIntArray polygonCounts;

		MIntArray polygonConnects;

		for (auto& index : mesh.faces)
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

			polygonCounts.append(3);

			polygonConnects.append(index.x);
			polygonConnects.append(index.y);
			polygonConnects.append(index.z);
		}

#ifdef _DEBUG
		if (polygonCounts.length() * 3 != polygonConnects.length())
			MGlobal::displayError("polygonCounts / polygonConnects mismatch detected!");
#endif

		MIntArray flipedPolygons = flipFaces(polygonCounts, polygonConnects);

		MStatus createStatus;

		MObject meshObj = meshFn.create(mvd.positionArray.length(), polygonCounts.length(), mvd.positionArray, polygonCounts, flipedPolygons, MObject::kNullObj, &createStatus);

		if (!createStatus)
		{
			MGlobal::displayError("Failed to create mesh " + meshName + " EC = " + createStatus.errorString());
			return createStatus;
		}

		createStatus = meshFn.setVertexNormals(mvd.normalArray, mvd.vertexIdArray, MSpace::kObject);

		if (!createStatus)
		{
			MGlobal::displayError("Failed to apply normals to " + meshName + " EC = " + createStatus.errorString());
			createStatus;
		}

		MGlobal::executeCommand(MString("sets -e -forceElement initialShadingGroup ") + MFnDagNode(meshObj).fullPathName());
	
		return createStatus;
	}

	MeshVertexData XpsFileTranslator::parseVertices(const std::vector<XpsVertex>& verts)
	{
		MeshVertexData result = {};

		size_t currentVertId = 0;

		for (auto& vertex : verts)
		{
			result.positionArray.append(vertex.coordDelta.x, vertex.coordDelta.y, vertex.coordDelta.z);

			MVector n(vertex.normal.x, vertex.normal.y, vertex.normal.z);
			result.normalArray.append(n);
			result.vertexIdArray.append(currentVertId);
			currentVertId++;
		}

		return result;
	}
}