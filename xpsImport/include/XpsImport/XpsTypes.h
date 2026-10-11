#pragma once
#include "XpsCore.h"

namespace XpsImport
{
	struct XpsVector3
	{
		float x;
		float y;
		float z;
	};

	struct XpsBone
	{
		uint32_t id = 0;
		std::string name = std::string();
		uint32_t co = 0;
		uint32_t parentId = 0;
	};

	struct XpsBonePose
	{
		std::string boneName = std::string();
		XpsVector3 coordDelta = {};
		XpsVector3 rotDelta = {};
		XpsVector3 scale = {};
	};

	struct XpsHeader
	{
		uint32_t magicNumber = XPS_MAGIC_NUMBER;
		uint16_t versionMajor = XPS_VERSION_MAJOR;
		uint16_t versionMinor = XPS_VERSION_MINOR;
		std::string xnaAral = XPS_ARAL;
		uint32_t settingsLen = XPS_STRLEN;
		std::string machine = std::string();
		std::string user = std::string();
		std::string settings = std::string();
		std::string pose = std::string();
		std::string files = std::string();
	};

	struct XpsFlags
	{
		bool backFaceCulling = false;
		bool alwaysForceCulling = false;
		bool modelCastShadows = true;
		int tangentSpaceRed = 0;
		int tangentSpaceGreen = 1;
		int tangentSpaceBlue = 0;
		int gloss = 10;
		bool hasBoneDirections = false;
	};
}