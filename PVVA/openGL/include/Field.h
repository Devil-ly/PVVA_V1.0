#pragma once
#ifndef FIELD_H
#define FIELD_H

#include <gl/glut.h>
#include <vector>
#include <fstream>
#include <complex>
#include "tool/GEMS_Memory.h"
#include "openGL/include/GraphTrans.h"
#include "openGL/include/Gaussian.h"
#include "tool/Vector3.h"

class Field
{
public: 
	Field();
	Field(const Field & _field); // ��д���캯��
	Field(Gaussian * const _gaussian); // ����һ����˹Դ;

	~Field();

	void init();

	void freeMemory();
	void allocateMemory();

	void setNM(int N, int M);
	void setPlane(Vector3 ** _Plane, double factor);
	void setField(complex<double> ** _Ex, complex<double> ** Ey, complex<double> ** _Ez,
		complex<double> ** _Hx, complex<double> ** _Hy, complex<double> ** _Hz);

	complex<double> ** getEx() const;
	complex<double> ** getEy() const;

	//�õ����Ĳ���
	void getSourcePara(GraphTrans & _graphTransPara, int & _N_width, int & _M_depth, 
		double &_ds) const;

	void setContent(int _content);
	void draw() const;

private:
	void setDrawData(complex<double> ** data);
	void setRGB(float var) const;
	
private:
	vector<float> para; // ģ�Ͳ���
	GraphTrans graphTransPara; // ��ת����

	Vector3 ** Plane;  // ƽ���������꣨�������꣩

	complex<double> ** Ex, ** Ey, ** Ez;	// ������
	complex<double> ** Hx, ** Hy, ** Hz;

	float ** drawData; // ��Ҫ��ͼ������
	float max, min;
	int content;
	// ��Ҫ��ͼ������ 0~5��ʾEx~Hz�����Է��� 6~11��ʾEx~Hz��������λ
	// 12~17��ʾEx~Hz��db���� 18~24��ʾEx~Hz��db��λ

	int N_width, M_depth;  //N_width = para[0] /ds

	double ds;

	bool isSource;
};

#endif // !DRAW_H
