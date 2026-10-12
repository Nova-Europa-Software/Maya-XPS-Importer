#pragma once
#include "XpsCore.h"

namespace XpsImport
{
	struct XpsVector2
	{
		float x = 0;
		float y = 0;
	};

	struct XpsVector3
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};

	struct XpsVector4
	{
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 0;
	};

	struct XpsBone
	{
		uint32_t id = 0;
		std::string name = std::string();
		XpsVector3 co = {};
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

	struct XpsTexture
	{
		uint32_t id = 0;
		std::string file = std::string();
		uint32_t uvLayer = 0;
	};

	struct XpsVertex
	{
		uint32_t id = {};
		XpsVector3 coordDelta = {};
		XpsVector3 normal = {};
		XpsVector2 uv = {};
		XpsVector3 color = {};
		XpsVector4 boneWeights = {};
		bool merged = false;
	};

	struct XpsMesh
	{
		std::string name = std::string();
		std::vector<XpsTexture> textures = std::vector<XpsTexture>();
		std::vector<XpsVertex> vertices = std::vector<XpsVertex>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();
		size_t uvCount = 0;
	};
}