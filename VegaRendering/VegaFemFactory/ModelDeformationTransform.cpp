#include "ModelDeformationTransform.h"
#include <chrono>

CModelDeformationTransform::CModelDeformationTransform(const std::string & vFileName)
{
	SceneObjectDeformable *m_BaseFileStruct = new SceneObjectDeformable(vFileName.c_str());
	m_BaseFileMesh = m_BaseFileStruct->GetMesh();
	__VertexFaceRelated();
}

void CModelDeformationTransform::__VertexFaceRelated()
{
	m_VerticesNumber = m_BaseFileMesh->getNumVertices();
	for (unsigned int i = 0; i < m_BaseFileMesh->getNumGroups(); i++)
	{
		m_Groups.push_back(BaseObjConstruct::SGroup(m_BaseFileMesh->getGroup(i).getName(), m_BaseFileMesh->getGroup(i).getNumFaces()));
		std::vector<int> tempVertexIndex;
		for (unsigned int iFace = 0; iFace < m_BaseFileMesh->getGroup(i).getNumFaces(); iFace++)
		{
			ObjMesh::Face face = m_BaseFileMesh->getGroup(i).getFace(iFace);

			for (int j = 0; j < 3; j++)
			{
				tempVertexIndex.push_back(face.getVertex(j).getPositionIndex());
			}
			m_Groups[i].Faces.push_back(BaseObjConstruct::SFace(tempVertexIndex));
			tempVertexIndex.clear();
		}
	}
	std::cout << "Finish Load Vega Vertex And Face Relationship" << std::endl;
}

void CModelDeformationTransform::ConvertVertex2mutileVerteices(Common::SFileData &vBaseFileDeformation)
{

	/*std::chrono::high_resolution_clock clock;
	auto start = clock.now();*/

	if (m_VerticesNumber != vBaseFileDeformation.BaseFileDeformations.size()) std::cout << "the DataSet not equal to baseFile" << std::endl;

	for (unsigned int i = 0; i < m_BaseFileMesh->getNumGroups(); i++)
	{
		Common::SFileDataGroup tempDataGroup(i);
		for (unsigned int faceid = 0; faceid < m_Groups[i].FaceSize; faceid++)
		{
			for (unsigned int vertexindex = 0; vertexindex < 3; vertexindex++)
			{
				tempDataGroup.PositionsDeformation.push_back(vBaseFileDeformation.BaseFileDeformations[m_Groups[i].Faces[faceid].VertexIndex[vertexindex]]);
			}
		}
		vBaseFileDeformation.FileDeformation.push_back(tempDataGroup);
	}

	//	std::vector<Common::SFileDataGroup> tempGroups;
	//	for (int i = 0; i < m_BaseFileMesh->getNumGroups(); i++)
	//	{
	//		Common::SFileDataGroup tempDataGroup(i);
	//		tempGroups.push_back(tempDataGroup);
	//	}
	//#pragma omp parallel for num_threads(6)// m_BaseFileMesh->getNumGroups())
	//	for (int i = 0; i < m_BaseFileMesh->getNumGroups(); i++)
	//	{
	//		//Common::SFileDataGroup tempDataGroup(i);
	//		for (unsigned int faceid = 0; faceid < m_Groups[i].FaceSize; faceid++)
	//		{
	//			for (unsigned int vertexindex=0; vertexindex < 3; vertexindex++)
	//			{
	//				/*tempDataGroup.PositionsDeformation.push_back(vBaseFileDeformation.BaseFileDeformations[m_Groups[i].Faces[faceid].VertexIndex[vertexindex]]);*/
	//				tempGroups[i].PositionsDeformation.push_back(vBaseFileDeformation.BaseFileDeformations[m_Groups[i].Faces[faceid].VertexIndex[vertexindex]]);
	//			}	
	//		}
	//		
	//	/*	std::cout << "Finish Product MutileVertices In Group: " << i << std::endl;
	//		std::cout << "this OpenMp threadId is : " << omp_get_thread_num() << std::endl;*/
	//	}
	//	for (int i = 0; i < m_BaseFileMesh->getNumGroups(); i++)
	//	{
	//		vBaseFileDeformation.FileDeformation.push_back(tempGroups[i]);
	//	}

		/*auto end = clock.now();

		std::cout << "Duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;*/
		//vBaseFileDeformation.FileDeformation.push_back(tempGroups);
}

void CModelDeformationTransform::SaveDeformationVertexFromBaseModel(const double* u, const int vDeformationSize, std::string vSaveFileName, int vtimeStepCounter)
{
	if (!vSaveFileName.empty())
	{
		char s[4096];
		FILE * file = fopen(vSaveFileName.c_str(), "a");
		if (!file)
		{
			printf("Can't open output file: %s.\n", s);
		}
		else
		{
			sprintf(s, "Position%04d", vtimeStepCounter);
			fprintf(file, "%s \n", s);
			sprintf(s, "%d", vDeformationSize);
			fprintf(file, "%s \n", s);
			for (unsigned int i = 0; i < vDeformationSize; i++)
			{
				fprintf(file, "%.10lf %.10lf %.10lf ", u[3 * i + 0], u[3 * i + 1], u[3 * i + 2]);
			}
			fprintf(file, "\n");
		}
		fclose(file);
	}
}