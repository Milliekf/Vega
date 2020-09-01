#include "ModelDeformationTransform.h"

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
				tempVertexIndex.push_back(face.getVertex(j).getPositionIndex() + 1);
			}
			m_Groups[i].Faces.push_back(BaseObjConstruct::SFace(tempVertexIndex));
		}
	}
}

void CModelDeformationTransform::ConvertVertex2mutileVerteices(Common::SFileData &vBaseFileDeformation)
{
	if (m_VerticesNumber != vBaseFileDeformation.BaseFileDeformations.size()) std::cout << "the DataSet not equal to baseFile" << std::endl;
	
	for (unsigned int i = 0; i < m_BaseFileMesh->getNumGroups(); i++)
	{
		Common::SFileDataGroup tempDataGroup(i);
		for (unsigned int faceid = 0; faceid < m_Groups[i].FaceSize; faceid++)
		{
			for (unsigned int vertexindex=0; vertexindex < 3; vertexindex++)
			{
				tempDataGroup.PositionsDeformation.push_back(vBaseFileDeformation.BaseFileDeformations[m_Groups[i].Faces[faceid].VertexIndex[vertexindex]]);
			}	
		}
		vBaseFileDeformation.FileDeformation.push_back(tempDataGroup);
	}
}

void CModelDeformationTransform::SaveDeformationVertexFromBaseModel(const double* u,const int vDeformationSize, std::string vSaveFileName, int vtimeStepCounter)
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