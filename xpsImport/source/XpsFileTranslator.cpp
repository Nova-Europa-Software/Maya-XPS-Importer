#include <XpsImport/XpsFileTranslator.h>

namespace XpsImport
{
	void* XpsFileTranslator::creator()
	{
		return new XpsFileTranslator();
	}

	MStatus XpsFileTranslator::reader(const MFileObject& file, const MString& options, FileAccessMode mode)
	{
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