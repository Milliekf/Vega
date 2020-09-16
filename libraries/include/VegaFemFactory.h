#pragma once

#include <iostream>
#include <fstream>
#include<sstream>
#include <assert.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
//#include "../RenderingProcess/Mesh.h"
//#include "../RenderingProcess/Sence.h"
//#include "../RenderingProcess/Shader.h"
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
	//根据文件的绝对路径取文件名字,如：positionthe0phi0force500,500,500
	std::string getFileName(const std::string & vFileDirectory);
	//返回一个形变文件
	Common::SFileFrames getFileFrames(int vIndex) { return m_FilesData[vIndex]; }
	void setDeformationStateFromFileName();
	CModelDeformationTransform*  getModelTransformStruct() { return m_ModelTransformStruct;}
	std::vector<Common::SFileFrames> searchFileFrames(const int vTheta, const int vPhi, const std::vector<int> & vForceFluctuationSequence);
	std::vector<std::vector<glm::vec3>> objDeformation(std::pair<int, int> vForceDirection, std::vector<int> vForceFluctuationSequence);
	void cleanSFileDataGroup(int vConnectionIndex, int vTimestep);

	

private:

	void __InternalClassAddFilesData(std::string vFileIndex);
	
	//路径下面的每个文件的绝对路径集合，如：D:\GraduationProject\Vega\models\8.10\test\positionthe0phi0force500,500,500.txt
	std::vector<std::string> m_FilePathList;
	//形变文件的集合
	std::vector<Common::SFileFrames> m_FilesData;
	std::vector<Common::SConnectedFemFiles> m_AllReallyLoadConnectedFem;
	//可以看作是一个obj的model对象，有mesh集合，以及group组集合
	CModelDeformationTransform* m_ModelTransformStruct;
};