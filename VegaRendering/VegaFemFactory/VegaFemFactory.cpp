#include "VegaFemFactory.h"

CVegaFemFactory::CVegaFemFactory(const std::string & vDirectoryName,const std::string & vMutilVerticesBaseFile)
{
	readFilePath4Directory(vDirectoryName);
	setDeformationStateFromFileName();
	m_ModelTransformStruct=new CModelDeformationTransform(vMutilVerticesBaseFile);
}

//将文件夹下所有文件创建多个SFileFrames(但并未加载内容)
void CVegaFemFactory::readFilePath4Directory(const std::string & vDirectoryName)
{
	intptr_t  hFile = 0;
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(vDirectoryName).append("\\*.txt").c_str(), &fileinfo)) != -1)
	{
		do
		{
			m_FileList.push_back(p.assign(vDirectoryName).append("\\").append(fileinfo.name));

		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	assert(m_FileList.size() > 0);

	for (auto i = 0; i < m_FileList.size(); i++)
	{
		int timeStepCount = 0;
		std::ifstream positionFile(m_FileList[i]);
		std::string lineString;
		char s[4096];
		double position[3];
		if (!positionFile.is_open())
		{
			std::cout << "Error: could not open vertex file" << m_FileList[i] << std::endl;
		}
		int Frameindex = 0;
		m_FilesData.push_back(Common::SFileFrames(getFileName(m_FileList[i]),m_FileList[i]));
		positionFile.close();
	}

}


//对于搜索出的SFileFrames加载数据
void CVegaFemFactory::readFramesDeformationData(std::vector<Common::SFileFrames>& vSearchFrames,int vSearchConnectionIndex)
{
	Common::SConnectedFemFiles tempConnectedFile(vSearchConnectionIndex);
	int counterConnectFileNumber = 0;
	for (auto searchindex = 0; searchindex < vSearchFrames.size(); searchindex++)
	{
		for (auto fileIndex = 0; fileIndex < m_FilesData.size(); fileIndex++)
		{
			if (vSearchFrames[searchindex].FileIndex == m_FilesData[fileIndex].FileIndex)
			{
				int timeStepCount = 1;
				std::ifstream positionFile(m_FilesData[fileIndex].FilePath);
				std::string lineString;
				char s[4096];
				double position[3];
				if (!positionFile.is_open())
				{
					std::cout << "Error: could not open vertex file" << m_FilesData[fileIndex].FilePath << std::endl;
				}
				int Frameindex = 0;

				while (getline(positionFile, lineString))
				{
					sprintf(s, "Position%04d", timeStepCount);
					std::istringstream sin(lineString);
					std::string str;
					sin >> str;//Position%04d后面有空格
					sin.clear();
					if (str == s)
					{
						std::string vertexsize;
						getline(positionFile, vertexsize);
						Common::SFileData tempFileData(Frameindex);
						getline(positionFile, lineString);
						std::vector<double>frameArray;
						std::istringstream dataset(lineString);
						for (int j = 0; j < atoi(vertexsize.c_str()); j++)
						{
							dataset >> position[0] >> position[1] >> position[2];
							tempFileData.BaseFileDeformations.push_back(glm::vec3(position[0], position[1], position[2]));
						}
						timeStepCount++;
						Frameindex++;
						m_FilesData[fileIndex].Frames.push_back(tempFileData);										
					}
				}
				m_FilesData[fileIndex].isLoadDataSet = true;
				tempConnectedFile.FemDataset.push_back(&m_FilesData[fileIndex]);
				counterConnectFileNumber++;
			}
		}
	}
	if (counterConnectFileNumber == 1) tempConnectedFile.Type = Common::EFileFramesType::OneRelatedFile;
	else if (counterConnectFileNumber == 2) tempConnectedFile.Type = Common::EFileFramesType::TwoRelatedFiles;
	else if (counterConnectFileNumber == 4) tempConnectedFile.Type = Common::EFileFramesType::FourRelatedFiles;

	m_AllReallyLoadConnectedFem.push_back(tempConnectedFile);
	//for (auto searchindex = 0; searchindex < vSearchFrames.size(); searchindex++)
	//{
	//	for (auto fileIndex = 0; fileIndex < m_FileList.size(); fileIndex++)
	//	{
	//		if (vSearchFrames[searchindex].FileIndex == getFileName(m_FileList[fileIndex]))
	//		{
	//			int timeStepCount = 1;
	//			std::ifstream positionFile(m_FileList[fileIndex]);
	//			std::string lineString;
	//			char s[4096];
	//			double position[3];
	//			if (!positionFile.is_open())
	//			{
	//				std::cout << "Error: could not open vertex file" << m_FileList[fileIndex] << std::endl;
	//			}
	//			int Frameindex = 0;

	//			while (getline(positionFile, lineString))
	//			{
	//				sprintf(s, "Position%04d", timeStepCount);
	//				std::istringstream sin(lineString);
	//				std::string str;
	//				sin >> str;//Position%04d后面有空格
	//				sin.clear();
	//				if (str == s)
	//				{
	//					std::string vertexsize;
	//					getline(positionFile, vertexsize);
	//					Common::SFileData tempFileData(Frameindex);
	//					getline(positionFile, lineString);	
	//					std::vector<double>frameArray;
	//					std::istringstream dataset(lineString);
	//					for (int j = 0; j < atoi(vertexsize.c_str()); j++)
	//					{
	//						dataset >> position[0] >> position[1] >> position[2];
	//						tempFileData.BaseFileDeformations.push_back(glm::vec3(position[0], position[1], position[2]));
	//					}
	//					timeStepCount++;
	//					Frameindex++;
	//					vSearchFrames[searchindex].Frames.push_back(tempFileData);
	//					vSearchFrames[searchindex].isLoadDataSet = true;
	//				}
	//			}
}

//对于已经创建的多个SFileFrames对象中给与FileIndex 以及其他标识符
void CVegaFemFactory::setDeformationStateFromFileName()
{
	int thetaPos;
	int phiPos;
	int forcePos;
	for (auto i = 0; i < m_FilesData.size(); i++)
	{
		std::vector<std::string> ForceSequence;
		thetaPos = m_FilesData[i].FileIndex.find("the");
		phiPos = m_FilesData[i].FileIndex.find("phi");
		forcePos = m_FilesData[i].FileIndex.find("force");
		m_FilesData[i].Theta = std::stoi(m_FilesData[i].FileIndex.substr(thetaPos + 3, phiPos - thetaPos - 3));
		m_FilesData[i].Phi = std::stoi(m_FilesData[i].FileIndex.substr(phiPos + 3, forcePos - phiPos - 3));
		std::string tempsequence = m_FilesData[i].FileIndex.substr(forcePos + 5);
		boost::split(ForceSequence, m_FilesData[i].FileIndex.substr(forcePos + 5), boost::is_any_of(","), boost::token_compress_off);
		std::vector<std::string>::iterator it;
		for (it = ForceSequence.begin(); it != ForceSequence.end(); ++it)
		{
			m_FilesData[i].ForceFluctuationSequence.push_back(std::stoi(*it));
		}
	}
}

std::string CVegaFemFactory::getFileName(const std::string & vFileDirectory)
{
	int firstPos = vFileDirectory.find_last_of("\\");
	int secondPos = vFileDirectory.find_last_of(".");
	std::string fileName = vFileDirectory.substr(firstPos + 1, secondPos - firstPos - 1);
	return fileName;
}

//将模型的原始形变数据扩大。
void CVegaFemFactory::getConnectedFemMutileDeformation(int vConnectionIndex, int vTimestep)
{
	//搜索所有已经存储的相关Connection项
	for (int connectIndex = 0; connectIndex < m_AllReallyLoadConnectedFem.size(); connectIndex++)
	{
		if (vConnectionIndex == m_AllReallyLoadConnectedFem[connectIndex].ConnectedIndex)
		{
			if (m_AllReallyLoadConnectedFem[connectIndex].Type == Common::EFileFramesType::OneRelatedFile)
			{
				for (int time = 0; time < Common::MaxTimeStep; time++)
				{
					if ((m_AllReallyLoadConnectedFem[connectIndex].FemDataset[0])->Frames[time].FrameIndex == time)
					{
						//返回模型的增量
						m_ModelTransformStruct->ConvertVertex2mutileVerteices((m_AllReallyLoadConnectedFem[connectIndex].FemDataset[0])->Frames[time]);


					}
					
				}			
			}
		}
	}
}

std::vector<std::vector<glm::vec3>> CVegaFemFactory::objDeformation(std::pair<int, int> vForceDirection, std::vector<int> vForceFluctuationSequence)
{
	std::vector<Common::SFileFrames> proximityFileFrames = searchFileFrames(vForceDirection.first, vForceDirection.second, vForceFluctuationSequence);
	std::vector<std::vector<glm::vec3>> u;
	for (auto frame : proximityFileFrames)
	{

		//u.push_back(frame.Frames[0].PositionsDeformation);
	}

	//if(proximityFileFrames.size()==4)
	//search DataSet
	//权重设置,双线性插值
	/*std::string FileIndex=*/
	//返回一个模型的位移再由四面体转换到物体顶点的所有位移量。
	return u;
}

//在已经加载FileIndex但尚未加载位移数据的SFileFrames所有搜索相关的SFileFrames并返回
//返回临近的多个方向数据索引标识
std::vector<Common::SFileFrames> CVegaFemFactory::searchFileFrames(const int vTheta, const int vPhi, const std::vector<int>& vForceFluctuationSequence)
{
	int ThetaIndex = vTheta / 30;
	int PhiIndex = vPhi / 30;
	int ThetaSecondIndex = ThetaIndex + 1;
	int PhiSecondIndex = PhiIndex + 1;
	if (vTheta < 0)
	{
		ThetaSecondIndex = ThetaIndex - 1;
	}
	if (vPhi < 0)
	{
		PhiSecondIndex = PhiIndex - 1;
	}
	std::vector<Common::SFileFrames> fileFrames;
	if (vTheta % 30 == 0 && vPhi % 30 == 0)
	{
		for (auto file : m_FilesData)
		{
			if (file.Theta == (ThetaIndex * 30) && file.Phi == (PhiIndex * 30))
			{
				if (vForceFluctuationSequence == file.ForceFluctuationSequence)
				{
					fileFrames.push_back(file);
				}
			}
		}
	}
	else if (vTheta % 30 == 0 && vPhi % 30 != 0)
	{
		for (auto file : m_FilesData)
		{
			if (file.Theta == (ThetaIndex * 30) && file.Phi == (PhiIndex * 30))
			{
				if (vForceFluctuationSequence == file.ForceFluctuationSequence)
				{
					fileFrames.push_back(file);
				}
			}
			else if (file.Theta == (ThetaIndex * 30) && file.Phi == (PhiSecondIndex * 30))
			{
				if (vForceFluctuationSequence == file.ForceFluctuationSequence)
				{
					fileFrames.push_back(file);
				}
			}
		}
	}
	else if (vTheta % 30 != 0 && vPhi % 30 == 0)
	{
		for (auto file : m_FilesData)
		{
			if (file.Theta == (ThetaIndex * 30) && file.Phi == (PhiIndex * 30))
			{
				if (vForceFluctuationSequence == file.ForceFluctuationSequence)
				{
					fileFrames.push_back(file);
				}
			}
			else if (file.Theta == (ThetaSecondIndex * 30) && file.Phi == (PhiIndex * 30))
			{
				if (vForceFluctuationSequence == file.ForceFluctuationSequence)
				{
					fileFrames.push_back(file);
				}
			}
		}
	}
	else
	{
		for (auto file : m_FilesData)
		{
			if (file.Theta == (ThetaIndex * 30) && file.Phi == (PhiIndex * 30))
			{
				if (vForceFluctuationSequence == file.ForceFluctuationSequence)
				{
					fileFrames.push_back(file);
				}
			}
			else if (file.Theta == (ThetaIndex * 30) && file.Phi == (PhiSecondIndex * 30))
			{
				if (vForceFluctuationSequence == file.ForceFluctuationSequence)
				{
					fileFrames.push_back(file);
				}
			}
			else if (file.Theta == (ThetaSecondIndex * 30) && file.Phi == (PhiIndex * 30))
			{
				if (vForceFluctuationSequence == file.ForceFluctuationSequence)
				{
					fileFrames.push_back(file);
				}
			}
			else if (file.Theta == (ThetaSecondIndex * 30) && file.Phi == (PhiSecondIndex * 30))
			{
				if (vForceFluctuationSequence == file.ForceFluctuationSequence)
				{
					fileFrames.push_back(file);
				}
			}
		}
	}
	return fileFrames;
}

void __InternalClassAddFilesData(std::string vFileIndex)
{

}