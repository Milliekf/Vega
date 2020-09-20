//#include "TreeInstanceMesh.h"
//
//CTreeInstanceMesh::CTreeInstanceMesh(CSence vModel)
//{
//	m_Model = vModel;
//	m_GroupsIndex = vModel.getGroupsIndex();
//	__setupMesh();
//}
//
////****************************************************************************************************
////FUNCTION:
//void CTreeInstanceMesh::__setupMesh()
//{
//	m_VBOGroupIndex = new unsigned int[m_GroupsIndex.size()];
//	for (int i = 0; i < m_GroupsIndex.size(); i++)
//	{
//		glGenBuffers(1, &(m_VBOGroupIndex[i]));
//		glBindBuffer(GL_ARRAY_BUFFER, m_VBOGroupIndex[i]);
//		glBufferData(GL_ARRAY_BUFFER, m_GroupsIndex[i].size() * sizeof(int), &(m_GroupsIndex[i][0]), GL_STATIC_DRAW);
//	}
//
//	int count = 0;
//	for (const auto& Mesh : m_Model.getMeshes())
//	{
//		//每一个mesh的形变量数据
//		/*glBindVertexArray(Mesh.getVAO());
//		glBindBuffer(GL_ARRAY_BUFFER, m_VBODeformation);
//		glEnableVertexAttribArray(3);
//		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);*/
//
//		//每一个mesh中的group索引已经设置好
//		glBindVertexArray(Mesh.getVAO());
//		glBindBuffer(GL_ARRAY_BUFFER, m_VBOGroupIndex[count]);
//		glEnableVertexAttribArray(3);
//		glVertexAttribIPointer(3, 1, GL_INT, sizeof(int), (void*)0);
//		count++;
//	}
//}
//
//void CTreeInstanceMesh::draw(const CShader& vShader)
//{
//	for (const auto& Mesh : m_Model.getMeshes())
//		Mesh.draw(vShader);
//}