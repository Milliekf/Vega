#pragma once
#include <glm/glm.hpp>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "sceneObjectDeformable.h"
#include "ObjStruct.h"
#include "../Common/common.h"

//�����Ҳ�ܽ��ƵĿ���һ��model
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

	//objģ���е�mesh�ļ���
	ObjMesh *m_BaseFileMesh;
	//obj�ܶ�����
	int m_VerticesNumber;
	//һϵ�е�group�ļ���
	std::vector<BaseObjConstruct::SGroup> m_Groups;
};