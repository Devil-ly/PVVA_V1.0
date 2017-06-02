#pragma once
#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <QString>
#include <list>
#include <fstream>
#include "tool/GEMS_Memory.h"
#include "tool/Vector3.h"
#include "openGL/include/Position3D.h"
#include "openGL/include/Matrix4D.h"
#include "openGL/include/Vector3D.h"

#include "tool/Vector3.h"
#include "openGL/include/GraphTrans.h"
#include "tool/ModleID.h"
#include "openGL/include/SurfaceList.h"

using namespace std;
//#include "openGL/include/Paraboloid.h"
class LightSource
{
public:
	LightSource();
	LightSource(float _a, float _b, int _Num_a,int _Num_b, const GraphTrans &_graphTransPara,
		int _judgeNum, const QString &_name, int _ID, float _finalLength);
	LightSource(const LightSource & _LightSource); // 重写构造函数
	~LightSource();

	const LightSource & operator = (const LightSource & rightHandSide);
	// 由于内部有new新的内存 需重写赋值函数

	void getData(float &_a, float &_b, int &_Num_a, int &_Num_b, GraphTrans &_graphTransPara, 
		float &_finalLength, int &_judgeNum, QString &_name, int &_NumSel) const;
	void setData(float _a, float _b, int _Num_a, int _Num_b, const GraphTrans &_graphTransPara,
		int _judgeNum, const QString &_name, float _finalLength);

	void setSelSurfaces(list<modleID> _SelSurf); // 设置选择面的ID
	list<modleID> getSelSurfaces() const;

	int getID() const;
	void setID(int _ID);

	void draw() const;

	void calReflectPoint();
	void reCalReflectPoint();

	QString getName() const;

	Position3D *** getRayPositionPointer() const; // 返回rayPosition的指针

	// 将面的数据头指针传给源
	void setSurfaceListPtr(SurfaceList const * _SurfaceListPtr);
	SurfaceList const * getSurfaceListPtr() const;

	void save(ofstream & savefile) const;
	void open(ifstream & openfile);

private:

	void getBeginPoint();

	void getEndPoint();

	void calParaboloid_ReflectPoint(float _r, float _f, GraphTrans _graphTransPara, 
		Position3D *** rayPosition, int n); // 光线追踪

	Position3D LightSource::Analytic_Paraboloid_InterPoint(const Position3D & O, 
		const Position3D & n, float Focus, float r);

	Vector3 ReflectLight(Vector3 a, Vector3 n);

	void allocate();
	void freeMemory();
	// 重新给RayPosition和normalVector分配内存
private:

	GraphTrans graphTransPara;
	float a, b;  //为圆的时候a 表示半径， 为点源的时候a表示张角， 为矩形的时候a 表示宽， b表示长
	int Num_a, Num_b;

	int judgeNum; // 0为矩形 1为圆 2为点源 

	int ID; // 辨认id

	//int isCal; // 判断是否计算 1计算 0 不计算

	int NumReflect; // 求反射的次数

	QString name;

	Position3D *** rayPosition; // 保存计算反射后的点
	int Free_rayTracePosition; // free rayPosition 用的参数

	Position3D ** normalVector;

	// 选择的面
	//int NumSel; // 反射面的个数
	list<modleID> SelSurf;
	//list <Surface const *> SelSurfacesPtr; // 选择的反射面的指针

	float finalLength; // 最后一次反射后光线的长度

	SurfaceList const * SurfaceListPtr;
};

#endif