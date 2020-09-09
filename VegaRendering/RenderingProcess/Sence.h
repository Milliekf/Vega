#pragma once
#include <string.h>
#include <stdio.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "Mesh.h"
//#include "TreeInstanceMesh.h"
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Common/stb_image.h"
#include "../Common/common.h"
//#include "TreeInstanceMesh.h"
#include "../VegaFemFactory/VegaFemFactory.h"
#include "../VegaFemFactory/ObjStruct.h"
#include "objMesh.h"
#include "sceneObject.h"
#include "objMeshRender.h"

class CVegaFemFactory;

class CSence
{
public:
	CSence() = default;
	CSence(const std::string& vModelPath, bool vGamma = false, bool vloadNormalizeModelAndFaceNormal = false) :gammaCorrection(vGamma) { __loadModel(vModelPath, vloadNormalizeModelAndFaceNormal); };
	CSence(const ObjMesh * vobjMesh, double * vRestVertexs) : m_Mesh(vobjMesh), m_RestVertexs(vRestVertexs) { __changeObjMeshStruct2Charptr(1); __loadModelFromMemory(); };
	CSence(const CMesh& vMesh);
	~CSence() = default;

	std::vector<std::vector<glm::vec3>>& getGroupDeformationData() { return m_EachFrameOfGroupData; };
	void SetParaMesh();
	void setGroupsIndex(CVegaFemFactory& vfemFactoryObject);
	void setVerticesNumber(CVegaFemFactory& vfemFactoryObject);
	std::vector<std::vector<int>> getGroupsIndex() { return m_GroupsIndex; }
	std::vector<CMesh> getMeshes() { return m_Meshes; }
	void senceDraw(const CShader& vShader, std::vector<std::vector<glm::vec3>> deformationFrames);
	void setFileDirectiory(std::string vFileDirectory) { m_FileDirectory = vFileDirectory; }
	void draw(const CShader& vShader, bool instance);
	/*CTreeInstanceMesh addMeshDeformationForTree(std::vector<Common::SFileDataGroup> deformationFrames);
	CTreeInstanceMesh addDeformationData(CMesh vMesh, std::vector<glm::vec3> deformationFrames);*/
	/*std::vector<CMesh> getMeshes() const { return m_Meshes; }
	void setFileDirectiory(std::string vFileDirectory) { m_FileDirectory = vFileDirectory; }
	CTreeInstanceMesh addDeformationData(CMesh vMesh, std::vector<glm::vec3> deformationFrames);
	void draw(const CShader& vShader, bool instance, std::vector<std::vector<glm::vec3>> deformationFrames);*/
	bool gammaCorrection;

private:
	void __changeObjMeshStruct2Charptr(int vOutputMaterials=1);
	void __loadModel(const std::string& vModelPath, bool vloadNormalizeModel);
	void __loadModelFromMemory();
	void __processNode(const aiNode* vNode, const aiScene* vScene,bool vSaveDeformationOrLoadData);
	CMesh __processMesh(const aiMesh* vMesh, const aiScene* vScene);
	void __processSaveDeformation(const aiMesh*vMesh, const aiScene*vScene);
	std::vector<Common::STexture> loadMaterialTextures(aiMaterial *vMat, aiTextureType vType, std::string vtypeName);
	unsigned int TextureFromFile(const char *vPath, const std::string &vDirectory, bool vGamma = false);

	const ObjMesh * m_Mesh;
	std::vector<CMesh> m_Meshes;
	double *m_RestVertexs;
	std::vector<Common::STexture> m_Textures;
	std::string m_FileDirectory;
	double alphaBlendingThreshold;
	std::map<int, std::set<int>> hiddenFaces;
	std::string m_Data;
	const char* m_DataPtr = nullptr;
	unsigned int m_DataSize;
	std::vector<std::vector<glm::vec3>> m_EachFrameOfGroupData;
	//所有的group中面的顶点索引
	std::vector<std::vector<int>> m_GroupsIndex;
	int m_VerticesNumber;
};
