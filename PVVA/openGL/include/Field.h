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
	Field(const Field & _field); // 重写构造函数
	Field(Gaussian * const _gaussian); // 加入一个高斯源;

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

	//得到场的参数
	void getSourcePara(GraphTrans & _graphTransPara, int & _N_width, int & _M_depth, 
		double &_ds) const;

	void setContent(int _content);
	void draw() const;

private:
	void setDrawData(complex<double> ** data);
	void setRGB(float var) const;
	
private:
	vector<float> para; // 模型参数
	GraphTrans graphTransPara; // 旋转参数

	Vector3 ** Plane;  // 平面各点的坐标（绝对坐标）

	complex<double> ** Ex, ** Ey, ** Ez;	// 场分量
	complex<double> ** Hx, ** Hy, ** Hz;

	float ** drawData; // 需要画图的数据
	float max, min;
	int content;
	// 需要画图的内容 0~5表示Ex~Hz的线性幅度 6~11表示Ex~Hz的线性相位
	// 12~17表示Ex~Hz的db幅度 18~24表示Ex~Hz的db相位

	int N_width, M_depth;  //N_width = para[0] /ds

	double ds;

	bool isSource;
};

#endif // !DRAW_H
