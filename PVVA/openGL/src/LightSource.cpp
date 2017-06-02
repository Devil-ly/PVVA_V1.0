#include "opengl/include/LightSource.h"
#include <gl/glut.h>
#include <fstream>

LightSource::LightSource()
{
	a = 0;  b = 0;
	Num_a = 0;
	Num_b = 0;

	//graphTransPara = _graphTransPara;

	judgeNum = 0;

	name = "";

	ID = -1;

	finalLength = 0;

	NumReflect =  0;

	Free_rayTracePosition = 0;

	rayPosition = NULL;

	normalVector = NULL;

	SurfaceListPtr = NULL;

}

LightSource::LightSource(float _a, float _b, int _Num_a, int _Num_b, const GraphTrans &_graphTransPara,
	int _judgeNum, const QString &_name, int _ID, float _finalLength)
{
	a = _a;  b = _b;
	Num_a = _Num_a;
	Num_b = _Num_b;

	graphTransPara = _graphTransPara;

	judgeNum = _judgeNum;

	name = _name;

	ID = _ID;

	finalLength = _finalLength;

	NumReflect = 2;

	rayPosition = NULL;

	normalVector = NULL;

	SurfaceListPtr = NULL;
}

LightSource::LightSource(const LightSource & _LightSource)
	:a(_LightSource.a), b(_LightSource.b), Num_a(_LightSource.Num_a), Num_b(_LightSource.Num_b),
	graphTransPara(_LightSource.graphTransPara), judgeNum(_LightSource.judgeNum), 
	name(_LightSource.name), ID(_LightSource.ID), finalLength(_LightSource.finalLength),
	SelSurf(_LightSource.SelSurf), NumReflect(_LightSource.NumReflect),
	SurfaceListPtr(_LightSource.SurfaceListPtr)
{
	rayPosition = NULL;
	Free_rayTracePosition = 0;
	normalVector = NULL;
}

LightSource::~LightSource()
{
	freeMemory();
}

const LightSource & LightSource::operator=(const LightSource & rightHandSide)
{

	if (this != &rightHandSide) // 确实不是自我赋值
	{
		float _a; float _b; int _Num_a; int _Num_b; GraphTrans _graphTransPara;
		float _finalLength; int _judgeNum; QString _name;
		int _NumSel;
		rightHandSide.getData(_a, _b, _Num_a, _Num_b, _graphTransPara, _finalLength,
			_judgeNum, _name, _NumSel);
		setData(_a, _b, _Num_a, _Num_b, _graphTransPara, _judgeNum, _name,
			 _finalLength);
		setID(rightHandSide.getID());
		setSurfaceListPtr(rightHandSide.getSurfaceListPtr());
		setSelSurfaces(rightHandSide.getSelSurfaces());
		
	}
	//calReflectPoint();
	return *this;
}

void LightSource::freeMemory()
{
	if (rayPosition != NULL) //非空 释放内存
	{
		Free_3D(rayPosition, Free_rayTracePosition);
		Free_2D(normalVector);
		normalVector = NULL;
		rayPosition = NULL;
		Free_rayTracePosition = 0;
	}
}

void LightSource::allocate()
{
	freeMemory();
	rayPosition = Allocate_3D(rayPosition, Num_a, Num_b, NumReflect); //分配内存
	Free_rayTracePosition = Num_a;

	normalVector = Allocate_2D(normalVector, Num_a, Num_b);
}

void LightSource::getData(float &_a, float &_b, int &_Num_a, int &_Num_b, GraphTrans &_graphTransPara,
	float &_finalLength, int &_judgeNum, QString &_name, int &_NumSel) const
{
	_a = a;  _b = b;
	_Num_a = Num_a;
	_Num_b = Num_b;

	_graphTransPara = graphTransPara;

	_judgeNum = judgeNum;

	_finalLength = finalLength;

	_name = name;

	_NumSel = NumReflect - 2;
}

void LightSource::setData(float _a, float _b, int _Num_a, int _Num_b, const GraphTrans &_graphTransPara,
	int _judgeNum, const QString &_name, float _finalLength)
{
	a = _a;  b = _b;
	Num_a = _Num_a;
	Num_b = _Num_b;

	graphTransPara = _graphTransPara;

	judgeNum = _judgeNum;

	name = _name;

	finalLength = _finalLength;

	//reallocate(); // 重新分配内存
}

void LightSource::setSelSurfaces(list<modleID> _SelSurf)
{
	SelSurf = _SelSurf;
	NumReflect = SelSurf.size() + 2;
}

list<modleID> LightSource::getSelSurfaces() const
{
	return SelSurf;
}

