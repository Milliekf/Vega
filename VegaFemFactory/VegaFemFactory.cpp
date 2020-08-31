#include "stdafx.h"
#include "VegaFemFactory.h"

CVegaFemFactory::CVegaFemFactory(const std::string & vDirectoryName)
{
	readFilePath4Directory(vDirectoryName);
	setDeformationStateFromFileName();
}

void CVegaFemFactory::readFilePath4Directory(const std::string & vDirectoryName)
{
	intptr_t  hFile = 0;
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(vDirectoryName).append("\\*.txt").c_str(), &fileinfo)) != -1)
	{
		do
		{
			m_FileList.push_back(p.assignz(vDirectoryName).append("\\").append(fileinfo.name));

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
		m_FilesData.push_back(Common::SFileFrames(getFileName(m_FileList[i])));
		positionFile.close();
	}

}

void CVegaFemFactory::readFramesDeformationData(std::vector<Common::SFileFrames>& vSearchFrames)
{
	for (auto searchindex = 0; searchindex < vSearchFrames.size(); searchindex++)
	{
		for (auto fileIndex = 0; fileIndex < m_FileList.size(); fileIndex++)
		{
			if (vSearchFrames[searchindex].FileIndex == getFileName(m_FileList[fileIndex]))
			{
				int timeStepCount = 1;
				std::ifstream positionFile(m_FileList[fileIndex]);
				std::string lineString;
				char s[4096];
				double position[3];
				if (!positionFile.is_open())
				{
					std::cout << "Error: could not open vertex file" << m_FileList[fileIndex] << std::endl;
				}
				int Frameindex = 0;

				
				//m_FilesData.push_back(Common::SFileFrames(getFileName(m_FileList[i])));

				while (getline(positionFile, lineString))
				{
					sprintf(s, "Position%04d", timeStepCount);
					std::istringstream sin(lineString);
					std::string str;
					sin >> str;//Position%04d后面有空格
					sin.clear();
					if (str == s)
					{
						Common::SFileData tempFileData(Frameindex);
						getline(positionFile, lineString);
						std::istringstream isgroupsize(lineString);						
						isgroupsize >> str;
						int groupsize = atoi(str.c_str());
						
						for (int i = 0; i < groupsize; i++)
						{
							Common::SFileDataGroup tempGroupData(i);
							sprintf(s, "group%04d", i);
							getline(positionFile, lineString);
							std::istringstream isgroupid(lineString);							
							isgroupid >> str;
							if (str == s)
							{
								getline(positionFile, lineString);
								std::istringstream isgroupdatasize(lineString);
								std::string groupdatasize;
								isgroupdatasize >> groupdatasize;

								getline(positionFile, lineString);
								std::istringstream datasize(lineString);
								std::vector<double>frameArray;
								for (int j = 0; j < atoi(groupdatasize.c_str()); j++)
								{
									datasize >> position[0] >> position[1] >> position[2];
									tempGroupData.PositionsDeformation.push_back(glm::vec3(position[0], position[1], position[2]));
								}

							}
							tempFileData.FileDeformation.push_back(tempGroupData);
						}
						timeStepCount++;
						Frameindex++;
						vSearchFrames[searchindex].Frames.push_back(tempFileData);
						vSearchFrames[searchindex].isLoadDataSet = true;
					}
				}
			}
		}
		
	}
}

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