//#version 430 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoords;
//layout (location = 3) in int faceId;
//
//out vec2 TexCoords;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//uniform int frameIndex;
//
//layout (std430, binding=2) buffer DeformationArray
//{
//	float u[2];
//};
//
//void main()
//{
//    TexCoords = aTexCoords;   
//    //gl_Position = projection * view * model * vec4(vec3(aPos.x+u[0],aPos.y,aPos.z), 1.0);
//	gl_Position = vec4(vec3(aPos.x+u[0],aPos.y],aPos.z), 1.0);
//}

#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

layout (std430) buffer DeformationArray
{
	float u[2];
};

out vec3 ourColor;

void main()
{
   gl_Position = vec4(vec3(aPos.x+u[0],aPos.y+u[1],aPos.z), 1.0);
   //gl_Position = vec4(aPos,1.0);
    ourColor = aColor;
}