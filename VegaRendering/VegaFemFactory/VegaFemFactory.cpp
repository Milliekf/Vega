#include "VegaFemFactory.h"

CVegaFemFactory::CVegaFemFactory(const std::string & vDirectoryName, const std::string & vMutilVerticesBaseFile)
{
	//m_FilesData��ÿ��������ļ����;��·������
	readFilePath4Directory(vDirectoryName);
	//��������������ļ�����m_FilesDataÿ������ĽǶȺͲ������м���
	setDeformationStateFromFileName();
	//���Կ�����һ��obj��model������mesh���ϣ��Լ�group�鼯�ϣ�
	m_ModelTransformStruct = new CModelDeformationTransform(vMutilVerticesBaseFile);
}

//���ļ����������ļ��������SFileFrames�ļ����󣬵���δ�������ݣ�ֻ�ǽ��ļ���Ӧ�����ֺ;���·������
void CVegaFemFactory::readFilePath4Directory(const std::string & vDirectoryName)
{
	intptr_t  hFile = 0;
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(vDirectoryName).append("\\*.txt").c_str(), &fileinfo)) != -1)
	{
		do
		{
			m_FilePathList.push_back(p.assign(vDirectoryName).append("\\").append(fileinfo.name));

		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	assert(m_FilePathList.size() > 0);

	for (auto i = 0; i < m_FilePathList.size(); i++)
	{
		m_FilesData.push_back(Common::SFileFrames(getFileName(m_FilePathList[i]), m_FilePathList[i]));
	}
	std::cout << "Load Multiple SFileFrames File:" << m_FilePathList.size() << std::endl;

}


//������������SFileFrames��������
void CVegaFemFactory::readFramesDeformationData(std::vector<Common::SFileFrames>& vSearchFrames, int vSearchConnectionIndex)
{
	Common::SConnectedFemFiles tempConnectedFile(vSearchConnectionIndex);
	//��¼�����Ƕ��µ�λ����Ҫ����λ���ļ�����ֵ
	int counterConnectFileNumber = 0;
	//��������ָ���Ƕ�����ص�λ���ļ�����
	time_t current = time(NULL);
	for (auto searchindex = 0; searchindex < vSearchFrames.size(); searchindex++)
	{
		//forѭ��·�������е�λ���ļ�
		for (auto fileIndex = 0; fileIndex < m_FilesData.size(); fileIndex++)
		{
			if (vSearchFrames[searchindex].FileName == m_FilesData[fileIndex].FileName)
			{
				//readDeformationDataByMutileThread(m_FilesData[fileIndex], m_FilesData[fileIndex].FilePath, fileIndex);
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
				int stop = 0;
				while (getline(positionFile, lineString))
				{
					//getline(positionFile, lineString);
					sprintf(s, "Position%04d", timeStepCount);
					std::istringstream sin(lineString);
					std::string str;
					sin >> str;//Position%04d�����пո�
					//һ֡
					if (str == s)
					{
						std::string VertexSizeStr;
						getline(positionFile, VertexSizeStr);
						int VertexSize = atoi(VertexSizeStr.c_str());

						getline(positionFile, lineString);
						std::istringstream dataset(lineString);

						//һ��λ���ļ������е�һ֡����λ��
						Common::SFileData tempFileData(Frameindex);
						for (int j = 0; j < VertexSize; j++)
						{
							dataset >> position[0] >> position[1] >> position[2];
							tempFileData.BaseFileDeformations.push_back(glm::vec3(position[0], position[1], position[2]));
						}
						timeStepCount++;
						Frameindex++;
						m_FilesData[fileIndex].Frames.push_back(tempFileData);
					}
					//stop++;
				}
				m_FilesData[fileIndex].isLoadDataSet = true;
				tempConnectedFile.FemDataset.push_back(&m_FilesData[fileIndex]);
				counterConnectFileNumber++;
			}
		}
	}
	time_t last = time(NULL);
	std::cout<<"time is"<<last-current<<std::endl;
	if (counterConnectFileNumber == 1) tempConnectedFile.Type = Common::EFileFramesType::OneRelatedFile;
	else if (counterConnectFileNumber == 2) tempConnectedFile.Type = Common::EFileFramesType::TwoRelatedFiles;
	else if (counterConnectFileNumber == 4) tempConnectedFile.Type = Common::EFileFramesType::FourRelatedFiles;

	m_AllReallyLoadConnectedFem.push_back(tempConnectedFile);

	std::cout << "Finish Load Search FileData To ConnectedFileFrames" << std::endl;
}

//���̶߳�ȡһ���ļ�
//�ڶ��������Ѿ����صĽṹ��ĵڼ���FileFrames
void CVegaFemFactory::readDeformationDataByMutileThread(Common::SFileFrames & vBaseFileFramesStruct, const std::string & vFilePath, int vSFileFramesIndex)
{
	std::vector<long long> BlockSizes;
	__getFileSeekDirOfEachBlock(vFilePath, BlockSizes);

	std::vector<Common::SFileData> Frames;
	for (int i = 0; i < Common::readFrames; i++)
	{
		Common::SFileData tempFileData(i);
		Frames.push_back(tempFileData);
	}

	//int i = 2;
	//readDeformationDataInOneThread(Frames[i], i, vFilePath, BlockSize);
	std::vector<std::thread> ReadFrameThreads;
	for (int i = 0; i < Common::readFrames; i++)
	{
		Common::SFileData& temp = Frames[i];
		std::thread th(&CVegaFemFactory::readDeformationDataInOneThread, this, std::ref(Frames[i]), i, vFilePath, BlockSizes[i]);
		ReadFrameThreads.emplace_back(std::move(th));
	}

	auto it = ReadFrameThreads.begin();
	for (; it != ReadFrameThreads.end(); ++it)
		(*it).join();

	std::this_thread::sleep_for(std::chrono::seconds(3));
	vBaseFileFramesStruct.Frames = Frames;
}

void CVegaFemFactory::readDeformationDataInOneThread(Common::SFileData & vFileData, int vTid, const std::string & vFilePath, long long vBlockSize)
{
	std::ifstream positionFile(vFilePath.c_str(), std::ios::in);
	if (!positionFile.good())
	{
		std::cout << "Thread :" << vTid << "failed to Open" << std::endl;
		return;
	}
	std::cout << "vTid" << vTid * vBlockSize << std::endl;
	positionFile.seekg(vBlockSize);
	char PositionIndex[4096];
	double position[3];
	std::string lineString;

	sprintf(PositionIndex, "Position%04d", vTid + 1);
	getline(positionFile, lineString);
	std::istringstream sin(lineString);
	std::string FilePositonIndex;
	sin >> FilePositonIndex;
	if (FilePositonIndex == PositionIndex)
	{
		std::cout << FilePositonIndex << std::endl;
		std::string VertexSizeStr;
		getline(positionFile, VertexSizeStr);
		int VertexSize = atoi(VertexSizeStr.c_str());

		getline(positionFile, lineString);
		std::istringstream Dataset(lineString);

		for (int j = 0; j < VertexSize; j++)
		{
			Dataset >> position[0] >> position[1] >> position[2];
			vFileData.BaseFileDeformations.push_back(glm::vec3(position[0], position[1], position[2]));
		}
	}
}

void CVegaFemFactory::addSeekgOfEachFramesBlock(const std::string & vFilePath)
{
	const size_t last_slash_idx = vFilePath.rfind('.txt');
	std::string FramesBlockFileName = vFilePath.substr(0, last_slash_idx - 3);
	FramesBlockFileName = FramesBlockFileName + "Block.block";
	std::ifstream positionFile(vFilePath.c_str(), std::ios::in);
	if (!positionFile.good())
	{
		std::cout << "failed to Open File" << vFilePath << "Can't get File Para" << std::endl;
		return;
	}
	std::vector<long long>tempBlockSizes;
	tempBlockSizes.push_back(0);
	std::string lineString;
	while (getline(positionFile, lineString))
	{
		getline(positionFile, lineString);
		getline(positionFile, lineString);
		long long BlockSize = positionFile.tellg();
		tempBlockSizes.push_back(BlockSize);
	}
	positionFile.close();

	std::ofstream writePositionFile(FramesBlockFileName, std::ios::out);
	for (auto i = 0; i < tempBlockSizes.size(); i++)
	{
		writePositionFile << tempBlockSizes[i] << " ";
	}
	writePositionFile << std::endl;
	writePositionFile.close();
}

//�����Ѿ������Ķ��SFileFrames���󣬸���ÿ��������ļ������������Ӧ�������Ƕ��Լ����Ĳ������У����ж������䣬��ʱ����û�ж���ÿ֡��λ������
void CVegaFemFactory::setDeformationStateFromFileName()
{
	int thetaPos;
	int phiPos;
	int forcePos;
	for (auto i = 0; i < m_FilesData.size(); i++)
	{
		std::vector<std::string> ForceSequence;
		thetaPos = m_FilesData[i].FileName.find("the");
		phiPos = m_FilesData[i].FileName.find("phi");
		forcePos = m_FilesData[i].FileName.find("force");
		m_FilesData[i].Theta = std::stoi(m_FilesData[i].FileName.substr(thetaPos + 3, phiPos - thetaPos - 3));
		m_FilesData[i].Phi = std::stoi(m_FilesData[i].FileName.substr(phiPos + 3, forcePos - phiPos - 3));
		//std::string tempsequence="500,500,500"
		//std::string tempsequence = m_FilesData[i].FileIndex.substr(forcePos + 5);
		//std::vector<std::string> ForceSequence={500,500,500}
		boost::split(ForceSequence, m_FilesData[i].FileName.substr(forcePos + 5), boost::is_any_of(","), boost::token_compress_off);
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

//��ģ�͵�ԭʼ�α���������
//����ֵ���µı����Ƿ���������ã��о����У���ֵ���Ĳ�����SFileData���ҵ���
//vConnectionIndex�����˵�i������vTimestep�����˵ڼ�֡
std::vector<Common::SFileDataGroup> CVegaFemFactory::getConnectedFemMutileDeformation(int vConnectionIndex, int vTimestep)
{
	//���������Ѿ��洢�����Connection��,�ó�ÿһ����λ������Ҫ�����λ���ļ�
	for (int connectIndex = 0; connectIndex < m_AllReallyLoadConnectedFem.size(); connectIndex++)
	{
		//�ҵ��˵�vConnectionIndex����Ҫ�Ĳ�ֵλ���ļ�
		if (vConnectionIndex == m_AllReallyLoadConnectedFem[connectIndex].ConnectedIndex)
		{
			//������ݶ���ֵ�ļ����Ǳ���������Ҫͨ�������ļ�����ֵ
			if (m_AllReallyLoadConnectedFem[connectIndex].Type == Common::EFileFramesType::OneRelatedFile)
			{
				//��һ��λ���ļ��е�����֡������ѭ��
				for (int time = 0; time < (m_AllReallyLoadConnectedFem[connectIndex].FemDataset[0])->Frames.size(); time++)
				{
					//�ҵ���ǰҪ���Ƶ�֡��
					if ((m_AllReallyLoadConnectedFem[connectIndex].FemDataset[0])->Frames[time].FrameIndex == vTimestep)
					{
						//����ģ�͵�����
						m_ModelTransformStruct->ConvertVertex2mutileVerteices((m_AllReallyLoadConnectedFem[connectIndex].FemDataset[0])->Frames[time]);

						return (m_AllReallyLoadConnectedFem[connectIndex].FemDataset[0])->Frames[time].FileDeformation;

					}
				}
				std::cout << "Finish Load Multile Vertex" << std::endl;
			}
		}


	}
}

void CVegaFemFactory::cleanSFileDataGroup(int vConnectionIndex, int vTimestep)
{
	for (int connectIndex = 0; connectIndex < m_AllReallyLoadConnectedFem.size(); connectIndex++)
	{
		if (vConnectionIndex == m_AllReallyLoadConnectedFem[connectIndex].ConnectedIndex)
		{
			for (int time = 0; time < (m_AllReallyLoadConnectedFem[connectIndex].FemDataset[0])->Frames.size(); time++)
			{
				if ((m_AllReallyLoadConnectedFem[connectIndex].FemDataset[0])->Frames[time].FrameIndex == vTimestep)
				{
					(m_AllReallyLoadConnectedFem[connectIndex].FemDataset[0])->Frames[time].FileDeformation.clear();
				}
			}
		}
	}
}

void CVegaFemFactory::__getFileSeekDirOfEachBlock(const std::string & vFilePath, std::vector<long long>& vBlock)
{
	const size_t last_slash_idx = vFilePath.rfind('.txt');
	std::string FramesBlockFileName = vFilePath.substr(0, last_slash_idx - 3);
	FramesBlockFileName = FramesBlockFileName + "Block.block";

	std::ifstream positionFile(FramesBlockFileName.c_str(), std::ios::in);
	if (!positionFile.good())
	{
		std::cout << "failed to Open File" << vFilePath << "Can't get File Para" << std::endl;
		addSeekgOfEachFramesBlock(vFilePath);
		std::cout << "create Data Block" << std::endl;
	}
	positionFile.close();

	std::ifstream preparepositionFile(FramesBlockFileName.c_str(), std::ios::in);
	std::string lineString;
	getline(preparepositionFile, lineString);
	std::istringstream Dataset(lineString);
	for (int i = 0; i < Common::readFrames; i++)
	{
		long long tempData;
		Dataset >> tempData;
		vBlock.push_back(tempData);
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
	//Ȩ������,˫���Բ�ֵ
	/*std::string FileIndex=*/
	//����һ��ģ�͵�λ������������ת�������嶥�������λ������
	return u;
}

//���Ѿ�����FileIndex����δ����λ�����ݵ�SFileFrames����������ص�SFileFrames������
//�����ٽ��Ķ����������������ʶ
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