void LightSource::draw() const
{
	glColor3f(1.0f, 0.0f, 0.0f);

	for (int i = 0; i < Num_a; i++)
		for (int j = 0; j < Num_b; j++)
			for (int k = 0; k < NumReflect - 1; k++)
			{
				glBegin(GL_LINES);
				glVertex3f(rayPosition[i][j][k].X(), rayPosition[i][j][k].Y(), rayPosition[i][j][k].Z());
				glVertex3f(rayPosition[i][j][k + 1].X(), rayPosition[i][j][k + 1].Y(), rayPosition[i][j][k + 1].Z());
				glEnd();
			}
}

int LightSource::getID() const 
{
	return ID;
}
void LightSource::setID(int _ID)
{
	ID = _ID;
}

void LightSource::getBeginPoint()
{
	Vector3D RotateAsix(graphTransPara.getRotate_x(), graphTransPara.getRotate_y(), graphTransPara.getRotate_z());
	Matrix4D rotatMatrix = Matrix4D::getRotateMatrix(graphTransPara.getRotate_theta(), RotateAsix);
	Matrix4D translateMatrix = Matrix4D::getTranslateMatrix(graphTransPara.getTrans_x(), graphTransPara.getTrans_y(), graphTransPara.getTrans_z());

	float var_a = a / (Num_a - 1);
	float var_b = b / (Num_b - 1);
	for (int i = 0; i < Num_a; i++)
		for (int j = 0; j < Num_b; j++)
		{
			if (judgeNum == 0) // 为矩形
			{
				Position3D temp(var_a * i - a / 2, var_b * j - b / 2, 0);
				rayPosition[i][j][0] = translateMatrix * rotatMatrix * temp;
			}
			normalVector[i][j] = rotatMatrix * Position3D(0, 0, 1);
		}

}

void LightSource::getEndPoint()
{
	for (int i = 0; i < Num_a; i++)
		for (int j = 0; j < Num_b; j++)
		{
			// 单位化
			Vector3D tempNormal(normalVector[i][j].X(), normalVector[i][j].Y(), normalVector[i][j].Z());
			tempNormal.normalize();
			Position3D temp(rayPosition[i][j][NumReflect - 2].X() + tempNormal.X() * finalLength,
				rayPosition[i][j][NumReflect - 2].Y() + tempNormal.Y() * finalLength,
				rayPosition[i][j][NumReflect - 2].Z() + tempNormal.Z() * finalLength);

			rayPosition[i][j][NumReflect - 1] = temp;
		}
}

void LightSource::calReflectPoint()
{
	allocate();//分配内存

	getBeginPoint();

	list<modleID>::iterator itermod;
	
	int n = 1;
	for (itermod = SelSurf.begin(); itermod != SelSurf.end(); itermod++)
	{
		// 在所有面中找到需要计算反射的面
		Surface const * SurPtr = SurfaceListPtr->getSurfaceData(*itermod);  
		
		switch (SurPtr->getModleID().name)
		{
		case PARABOLOID_NAME:
			calParaboloid_ReflectPoint(SurPtr->getPara()[1], SurPtr->getPara()[0],
				SurPtr->getGraphTrans(), rayPosition, n);
			break;
		default:
			break;
		}
		n++;
	}

	getEndPoint();
}

void LightSource::reCalReflectPoint()
{
	//reallocate();
	calReflectPoint();
}

QString LightSource::getName() const
{
	return name;
}

Position3D *** LightSource::getRayPositionPointer() const
{
	return rayPosition;
}

void LightSource::setSurfaceListPtr(SurfaceList const * _SurfaceListPtr)
{
	SurfaceListPtr = _SurfaceListPtr;
}

SurfaceList const * LightSource::getSurfaceListPtr() const
{
	return SurfaceListPtr;
}

void LightSource::save(ofstream & savefile) const
{
	// 保存lightsource的基本参数
	savefile << a << " " << b << " " << Num_a << " " << Num_b << " " << judgeNum 
		<< " " << ID << " " << NumReflect << " " << finalLength << " " <<name.toStdString()
		<< endl;
	graphTransPara.save(savefile);

	// 保存lightsource的选择需要计算反射面的ID
	list<modleID>::const_iterator itSur;
	//Surface const * SurPtr;
	for (itSur = SelSurf.begin(); itSur != SelSurf.end(); itSur++)
	{
		savefile << itSur->name <<  " " << itSur->ID << endl;
	}
}

void LightSource::open(ifstream & openfile)
{
	// 读lightsource的基本参数
	string tempName;
	openfile >> a >> b >> Num_a >> Num_b >> judgeNum >> ID >> NumReflect
		>> finalLength >> tempName;
	name.clear();
	for (int i = 0; i < tempName.size(); i++)
		name.push_back(tempName[i]);

	graphTransPara.open(openfile);
	// 读需要进行反射计算的面
	SelSurf.clear();
	for (int i = 0; i < NumReflect-2; i++)
	{
		modleID modleIDpara;
		openfile >> modleIDpara.name >> modleIDpara.ID;
		SelSurf.push_back(modleIDpara);
	}
}

