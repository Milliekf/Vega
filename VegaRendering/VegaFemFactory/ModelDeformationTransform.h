#pragma once
#include <glm/glm.hpp>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "sceneObjectDeformable.h"
#include "ObjStruct.h"
#include "../Common/common.h"

class CModelDeformationTransform
{
public:
	CModelDeformationTransform() = default;
	~CModelDeformationTransform() = default;
	CModelDeformationTransform(const std::string& vFileName);
	void SaveDeformationVertexFromBaseModel(const double* u, int vDeformationSize, std::string vSaveFileName, int vtimeStepCounter);
	void ConvertVertex2mutileVerteices(Common::SFileData &vBaseFileDeformation);

private:
	void __VertexFaceRelated();

	ObjMesh *m_BaseFileMesh;
	int m_VerticesNumber;
	std::vector<BaseObjConstruct::SGroup> m_Groups;
};