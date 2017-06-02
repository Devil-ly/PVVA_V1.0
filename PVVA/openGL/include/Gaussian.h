#pragma once
#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <vector>
#include <fstream>
#include <QString>
#include <complex>
#include <cmath>
#include "openGL/include/GraphTrans.h"
#include "tool/Constant_Var.h"

using namespace std;
class Gaussian
{
public:
	Gaussian()
	{

	}
	Gaussian(vector<float> _para,const GraphTrans & _graphTransPara, double _z0, double _w0,
		double _fre, double _ds, const QString & _name)
	{
		para = _para;
		graphTransPara = _graphTransPara;
		z0 = _z0;
		w0 = _w0;
		fre = _fre;
		ds = _ds;
		name = _name;
	}
	~Gaussian()
	{

	}

	void setGaussian(const vector<float> & _para, const GraphTrans & _graphTransPara, const double & _z0,
		const double & _w0, const double & _fre, const double & _ds, const QString & _name)
	{
		para = _para;
		graphTransPara = _graphTransPara;
		z0 = _z0;
		w0 = _w0;
		fre = _fre;
		ds = _ds;
		name = _name;
		factor = 0.001;
	}

	void getGaussian(vector<float> &_para, GraphTrans &_graphTransPara, double &_z0, double &_w0,
		double &_fre, double &_ds, QString &_name) const
	{
		_para = para;
		_graphTransPara = graphTransPara;
		_z0 = z0;
		_w0 = w0;
		_fre = fre;
		_ds = ds;
		_name = name;
	}

	void setUnit(double _factor) // 设置单位
	{
		factor = _factor;
	}

	double getDs() { return ds; } const
	double getZ0() { return z0; } const
	double getW0() { return w0; } const
	double getFre() { return fre * 1e9; } const
	vector<float> getPara() { return para; } const
	GraphTrans getGraphTransPara() { return graphTransPara; } const

	int getN() { return int(para[0] / ds); } const // width上的点数
	int getM() { return int(para[1] / ds); } const // depth上的点数

	QString getName() const
	{
		return name;
	}

	//生成高斯源
	void CreateSource(complex<double> ** Ex_In, complex<double> ** Ey_In, int N, int M)
	{
		//生成Ex_In, Ey_IN;
		//double z0_back = z0; // 目标距离
		//double w0_back = w0; // 束腰
		//double fre_back = fre; // 频率
	
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < M; j++)
			{
				double tempX = (i - (N - 1) / 2) * ds * factor;
				double tempY = (j - (M - 1) / 2) * ds * factor;
				double tempr1;
				tempr1 = pow((tempX * tempX + tempY * tempY), 0.5);
				double temper, tempei;
				CreateGaussian(tempr1, w0 * factor, fre * 1e9, z0 * factor, temper, tempei);
				Ex_In[i][j] = complex<double>(0, 0);
				Ey_In[i][j] = complex<double>(temper, tempei);
			}
		}	
	
	}

protected:
	QString name;
	double factor;
	double ds;
	double z0; // 目标距离
	double w0; // 束腰
	double fre; // 频率
private:
	void CreateGaussian(double r, double w0, double fre, double z0, double &Real, double &Imag) {
		double lamda = C_Speed / fre;
		double k = 2 * Pi * fre / C_Speed;
		double w, Rdl, theta, r2, temp;

		w = w0 * pow((1 + pow(((lamda * z0) / (Pi * w0 * w0)), 2)), 0.5);
		Rdl = z0 / (z0 * z0 + pow((Pi * w0 * w0 / lamda), 2));
		theta = atan((lamda * z0) / (Pi * w0 * w0));
		r2 = r * r;
		temp = pow((2 / w / w / Pi), 0.5) * exp(-r2 / w / w);

		Real = temp * cos(theta - k * z0 - Pi * r2 / lamda * Rdl);
		Imag = temp * sin(theta - k * z0 - Pi * r2 / lamda * Rdl);
	}

private:
	vector<float> para; // 模型参数
	GraphTrans graphTransPara; // 旋转参数


};

#endif // !GAUSSIAN_H
