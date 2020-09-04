#include "cuda_runtime.h"
#include "device_launch_parameters.h"

// OpenGL Graphics includes
#include <gl/glew.h>
#include <gl/glut.h>

#include "cuda_gl_interop.h"

#include <math.h>

const int DIM = 1024;

//同一个缓冲区中的不同句柄，之所以需要两个独立的变量，是因为OPenGL和cuda对于这个缓冲区有着不同的名字
GLuint bufferObj;
cudaGraphicsResource * resource;

__global__ void kernel(uchar4 * ptr)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	int offset = x + y * blockDim.x * gridDim.x;

	float fx = x / (float)DIM - 0.5f;
	float fy = y / (float)DIM - 0.5f;
	unsigned char green = 128 + 127 * sin((fx * 100) - (fy * 100));

	ptr[offset].x = 0;
	ptr[offset].y = green;
	ptr[offset].z = 0;
	ptr[offset].w = 255;
}

int main()
{
	float vertices[] = {
		// positions          // texture coords
		 0.3f,  0.3f, 0.0f,   1.0f, 1.0f, // top right
		 0.3f, -0.3f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.3f, -0.3f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.3f,  0.3f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//将顶点数据设置到缓冲对象中去
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//GLuint vertexArray;
	cudaGLRegisterBufferObject(VBO);
	void * vertexPointer;
	// Map the buffer to CUDA
	cudaGLMapBufferObject(&ptr, vertexBuffer);
	// Run a kernel to create/manipulate the data
	MakeVerticiesKernel << <gridSz, blockSz >> > (ptr, numVerticies);
	// Unmap the buffer
	cudaGLUnmapbufferObject(vertexBuffer);

	//cudaMalloc((void**)&positions, sizeof(indices));
}
cudaDeviceProp prop;
int dev;
