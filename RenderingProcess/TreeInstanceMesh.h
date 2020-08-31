#pragma once
#include "BaseMesh.h"
#include "Mesh.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Common/common.h"

class CTreeInstanceMesh :public CBaseMesh
{
public:
	CTreeInstanceMesh() = default;
	CTreeInstanceMesh(CMesh & vmesh, const std::vector<glm::vec3> deformationFrames);
	//CTreeInstanceMesh(int vVAO,)
	~CTreeInstanceMesh() = default;

	void RandomRotation();
	void __setupMesh();
	void draw(const CShader& vShader) const;

private:
	CMesh m_Mesh;
	std::vector<std::vector<glm::vec3>> m_Deformations;
	std::vector<glm::vec3> m_Deformation;
	unsigned int m_VBOTree;
	unsigned int m_VBODeformation;
	glm::mat4* m_modelMatrices;
};

#pragma once
