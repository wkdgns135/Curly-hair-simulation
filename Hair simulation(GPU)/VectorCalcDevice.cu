#pragma once
#include "VectorCalcdevice.cuh"
#include "math_functions.h"
#include "vector_functions.h"

__global__ void array_init(float3 *arr) {
	int tid = blockIdx.x * blockDim.x + threadIdx.x;
	arr[tid] = make_float3(0.0, 0.0, 0.0);
}

__device__ void array_copy_k(float3 *a, float3 *b) {
	for (int i = 0; i < sizeof(a) / sizeof(float3); i++) {
		a[i].x = b[i].x;
		a[i].y = b[i].y;
		a[i].z = b[i].z;
	}
}

__device__ __host__  float3 vector_multiply_k(float3 a, float3 b) {
	float3 tmp;
	tmp.x = a.x * b.x;
	tmp.y = a.y * b.y;
	tmp.z = a.z * b.z;
	return tmp;
}


__device__ __host__  float3 vector_multiply_k(float3 a, double b) {
	float3 tmp;
	tmp.x = a.x * b;
	tmp.y = a.y * b;
	tmp.z = a.z * b;

	return tmp;
}

__device__ __host__  float3 vector_add_k(float3 a, float3 b) {
	float3 tmp;
	tmp.x = a.x + b.x;
	tmp.y = a.y + b.y;
	tmp.z = a.z + b.z;

	return tmp;
}


__device__ __host__ float3 vector_add_k(float3 a, double b) {
	float3 tmp;
	tmp.x = a.x + b;
	tmp.y = a.y + b;
	tmp.z = a.z + b;

	return tmp;
}

__device__ __host__ float3 vector_sub_k(float3 a, float3 b) {
	float3 tmp;
	tmp.x = a.x - b.x;
	tmp.y = a.y - b.y;
	tmp.z = a.z - b.z;

	return tmp;
}

__device__ __host__ float3 vector_sub_k(float3 a, double b) {
	float3 tmp;
	tmp.x = a.x - b;
	tmp.y = a.y - b;
	tmp.z = a.z - b;

	return tmp;
}

__device__ __host__ float3 operator + (float3 a, float3 &b) {
	float3 tmp;
	tmp.x = a.x + b.x;
	tmp.y = a.y + b.y;
	tmp.z = a.z + b.z;

	return tmp;
}

__device__ __host__ float3 operator + (float3 a, float &b) {
	float3 tmp;
	tmp.x = a.x + b;
	tmp.y = a.y + b;
	tmp.z = a.z + b;

	return tmp;
}

__device__ __host__ float3 operator - (float3 a, float3 &b) {
	float3 tmp;
	tmp.x = a.x - b.x;
	tmp.y = a.y - b.y;
	tmp.z = a.z - b.z;

	return tmp;
}

__device__ __host__ float3 operator - (float3 a, float &b) {
	float3 tmp;
	tmp.x = a.x - b;
	tmp.y = a.y - b;
	tmp.z = a.z - b;

	return tmp;
}
__device__ __host__ float3 operator * (float3 a, float3 &b) {
	float3 tmp;
	tmp.x = a.x * b.x;
	tmp.y = a.y * b.y;
	tmp.z = a.z * b.z;

	return tmp;
}

__device__ __host__ float3 operator * (float3 a, float &b) {
	float3 tmp;
	tmp.x = a.x * b;
	tmp.y = a.y * b;
	tmp.z = a.z * b;

	return tmp;
}

__device__ __host__ float3 operator / (float3 a, float3 &b) {
	float3 tmp;
	tmp.x = a.x / b.x;
	tmp.y = a.y / b.y;
	tmp.z = a.z / b.z;

	return tmp;
}

__device__ __host__ float3 operator / (float3 a, float &b) {
	float3 tmp;
	tmp.x = a.x / b;
	tmp.y = a.y / b;
	tmp.z = a.z / b;

	return tmp;
}

__device__ __host__ int3 operator + (int3 a, int3 b) {
	int3 tmp;
	tmp.x = a.x + b.x;
	tmp.y = a.y + b.y;
	tmp.z = a.z + b.z;
	return tmp;
}


__device__ __host__ double vector_length_k(float3 a) {
	return sqrt(float(a.x*a.x + a.y * a.y + a.z * a.z));
}

__device__ __host__ float3 vector_normalized_k(float3 a) {
	float3 tmp;
	double norm = vector_length_k(a);
	if (norm != 0) {
		tmp.x = a.x / norm;
		tmp.y = a.y / norm;
		tmp.z = a.z / norm;
	}
	return tmp;
}


__device__ __host__  float3	vector_cross_k(float3 a, float3 b) {
	float3 tmp;
	tmp.x = ((a.y*b.z) - (a.z*b.y));
	tmp.y = ((a.z*b.x) - (a.x*b.z));
	tmp.z = ((a.x*b.y) - (a.y*b.x));
	return tmp;
}

