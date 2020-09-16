#pragma once
#include "BaseMesh.h"
#include "Mesh.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "common.h"
#include "Sence.h"

class CTreeInstanceMesh 
{
public:
	CTreeInstanceMesh() = default;
	CTreeInstanceMesh(CSence vModel, const Common::SFileFrames& vFileFrames);
	//CTreeInstanceMesh(int vVAO,)
	~CTreeInstanceMesh() = default;

	void RandomRotation();
	//void setDeformation(const std::vector<glm::vec3> vDeformationOfFrame) { m_Deformation = vDeformationOfFrame; }
	//void setDeformationFileFrames(const Common::SFileFrames vFileFrames) { m_FileFrames = vFileFrames; }
	int getSizeOfGroupsIndex();
	void __setupMesh();
	void draw(const CShader& vShader);
	//void draw(const CShader& vShader) const;

private:
	CSence m_Model;
	//u
	std::vector<std::vector<glm::vec3>> m_Deformations;
	//std::vector<glm::vec3> m_Deformation;
	//unsigned int m_VBOTree;
	//unsigned int m_VBODeformation;
	//unsigned int m_VBOGroupsIndex;
	unsigned int *m_VBOGroupIndex;
	glm::mat4* m_modelMatrices;

	//另一种形式的u
	Common::SFileFrames m_FileFrames;
	std::vector<std::vector<int>> m_GroupsIndex;
};

#pragma once
