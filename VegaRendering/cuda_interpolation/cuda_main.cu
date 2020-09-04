#include "cuda_runtime.h"  
#include "cublas_v2.h"  

#include <time.h>  
#include <iostream>  

using namespace std;

// ������Ծ����ά��  
int const M = 5;
int const N = 10;

int main()
{
	// ����״̬����  
	cublasStatus_t status;

	// �� �ڴ� ��Ϊ��Ҫ����ľ��󿪱ٿռ�  
	float *h_A = (float*)malloc(N*M * sizeof(float));
	float *h_B = (float*)malloc(N*M * sizeof(float));

	// �� �ڴ� ��Ϊ��Ҫ����������ľ��󿪱ٿռ�  
	float *h_C = (float*)malloc(M*M * sizeof(float));

	// Ϊ����������Ԫ�ظ��� 0-10 ��Χ�ڵ������  
	for (int i = 0; i < N*M; i++) {
		h_A[i] = (float)(rand() % 10 + 1);
		h_B[i] = (float)(rand() % 10 + 1);

	}

	// ��ӡ�����Եľ���  
	cout << "���� A :" << endl;
	for (int i = 0; i < N*M; i++) {
		cout << h_A[i] << " ";
		if ((i + 1) % N == 0) cout << endl;
	}
	cout << endl;
	cout << "���� B :" << endl;
	for (int i = 0; i < N*M; i++) {
		cout << h_B[i] << " ";
		if ((i + 1) % M == 0) cout << endl;
	}
	cout << endl;

	/*
	** GPU ����������
	*/

	// ��������ʼ�� CUBLAS �����  
	cublasHandle_t handle;
	status = cublasCreate(&handle);

	if (status != CUBLAS_STATUS_SUCCESS)
	{
		if (status == CUBLAS_STATUS_NOT_INITIALIZED) {
			cout << "CUBLAS ����ʵ��������" << endl;
		}
		getchar();
		return EXIT_FAILURE;
	}

	float *d_A, *d_B, *d_C;
	// �� �Դ� ��Ϊ��Ҫ����ľ��󿪱ٿռ�  
	cudaMalloc(
		(void**)&d_A,    // ָ�򿪱ٵĿռ��ָ��  
		N*M * sizeof(float)    //����Ҫ���ٿռ���ֽ���  
	);
	cudaMalloc(
		(void**)&d_B,
		N*M * sizeof(float)
	);

	// �� �Դ� ��Ϊ��Ҫ����������ľ��󿪱ٿռ�  
	cudaMalloc(
		(void**)&d_C,
		M*M * sizeof(float)
	);

	// ���������ݴ��ݽ� �Դ� ���Ѿ����ٺ��˵Ŀռ�  
	cublasSetVector(
		N*M,    // Ҫ�����Դ��Ԫ�ظ���  
		sizeof(float),    // ÿ��Ԫ�ش�С  
		h_A,    // ��������ʼ��ַ  
		1,    // ����Ԫ��֮��Ĵ洢���  
		d_A,    // GPU ����ʼ��ַ  
		1    // ����Ԫ��֮��Ĵ洢���  
	);
	cublasSetVector(
		N*M,
		sizeof(float),
		h_B,
		1,
		d_B,
		1
	);

	// ͬ������  
	cudaThreadSynchronize();

	// ���ݽ�������˺����еĲ��������庬����ο������ֲᡣ  
	float a = 1; float b = 0;
	// ������ˡ��ú�����Ȼ���������������������  
	cublasSgemm(
		handle,    // blas �����   
		CUBLAS_OP_T,    // ���� A ���Բ���  
		CUBLAS_OP_T,    // ���� B ���Բ���  
		M,    // A, C ������   
		M,    // B, C ������  
		N,    // A �������� B ������  
		&a,    // ����ʽ�� �� ֵ  
		d_A,    // A ���Դ��еĵ�ַ  
		N,    // lda  
		d_B,    // B ���Դ��еĵ�ַ  
		M,    // ldb  
		&b,    // ����ʽ�� �� ֵ  
		d_C,    // C ���Դ��еĵ�ַ(�������)  
		M    // ldc  
	);

	// ͬ������  
	cudaThreadSynchronize();

	// �� �Դ� ��ȡ���������� �ڴ���ȥ  
	cublasGetVector(
		M*M,    //  Ҫȡ��Ԫ�صĸ���  
		sizeof(float),    // ÿ��Ԫ�ش�С  
		d_C,    // GPU ����ʼ��ַ  
		1,    // ����Ԫ��֮��Ĵ洢���  
		h_C,    // ��������ʼ��ַ  
		1    // ����Ԫ��֮��Ĵ洢���  
	);

	// ��ӡ������  
	cout << "��������ת�� ( (A*B)��ת�� )��" << endl;

	for (int i = 0; i < M*M; i++) {
		cout << h_C[i] << " ";
		if ((i + 1) % M == 0) cout << endl;
	}

	// �����ʹ�ù����ڴ�  
	free(h_A);
	free(h_B);
	free(h_C);
	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);

	// �ͷ� CUBLAS �����  
	cublasDestroy(handle);

	getchar();

	return 0;
}