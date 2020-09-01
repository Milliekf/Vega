#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <boost/serialization/vector.hpp>
#include <algorithm>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

namespace BaseObjConstruct
{
	struct SFace
	{
		glm::vec3 VertexIndex;
		SFace() = default;
		SFace(glm::vec3 vVertexIndex)
		{
			VertexIndex = vVertexIndex;
		}
	};

	struct SGroup
	{
		std::string FaceName;
		std::vector<SFace> Faces;
		unsigned int FaceSize;
		SGroup() = default;
		SGroup(std::string vFaceName, unsigned int vFaceSize)
		{
			FaceName = vFaceName;
			FaceSize = vFaceSize;
		}
	};
}