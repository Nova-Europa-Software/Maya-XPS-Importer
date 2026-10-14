#include <XpsImport/XpsBinUtils.h>
#include <fstream>
#include <vector>

namespace XpsImport
{

	/**
	 * @brief Looks if the magic number signature that is at the beggining of each XPS file.
	 *
	 * Checks for magic number signature (323232) that should be at the beggining of
	 * every XPS file. If signature is found indicating that an XPS file was provided
	 * a call to @b readHeader() function is made. 
	 *
	 * @param fileStream a std::ifstream of an currently parsed XPS file.
	 *
	 * @return A XpsHeader structure filled with parsed header data upon succes or
	 * an empty optional upon failure.
	 * 
	 * @see readHeader
	*/
	std::optional<XpsHeader> XpsBinUtils::findHeader(std::ifstream& fileStream)
	{
		XpsHeader result = {};

		// Read first 4 bytes from the file
		uint32_t magic_number = 0;
		fileStream.read((char*)&magic_number, sizeof(uint32_t));
		fileStream.seekg(0);

		// Check read bytes against expected signature (323232)
		if (magic_number == XPS_MAGIC_NUMBER)
		{
			// If signature matches expected value begin parsing the header
			MGlobal::displayInfo("XPS header found!");
			result = readHeader(fileStream);
			return result;
		}
		else
		{
			// Otherwise print error message and return empty optional
			MString recievedStr(std::to_string(magic_number).c_str());
			MString expectedStr(std::to_string(XPS_MAGIC_NUMBER).c_str());

			MGlobal::displayWarning("Invalid magic number, expected " + expectedStr + ", got " + recievedStr);
			return {};
		}

		// In case if we go outside of if-else block return empty optional
		// (though this should not happen)
		return {};
	}

	/**
	 * @brief Reads and parses the data from XPS header.
	 * 
	 * Reads and parses (in this order) magic number, XPS file version, XNAaral string,
	 * settings length (in bytes), machine name and user name of the XPS file creator
	 * and all referenced files. Later, checks if file contains tangent data (appears only
	 * in certain versions), then reads and parses additional flags (culling, shadow casting,
	 * tangent space, glossines, etc.)
	 * 
	 * @return Returns an parsed header as XpsHeader struct.
	 * 
	 * @see hasTangentVersion
	*/
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
			// If hasTangent is set to true read additional data so the
			// file stream is in correct place later, but don't parse the data.
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
					uint32_t readCount = roundUpToMultiple(optCount, XPS_ROUND_MULTIPLE);
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

