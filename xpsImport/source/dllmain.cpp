#include <XpsImport/XpsFileTranslator.h>

#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, "Nova Europa Software", "1.0", "Any");
	
	status = plugin.registerFileTranslator("XPS", "", XpsImport::XpsFileTranslator::creator, "", "", false, MFnPlugin::kDefaultDataLocation);
	
	if (!status)
	{
		status.perror("registerFileTranslator");
		return status;
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj);
	status = plugin.deregisterFileTranslator("XPS");
	if (!status) {
		status.perror("deregisterFileTranslator");
		return status;
	}

	return status;
}