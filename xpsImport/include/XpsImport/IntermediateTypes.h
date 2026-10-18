#pragma once
#include "XpsCore.h"

namespace XpsImport
{
	/**
	 * @brief Struct containing mesh UV data for current set
	*/
	struct MeshUvSet
	{
		MString setName = {};
		MFloatArray uArray = {};
		MFloatArray vArray = {};
	};

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
		/** Vector containing UV data for all mesh sets */
		std::vector<MeshUvSet> uvDataVec = {};
	};

	/**
	 * @brief Struct containing data about mesh indices
	*/
	struct MeshIndexData
	{
		/** Number of points per face */
		MIntArray polygonCounts = {};
		/** Array containing data about how vertices are connected */
		MIntArray polygonConnects = {};
	};
}