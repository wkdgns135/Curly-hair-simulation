#pragma once
#include "vector_calc.h"
#include <math.h>

void HairModel::array_copy(float3 *a, float3 *b) {
	int index = 0;
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].size(); j++) {
			a[index].x = b[index].x;
			a[index].y = b[index].y;
			a[index].z = b[index].z;
			index++;
		}
	}
}

float3 operator * (float3 a, float3 b) {
	float3 tmp;
	tmp.x = a.x * b.x;
	tmp.y = a.y * b.y;
	tmp.z = a.z * b.z;
	return tmp;
}

float3 operator * (float3 a, double b) {
	float3 tmp;
	tmp.x = a.x * b;
	tmp.y = a.y * b;
	tmp.z = a.z * b;

	return tmp;
}

float3 operator + (float3 a, float3 b) {
	float3 tmp;
	tmp.x = a.x + b.x;
	tmp.y = a.y + b.y;
	tmp.z = a.z + b.z;

	return tmp;
}


float3 operator + (float3 a, double b) {
	float3 tmp;
	tmp.x = a.x + b;
	tmp.y = a.y + b;
	tmp.z = a.z + b;

	return tmp;
}

float3 operator-(float3 a, float3 b) {
	float3 tmp;
	tmp.x = a.x - b.x;
	tmp.y = a.y - b.y;
	tmp.z = a.z - b.z;

	return tmp;
}

float3 operator -(float3 a, double b) {
	float3 tmp;
	tmp.x = a.x - b;
	tmp.y = a.y - b;
	tmp.z = a.z - b;

	return tmp;
}

float3 operator /(float3 a, float3 b) {
	float3 tmp;
	tmp.x = a.x / b.x;
	tmp.y = a.y / b.y;
	tmp.z = a.z / b.z;

	return tmp;
}

float3 operator /(float3 a, double b) {
	float3 tmp;
	tmp.x = a.x / b;
	tmp.y = a.y / b;
	tmp.z = a.z / b;

	return tmp;
}


double vector_length(float3 a) {
	return sqrt(a.x*a.x + a.y * a.y + a.z * a.z);
}

void vector_normalize(float3 &a) {
	double norm = vector_length(a);
	if (norm != 0) {
		a.x = a.x / norm;
		a.y = a.y / norm;
		a.z = a.z / norm;
	}
}

float3 vector_normalized(float3 a) {
	double norm = vector_length(a);
	if (norm != 0) {
		a.x = a.x / norm;
		a.y = a.y / norm;
		a.z = a.z / norm;
	}
	return a;
}


float3	vector_cross(float3 a, float3 b) {
	float3 tmp;
	tmp.x = ((a.y*b.z) - (a.z*b.y));
	tmp.y = ((a.z*b.x) - (a.x*b.z));
	tmp.z = ((a.x*b.y) - (a.y*b.x));
	return tmp;
}

double vector_dot(float3 a, float3 b) {
	return (a.x*b.x + a.y * b.y + a.z * b.z);
}

void HairModel::compute_frame(Frame *f, float3 *p) {
	int index = 0;
	for (int i = 0; i < v.size(); i++) {
		float3 aim = p[index + 1] - p[index];
		vector_normalize(aim);

		float3 up;
		up.x = aim.z - aim.y;
		up.y = aim.x - aim.z;
		up.z = aim.y - aim.x;
		vector_normalize(up);

		float3 cross = vector_cross(aim, up);
		vector_normalize(cross);


		f[index].aim.x = aim.x;
		f[index].aim.y = up.x;
		f[index].aim.z = cross.x;

		f[index].up.x = aim.y;
		f[index].up.y = up.y;
		f[index].up.z = cross.y;

		f[index].cross.x = aim.z;
		f[index].cross.y = up.z;
		f[index].cross.z = cross.z;
		index++;

		for (int j = 1; j < v[i].size() - 1; j++) {
			float3 aim = p[index + 1] - p[index];
			vector_normalize(aim);

			float3 cross = vector_cross(aim, up);
			vector_normalize(cross);

			up = vector_cross(cross, aim);
			vector_normalize(up);

			f[index].aim.x = aim.x;
			f[index].aim.y = up.x;
			f[index].aim.z = cross.x;

			f[index].up.x = aim.y;
			f[index].up.y = up.y;
			f[index].up.z = cross.y;

			f[index].cross.x = aim.z;
			f[index].cross.y = up.z;
			f[index].cross.z = cross.z;
			index++;
		}
		index++;
	}
}

