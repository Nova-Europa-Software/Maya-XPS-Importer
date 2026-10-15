#pragma once
#include "XpsCore.h"
#include "XpsBinUtils.h"

namespace XpsImport
{
	class XpsFileTranslator : public MPxFileTranslator
	{
	public:
		XpsFileTranslator() = default;
		virtual ~XpsFileTranslator() = default;

		static void* creator();

		inline bool haveReadMethod() const override { return true; }
		inline bool haveWriteMethod() const override { return false; }

		inline MString defaultExtension() const override { return "xps"; }
		MStatus reader(const MFileObject& file, const MString& options, FileAccessMode mode) override;
		MPxFileTranslator::MFileKind identifyFile(const MFileObject& file, const char* buffer, short size);
	
	private:
		MIntArray flipFaces(const MIntArray& polygonCounts, const MIntArray& polygonConnects);
	};
}