void LightSource::calParaboloid_ReflectPoint(float _r, float _f, GraphTrans _graphTransPara, Position3D *** rayPosition, int n)
{
	// 将光线的点变到抛物面的坐标系
	Vector3D RotateAsix(_graphTransPara.getRotate_x(), _graphTransPara.getRotate_y(), _graphTransPara.getRotate_z());
	Matrix4D rotatMatrix1 = Matrix4D::getRotateMatrix(-_graphTransPara.getRotate_theta(), RotateAsix);
	
	Vector3D rotatTranslate(_graphTransPara.getTrans_x(), _graphTransPara.getTrans_y(), _graphTransPara.getTrans_z());
	rotatTranslate = rotatMatrix1 * rotatTranslate; // 先旋转在平移（把平移的坐标一起旋转）
	Matrix4D translateMatrix1 = Matrix4D::getTranslateMatrix( rotatTranslate * (-1));

	Matrix4D rotatMatrix2 = Matrix4D::getRotateMatrix(_graphTransPara.getRotate_theta(), RotateAsix);
	Matrix4D translateMatrix2 = Matrix4D::getTranslateMatrix(_graphTransPara.getTrans_x(), _graphTransPara.getTrans_y(), _graphTransPara.getTrans_z());

	Position3D tempPoint, tempnormalVector;
	Vector3 Recflect;

	for (int i = 0; i < Num_a; i++)
		for (int j = 0; j < Num_b; j++)
		{
			tempnormalVector = rotatMatrix1 * normalVector[i][j];

			tempPoint = translateMatrix1 * rotatMatrix1 * rayPosition[i][j][n - 1];

			tempPoint = Analytic_Paraboloid_InterPoint(tempPoint, tempnormalVector, _f, _r);

			if (tempPoint.W() == 1) // 表示有交点
			{
				Recflect = ReflectLight(Vector3(tempnormalVector.X(), tempnormalVector.Y(), tempnormalVector.Z()),
					Vector3(-tempPoint.X() * 2, -tempPoint.Y() * 2, 4 * _f));

				normalVector[i][j] = Position3D(Recflect.x, Recflect.y, Recflect.z);

				rayPosition[i][j][n] = translateMatrix2 * rotatMatrix2 * tempPoint; // 反变换

				normalVector[i][j] = rotatMatrix2 * normalVector[i][j];
			}
			else
			{
				rayPosition[i][j][n] = rayPosition[i][j][n - 1];
			}
			

		}
}

Position3D LightSource::Analytic_Paraboloid_InterPoint(const Position3D & O, const Position3D & n,
	float Focus, float r)
{
	double x0 = O.X(), y0 = O.Y(), z0 = O.Z();
	double u = n.X(), v = n.Y(), w = n.Z();
	double p = Focus * 2;
	double a = u * u + v * v;
	double b = 2 * v * y0 + 2 * u * x0 - 2 * p * w;
	double c = x0 * x0 + y0 * y0 - 2 * p * z0;
	double x, y, z;
	double t;

	if (a != 0)
	{
		double temp = b * b - 4 * a * c;
		if (temp >= 0)
			temp = pow(temp, 0.5);
		else
			return Position3D(0, 0, 0, 0); // ws = 0 表示没有交点

		double tempt1, tempt2;
		tempt1 = (-b + temp) / 2.0 / a;
		tempt2 = (-b - temp) / 2.0 / a; // 求根公式的两个解

		if (tempt1 <= 0 && tempt2 <= 0) // 都小于0无解
			return Position3D(0, 0, 0, 0); // ws = 0 表示没有交点

		else if (tempt1 > 0 && tempt2 <= 0) // 有一个大于0 取
			t = tempt1;
		else if (tempt2 > 0 && tempt1 <= 0)
			t = tempt2;
		else                              // 都大于0的取小的
		{
			if (tempt1 > tempt2)
				t = tempt2;
			else
				t = tempt1;
		}

	}
	else                          // 只有一个交点，与法线平行
	{
		t = -c / b;
		if (t <= 0)
			return Position3D(0, 0, 0, 0); // ws = 0 表示没有交点
	}
	x = x0 + u * t;
	y = y0 + v * t;
	z = z0 + w * t;
	double _r = x * x + y * y;
	if(_r < r * r )
		return Position3D(x, y, z);
	else
		return Position3D(0, 0, 0, 0);
}

Vector3 LightSource::ReflectLight(Vector3 a, Vector3 n)
{
	//先单位化
	
	float absa = pow(a.Dot(a), 0.5);
	float absn = pow(n.Dot(n), 0.5);
	a = a * (1 / absa);
	n = n * (1 / absn);
	float I = 2 * n.Dot(a);
	if (I < 0)
		I = -I;
	else
		a = Vector3(0.0, 0.0, 0.0) - a;

	return n * I + a;
}


