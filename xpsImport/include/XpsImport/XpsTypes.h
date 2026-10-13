#pragma once
#include "XpsCore.h"

namespace XpsImport
{
	/**
	 * @brief Struct containing two vectors as floats
	*/
	struct XpsVector2
	{
		float x = 0;
		float y = 0;
	};

	/**
	 * @brief Struct containing three vectors as floats
	*/
	struct XpsVector3
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};

	/**
	 * @brief Struct containing three vectors as unsigned integers (uint32_t)
	*/
	struct XpsIuVector3
	{
		uint32_t x;
		uint32_t y;
		uint32_t z;
	};

	/**
	 * @brief Struct containing four vectors as floats
	*/
	struct XpsVector4
	{
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 0;
	};

	/**
	 * @brief Struct containing four vectors as unsigned integers (uint32_t)
	*/
	struct XpsIuVector4
	{
		uint32_t x;
		uint32_t y;
		uint32_t z;
		uint32_t w;
	};

	/**
	 * @brief Struct containing four vectors as signed chars (int8_t)
	*/
	struct XpsCVector4
	{
		int8_t x;
		int8_t y;
		int8_t z;
		int8_t w;
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

	struct XpsBoneWeight
	{
		uint16_t id = 0;
		float weight = 0.0f;
	};

	struct XpsVertex
	{
		uint32_t id = {};
		XpsVector3 coordDelta = {};
		XpsVector3 normal = {};
		std::vector<XpsVector2> uv = {};
		XpsCVector4 color = {};
		std::vector<XpsBoneWeight> boneWeights = {};
		bool merged = false;
	};

	struct XpsMesh
	{
		std::string name = std::string();
		std::vector<XpsTexture> textures = std::vector<XpsTexture>();
		std::vector<XpsVertex> vertices = std::vector<XpsVertex>();
		std::vector<XpsIuVector3> faces = std::vector<XpsIuVector3>();
		size_t uvCount = 0;
	};
}