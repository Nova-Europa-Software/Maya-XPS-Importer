#pragma once
#include "XpsCore.h"
#include "XpsTypes.h"

namespace XpsImport
{
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
		static std::vector<XpsBone> readBones(std::ifstream& fileStream, XpsHeader& header);
		static std::optional<XpsHeader> findHeader(std::ifstream& fileStream);
		static std::optional<std::string> readString(std::ifstream& fileStream);
		static std::vector<XpsMesh> readMeshes(std::ifstream& fileStream, std::optional<XpsHeader> headerOpt, bool hasBones);

	private:
		static XpsHeader readHeader(std::ifstream& fileStream);
		static bool hasTangentVersion(const uint16_t& verMaj, const uint16_t& verMin, bool hasHeader = true);
		static std::unordered_map<std::string, XpsBonePose> readDefaultPose(std::ifstream& fileStream, uint32_t poseLenUnround, uint32_t poseBones);
		static uint32_t roundUpToMultiple(uint32_t numToRound, uint32_t multiple);
		static XpsFlags readFlags(std::ifstream& fileStream, uint32_t optCount);
		static int flagValue(uint32_t flag, uint32_t value);
		static std::unordered_map<std::string, XpsBonePose> poseData(std::string poseString);
		static std::vector<std::string> splitStringByChar(const std::string& s, char c);
		static std::vector<char> fillArray(const std::vector<char>& array, uint32_t minLength, char value);
		static bool hasVariableWeights(uint32_t verMajor, uint32_t verMinor, bool hasHeader);
	};

	
}