__device__ __host__ double vector_dot_k(float3 a, float3 b) {
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

__device__ __host__ matrix3 operator +(matrix3 a, matrix3 b) {
	matrix3 tmp;

	tmp.x = make_float3(a.x.x + b.x.x, a.x.y + b.x.y, a.x.z + b.x.z);
	tmp.y = make_float3(a.y.x + b.y.x, a.y.y + b.y.y, a.y.z + b.y.z);
	tmp.z = make_float3(a.z.x + b.z.x, a.z.y + b.z.y, a.z.z + b.z.z);

	return tmp;
}

__device__ __host__ matrix3 operator *(matrix3 a, matrix3 b) {
	matrix3 tmp;

	tmp.x.x = a.x.x * b.x.x + a.x.y * b.y.x + a.x.z * b.z.x;
	tmp.x.y = a.x.x * b.x.y + a.x.y * b.y.y + a.x.z * b.z.y;
	tmp.x.z = a.x.x * b.x.z + a.x.y * b.y.z + a.x.z * b.z.z;

	tmp.y.x = a.y.x * b.x.x + a.y.y * b.y.x + a.y.z * b.z.x;
	tmp.y.y = a.y.x * b.x.y + a.y.y * b.y.y + a.y.z * b.z.y;
	tmp.y.z = a.y.x * b.x.z + a.y.y * b.y.z + a.y.z * b.z.z;

	tmp.z.x = a.z.x * b.x.x + a.z.y * b.y.x + a.z.z * b.z.x;
	tmp.z.y = a.z.x * b.x.y + a.z.y * b.y.y + a.z.z * b.z.y;
	tmp.z.z = a.z.x * b.x.z + a.z.y * b.y.z + a.z.z * b.z.z;
	
	return tmp;
}

__device__ __host__ matrix3 operator *(matrix3 a, float b) {
	matrix3 tmp;

	tmp.x = a.x * b;
	tmp.y = a.y * b;
	tmp.z = a.z * b;
	return tmp;
}

__device__ __host__ void set_identity(matrix3 &a) {
	a.x = make_float3(1, 0, 0);
	a.y = make_float3(0, 1, 0);
	a.z = make_float3(0, 0, 1);
}

__device__ __host__ float3 rot_vec_by_mat(float3 a, matrix3 b) {
	float3 tmp;
	tmp.x = a.x * b.x.x + a.y * b.x.y + a.z * b.x.z;
	tmp.y = a.x * b.y.x + a.y * b.y.y + a.z * b.y.z;
	tmp.z = a.x * b.z.x + a.y * b.z.y + a.z * b.z.z;

	return tmp;
}

__device__ __host__ float mat_index(matrix3 mat, int i, int j) {
	
	if (i == 0 && j == 0)return mat.x.x;
	if (i == 0 && j == 1)return mat.x.y;
	if (i == 0 && j == 2)return mat.x.z;
	if (i == 1 && j == 0)return mat.y.x;
	if (i == 1 && j == 1)return mat.y.y;
	if (i == 1 && j == 2)return mat.y.z;
	if (i == 2 && j == 0)return mat.z.x;
	if (i == 2 && j == 1)return mat.z.y;
	if (i == 2 && j == 2)return mat.z.z;

	return 0;
}

__device__ __host__ matrix3 rot_mat_from_two_vectors(float3 a, float3 b) {
	a = vector_normalized_k(a);
	b = vector_normalized_k(b);

	float3 v = vector_cross_k(a, b);
	float s = vector_length_k(v);
	float c = vector_dot_k(a,b);
	matrix3 vx;
	vx.x = make_float3(0, -v.z, v.y);
	vx.y = make_float3(v.z, 0, -v.x);
	vx.z = make_float3(-v.y, v.x, 0);

	matrix3 r;
	set_identity(r);

	if (s != 0) {
		r = r + vx + vx * vx * ((1 - c) / pow(s, 2));
	}

	return r;
}


//
//__device__ float3 multiply_transpose_frame_k(Frame f, float3 e) {
//	float3 tmp;
//	tmp.x =
//		e.x * f.aim.x +
//		e.y * f.up.x +
//		e.z * f.cross.x;
//
//	tmp.y =
//		e.x * f.aim.y +
//		e.y * f.up.y +
//		e.z * f.cross.y;
//
//	tmp.z =
//		e.x * f.aim.z +
//		e.y * f.up.z +
//		e.z * f.cross.z;
//	return tmp;
//}
//
//__device__ float3 multiply_frame_k(Frame f, float3 e) {
//	float3 tmp;
//	tmp.x =
//		e.x * f.aim.x +
//		e.y * f.aim.y +
//		e.z * f.aim.z;
//
//	tmp.y =
//		e.x * f.up.x +
//		e.y * f.up.y +
//		e.z * f.up.z;
//
//	tmp.z =
//		e.x * f.cross.x +
//		e.y * f.cross.y +
//		e.z * f.cross.z;
//	return tmp;
//}