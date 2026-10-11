#include <XpsImport/XpsFileTranslator.h>

namespace XpsImport
{
	void* XpsFileTranslator::creator()
	{
		return new XpsFileTranslator();
	}

	MStatus XpsFileTranslator::reader(const MFileObject& file, const MString& options, FileAccessMode mode)
	{
		MString path = file.fullName();

		std::ifstream inFile(path.asChar(), std::ios::binary);

		if (!inFile.is_open())
		{
			MGlobal::displayError("Cannot open file.");
			return MS::kFailure;
		}

		std::optional<XpsHeader> header = XpsBinUtils::findHeader(inFile);

		if (!header.has_value())
		{
			MGlobal::displayError("Cannot read header! This may indicate corrupted or incompatible file.");
			return MS::kFailure;
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
}