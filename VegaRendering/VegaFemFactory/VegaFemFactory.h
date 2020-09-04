#pragma once

#include <iostream>
#include <fstream>
#include<sstream>
#include <assert.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "../RenderingProcess/Mesh.h"
#include "../RenderingProcess/Sence.h"
#include "../RenderingProcess/Shader.h"
#include "../Common/common.h"
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
	std::string getFileName(const std::string & vFileDirectory);
	void setDeformationStateFromFileName();
	std::vector<Common::SFileFrames> searchFileFrames(const int vTheta, const int vPhi, const std::vector<int> & vForceFluctuationSequence);

	std::vector<std::vector<glm::vec3>> objDeformation(std::pair<int, int> vForceDirection, std::vector<int> vForceFluctuationSequence);
	void cleanSFileDataGroup(int vConnectionIndex, int vTimestep);

private:

	void __InternalClassAddFilesData(std::string vFileIndex);
	CModelDeformationTransform* m_ModelTransformStruct;
	std::vector<std::string> m_FileList;
	std::vector<Common::SFileFrames> m_FilesData;
	std::vector<Common::SConnectedFemFiles> m_AllReallyLoadConnectedFem;
};