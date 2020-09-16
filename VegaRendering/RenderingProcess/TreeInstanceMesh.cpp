#include "TreeInstanceMesh.h"

CTreeInstanceMesh::CTreeInstanceMesh(CSence vModel, const Common::SFileFrames& vFileFrames)
{
	m_Model = vModel;
	m_FileFrames = vFileFrames;
	for (int i = 0; i < vFileFrames.Frames.size(); i++)
	{
		Common::SFileData frame = vFileFrames.Frames[i];
		m_Deformations.push_back(frame.BaseFileDeformations);
	}
	m_GroupsIndex = vModel.getGroupsIndex();
	__setupMesh();
}

//void CTreeInstanceMesh::draw(const CShader& vShader) const
//{
//	// bind appropriate textures
//	unsigned int diffuseNr = 1;
//	unsigned int specularNr = 1;
//	unsigned int normalNr = 1;
//	unsigned int heightNr = 1;
//	for (unsigned int i = 0; i < m_Mesh.getTextures().size(); i++)
//	{
//		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
//		// retrieve texture number (the N in diffuse_textureN)
//		std::string Number;
//		std::string Name = m_Mesh.getTextures()[i].type;
//		if (Name == "texture_diffuse")
//			Number = std::to_string(diffuseNr++);
//		else if (Name == "texture_specular")
//			Number = std::to_string(specularNr++); // transfer unsigned int to stream
//		else if (Name == "texture_normal")
//			Number = std::to_string(normalNr++); // transfer unsigned int to stream
//		else if (Name == "texture_height")
//			Number = std::to_string(heightNr++); // transfer unsigned int to stream
//
//												 // now set the sampler to the correct texture unit
//
//		auto temp = glGetUniformLocation(vShader.getID(), (Name + Number).c_str());
//		glUniform1i(-1, i);
//		// and finally bind the texture
//		glBindTexture(GL_TEXTURE_2D, m_Mesh.getTextures()[i].id);
//	}
//	// draw mesh
//	glBindVertexArray(m_Mesh.getVAO());
//	glDrawElements(GL_TRIANGLES, m_Mesh.getIndices().size(), GL_UNSIGNED_INT, 0);
//	//glDrawElementsInstanced(GL_TRIANGLES, m_Mesh.getIndices().size(), GL_UNSIGNED_INT, 0, Common::TreesNumber);
//	glBindVertexArray(0);
//
//	// always good practice to set everything back to defaults once configured.
//	glActiveTexture(GL_TEXTURE0);
//}

int CTreeInstanceMesh::getSizeOfGroupsIndex()
{
	int tempCount = 0;
	for (int i = 0; i < m_GroupsIndex.size(); i++)
	{
		for (int k = 0; k < m_GroupsIndex[i].size(); k++)
		{
			tempCount++;
		}
	}
	return tempCount;
}

//****************************************************************************************************
//FUNCTION:
void CTreeInstanceMesh::__setupMesh()
{
	//glGenVertexArrays(1, &m_VAO);
	//glGenBuffers(1, &m_VBO);
	//glGenBuffers(1, &m_EBO);

	//glBindVertexArray(m_VAO);
	////// load data into vertex buffers
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	////// A great thing about structs is that their memory layout is sequential for all its items.
	////// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	////// again translates to 3/2 floats which translates to a byte array.
	//glBufferData(GL_ARRAY_BUFFER, m_Mesh.getVertices().size() * sizeof(Common::SVertex), &m_Mesh.getVertices()[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Mesh.getIndices().size() * sizeof(unsigned int), &m_Mesh.getIndices()[0], GL_STATIC_DRAW);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Common::SVertex), (void*)0);
	//// vertex normals
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Common::SVertex), (void*)offsetof(Common::SVertex, Normal));
	//// vertex texture coords
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Common::SVertex), (void*)offsetof(Common::SVertex, TexCoords));

	/*glGenBuffers(1, &m_VBODeformation);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBODeformation);
	glBufferData(GL_ARRAY_BUFFER, m_Deformation.size() * sizeof(glm::vec3), &m_Deformation[0], GL_STATIC_DRAW);
	unsigned int VAO = m_Mesh.getVAO();
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);*/

	/*glGenBuffers(1, &m_VBODeformation);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBODeformation);
	glBufferData(GL_ARRAY_BUFFER, m_Deformations[0].size()*m_Deformations.size() * sizeof(glm::vec3), &(m_Deformations[0][0]), GL_STATIC_DRAW);*/
	
	//glGenBuffers(1, &m_VBOGroupsIndex);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBOGroupsIndex);
	//glBufferData(GL_ARRAY_BUFFER, getSizeOfGroupsIndex() * sizeof(int), &(m_GroupsIndex[0][0]), GL_STATIC_DRAW);

	//for(int i=0;i<m_Model.get)

	m_VBOGroupIndex = new unsigned int[m_GroupsIndex.size()];
	for (int i = 0; i < m_GroupsIndex.size(); i++)
	{
		glGenBuffers(1, &(m_VBOGroupIndex[i]));
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOGroupIndex[i]);
		glBufferData(GL_ARRAY_BUFFER, m_GroupsIndex[i].size() * sizeof(int), &(m_GroupsIndex[i][0]), GL_STATIC_DRAW);
	}

	int count = 0;
	for (const auto& Mesh : m_Model.getMeshes())
	{
		//每一个mesh的形变量数据
		/*glBindVertexArray(Mesh.getVAO());
		glBindBuffer(GL_ARRAY_BUFFER, m_VBODeformation);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);*/

		//每一个mesh中的group索引已经设置好
		glBindVertexArray(Mesh.getVAO());
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOGroupIndex[count]);
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 1, GL_INT, sizeof(int), (void*)0);
		count++;
	}
	/*unsigned int VAO = m_Mesh.getVAO();
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);*/


	//RandomRotation();
	//glGenBuffers(1, &m_VBOTree);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBOTree);
	//glBufferData(GL_ARRAY_BUFFER, Common::TreesNumber * sizeof(glm::mat4), &m_modelMatrices[0], GL_STATIC_DRAW);
	//unsigned int VAO = m_Mesh.getVAO();
	//glBindVertexArray(VAO);
	//	// set attribute pointers for matrix (4 times vec4)
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	//glEnableVertexAttribArray(5);
	//glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	//glEnableVertexAttribArray(6);
	//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	//glVertexAttribDivisor(3, 1);
	//glVertexAttribDivisor(4, 1);
	//glVertexAttribDivisor(5, 1);
	//glVertexAttribDivisor(6, 1);

	//glBindVertexArray(0);

}

void CTreeInstanceMesh::draw(const CShader& vShader)
{
	for (const auto& Mesh : m_Model.getMeshes())
		Mesh.draw(vShader);
}

void CTreeInstanceMesh::RandomRotation()
{
	m_modelMatrices = new glm::mat4[Common::TreesNumber];

	float x = -1.75f;
	glm::mat4 model = glm::mat4(1.0f);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			model = glm::translate(model, glm::vec3(i, -1.75f, j));
			m_modelMatrices[j * 5 + i] = model;
		}
		// translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	}
}