#pragma once
#include "HairModel.h"
#include "vector_calc.h"

HairModel::HairModel() {
	p_i = (int*)malloc(sizeof(int) * STRAND_SIZE * PARTICLE_SIZE);
	p_p = (float3*)malloc(sizeof(float3) * STRAND_SIZE * PARTICLE_SIZE);
	s_p_p = (float3*)malloc(sizeof(float3) * STRAND_SIZE * PARTICLE_SIZE);
	r_p_p = (float3*)malloc(sizeof(float3) * STRAND_SIZE * PARTICLE_SIZE);
	r_s_p_p = (float3*)malloc(sizeof(float3) * STRAND_SIZE * PARTICLE_SIZE);
	p_v_d = (float3*)malloc(sizeof(float3) * STRAND_SIZE * PARTICLE_SIZE);
	r_s_f = (Frame*)malloc(sizeof(Frame) * STRAND_SIZE * PARTICLE_SIZE);
	t = (float3*)malloc(sizeof(float3) * STRAND_SIZE * PARTICLE_SIZE);
	r_p_l = (double*)malloc(sizeof(double) * STRAND_SIZE);

	for (int i = 0; i < STRAND_SIZE; i++) {
		for (int j = 0; j < PARTICLE_SIZE; j++) {
			int index = i * PARTICLE_SIZE + j;
			p_i[index] = index;

			double r = j / double(PARTICLE_SIZE) * 2 < 1 ? j / double(PARTICLE_SIZE) * 2 : 1;
			double t = j * 0.3;
			double x = cos(t) * r;
			double y = t * 0.2;
			double z = sin(t) * r;

			p_p[index].x = x;
			p_p[index].y = -y;
			p_p[index].z = z + (i / double(STRAND_SIZE)) * 20;

			r_p_p[index].x = x;
			r_p_p[index].y = -y;
			r_p_p[index].z = z + (i / double(STRAND_SIZE)) * 20;
		}

		double sum = 0;
		for (int j = 0; j < PARTICLE_SIZE - 1; j++) {
			int index = i * PARTICLE_SIZE + j;
			float3 edge = vector_sub(r_p_p[index + 1], r_p_p[index]);
			sum += vector_length(edge);
		}

		sum /= (PARTICLE_SIZE - 1);
		cout << "rest_length : ";
		cout << sum << endl;
		r_p_l[i] = sum;

		array_copy(r_s_p_p, smoothing_function(r_p_p, r_p_l, 0.23, true));
		
		compute_frame(r_s_f, r_s_p_p);

		for (int j = 1; j < PARTICLE_SIZE - 1; j++) {
			int index_1 = i * PARTICLE_SIZE + j-1;
			int index0 = i * PARTICLE_SIZE + j;
			int index1 = i * PARTICLE_SIZE + j + 1;
			float3 e = vector_sub(r_p_p[index1], r_p_p[index0]);
			t[index0] = multiply_transpose_frame(r_s_f[index_1], e);
		}
	}
	device_init();
}


void HairModel::draw_point() {
	glDisable(GL_LIGHTING);
	glPointSize(2.0f);
	glColor3f(0, 0, 0);

	for (int i = 0; i < STRAND_SIZE; i++) {
		for (int j = 0; j < PARTICLE_SIZE; j++) {
			int index = i * PARTICLE_SIZE + j;
			glBegin(GL_POINTS);
			glVertex3f(p_p[index].x, p_p[index].y, p_p[index].z);

			//glVertex3f(s[i].r_s_p_p[j].x, s[i].r_s_p_p[j].y, s[i].r_s_p_p[j].z);
		}
	}
	glEnd();
	glPointSize(1.0f);
	glEnable(GL_LIGHTING);
}

void HairModel::draw_wire() {
	glDisable(GL_LIGHTING);
	glPointSize(2.0f);
	glColor3f(0, 0, 0);
	for (int i = 0; i < STRAND_SIZE; i++) {
		glBegin(GL_LINES);
		for (int j = 0; j < PARTICLE_SIZE-1; j++) {
			int index0 = i * PARTICLE_SIZE + j;
			int index1 = i * PARTICLE_SIZE + j + 1;
			glVertex3f(p_p[index0].x, p_p[index0].y, p_p[index0].z);
			glVertex3f(p_p[index1].x, p_p[index1].y, p_p[index1].z);
			
			//glVertex3f(s[i].r_s_p_p[j].x, s[i].r_s_p_p[j].y, s[i].r_s_p_p[j].z);
			//glVertex3f(s[i].r_s_p_p[j+1].x, s[i].r_s_p_p[j+1].y, s[i].r_s_p_p[j+1].z);
		}
		glEnd();
	}
	glPointSize(1.0f);
	glEnable(GL_LIGHTING);
}

void HairModel::draw_frame() {
	glDisable(GL_LIGHTING);
	glPointSize(2.0f);

	for (int i = 0; i < STRAND_SIZE; i++) {
		for (int j = 0; j < PARTICLE_SIZE; j++) {
			glBegin(GL_LINES);
			glColor3f(1, 0, 0);
			int index = i * PARTICLE_SIZE + j;
			glVertex3f(r_s_p_p[index].x,  r_s_p_p[index].y,  r_s_p_p[index].z);
			glVertex3f(r_s_p_p[index].x + r_s_f[index].aim.x, r_s_p_p[index].y + r_s_f[index].aim.y, r_s_p_p[index].z + r_s_f[index].aim.z);

			glVertex3f(r_s_p_p[index].x,  r_s_p_p[index].y,  r_s_p_p[index].z);
			glVertex3f(r_s_p_p[index].x + r_s_f[index].up.x, r_s_p_p[index].y + r_s_f[index].up.y, r_s_p_p[index].z + r_s_f[index].up.z);

			glVertex3f(r_s_p_p[index].x,  r_s_p_p[index].y,  r_s_p_p[index].z);
			glVertex3f(r_s_p_p[index].x + r_s_f[index].cross.x, r_s_p_p[index].y + r_s_f[index].cross.y, r_s_p_p[index].z + r_s_f[index].cross.z);

			glEnd();
		}
	}
}
