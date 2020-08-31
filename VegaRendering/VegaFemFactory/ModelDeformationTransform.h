#pragma once
#include <glm/glm.hpp>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "sceneObjectDeformable.h"
#include "ObjStruct.h"

class CModelDeformationTransform
{
public:
	CModelDeformationTransform() = default;
	~CModelDeformationTransform() = default;
	CModelDeformationTransform(std::string& vFileName);
	

private:
	void __VertexFaceRelated();

	ObjMesh *m_BaseFileMesh;
	int m_VerticesNumber;
	std::vector<BaseObjConstruct::SGroup> m_Groups;
};