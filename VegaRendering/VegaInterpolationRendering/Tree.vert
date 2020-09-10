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
	//float u[10];
	vec3 u[80000];
};

void main()
{
	//gl_Position = vec4(aPos+0.5,1.0);
	vec3 tempPos=aPos+vec3(u[5*77308+faceId].x,u[5*77308+faceId].y,u[5*77308+faceId].z);
	gl_Position=projection * view * model*vec4(tempPos,1.0);
	//gl_Position = vec4(aPos+u[frameIndex*77308+faceId],1.0);
	//gl_Position = projection * view * model *vec4(aPos+u[frameIndex*77308+faceId],1.0);
	TexCoords = aTexCoords;  
}
