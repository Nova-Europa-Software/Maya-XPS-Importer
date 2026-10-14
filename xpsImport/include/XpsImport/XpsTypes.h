#pragma once
#include "XpsCore.h"

namespace XpsImport
{
	/**
	 * @brief Struct containing two vectors as floats
	*/
	struct XpsVector2
	{
		union
		{
			float x = 0.0f;
			float r;
		};

		union
		{
			float y = 0;
			float g;
		};
	};

	/**
	 * @brief Struct containing three vectors as floats
	*/
	struct XpsVector3
	{
		union
		{
			float x = 0.0f;
			float r;
		};

		union
		{
			float y = 0;
			float g;
		};

		union
		{
			float z = 0;
			float b;
		};
	};

	/**
	 * @brief Struct containing three vectors as unsigned integers (uint32_t)
	*/
	struct XpsIuVector3
	{
		union
		{
			uint32_t x = 0;
			uint32_t r;
		};

		union
		{
			uint32_t y = 0;
			uint32_t g;
		};

		union
		{
			uint32_t z = 0;
			uint32_t b;
		};
	};

	/**
	 * @brief Struct containing four vectors as floats
	*/
	struct XpsVector4
	{
		union
		{
			float x = 0.0f;
			float r;
		};

		union
		{
			float y = 0;
			float g;
		};

		union
		{
			float z = 0;
			float b;
		};

		union
		{
			float w = 0;
			float a;
		};
	};

	/**
	 * @brief Struct containing four vectors as unsigned integers (uint32_t)
	*/
	struct XpsIuVector4
	{
		union
		{
			uint32_t x = 0;
			uint32_t r;
		};

		union
		{
			uint32_t y = 0;
			uint32_t g;
		};

		union
		{
			uint32_t z = 0;
			uint32_t b;
		};

		union
		{
			uint32_t w = 0;
			uint32_t a;
		};
	};

	/**
	 * @brief Struct containing four vectors as signed chars (int8_t)
	*/
	struct XpsCVector4
	{
		union
		{
			int8_t x = 0;
			int8_t r;
		};

		union
		{
			int8_t y = 0;
			int8_t g;
		};

		union
		{
			int8_t z = 0;
			int8_t b;
		};

		union
		{
			int8_t w = 0;
			int8_t a;
		};
	};

	/**
	 * @brief Struct containing all relevant bone data
	*/
	struct XpsBone
	{
		/** Bone ID */
		uint32_t id = 0; 
		/** Bone name */
		std::string name = std::string(); 
		/** Bone coordinates (X,Y,Z) */
		XpsVector3 co = {}; 
		/** ID of parent bone */
		uint32_t parentId = 0; 
	};

	/**
	 * @brief Struct containing all bone transformations
	*/
	struct XpsBonePose
	{
		/** Name of related bone */
		std::string boneName = std::string(); 
		/** Translation in X,Y,Z axes */
		XpsVector3 coordDelta = {}; 
		/** Rotation in X,Y,Z axes */
		XpsVector3 rotDelta = {}; 
		/** Scale in X,Y,Z axes */
		XpsVector3 scale = {}; 
	};

	/**
	 * @brief Struct containing all header data
	*/
	struct XpsHeader
	{
		/** XPS magic number */
		uint32_t magicNumber = XPS_MAGIC_NUMBER; 
		/** XPS file version major */
		uint16_t versionMajor = XPS_VERSION_MAJOR; 
		/** XPS file version minor */
		uint16_t versionMinor = XPS_VERSION_MINOR; 
		/** XPS file string */
		std::string xnaAral = XPS_ARAL; 
		/** Length of settings in bytes */
		uint32_t settingsLen = XPS_STRLEN; 
		/** Device name that the file was created on */
		std::string machine = std::string(); 
		/** Name of the file creator */
		std::string user = std::string(); 
		/** String containing settings */
		std::string settings = std::string(); 
		/** String containing pose data */
		std::string pose = std::string(); 
		/** String containing all related files */
		std::string files = std::string(); 
	};

	/**
	 * @brief Struct containing all model flags
	*/
	struct XpsFlags
	{
		/** Determines if back face culling should be enabled */
		bool backFaceCulling = false; 
		/** Determines if face culling should be forced onto a model */
		bool alwaysForceCulling = false; 
		/** Determines if model should cast shadows */
		bool modelCastShadows = true; 
		/** Determines if tangent is in red channel */
		int tangentSpaceRed = 0; 
		/** Determines if tangent is in green channel */
		int tangentSpaceGreen = 1; 
		/** Determines if tangent is in blue channel */
		int tangentSpaceBlue = 0; 
		/** Determines model glossines */
		int gloss = 10; 
		/** Determines if bone have direction data */
		bool hasBoneDirections = false; 
	};

	/**
	 * @brief Struct containing all relevant texture data
	*/
	struct XpsTexture
	{
		/** ID of the texture */
		uint32_t id = 0; 
		/** Path to a related image file */
		std::string file = std::string(); 
		/** ID of related UV layer */
		uint32_t uvLayer = 0; 
	};

	/**
	 * @brief Struct containing vertex weight data
	*/
	struct XpsBoneWeight
	{
		/** ID of related bone */
		uint16_t id = 0; 
		/** Bone weight */
		float weight = 0.0f; 
	};

	/**
	 * @brief Struct containing all relevant vertex data
	*/
	struct XpsVertex
	{
		/** ID of the vertex */
		uint32_t id = {}; 
		/** Vertex position */
		XpsVector3 coordDelta = {}; 
		/** Vertex normal */
		XpsVector3 normal = {}; 
		/** Texture coordinates / UV data */
		std::vector<XpsVector2> uv = {}; 
		/** Vertex color */
		XpsCVector4 color = {}; 
		/** Bone data */
		std::vector<XpsBoneWeight> boneWeights = {}; 
		/** Unused value */
		bool merged = false; 
	};

	/**
	 * @brief Struct containing all relevant mesh data
	*/
	struct XpsMesh
	{
		/** Mesh name */
		std::string name = std::string(); 
		/** Mesh texture data */
		std::vector<XpsTexture> textures = std::vector<XpsTexture>(); 
		/** Mesh vertex data */
		std::vector<XpsVertex> vertices = std::vector<XpsVertex>(); 
		/** Mesh index data */
		std::vector<XpsIuVector3> faces = std::vector<XpsIuVector3>(); 
		/** Number of UV layers used by the mesh */
		size_t uvCount = 0; 
	};
}