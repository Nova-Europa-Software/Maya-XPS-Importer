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

	static std::unordered_map<int, std::string> flagList = {
		{0, BACK_FACE_CULLING},
		{1, ALWAYS_FORCE_CULLING},
		{2, MODEL_CAST_SHADOWS},
		{3, TANGENT_SPACE_RED},
		{4, TANGENT_SPACE_GREEN},
		{5, TANGENT_SPACE_BLUE},
		{6, GLOSS},
		{7, HAS_BONE_DIRECTIONS}
	};

	class XpsBinUtils
	{
	public:
		std::vector<XpsBone> readBones();
		static std::optional<XpsHeader> findHeader(std::ifstream& fileStream);
		static std::optional<std::string> readString(std::ifstream& fileStream);

	private:
		static XpsHeader readHeader(std::ifstream& fileStream);
		static bool hasTangentVersion(const uint16_t& verMaj, const uint16_t& verMin, bool hasHeader = true);
		static std::unordered_map<std::string, XpsBonePose> readDefaultPose(std::ifstream& fileStream, uint32_t poseLenUnround, uint32_t poseBones);
		static uint32_t roundToMultiple(uint32_t numToRound, uint32_t multiple);
		static XpsFlags readFlags(std::ifstream& fileStream, uint32_t optCount);
		static int flagValue(uint32_t flag, uint32_t value);
		static std::unordered_map<std::string, XpsBonePose> poseData(std::string poseString);
		static std::vector<std::string> splitStringByChar(const std::string& s, char c);
		static std::vector<char> fillArray(const std::vector<char>& array, uint32_t minLength, char value);
	};

	
}