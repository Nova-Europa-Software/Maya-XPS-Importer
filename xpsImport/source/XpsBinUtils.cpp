#include <XpsImport/XpsBinUtils.h>
#include <fstream>
#include <vector>

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

		// Read XPS file version
		fileStream.read((char*)&result.versionMajor, sizeof(uint16_t));
		fileStream.read((char*)&result.versionMinor, sizeof(uint16_t));

		// Read XNA aral string
		std::optional<std::string> xnaAral = readString(fileStream);
		if (xnaAral.has_value()) 
			result.xnaAral = xnaAral.value();

		// Read settings length
		fileStream.read((char*)&result.settingsLen, sizeof(uint32_t));

		// Read machine name (computer name that the model was created on)
		std::optional<std::string> machine = readString(fileStream);
		if (machine.has_value())
			result.machine = machine.value();

		// Read user name
		std::optional<std::string> userName = readString(fileStream);
		if (userName.has_value())
			result.user = userName.value();

		// Read referenced files
		std::optional<std::string> filesString = readString(fileStream);
		if (filesString.has_value())
			result.files = filesString.value();

		// Determine how to read additional data based on file version
		bool hasTangent = hasTangentVersion(result.versionMajor, result.versionMinor);

		if (hasTangent)
		{
#ifdef _DEBUG
			MGlobal::displayInfo("hasTangent was set to TRUE");
#endif
			std::string settingsStr = std::string();
			settingsStr.resize(result.settingsLen * 4);
			fileStream.read((char*)settingsStr.data(), sizeof(result.settingsLen * 4));
		}
		else
		{
#ifdef _DEBUG
			MGlobal::displayInfo("hasTangent was set to FALSE");
#endif

			uint16_t valuesRead = 0;
			
			uint32_t hash = 0;
			fileStream.read((char*)&hash, sizeof(uint32_t));
			valuesRead += sizeof(uint32_t);
			
			uint32_t items = 0;
			fileStream.read((char*)&items, sizeof(uint32_t));
			valuesRead += sizeof(uint32_t);

#ifdef _DEBUG
			MString hashStr(std::to_string(hash).c_str());
			MString itemsStr(std::to_string(items).c_str());

			MGlobal::displayInfo("Hash resolved to " + hashStr);
			MGlobal::displayInfo("Item count set to " + itemsStr);
#endif
			
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

#ifdef _DEBUG
				MString typeStr(std::to_string(optType).c_str());
				MString countStr(std::to_string(optCount).c_str());
				MString infoStr(std::to_string(optInfo).c_str());

				MGlobal::displayInfo("optType set to " + typeStr);
				MGlobal::displayInfo("optCount set to " + countStr);
				MGlobal::displayInfo("optInfo set to " + infoStr);
#endif

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

#ifdef _DEBUG
		MString mnStr(std::to_string(magic_number).c_str());
		MString vmjStr(std::to_string(result.versionMajor).c_str());
		MString vmiStr(std::to_string(result.versionMinor).c_str());
		MString xnaAralStr(result.xnaAral.c_str());
		MString slenStr(std::to_string(result.settingsLen).c_str());
		MString machineStr(result.machine.c_str());
		MString userStr(result.user.c_str());
		MString filesStr(result.files.c_str());

		MGlobal::displayInfo("XPS Header info: ");
		MGlobal::displayInfo("Magic number = " + mnStr);
		MGlobal::displayInfo("Version major = " + vmjStr);
		MGlobal::displayInfo("Version minor = " + vmiStr);
		MGlobal::displayInfo("XNAaral = " + xnaAralStr);
		MGlobal::displayInfo("Settings length = " + slenStr);
		MGlobal::displayInfo("Machine = " + machineStr);
		MGlobal::displayInfo("User = " + userStr);
		MGlobal::displayInfo("Files = " + filesStr);
#endif

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
#ifdef _DEBUG
		MGlobal::displayInfo("Reading flags...");
#endif

		XpsFlags flags = {};
		for (size_t i = 0; i < optCount; i++)
		{
			uint32_t flag = 0;
			fileStream.read((char*)&flag, sizeof(uint32_t));

			uint32_t value = 0;
			fileStream.read((char*)&value, sizeof(uint32_t));

			if (flagList.find(flag) != flagList.end())
			{
#ifdef _DEBUG
				MString valueStr(std::to_string(flagValue(flag, value)).c_str());
#endif

				std::string flagName = flagList[flag];
				switch (flag)
				{
				case 0: // Back face culling 
					flags.backFaceCulling = flagValue(flag, value);
#ifdef _DEBUG
					MGlobal::displayInfo("Back face culling set to " + valueStr);
#endif
					break;

				case 1: // Always force culling
					flags.alwaysForceCulling = flagValue(flag, value);
#ifdef _DEBUG
					MGlobal::displayInfo("Always force culling set to " + valueStr);
#endif
					break;

				case 2: // Model cast shadows
					flags.modelCastShadows = flagValue(flag, value);
#ifdef _DEBUG
					MGlobal::displayInfo("Model cast shadows set to " + valueStr);
#endif
					break;

				case 3: // Tangent space red
					flags.tangentSpaceRed = flagValue(flag, value);
#ifdef _DEBUG
					MGlobal::displayInfo("Tangent space red set to " + valueStr);
#endif
					break;

				case 4: // Tangent space green
					flags.tangentSpaceGreen = flagValue(flag, value);
#ifdef _DEBUG
					MGlobal::displayInfo("Tangent space green set to " + valueStr);
#endif
					break;

				case 5: // Tangent space blue
					flags.tangentSpaceBlue = flagValue(flag, value);
#ifdef _DEBUG
					MGlobal::displayInfo("Tangent space blue set to " + valueStr);
#endif
					break;

				case 6: // Gloss
					flags.gloss = flagValue(flag, value);
#ifdef _DEBUG
					MGlobal::displayInfo("Gloss set to " + valueStr);
#endif
					break;

				case 7: // Has bone directions
					flags.hasBoneDirections = flagValue(flag, value);
#ifdef _DEBUG
					MGlobal::displayInfo("Has bone directions set to " + valueStr);
#endif
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

	std::vector<XpsBone> XpsBinUtils::readBones(std::ifstream& fileStream, XpsHeader& header)
	{
		std::vector<XpsBone> bones;

		uint32_t boneCount = 0;
		fileStream.read((char*)&boneCount, sizeof(uint32_t));

		for (size_t i = 0; i < boneCount; i++)
		{
			std::string boneName = std::string();

			std::optional<std::string> nameOpt = readString(fileStream);
			if (nameOpt.has_value())
				boneName = nameOpt.value();
			else
				boneName = "Bone_" + std::to_string(i);

			uint16_t parentId = 0;
			fileStream.read((char*)&parentId, sizeof(uint16_t));

			XpsVector3 coord = {};
			fileStream.read((char*)&coord.x, sizeof(float));
			fileStream.read((char*)&coord.y, sizeof(float));
			fileStream.read((char*)&coord.z, sizeof(float));

			XpsBone bone = {};
			bone.id = i;
			bone.parentId = parentId;
			bone.name = boneName;
			bone.co = coord;

			bones.push_back(bone);

#ifdef _DEBUG
			MString xStr(std::to_string(coord.x).c_str());
			MString yStr(std::to_string(coord.y).c_str());
			MString zStr(std::to_string(coord.z).c_str());
			MString boneNameStr(boneName.c_str());
			MString boneIdStr(std::to_string(i).c_str());
			MString parentIdStr(std::to_string(parentId).c_str());

			MGlobal::displayInfo("Bone appended!");
			MGlobal::displayInfo("Bone name = " + boneNameStr);
			MGlobal::displayInfo("Bone id = " + boneIdStr);
			MGlobal::displayInfo("Parent bone id = " + parentIdStr);
			MGlobal::displayInfo("Bone coord X = " + xStr);
			MGlobal::displayInfo("Bone coord Y = " + yStr);
			MGlobal::displayInfo("Bone coord Z = " + zStr);
#endif
		}

		return bones;
	}

	std::vector<XpsMesh> XpsBinUtils::readMeshes(std::ifstream& fileStream, std::optional<XpsHeader> headerOpt, bool hasBones)
	{
		std::vector<XpsMesh> meshes = std::vector<XpsMesh>();

		uint32_t meshCount = 0;
		fileStream.read((char*)&meshCount, sizeof(uint32_t));

		uint32_t verMajor = 0;
		uint32_t verMinor = 0;

		if(headerOpt.has_value())
		{
			XpsHeader header = headerOpt.value();
			verMajor = header.versionMajor;
			verMinor = header.versionMinor;
		}

		bool hasTangent = hasTangentVersion(verMajor, verMinor, headerOpt.has_value());
		bool hasVWeights = hasVariableWeights(verMajor, verMinor, headerOpt.has_value());
		
		for(size_t i = 0; i < meshCount; i++)
		{
			std::string meshName = readString(fileStream);
			if(meshName.empty())
				meshName = "Mesh_" + std::to_string(i);

			uint32_t uvLayerCount = 0;
			fileStream.read((char*)&uvLayerCount, sizeof(uint32_t));

			uint32_t textureCount = 0;
			fileStream.read((char*)&textureCount, sizeof(uint32_t));

			std::vector<XpsTexture> textures = std::vector<XpsTexture>(textureCount);

			for(size_t j = 0; j < textureCount; j++)
			{
				std::string texturePath = readString(fileStream);
				uint32_t uvLayerId = 0;
				fileStream.read((char*)&uvLayerId, sizeof(uint32_t));
				
				XpsTexture tex = {};
				tex.id = j;
				tex.file = texturePath;
				tex.uvLayer = uvLayerId;

				textures[j] = tex;
			}

			uint32_t vertexCount = 0;
			fileStream.read((char*)&vertexCount, sizeof(uint32_t));

			for(size_t j = 0; j < vertexCount; j++)
			{
				XpsVector3 coord = {};
				fileStream.read((char*)&coord.x, sizeof(float));
				fileStream.read((char*)&coord.y, sizeof(float));
				fileStream.read((char*)&coord.z, sizeof(float));

				XpsVector3 normal = {};
				fileStream.read((char*)&normal.x, sizeof(float));
				fileStream.read((char*)&normal.y, sizeof(float));
				fileStream.read((char*)&normal.z, sizeof(float));

				std::vector<XpsVector2> uvs = std::vector<XpsVector2>(uvLayerCount);

				for(size_t k = 0; k < uvLayerCount; k++)
				{
					XpsVector2 uv = {};
					fileStream.read((char*)&uv.x, sizeof(float));
					fileStream.read((char*)&uv.y, sizeof(float));

					uvs[k] = uv;

					if(hasTangent)
					{
						XpsVector4 tangent = {};
						fileStream.read((char*)&tangent.x, sizeof(float));
						fileStream.read((char*)&tangent.y, sizeof(float));
						fileStream.read((char*)&tangent.z, sizeof(float));
						fileStream.read((char*)&tangent.w, sizeof(float));
					}

					if(hasBones)
					{
						uint16_t weightCount = 4;
						if(hasVariableWeights)
							fileStream.read((char*)&weightCount, sizeof(uint16_t));
					}
				}
			}
		}
	
	}

	bool XpsBinUtils::hasVariableWeights(uint32_t verMajor, uint32_t verMinor, bool hasHeader)
	{
		return (verMajor >= 3) && hasHeader;
	}
}