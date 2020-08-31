#include "ModelDeformationTransform.h"

CModelDeformationTransform::CModelDeformationTransform(std::string & vFileName)
{
	SceneObjectDeformable *m_BaseFileStruct = new SceneObjectDeformable(vFileName.c_str());
	m_BaseFileMesh = m_BaseFileStruct->GetMesh();
}

void CModelDeformationTransform::__VertexFaceRelated()
{
	m_VerticesNumber = m_BaseFileMesh->getNumVertices();
	for (unsigned int i = 0; i < m_BaseFileMesh->getNumGroups(); i++)
	{
		m_Groups.push_back(BaseObjConstruct::SGroup(m_BaseFileMesh->getGroup(i).getName(), m_BaseFileMesh->getGroup(i).getNumFaces()));
		glm::vec3 tempVertexIndex;
		for (unsigned int iFace = 0; iFace < m_BaseFileMesh->getGroup(i).getNumFaces(); iFace++)
		{
			ObjMesh::Face face = m_BaseFileMesh->getGroup(i).getFace(iFace);
			
			tempVertexIndex.x = face.getVertex(0).getPositionIndex() + 1;
			tempVertexIndex.y = face.getVertex(1).getPositionIndex() + 1;
			tempVertexIndex.z = face.getVertex(2).getPositionIndex() + 1;
			m_Groups[i].Faces.push_back(BaseObjConstruct::SFace(tempVertexIndex));
		}
	}
}