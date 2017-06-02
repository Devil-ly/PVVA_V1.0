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
	LightSource(const LightSource & _LightSource); // ��д���캯��
	~LightSource();

	const LightSource & operator = (const LightSource & rightHandSide);
	// �����ڲ���new�µ��ڴ� ����д��ֵ����

	void getData(float &_a, float &_b, int &_Num_a, int &_Num_b, GraphTrans &_graphTransPara, 
		float &_finalLength, int &_judgeNum, QString &_name, int &_NumSel) const;
	void setData(float _a, float _b, int _Num_a, int _Num_b, const GraphTrans &_graphTransPara,
		int _judgeNum, const QString &_name, float _finalLength);

	void setSelSurfaces(list<modleID> _SelSurf); // ����ѡ�����ID
	list<modleID> getSelSurfaces() const;

	int getID() const;
	void setID(int _ID);

	void draw() const;

	void calReflectPoint();
	void reCalReflectPoint();

	QString getName() const;

	Position3D *** getRayPositionPointer() const; // ����rayPosition��ָ��

	// ���������ͷָ�봫��Դ
	void setSurfaceListPtr(SurfaceList const * _SurfaceListPtr);
	SurfaceList const * getSurfaceListPtr() const;

	void save(ofstream & savefile) const;
	void open(ifstream & openfile);

private:

	void getBeginPoint();

	void getEndPoint();

	void calParaboloid_ReflectPoint(float _r, float _f, GraphTrans _graphTransPara, 
		Position3D *** rayPosition, int n); // ����׷��

	Position3D LightSource::Analytic_Paraboloid_InterPoint(const Position3D & O, 
		const Position3D & n, float Focus, float r);

	Vector3 ReflectLight(Vector3 a, Vector3 n);

	void allocate();
	void freeMemory();
	// ���¸�RayPosition��normalVector�����ڴ�
private:

	GraphTrans graphTransPara;
	float a, b;  //ΪԲ��ʱ��a ��ʾ�뾶�� Ϊ��Դ��ʱ��a��ʾ�Žǣ� Ϊ���ε�ʱ��a ��ʾ�� b��ʾ��
	int Num_a, Num_b;

	int judgeNum; // 0Ϊ���� 1ΪԲ 2Ϊ��Դ 

	int ID; // ����id

	//int isCal; // �ж��Ƿ���� 1���� 0 ������

	int NumReflect; // ����Ĵ���

	QString name;

	Position3D *** rayPosition; // ������㷴���ĵ�
	int Free_rayTracePosition; // free rayPosition �õĲ���

	Position3D ** normalVector;

	// ѡ�����
	//int NumSel; // ������ĸ���
	list<modleID> SelSurf;
	//list <Surface const *> SelSurfacesPtr; // ѡ��ķ������ָ��

	float finalLength; // ���һ�η������ߵĳ���

	SurfaceList const * SurfaceListPtr;
};

#endif