	/**
	 * @brief Attemps to read a string stored in an XPS file.
	 * 
	 * Attemps to read a string stored in a XPS file. Treats the
	 * first encountered byte as a string length descriptor.
	 * If the string length descriptor exceeds an XPS limit (128)
	 * then the following byte is also treated as a string length descriptor
	 * otherwise it is set to 0. The string length is calculated using following formula 
	 * @b (lengthByte1 % XPS_LIMIT) + (lengthByte2 * XPS_LIMIT). If any of the length
	 * bytes are read as 0 then function fails and returns empty optional. Otherwise
	 * the function reads N bytes (N = length calculated using previously provided formula)
	 * and returns them as a string.
	 * 
	 * @note The fileStream must already point to the byte containing string length.
	 * 
	 * @return Returns a string read from XPS file upon succes. Otherwise returns empty optional.
	 */
	std::optional<std::string> XpsBinUtils::readString(std::ifstream& fileStream)
	{
		uint8_t lengthByte1 = 0;
		fileStream.read((char*)&lengthByte1, sizeof(uint8_t));

		if (lengthByte1 == 0)
			return {};

		uint8_t lengthByte2 = 0;

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

	/**
	 * @brief Checks if provided file version has support for tangents
	 * 
	 * @note If hasHeader was set to false the function skips the check and
	 * returns true.
	 * 
	 * @return True if provided version has tangent support, false otherwise.
	*/
	bool XpsBinUtils::hasTangentVersion(const uint16_t& verMaj, const uint16_t& verMin, bool hasHeader)
	{
		if (hasHeader)
			return (verMin <= 12 && verMaj <= 2);
		else
			return true;
	}

	/**
	 * @brief Reads default pose for the model.
	 * 
	 * @return A default pose for a model.
	 * 
	 * @see poseData
	*/
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

		uint32_t poseLength = roundUpToMultiple(poseLenUnround, XPS_ROUND_MULTIPLE);
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

	/**
	 * @brief Rounds provided number to the nearest multiple of @b multiple
	 * 
	 * Round provided number to the nearest multiple of the @b multiple argument.
	 * (Example [multiple = 5]: [0...5] -> 5, [6...10] -> 10, [11...15] -> 15, etc.)
	 * 
	 * @note This function always rounds upwords! This means that the result is always
	 * greater that the @b numToRound parameter.
	*/
	uint32_t XpsBinUtils::roundUpToMultiple(uint32_t numToRound, uint32_t multiple)
	{
		if (multiple == 0)
			return numToRound;              // Nothing to round to

		uint32_t remainder = numToRound % multiple;
		if (remainder == 0)
			return numToRound;              // Already a multiple

		return numToRound + (multiple - remainder);   // Add what is missing
	}

	/**
	 * @brief Reads model flags from XPS file.
	 * 
	 * @return A XpsFlags structure containing all values parsed from XPS file.
	*/
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

	/**
	 * @brief Converts raw flag value (uint32_t) to its proper type.
	 * 
	 * Converts provided value to fit the desired flag. For flags between
	 * 0 - 2 and 6 - 7 the value is converted to a boolean like integer.
	 * For values between 3 - 5 the value is normalized to a value between
	 * -1 and 1 using modulo operator (%). For any other flag the value is left
	 * untouched.
	 * 
	 * @return Retuns converted value.
	*/
	int XpsBinUtils::flagValue(uint32_t flag, uint32_t value)
	{
		if ((flag >= 0 && flag <= 2) || (flag >= 6 && flag <= 7))
			return bool(flag);

		else if (flag >= 3 && flag <= 5)
			return (value % 2);

		else
			return value;
	}

	/**
	 * @brief Reads and parses pose data for each bone.
	 * 
	 * @return An unordered map containing pose data.
	*/
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

	/**
	 * @brief Splits one string into an array of strings using provided character as a delimiter
	 * 
	 * @returns An array of strings separated by provided delimiter.
	*/
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

	/**
	 * @brief Fill the remaining elements of the array with specified characters so it fits specified length
	 * 
	 * Copies elements from provided array to new array and fills additional indices with specifed value
	 * untill the length of the new array is at least equal to minLength array
	 * 
	 * @return Provided array with elements appended to it (if necessary)
	*/
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

	/**
	 * @brief Read models bone data
	 * 
	 * Reads model bone count and parses data for each bone
	 * (Name, position, parent bone, bone id).
	 * 
	 * @note If bone name cannot be obtained a new generic one is generated
	 * using bone id (Bone_[boneId]).
	*/
	std::vector<XpsBone> XpsBinUtils::readBones(std::ifstream& fileStream, XpsHeader& header)
	{

		// Read imported model bone count
		uint32_t boneCount = 0;
		fileStream.read((char*)&boneCount, sizeof(uint32_t));

		std::vector<XpsBone> bones(boneCount);

		// Parse each bone
		for (size_t i = 0; i < boneCount; i++)
		{
			std::string boneName = std::string();

			// Try to obtain a bone name
			std::optional<std::string> nameOpt = readString(fileStream);
			if (nameOpt.has_value())
				boneName = nameOpt.value();
			else
				// If bone name could not be obtained generate a new generic name for it
				boneName = "Bone_" + std::to_string(i);

			// Read parent bones
			uint16_t parentId = 0;
			fileStream.read((char*)&parentId, sizeof(uint16_t));

			// Read bone coordinates
			XpsVector3 coord = {};
			fileStream.read((char*)&coord.x, sizeof(float));
			fileStream.read((char*)&coord.y, sizeof(float));
			fileStream.read((char*)&coord.z, sizeof(float));

			XpsBone bone = {};
			bone.id = i;
			bone.parentId = parentId;
			bone.name = boneName;
			bone.co = coord;

			bones[i] = bone;

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

	/**
	 * @brief Reads and parses mesh data from XPS file
	 * 
	 * Parses data (name, textures, vertices and indices) for each mesh stored in a XPS file.
	 * The function firstly obtains number of meshes stored in a file. Then it
	 * loops over each mesh firstly parsing all textures related to it. After that the function
	 * loops over each vertex of a mesh parsing its data (position, normal, texture coords, tangents
	 * and weights). After the function finishes parsing all vertices of a mesh it starts parsing
	 * all indices of the mesh. When the function finishes parsing data for all meshes it returns
	 * a vector containing all parsed data.
	 * 
	 * @note If a mesh name cannot be obtained then a new generic one is generated
	 * 
	 * @note If a related texture name cannot be obtained then a new generic one is generated
	 * 
	 * @return Returns a vector containing data of all meshes included in XPS file.
	*/
	std::vector<XpsMesh> XpsBinUtils::readMeshes(std::ifstream& fileStream, std::optional<XpsHeader> headerOpt, bool hasBones)
	{
		// Read mesh count
		uint32_t meshCount = 0;
		fileStream.read((char*)&meshCount, sizeof(uint32_t));

		std::vector<XpsMesh> meshes(meshCount);

#ifdef _DEBUG
		MString meshCountStr(std::to_string(meshCount).c_str());
		MGlobal::displayInfo("Mesh count = " + meshCountStr);
#endif

		uint32_t verMajor = 0;
		uint32_t verMinor = 0;

		if(headerOpt.has_value())
		{
			XpsHeader header = headerOpt.value();
			verMajor = header.versionMajor;
			verMinor = header.versionMinor;
		}

		// Check if the model has tangent data and variable weights
		bool hasTangent = hasTangentVersion(verMajor, verMinor, headerOpt.has_value());
		bool hasVWeights = hasVariableWeights(verMajor, verMinor, headerOpt.has_value());

#ifdef _DEBUG
		if (hasTangent) MGlobal::displayInfo("hasTangent set to true!");
		else MGlobal::displayInfo("hasTangent set to false!");

		if (hasVWeights) MGlobal::displayInfo("hasVariableWeights set to true!");
		else MGlobal::displayInfo("hasVariableWeights set to false!");
#endif
		
		// Parse each mesh
		for(size_t i = 0; i < meshCount; i++)
		{
			XpsMesh mesh = {};

			// Try to obtain mesh name
			std::optional<std::string> meshStr = readString(fileStream);
			std::string meshName = std::string();
			
			if (meshStr.has_value())
				meshName = meshStr.value();
			else
				// If mesh name cannot be obtained then generate a generic one
				meshName = "Mesh_" + std::to_string(i);

			mesh.name = meshName;

			// Read number of mesh UV (texCoord) layers
			uint32_t uvLayerCount = 0;
			fileStream.read((char*)&uvLayerCount, sizeof(uint32_t));

			// Read count of textures referenced by mesh
			uint32_t textureCount = 0;
			fileStream.read((char*)&textureCount, sizeof(uint32_t));

#ifdef _DEBUG
			MString meshNameStr(meshName.c_str());
			MString meshIdStr(std::to_string(i).c_str());
			MString uvlCountStr(std::to_string(uvLayerCount).c_str());
			MString texCountStr(std::to_string(textureCount).c_str());

			MGlobal::displayInfo("Mesh id = " + meshIdStr);
			MGlobal::displayInfo("Mesh name = " + meshNameStr);
			MGlobal::displayInfo("UV Layer count = " + uvlCountStr);
			MGlobal::displayInfo("Texture count = " + texCountStr);
#endif

			std::vector<XpsTexture> textures = std::vector<XpsTexture>(textureCount);

			for(size_t j = 0; j < textureCount; j++)
			{
				// Try to obtain texture name / path
				std::optional<std::string> textureStr = readString(fileStream);

				// Read ID of UV layer related to the current texture
				uint32_t uvLayerId = 0;
				fileStream.read((char*)&uvLayerId, sizeof(uint32_t));
				
				XpsTexture tex = {};
				tex.id = j;
				tex.uvLayer = uvLayerId;
				if (textureStr.has_value())
					tex.file = textureStr.value();
				else
					// If texture name cannot be obtained generate a generic one
					tex.file = "texture_" + std::to_string(j);

#ifdef _DEBUG
				MString texStr(tex.file.c_str());
				MGlobal::displayInfo("Found related texture: " + texStr);
#endif

				textures[j] = tex;
			}

			// Read vertex count of the mesh
			uint32_t vertexCount = 0;
			fileStream.read((char*)&vertexCount, sizeof(uint32_t));

#ifdef _DEBUG
			MString vertCountStr(std::to_string(vertexCount).c_str());
			MGlobal::displayInfo("Mesh vertex count: " + vertCountStr);
#endif

			// Parse vertex data for all vertices
			for(size_t j = 0; j < vertexCount; j++)
			{
				XpsVertex vertex;

				// Read vertex position
				XpsVector3 coord = {};
				fileStream.read((char*)&coord.x, sizeof(float));
				fileStream.read((char*)&coord.y, sizeof(float));
				fileStream.read((char*)&coord.z, sizeof(float));

				// Read vertex normal
				XpsVector3 normal = {};
				fileStream.read((char*)&normal.x, sizeof(float));
				fileStream.read((char*)&normal.y, sizeof(float));
				fileStream.read((char*)&normal.z, sizeof(float));

				// Read vertex color
				XpsCVector4 color = {};
				fileStream.read((char*)&color.r, sizeof(int8_t));
				fileStream.read((char*)&color.g, sizeof(int8_t));
				fileStream.read((char*)&color.b, sizeof(int8_t));
				fileStream.read((char*)&color.a, sizeof(int8_t));

				// Read vertex texture coords (UVs)
				std::vector<XpsVector2> uvs = std::vector<XpsVector2>(uvLayerCount);

				for(size_t k = 0; k < uvLayerCount; k++)
				{
					XpsVector2 uv = {};
					fileStream.read((char*)&uv.x, sizeof(float));
					fileStream.read((char*)&uv.y, sizeof(float));

					uvs[k] = uv;

					// If model has tangent data read additional tangent data
					if(hasTangent)
					{
						XpsVector4 tangent = {};
						fileStream.read((char*)&tangent.x, sizeof(float));
						fileStream.read((char*)&tangent.y, sizeof(float));
						fileStream.read((char*)&tangent.z, sizeof(float));
						fileStream.read((char*)&tangent.w, sizeof(float));
					}
				}

				std::vector<XpsBoneWeight> boneWeightsVec = std::vector<XpsBoneWeight>();

				// If model contains skeleton read IDs of the bones related to current vertex
				// and their weights
				if (hasBones)
				{
					std::vector<uint16_t> boneIds = std::vector<uint16_t>();
					std::vector<float> boneWeight = std::vector<float>();

					// If model supports variable weights obtain weight count from the file
					// Otherwise default to 4
					int16_t weightCount = 4;
					if (hasVWeights)
						fileStream.read((char*)&weightCount, sizeof(int16_t));

					if (weightCount >= 0)
					{
						boneIds.resize(weightCount);
						boneWeight.resize(weightCount);
					}

					// Read Bone ID and its weight for all related bones
					for (size_t k = 0; k < weightCount; k++)
					{
						int16_t bone_id = 0;
						float weight = 0.0f;
						fileStream.read((char*)&bone_id, sizeof(int16_t));
						fileStream.read((char*)&weight, sizeof(float));

#ifdef _DEBUG
						if (k >= boneIds.size())
						{
							MGlobal::displayError("k >= boneIds.size(); k out of vector range!");
						}
						else
						{
							boneIds[k] = bone_id;
							boneWeight[k] = weight;
						}
#else
						boneIds[k] = bone_id;
						boneWeight[k] = weight;
#endif
					}

#ifdef _DEBUG
					if (boneIds.size() != boneWeight.size())
					{
						MGlobal::displayError("boneIds.size() != boneWeight.size()");
					}
					else
					{
						for (size_t k = 0; k < boneIds.size(); k++)
						{
							XpsBoneWeight bw = {};
							bw.id = boneIds[k];
							bw.weight = boneWeight[k];
							boneWeightsVec.push_back(bw);
						}
					}
#else
					for (size_t k = 0; k < boneIds.size(); k++)
					{
						XpsBoneWeight bw = {};
						bw.id = boneIds[k];
						bw.weight = boneWeight[k];
						boneWeightsVec.push_back(bw);
					}
#endif

				}

				vertex.coordDelta = coord;
				vertex.normal = normal;
				vertex.color = color;
				vertex.boneWeights = boneWeightsVec;
				vertex.uv = uvs;

				mesh.vertices.push_back(vertex);
			}			

			// Read number of faces / triangles of the current mesh
			uint32_t triCount = 0;
			fileStream.read((char*)&triCount, sizeof(uint32_t));

			MString triCountStr(std::to_string(triCount).c_str());
			MGlobal::displayInfo("Face count = " + triCountStr);

			std::vector<XpsIuVector3> faces(triCount);

			// Parse each face
			for (size_t j = 0; j < triCount; j++)
			{
				XpsIuVector3 triIds;

				fileStream.read((char*)&triIds.x, sizeof(uint32_t));
				fileStream.read((char*)&triIds.y, sizeof(uint32_t));
				fileStream.read((char*)&triIds.z, sizeof(uint32_t));

				MString idX(std::to_string(triIds.x).c_str());
				MString idY(std::to_string(triIds.y).c_str());
				MString idZ(std::to_string(triIds.z).c_str());

				MGlobal::displayInfo("Triangle data : " + idX + "," + idY + "," + idZ);

				faces[j] = triIds;
			}

			mesh.faces = std::vector<XpsIuVector3>(faces.begin(), faces.end());

			meshes[i] = mesh;
		}
	
		return meshes;

	}

	/**
	 * @brief Check if the file has support for variable weights
	 * 
	 * @return True if variable weights are supported, false otherwise
	*/
	bool XpsBinUtils::hasVariableWeights(uint32_t verMajor, uint32_t verMinor, bool hasHeader)
	{
		return (verMajor >= 3) && hasHeader;
	}
}