float3 multiply_transpose_frame(Frame f, float3 e) {
	float3 tmp;
	tmp.x =
		e.x * f.aim.x +
		e.y * f.up.x +
		e.z * f.cross.x;

	tmp.y =
		e.x * f.aim.y +
		e.y * f.up.y +
		e.z * f.cross.y;

	tmp.z =
		e.x * f.aim.z +
		e.y * f.up.z +
		e.z * f.cross.z;
	return tmp;
}

float3 multiply_frame(Frame f, float3 e) {
	float3 tmp;
	tmp.x =
		e.x * f.aim.x +
		e.y * f.aim.y +
		e.z * f.aim.z;

	tmp.y =
		e.x * f.up.x +
		e.y * f.up.y +
		e.z * f.up.z;

	tmp.z =
		e.x * f.cross.x +
		e.y * f.cross.y +
		e.z * f.cross.z;
	return tmp;
}

void HairModel::position_smoothing_function(float3 *lambda, float3 *dst, double *l, double alpha, bool is_position) {
	double beta = 0.0;
	//lambda가 파티클 위치일 경우 return하기위한 pos vector

	array_copy(particle_host.d, lambda);

	//beta formulation, l = 파티클간의 평균길이
	int index = 0;
	for (int i = 0; i < v.size(); i++) {
		particle_host.d[index] = lambda[index + 1] - lambda[index];
		beta = 1 > 1 - exp(-l[i] / alpha) ? 1 - exp(-l[i] / alpha) : 1;
		index++;
		for (int j = 1; j < v[i].size() - 1; j++) {
			int index_1 = j - 1 >= 0 ? index - 1 : 0;
			int index_2 = j - 2 >= 0 ? index - 2 : 0;

			float3 term1 = particle_host.d[index_1] * ( 2 * (1 - beta));
			float3 term2 = particle_host.d[index_2] * ((1 - beta) * (1 - beta));
			float3 term3 = term1 - term2;
			float3 term4 = (lambda[index + 1] - lambda[index]) *  (beta * beta);
			particle_host.d[index++] = term3 + term4;
		}
		index++;
	}

	index = 0;
	for (int i = 0; i < v.size(); i++) {
		dst[index] = lambda[index];
		index++;
		for (int j = 1; j < v[i].size(); j++) {
			dst[index] = particle_host.d[index - 1] +  dst[index - 1];
			index++;
		}
	}
}

void HairModel::velocity_smoothing_function(float3 *lambda, float3 *dst, double *l, double alpha, bool is_position) {
	double beta = 0.0;
	//lambda가 파티클 위치일 경우 return하기위한 pos vector

	array_copy(dst, lambda);

	//beta formulation, l = 파티클간의 평균길이

	int index = 0;
	for (int i = 0; i < v.size(); i++) {
		dst[index] = lambda[index + 1] - lambda[index];
		beta = 1 > 1 - exp(-l[i] / alpha) ? 1 - exp(-l[i] / alpha) : 1;
		index++;
		for (int j = 1; j < v[i].size() - 1; j++) {
			int index_1 = j - 1 >= 0 ? index - 1 : 0;
			int index_2 = j - 2 >= 0 ? index - 2 : 0;

			float3 term1 = particle_host.d[index_1] * (2 * (1 - beta));
			float3 term2 = particle_host.d[index_2] * ((1 - beta) * (1 - beta));
			float3 term3 = term1 - term2;
			float3 term4 = (lambda[index + 1] - lambda[index]) *  (beta * beta);
			particle_host.d[index++] = term3 + term4;
		}
		index++;
	}
}