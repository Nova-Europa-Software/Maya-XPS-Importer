#include <XpsImport/XpsBinUtils.h>

namespace XpsImport
{
	std::optional<XpsHeader> XpsBinUtils::findHeader(std::ifstream& fileStream)
	{
		XpsHeader result = {};

		uint32_t magic_number = 0;
		fileStream.read((char*)&magic_number, sizeof(uint32_t));
		fileStream.seekg(0);

		if (magic_number == XPS_MAGIC_NUMBER)
		{
			MGlobal::displayInfo("XPS header found!");
			result = readHeader(fileStream);
			return result;
		}
		else
		{
			MString recievedStr(std::to_string(magic_number).c_str());
			MString expectedStr(std::to_string(XPS_MAGIC_NUMBER).c_str());

			MGlobal::displayWarning("Invalid magic number, expected " + expectedStr + ", got " + recievedStr);
			return {};
		}

		return {};
	}

	XpsHeader XpsBinUtils::readHeader(std::ifstream& fileStream)
	{
		XpsHeader result = {};
		XpsFlags flags = {};

		uint32_t magic_number = 0;
		fileStream.read((char*)&magic_number, sizeof(uint32_t));

		if (magic_number != XPS_MAGIC_NUMBER)
		{
			MString recievedStr(std::to_string(magic_number).c_str());
			MGlobal::displayError("Invalid magic number: " + recievedStr);
			return result;
		}

		fileStream.read((char*)&result.versionMajor, sizeof(uint16_t));
		fileStream.read((char*)&result.versionMinor, sizeof(uint16_t));

		std::optional<std::string> xnaAral = readString(fileStream);
		if (xnaAral.has_value()) 
			result.xnaAral = xnaAral.value();

		fileStream.read((char*)&result.settingsLen, sizeof(uint32_t));

		std::optional<std::string> machine = readString(fileStream);
		if (machine.has_value())
			result.machine = machine.value();

		std::optional<std::string> userName = readString(fileStream);
		if (userName.has_value())
			result.user = userName.value();

		std::optional<std::string> filesString = readString(fileStream);
		if (filesString.has_value())
			result.files = filesString.value();

		bool hasTangent = hasTangentVersion(result.versionMajor, result.versionMinor);

		if (hasTangent)
		{
			std::string settingsStr = std::string();
			settingsStr.resize(result.settingsLen * 4);
			fileStream.read((char*)settingsStr.data(), sizeof(result.settingsLen * 4));
		}
		else
		{
			uint16_t valuesRead = 0;
			
			uint32_t hash = 0;
			fileStream.read((char*)&hash, sizeof(uint32_t));
			valuesRead += sizeof(uint32_t);
			
			uint32_t items = 0;
			fileStream.read((char*)&items, sizeof(uint32_t));
			valuesRead += sizeof(uint32_t);
			
			for (size_t i = 0; i < items; i++)
			{
				uint32_t optType = 0;
				fileStream.read((char*)&optType, sizeof(uint32_t));
				valuesRead += sizeof(uint32_t);

				uint32_t optCount = 0;
				fileStream.read((char*)&optCount, sizeof(uint32_t));
				valuesRead += sizeof(uint32_t);

				uint32_t optInfo = 0;
				fileStream.read((char*)&optInfo, sizeof(uint32_t));
				valuesRead += sizeof(uint32_t);

				if (optType == 0)
				{
					for (size_t j = 0; j < optCount; j++)
					{
						uint32_t waste = 0;
						fileStream.read((char*)&waste, sizeof(uint32_t));
					}

					valuesRead += optCount * 2;
				}
				else if (optType == 1)
				{
					auto xpsPoseData = readDefaultPose(fileStream, optCount, optInfo);
					uint32_t readCount = roundToMultiple(optCount, XPS_ROUND_MULTIPLE);
					valuesRead += readCount;
				}
				else if (optType == 2)
				{
					flags = readFlags(fileStream, optCount);
					valuesRead += optCount * 2 * 4;
				}
				else
				{
					uint32_t loopStart = valuesRead / 4;
					uint32_t loopFinish = result.settingsLen;

					for (size_t j = loopStart; j < loopFinish; j++)
					{
						uint32_t waste = 0;
						fileStream.read((char*)&waste, sizeof(uint32_t));
					}
				}
			}

		}

		return result;
	}

	std::optional<std::string> XpsBinUtils::readString(std::ifstream& fileStream)
	{
		uint8_t lengthByte1 = 0;
		fileStream.read((char*)&lengthByte1, sizeof(uint8_t));

		uint8_t lengthByte2 = 0;

		if (lengthByte1 == 0)
			return {};

		if (lengthByte1 >= XPS_LIMIT)
		{
			fileStream.read((char*)&lengthByte2, sizeof(uint8_t));
			if (lengthByte2 == 0)
				return {};
		}

		size_t length = (lengthByte1 % XPS_LIMIT) + (lengthByte2 * XPS_LIMIT);

		std::string result = std::string();
		result.resize(length);

		fileStream.read((char*)result.data(), length * sizeof(char));
		return result;
	}

