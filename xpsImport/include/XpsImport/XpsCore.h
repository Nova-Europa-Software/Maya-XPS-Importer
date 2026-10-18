#pragma once

// C++ stdlib headers
#include <filesystem>
#include <string>
#include <fstream>
#include <optional>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <cmath>
#include <thread>

// Maya SDK headers
#include <maya/MFnTransform.h>
#include <maya/MFnMesh.h>
#include <maya/MPxFileTranslator.h>
#include <maya/MGlobal.h>
#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>

// XPS default / fallback values
#define XPS_MAGIC_NUMBER 323232
#define XPS_VERSION_MAJOR 3
#define XPS_VERSION_MINOR 15
#define XPS_ARAL "XNAaraL"
#define XPS_SETTINGS_LENGTH = 1080
#define XPS_LIMIT 128
#define XPS_STRLEN 275
#define XPS_ROUND_MULTIPLE 4

#define ENCODING_READ "utf-8-sig"
#define ENCODING_WRITE "utf-8"

// XPS Flags
#define BACK_FACE_CULLING "backFaceCulling"
#define ALWAYS_FORCE_CULLING "alwaysForceCulling"
#define MODEL_CAST_SHADOWS "modelCastShadows"
#define TANGENT_SPACE_RED "TangentSpaceRed"
#define TANGENT_SPACE_GREEN "TangentSpaceGreen"
#define TANGENT_SPACE_BLUE "TangentSpaceBlue"
#define GLOSS "gloss"
#define HAS_BONE_DIRECTIONS "hasBoneDirections"