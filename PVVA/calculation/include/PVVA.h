/***********************************************************************
* version 1.0 2016/11/29
* Designer liyun
* E-mail 465120258@qq.com
* Fuction：
* 计算反射面
* 输入 源Ex_In,Ey_In, 源位置、传播方向， 反射面的网格参数（三角网格）
* 
***********************************************************************/
#pragma once
#ifndef PVVA_H
#define PVVA_H

#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include <vector>
#include <list>
#include <complex>

#include "tool/Vector3.h"
#include "tool/Vector2.h"
#include "tool/GEMS_Memory.h"
#include "tool/Constant_Var.h"
#include "calculation/include/Calculation.h"

#include "openGL/include/GraphTrans.h"
#include "openGL/include/Field.h"
#include "openGL/include/Surface.h"


using namespace std;

class PVVA
{
public:
	PVVA();
	~PVVA();
	
	void Initialization(); // 数据初始化
	
	void setUnit(double factor); // 设置单位

	void ReadDat(string ProjName);  //读.dat数据

	void setSource(list<Field>::const_iterator _field); // 设置源

	void setFre(double _fre);

	void setModle(const Surface * _surface);

	void Analytic_Calculate();

	void getField(Field & _field); // 返回结果

	void Analytic_CalZ0Theta(); //【解析】计算平面传播 并更新平面位置
	
	void CalZ0Theta(); // 计算z0 和 theta

	void CalPlane(); //计算平面传播 并更新平面位置

	void Reflect(); // 计算反射 改变了Exyz_R的相位和极化 更新了反射坐标

	void Analytic_Reflect(); // 【解析】 计算反射 改变了Exyz_R的相位和极化 更新了反射坐标
	
	void CalAmplitude(); // 计算反射前后的投影面积 更新Exyz_R幅度变换

	void InterVal(); // 对反射回来的波进行插值，使其落入标准网格中

	void DrawPlane();
							
private:

	void AllocateMemory();  //分配内存

	void DatePretreat();  //.dat读后预处理 （生成源的坐标）
			
	void Poynting(); //计算面的坡应廷矢量


	//复数的共轭相乘 A * B(共轭)
	complex<double> ConjugateMul(const complex<double> &A, const complex<double> &B) const;
	//复数的相乘 A * B
	complex<double> ComMul(const complex<double> &A, const complex<double> &B) const;

	void Split(const string& src, const string& separator, vector<string>& dest);//分割数据

	void JumpLine(int N, ifstream& infile);//跳行

	double CalDistance(const Vector3 &a, const Vector3 &b) const;

	//直线与三角形相交检测 返回false or true
	//orig射线起点 dir 射线方向 v0,v1,v2三角形三个点 
	//intersection 交点 若无交点intersection不做修改
	bool IsIntersect(const Vector3 &orig, const Vector3 &dir, const Vector3 &v0, 
		const Vector3 &v1, const Vector3 &v2, Vector3 &intersection, double &t);

	//直线与平面相交 这里为避免重复运算直接先求出E1,E2
	double IntersectPoint(const Vector3 &orig, const Vector3 &dir, const Vector3 &v0,
		const Vector3 E1, const Vector3 E2, Vector3 &intersection);

	//计算反射光线 输入入射向量 法向量 返回反射向量
	Vector3 ReflectLight(Vector3 a, Vector3 n);

	// 计算反射电场 输入法向量 入射电场ExEyEz 
	void CalReflectExyz(const Vector3 &n, const complex<double> &Ex, const complex<double> &Ey,
		const complex<double> &Ez, complex<double> &Ex_out, complex<double> &Ey_out, complex<double> &Ez_out);

	// 计算四个点围城的面积  等效于计算三角形ABC 和 三角形ADC的面积和
	double CalSquare(const Vector3 &A, const Vector3 &B, const Vector3 &C, const Vector3 &D) const;
	double CalSquare(const Vector3 &A, const Vector3 &B, const Vector3 &C) const;
	
	// 解析方法计算抛物面交点 输入直线的起点和法向量 输出交点
	bool Analytic_Paraboloid_InterPoint(const Vector3 &O, const Vector3 &n, double Focus, double r,
		double &t, Vector3 & InterPoint);

	// 插值用的——判断射线与线段是否相交
	bool InterVal_IsInsect(const Vector2 &A, const Vector2 &B, const Vector2 &C, const Vector2 &D);

	// 生成高斯波，TEST用
	void CreateGaussian(double r, double z, double w0, double f, double &Real, double &Imag);

private:
	int N, M; // 计算矩形的长宽 N*M
	double f; // 频率 默认 10.65e9
	double lamda; // 波长
	double k;
	double theta; // 经过反射后源相对于z正轴的夹角
	double ds;
	double w0; // 束腰
	double z0; //目标距离

	double unit_factor; // 保存模型的单位转化为米的系数

	//模型参数 (解析式用)
	vector<float> ModlePara; // 模型参数
	GraphTrans ModleGraphTrans; // 旋转参数
	int ModleIndex;

	GraphTrans SourceGraphTrans;// 源的旋转参数

	complex<double> ** Px, ** Py, ** Pz;	// 坡应廷矢量 
	complex<double> ** Ex1, ** Ey1, ** Ez1;	// 经过calculation计算后的到的电磁场
	complex<double> ** Hx1, ** Hy1, ** Hz1;	// 

	complex<double> ** Ex_In, ** Ey_In;	// 源 
	
	complex<double> ** Ex_R, ** Ey_R, ** Ez_R;	// 经过反射计算后的到的电场
	complex<double> ** Ex_R_Inter, ** Ey_R_Inter, ** Ez_R_Inter;	// 插值后的电场


	// 用于保存元素的3个节点和其属性
	struct Element
	{
		int N1, N2, N3, Mat;
	};

	string ProjName;   // 数据的地址
	int NodesNum, EleNum;  // .dat的元素和节点的个数
	Element *EleDate;  // 保存.dat内的元素数据
	Vector3 *NodesXYZ; // 保存.dat内的节点坐标

	Vector3 Org_Source, n_Source; //Org_Source平面中点 n_Source平面法向量
	double theta_Source;   // 源与-Z轴的夹角

	Vector3 **Plane;  // 平面各点的坐标（绝对坐标）
	Vector3 **n_Plane;  // 平面传播各点后的法向量
	Vector3 **R_Plane; // 反射后的各点的坐标

	Vector3 **InterVal_Plane; // 插值平面
	struct coordinate_ij
	{
		int i, j;
	};
	vector<coordinate_ij> **Interpolation; //插值用的中间变量
    // test用 删
	Vector3 InterPoint_test; //
	Vector3 **Plane_org;   // 反射后的向量
};


#endif