	bool XpsBinUtils::hasTangentVersion(const uint16_t& verMaj, const uint16_t& verMin, bool hasHeader)
	{
		if (hasHeader)
			return (verMin <= 12 && verMaj <= 2);
		else
			return true;
	}

	std::unordered_map<std::string, XpsBonePose> XpsBinUtils::readDefaultPose(std::ifstream& fileStream, uint32_t poseLenUnround, uint32_t poseBones)
	{
		std::string poseBytes = std::string();
		if (poseLenUnround && poseBones > 0)
		{
			for (size_t i = 0; i < poseBones; i++)
			{
				std::string line;
				std::getline(fileStream, line);
				if (!line.empty())
					poseBytes += line;
			}
		}

		uint32_t poseLength = roundToMultiple(poseLenUnround, XPS_ROUND_MULTIPLE);
		uint32_t emptyBytes = poseLength - poseLenUnround;
		if (emptyBytes > 0)
		{
			std::string poseString = std::string();
			poseString.resize(emptyBytes);
			fileStream.read((char*)poseString.data(), sizeof(emptyBytes));
		}

		std::unordered_map<std::string, XpsBonePose> result = poseData(poseBytes);
		return result;
	}

	uint32_t XpsBinUtils::roundToMultiple(uint32_t numToRound, uint32_t multiple)
	{
		return ((numToRound + multiple - 1) / multiple) * multiple;
	}

	XpsFlags XpsBinUtils::readFlags(std::ifstream& fileStream, uint32_t optCount)
	{
		XpsFlags flags = {};
		for (size_t i = 0; i < optCount; i++)
		{
			uint32_t flag = 0;
			fileStream.read((char*)&flag, sizeof(uint32_t));

			uint32_t value = 0;
			fileStream.read((char*)&value, sizeof(uint32_t));

			if (flagList.find(flag) != flagList.end())
			{
				std::string flagName = flagList[flag];
				switch (flag)
				{
				case 0: // Back face culling 
					flags.backFaceCulling = flagValue(flag, value);
					break;

				case 1: // Always force culling
					flags.alwaysForceCulling = flagValue(flag, value);
					break;

				case 2: // Model cast shadows
					flags.modelCastShadows = flagValue(flag, value);
					break;

				case 3: // Tangent space red
					flags.tangentSpaceRed = flagValue(flag, value);
					break;

				case 4: // Tangent space green
					flags.tangentSpaceGreen = flagValue(flag, value);
					break;

				case 5: // Tangent space blue
					flags.tangentSpaceBlue = flagValue(flag, value);
					break;

				case 6: // Gloss
					flags.gloss = flagValue(flag, value);
					break;

				case 7: // Has bone directions
					flags.hasBoneDirections = flagValue(flag, value);
					break;

				default:
					MGlobal::displayWarning("Unknown flag encountered!");
					break;
				}
			}
		}

		return flags;
	}

	int XpsBinUtils::flagValue(uint32_t flag, uint32_t value)
	{
		if ((flag >= 0 && flag <= 2) || (flag >= 6 && flag <= 7))
			return bool(flag);

		else if (flag >= 3 && flag <= 5)
			return (value % 2);

		else
			return value;
	}

	std::unordered_map<std::string, XpsBonePose> XpsBinUtils::poseData(std::string poseString)
	{
		std::unordered_map<std::string, XpsBonePose> poseData;

		std::vector<std::string> poseList = splitStringByChar(poseString, '\n');

		for (auto& bonePose : poseList)
		{
			std::vector<std::string> pose = splitStringByChar(bonePose, ':');

			if (pose.size() < 2)
				continue;

			std::string boneName = pose[0];
			if (boneName.empty())
				continue;

			std::vector<char> chars(pose[1].begin(), pose[1].end());
			std::vector<char> dataList = fillArray(chars, 9, 1);

			XpsVector3 rotDelta = {};
			rotDelta.x = float(dataList[0]);
			rotDelta.y = float(dataList[1]);
			rotDelta.z = float(dataList[2]);

			XpsVector3 coordDelta = {};
			coordDelta.x = float(dataList[3]);
			coordDelta.y = float(dataList[4]);
			coordDelta.z = float(dataList[5]);

			XpsVector3 scale = {};
			scale.x = float(dataList[6]);
			scale.x = float(dataList[7]);
			scale.x = float(dataList[8]);

			XpsBonePose bp = {};
			bp.boneName = boneName;
			bp.coordDelta = coordDelta;
			bp.rotDelta = rotDelta;
			bp.scale = scale;

			poseData[boneName] = bp;
		}

		return poseData;
	}

	std::vector<std::string> XpsBinUtils::splitStringByChar(const std::string& s, char c)
	{
		// Wrap the string in a stream to use stream-based parsing.
		std::istringstream iss(s);
		std::string split;
		std::vector<std::string> v_result;

		// Continuously extract characters until the next delimiter 'c' is found.
		while (std::getline(iss, split, c))
			v_result.push_back(split);

		return v_result;
	}

	std::vector<char> XpsBinUtils::fillArray(const std::vector<char>& array, uint32_t minLength, char value)
	{
		std::vector<char> result = array;

		size_t len = array.size();
		
		while (len < minLength)
		{
			result.push_back(value);
			len++;
		}

		return result;
	}
}