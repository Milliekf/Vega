#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in int faceId;
layout (location = 4) in mat4 instanceMatrix;

out vec2 TexCoords;

uniform int frameNums;
uniform int vertexNums;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform int frameIndex;
uniform int treeIndex;

layout (std430, binding=1) buffer DeformationArray
{
	vec4 u[];
};

void main()
{
	vec4 tempPos=vec4(aPos,1.0)+u[treeIndex*frameNums*vertexNums+frameIndex*vertexNums+faceId];
//	vec4 tempPos;
//	if(gl_InstanceID<5)
//	{
//		tempPos=vec4(aPos,1.0)+u[gl_InstanceID*frameNums*vertexNums+frameIndex*vertexNums+faceId];
//	}
//	else
//	{
//		int tempInstanceIndex= gl_InstanceID / 100;
//		tempPos=vec4(aPos,1.0)+u[tempInstanceIndex*frameNums*vertexNums+frameIndex*vertexNums+faceId];
//	}
	gl_Position = projection * view * model * instanceMatrix * tempPos;
	TexCoords = aTexCoords;  
}
