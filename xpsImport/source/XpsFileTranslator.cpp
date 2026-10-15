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
#ifdef _DEBUG
			if (xpsMesh.vertices.size() == 0 || xpsMesh.faces.size() == 0)
			{
				MGlobal::displayWarning("Ghost mesh encountered!");
				continue;
			}
#else
			if (xpsMesh.vertices.size() == 0 || xpsMesh.faces.size() == 0)
				continue;
#endif

			MString meshName(xpsMesh.name.c_str());
			MString vertCountStr(std::to_string(xpsMesh.vertices.size()).c_str());
			MString faceCountStr(std::to_string(xpsMesh.faces.size()).c_str());

			MGlobal::displayInfo("Creating mesh " + meshName);
			MGlobal::displayInfo("Vertex count = " + vertCountStr);
			MGlobal::displayInfo("Face count = " + faceCountStr);

			MPointArray points = {};
			MVectorArray normals = {};
			MIntArray vertexIds = {};
			
			size_t currentVertId = 0;

			for (auto& vertex : xpsMesh.vertices)
			{
				points.append(vertex.coordDelta.x, vertex.coordDelta.y, vertex.coordDelta.z);
				
				MVector n(vertex.normal.x, vertex.normal.y, vertex.normal.z);
				normals.append(n);
				vertexIds.append(currentVertId);
				currentVertId++;
			}

			MFnMesh meshFn = {};

			MIntArray polygonCounts;

			MIntArray polygonConnects;

			for (auto& index : xpsMesh.faces)
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

			MObject meshObj = meshFn.create(points.length(), polygonCounts.length(), points, polygonCounts, flipedPolygons, MObject::kNullObj, &createStatus);
			
			if (!createStatus)
			{
				MGlobal::displayError("Failed to create mesh " + meshName + " EC = " + createStatus.errorString());
				continue;
			}

			createStatus = meshFn.setVertexNormals(normals, vertexIds, MSpace::kObject);

			if (!createStatus)
			{
				MGlobal::displayError("Failed to apply normals to " + meshName + " EC = " + createStatus.errorString());
				continue;
			}

			MGlobal::executeCommand(MString("sets -e -forceElement initialShadingGroup ") + MFnDagNode(meshObj).fullPathName());
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
}