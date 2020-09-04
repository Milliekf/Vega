#pragma once
//#include <glm/vec3.hpp>

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <io.h>
#include <assimp/mesh.h>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

namespace Common
{
	enum EFileFramesType
	{
		OneRelatedFile,
		TwoRelatedFiles,
		FourRelatedFiles
	};
	constexpr size_t NumOfBoundingBoxVertices = 8;
	static inline glm::vec3 vec3_cast(const aiVector3D &vec3) { return glm::vec3(vec3.x, vec3.y, vec3.z); }
	static inline glm::vec2 vec2_cast(const aiVector3D &vec2) { return glm::vec2(vec2.x, vec2.y); } // it's aiVector3D because assimp's texture coordinates use that
	/*static inline glm::quat quat_cast(const aiQuaternion &q) { return glm::quat(q.w, q.x, q.y, q.z); }
	static inline glm::mat4 mat4_cast(const aiMatrix4x4 &m) { return glm::transpose(glm::make_mat4(&m.a1)); }
	static inline glm::mat4 mat4_cast(const aiMatrix3x3 &m) { return glm::transpose(glm::make_mat3(&m.a1)); }*/
	//if this struct need not add normal or other info
	//Consider change this struct to glm::vec3
	static bool ModelHaveTangentAndBitangent = false;

	static bool InstanceTrees = true;

	static int TreesNumber = 25;

	static int MaxTimeStep = 60;

	struct SVertex
	{
		glm::vec3 Position;
		// normal
		glm::vec3 Normal;
		// texCoords
		glm::vec2 TexCoords;
		// tangent
		glm::vec3 Tangent;
		// bitangent
		glm::vec3 Bitangent;

		SVertex() = default;
		SVertex(const const glm::vec3& vPosition, const glm::vec3& vNormal, const glm::vec2& vTexCoords, const glm::vec3& vTangent, const glm::vec3& vBitangent)
		{
			Position = vPosition;
			Normal = vNormal;
			TexCoords = vTexCoords;
			Tangent = vTangent;
			Bitangent = vBitangent;
		}

	};

	struct SAssociatedMassPoint
	{
		int AssociatedMassPointIndex;
		float RestLength;
		float SpringConstant;
		float DampingConstant;
	};

	struct SMassPoint
	{
		SMassPoint() = default;
		SMassPoint(const int& vMassIndex, const SVertex& vVertex, const float& vMassValue, const glm::vec3& vVelocity, const std::vector<SAssociatedMassPoint> vAssociatedMassPoint)
		{
			MassPointIndex = vMassIndex;
			Vertex = vVertex;
			MassValue = vMassValue;
			Velocity = vVelocity;
			AssociatedMassPoint = vAssociatedMassPoint;
		}
		int MassPointIndex;
		SVertex Vertex;
		float MassValue;
		glm::vec3 Velocity;
		//�ö����滻λ��
		//glm::vec3 Gravity = glm::vec3(0.0, 0.0, 0.0);
		std::vector<SAssociatedMassPoint> AssociatedMassPoint;
	};

	//struct SDrawMassPoint
	//{
	//	glm::vec3 Vertex;

	//	glm::vec3 Velocity = glm::vec3(0.0, 0.0, 0.0);
	//};

	struct STexture {
		unsigned int id;
		std::string type;
		std::string path;
	};

	struct SCameraInfo
	{
		glm::vec3 Eye;
		glm::vec3 LookAt;
		glm::vec3 Up;

		SCameraInfo() = default;
		SCameraInfo(const glm::vec3& vEye, const glm::vec3& vLookAt, const glm::vec3& vUp)
		{
			Eye = vEye;
			LookAt = vLookAt;
			Up = vUp;
		}

		bool operator ==(const SCameraInfo& vCameraInfo) const
		{
			return vCameraInfo.Eye == this->Eye && vCameraInfo.LookAt == this->LookAt && vCameraInfo.Up == this->Up;
		}
	};

	struct SWindow
	{
		float BottomLeftX = FLT_MAX;
		float BottomLeftY = FLT_MAX;
		float Width = FLT_MAX;
		float Height = FLT_MAX;

		SWindow() = default;
		SWindow(float vBottomLeftX, float vBottomLeftY, float vWidth, float vHeight)
		{
			BottomLeftX = vBottomLeftX;
			BottomLeftY = vBottomLeftY;
			Width = vWidth;
			Height = vHeight;
		}

		bool isNormalizedWindow() const
		{
			return BottomLeftX >= 0.0f && BottomLeftX <= 1.0f && Width > 0.0f && BottomLeftX + Width <= 1.0f
				&& BottomLeftY >= 0.0f && BottomLeftY <= 1.0f && Height > 0.0f && BottomLeftY + Height <= 1.0f;
		}
	};

	const size_t NumOfAxis = 3;

	//ÿһ֡��Multi����
	struct SFileDataGroup
	{
		unsigned int GroupIndex;
		std::vector<glm::vec3> PositionsDeformation;
		SFileDataGroup(unsigned int vGroupIndex)
		{
			GroupIndex = vGroupIndex;
		}
		void clear() {
			PositionsDeformation.clear();
		}
	};

	//һ���ļ��е�ÿһ֡
	struct SFileData
	{
		unsigned int FrameIndex;
		std::vector<glm::vec3> BaseFileDeformations;
		std::vector<SFileDataGroup> FileDeformation;
		SFileData() = default;
		SFileData(unsigned int vFrameIndex)
		{
			FrameIndex = vFrameIndex;
		}
	};

	//ÿһ����һ���ļ�
	struct SFileFrames
	{
		std::string FileIndex;
		std::string FilePath;
		bool isLoadDataSet = false;
		int Theta;
		int Phi;
		std::vector<int> ForceFluctuationSequence;
		std::vector<SFileData> Frames;
		SFileFrames() = default;
		SFileFrames(std::string vIndex, std::string vFilePath)
		{
			FileIndex = vIndex;
			FilePath = vFilePath;
		}
	};
	struct SConnectedFemFiles
	{
		unsigned int ConnectedIndex;
		EFileFramesType Type;
		std::vector<SFileFrames*> FemDataset;
		SConnectedFemFiles() = default;
		SConnectedFemFiles(unsigned int vConnectedIndex)
		{
			ConnectedIndex = vConnectedIndex;
		}
	};

}