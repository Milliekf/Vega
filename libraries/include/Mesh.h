#pragma once
#include "Shader.h"
#include <string>
#include "sceneObject.h"
#include "BaseMesh.h"

class CMesh:public CBaseMesh
{
public:
	CMesh() = default;
	CMesh(const std::vector<Common::SVertex>& vVertices, const std::vector<unsigned int>& vIndices = std::vector<unsigned int>(), const std::vector<Common::STexture> vTestures = std::vector<Common::STexture>());
	~CMesh() = default;

	std::vector<Common::SVertex> getVertices() const { return m_Vertices; }
	//unsigned int getVAO() const { return m_VAO; }
	std::vector<unsigned int> getIndices() const { _ASSERTE(m_Indices.size() > 0); return m_Indices; }
	std::vector<Common::STexture> getTextures() const { return m_Textures; }
	unsigned int getIndicesSize() const { return m_Indices.size(); }
	void draw(const CShader& vShader) const;
	void reset(const std::vector<Common::SVertex>& vVertices, const std::vector<unsigned int>& vIndices, const std::vector<Common::STexture> vTestures);
	void setRotation(glm::mat4* temp);
	void setGroupIndex(std::vector<int>& vGroupIndex);

private:
	unsigned int m_InstanceVBO;
	unsigned int m_GroupIndexVBO;
	
	void __setupMesh();
	void __setupInstanceMesh();
	void __setupGroupIndexMesh();
	void __setParameter(const std::vector<Common::SVertex>& vVertices, const std::vector<unsigned int>& vIndices, const std::vector<Common::STexture> vTestures);

	std::vector<Common::SVertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Common::STexture> m_Textures;
	std::vector<int> m_GroupIndex;
	glm::mat4* m_modelMatrices;
};