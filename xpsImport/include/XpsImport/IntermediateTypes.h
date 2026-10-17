#pragma once
#include "XpsCore.h"

namespace XpsImport
{
	/**
	 * @brief Struct containing parsed XPS data ready to be passed to Maya
	*/
	struct MeshVertexData
	{
		/** Array containing ID of each vertex */
		MIntArray vertexIdArray = {};
		/** Array normal data for each vertex */
		MVectorArray normalArray = {};
		/** Array position data for each vertex */
		MPointArray positionArray = {};
	};
}