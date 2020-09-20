#pragma once

#include <iostream>
#include <fstream>
#include<sstream>
#include <assert.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "common.h"
#include "ModelDeformationTransform.h"

class CVegaFemFactory
{
public:

	CVegaFemFactory() = default;
	CVegaFemFactory(const std::string & vDirectoryName, const std::string & vMutilVerticesBaseFile);
	~CVegaFemFactory() = default;

	void readFilePath4Directory(const std::string & vDirectoryName);
	void readFramesDeformationData(std::vector<Common::SFileFrames>& vSearchFrames, int vSearchConnectionIndex);
	std::vector<Common::SFileDataGroup> getConnectedFemMutileDeformation(int vConnectionIndex, int vTimestep);
	//�����ļ��ľ���·��ȡ�ļ�����,�磺positionthe0phi0force500,500,500
	std::string getFileName(const std::string & vFileDirectory);
	//����һ���α��ļ�
	Common::SFileFrames getFileFrames(int vIndex) { return m_FilesData[vIndex]; }
	void setDeformationStateFromFileName();
	CModelDeformationTransform*  getModelTransformStruct() { return m_ModelTransformStruct;}
	std::vector<Common::SFileFrames> searchFileFrames(const int vTheta, const int vPhi, const std::vector<int> & vForceFluctuationSequence);
	std::vector<std::vector<glm::vec3>> objDeformation(std::pair<int, int> vForceDirection, std::vector<int> vForceFluctuationSequence);
	void cleanSFileDataGroup(int vConnectionIndex, int vTimestep);

	

private:
	
	//·�������ÿ���ļ��ľ���·�����ϣ��磺D:\GraduationProject\Vega\models\8.10\test\positionthe0phi0force500,500,500.txt
	std::vector<std::string> m_FilePathList;
	//�α��ļ��ļ���
	std::vector<Common::SFileFrames> m_FilesData;
	std::vector<Common::SConnectedFemFiles> m_AllReallyLoadConnectedFem;
	//���Կ�����һ��obj��model������mesh���ϣ��Լ�group�鼯��
	CModelDeformationTransform* m_ModelTransformStruct;
};