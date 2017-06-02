#include "opengl/include/RoamingScenceManager.h"
#include "Qt/include/MainWindow.h"
#include"opengl/include/Vector3D.h"
#include"opengl/include/Position3D.h"
#include"opengl/include/Matrix4D.h"
#include <windows.h>

#include"GL/glut.h"

RoamingScenceManager::RoamingScenceManager()
{
	
}

RoamingScenceManager::~RoamingScenceManager()
{
	delete OldMouse;
	delete Mouse;
	delete NewEye;
	delete NewUp;
	delete NewView;
	delete AuxX;
	delete AuxY;
	delete AuxZ;
	delete TempTranslateVec;
}

void RoamingScenceManager::init()
{
	OldMouse = new Position3D();
	Mouse = new Position3D();

	NewEye = new Vector3D(1000, 0, 0);
	NewUp = new Vector3D(0, 0, 1);
	NewView = new Vector3D(0, 0, 0);

	AuxY = new Vector3D(0, 1, 0);
	AuxZ = new Vector3D();
	*AuxZ = *NewEye - *NewView;
	AuxX = new Vector3D();
	*AuxX = Vector3D::crossProduct(*AuxY, *AuxZ);
	AuxX->normalize();

	TempTranslateVec = new Vector3D(0, 0, 0);
	TempscaleFactor = 1;

	init_CoordinaryDisplayList();
	glShadeModel(GL_SMOOTH);
	//glClearColor(0.156, 0.156, 0.168, 0.0);
	glClearColor(1, 1, 1, 0.0);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10000 / 2, 10000 / 2, -10000 / 2, 10000 / 2, -10000 / 2, 10000 / 2);
}

void RoamingScenceManager::render()
{
	//glClearColor(0.156, 0.156, 0.168, 0.0);
	glClearColor(1, 1, 1, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	*NewView = *NewEye * -1;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(NewEye->X(), NewEye->Y(), NewEye->Z(),
		NewView->X(), NewView->Y(), NewView->Z(),
		NewUp->X(), NewUp->Y(), NewUp->Z());
	glTranslatef(TempTranslateVec->X(), TempTranslateVec->Y(), TempTranslateVec->Z());
	//glPushMatrix();

	glScalef(TempscaleFactor, TempscaleFactor, TempscaleFactor);

	//绘制坐标轴
	glColor3d(1.0, 0.0, 0.0);
	axis(1000);
	glPushMatrix();
	glRotated(90, 0, 1.0, 0);
	glColor3d(0.0, 1.0, 0.0);
	axis(1000);
	glRotated(-90, 1.0, 0, 0);
	glColor3d(0.0, 0.0, 1.0);
	axis(1000);
	glPopMatrix();
	glColor3f(1, 0, 0);

	
}

void RoamingScenceManager::executeRotateOperation(int x, int y)
{
	Mouse->setX(x);
	Mouse->setY(y);

	Vector3D MouseTrace = *AuxY*(OldMouse->Y() - Mouse->Y()) + *AuxX*(Mouse->X() - OldMouse->X());
	Vector3D RotateAsix = Vector3D::crossProduct(MouseTrace, *AuxZ);
	RotateAsix.normalize();

	float angle = MouseTrace.length();
	Matrix4D rotatMatrix = Matrix4D::getRotateMatrix(angle, RotateAsix);

	*NewEye = rotatMatrix*(*NewEye);
	*NewUp = rotatMatrix*(*NewUp);

	NewUp->normalize();
	*AuxY = *NewUp;
	*AuxZ = *NewEye - *NewView;
	*AuxX = Vector3D::crossProduct(*AuxY, *AuxZ);
	AuxX->normalize();
	//更新鼠标
	*OldMouse = *Mouse;
}

void RoamingScenceManager::executeScaleOperation(float factor)
{
	if (TempscaleFactor <= 0)
	{
		TempscaleFactor = 0.1;
	}
	else
	{
		TempscaleFactor += factor;
	}
}

void RoamingScenceManager::executeTranslateOperation(int x, int y)
{
	Mouse->setX(x);
	Mouse->setY(y);
	Vector3D vec(-OldMouse->X() + Mouse->X(), -Mouse->Y() + OldMouse->Y(), 0);
	*TempTranslateVec = *TempTranslateVec + vec;
	*OldMouse = *Mouse;
}

void RoamingScenceManager::getInitPos(int x, int y)
{
	Mouse->setX(x);
	Mouse->setY(y);
	*OldMouse = *Mouse;
}

void RoamingScenceManager::init_CoordinaryDisplayList()
{
	ID_COORDINATY = glGenLists(1);
	glNewList(ID_COORDINATY, GL_COMPILE);
	/*红色轴是X轴，绿色是Y轴，蓝色是Z轴*/

	int length = 100;
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(length, 0.0, 0.0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, length, 0.0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, length);
	glEnd();

  

	glLineWidth(1);
	glEnd();
	glEndList();

	
}

void RoamingScenceManager::axis(double length)
{
	glPushMatrix();
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, length);
	glEnd();
	glTranslated(0, 0, length - 0.5);
	glutSolidCone(20, 100, 12, 9);
	glPopMatrix();
}

void RoamingScenceManager::drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // 如果是第一次调用，执行初始化
					   // 为每一个ASCII字符产生一个显示列表
		isFirstCall = 0;

		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);

		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

void RoamingScenceManager::selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void RoamingScenceManager::save(ofstream & saveFile) const
{
	saveFile << *AuxX << " " << *AuxY << " " << *AuxZ << " " << *NewEye << " "
		<< *NewUp << " " << *NewView << " " << *TempTranslateVec << " " <<
		TempscaleFactor << endl;
}

void RoamingScenceManager::open(ifstream & openFile)
{
	openFile >> *AuxX >> *AuxY >> *AuxZ >> *NewEye >> *NewUp >> *NewView
		>> *TempTranslateVec >> TempscaleFactor;
}

void RoamingScenceManager::setView(int NumView)
{
	init();
	Vector3D RotateAsix;
	float angle;
	Matrix4D rotatMatrix;
	switch (NumView)
	{
	case 0:
		return;
	case 1:
		RotateAsix = Vector3D(0, 0, 1);  // XZ
		angle = -90; 
		break;
	case 2:
		RotateAsix = Vector3D(0, 1, 0);  // XY
		angle = 90;
		break;
	case 3:
		RotateAsix = Vector3D(0, 0, 1); // -YZ
		angle = 180;
		break;
	case 4:
		RotateAsix = Vector3D(0, 0, 1); // -XZ
		angle = 90;
		break;
	case 5:
		RotateAsix = Vector3D(0, 1, 0); // -XY
		angle = 90;
		rotatMatrix = Matrix4D::getRotateMatrix(angle, RotateAsix);

		*NewEye = rotatMatrix*(*NewEye);
		*NewUp = rotatMatrix*(*NewUp);

		NewUp->normalize();
		*AuxY = *NewUp;
		*AuxZ = *NewEye - *NewView;
		*AuxX = Vector3D::crossProduct(*AuxY, *AuxZ);
		AuxX->normalize();
		RotateAsix = Vector3D(0, 0, 1); 
		angle = 90;
		break;
	default:
		break;
	}

	rotatMatrix = Matrix4D::getRotateMatrix(angle, RotateAsix);

	*NewEye = rotatMatrix*(*NewEye);
	*NewUp = rotatMatrix*(*NewUp);

	NewUp->normalize();
	*AuxY = *NewUp;
	*AuxZ = *NewEye - *NewView;
	*AuxX = Vector3D::crossProduct(*AuxY, *AuxZ);
	AuxX->normalize();
}