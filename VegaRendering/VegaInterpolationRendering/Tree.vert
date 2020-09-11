#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in int faceId;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int frameIndex;

layout (std430, binding=1) buffer DeformationArray
{
	vec4 u[];
};

void main()
{
	vec4 tempPos=vec4(aPos,1.0)+u[frameIndex*77380+faceId];
	gl_Position = projection*view*model*tempPos;
	TexCoords = aTexCoords;  
}
