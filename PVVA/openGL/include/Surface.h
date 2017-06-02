/*
*  定义表面
*
*  画图函数
*/
#pragma once
#ifndef SURFACE_H
#define SURFACE_H
#include <gl/glut.h>
#include <QString>
#include <cmath>
#include <vector>
#include <fstream>
#include "openGL/include/GraphTrans.h"
#include "tool/modleID.h"

using namespace std;

class Surface
{
public:
	Surface()
	{
		initial();
	}

	Surface(vector<float> _para, GraphTrans _graphTransPara, modleID _paraModleID, QString _name)
		:para(_para), graphTransPara(_graphTransPara), paraModleID(_paraModleID)
	{
		initial();
	};

	~Surface()
	{
		para.clear();
	}

	void setModleID(modleID _paraModleID)
	{
		paraModleID = _paraModleID;
	}
	modleID getModleID() const
	{
		return paraModleID;
	}
	void saveModleID(ofstream & savefile) const 
	{
		savefile << paraModleID.name << " " << paraModleID.ID << endl;
	}

	void setSurfaceData(const vector<float> &_para, const GraphTrans &_graphTransPara, const QString &_name)
	{
		para = _para;
		graphTransPara = _graphTransPara;
		
		name = _name;
	}

	void getSurfaceData(vector<float> &_para, GraphTrans &_graphTransPara, QString &_name) const
	{
		_para = para;
		_graphTransPara = graphTransPara;
		_name = name;
		
	}

	vector<float> getPara() const
	{
		return para;
	}

	GraphTrans getGraphTrans() const
	{
		return graphTransPara;
	}

	QString getName() const
	{
		return name;
	}

	void draw() const
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glPushMatrix();

		glTranslated(graphTransPara.getTrans_x(), graphTransPara.getTrans_y(), graphTransPara.getTrans_z());
		glRotated(graphTransPara.getRotate_theta(), graphTransPara.getRotate_x(),
			graphTransPara.getRotate_y(), graphTransPara.getRotate_z());

		switch (paraModleID.name)
		{
		case PARABOLOID_NAME: //para[0] 为 f  para[1] 为 r 
			glPushMatrix();
			glTranslated(0, 0, para[1] * para[1] / 4 / para[0]);
			drawEllipse(para[1], para[1]);
			glPopMatrix();

			glPushMatrix();
			drawParabola(para[0], para[1]);
			glRotated(90, 0, 0, 1);
			drawParabola(para[0], para[1]);
			glRotated(90, 0, 0, 1);
			drawParabola(para[0], para[1]);
			glRotated(90, 0, 0, 1);
			drawParabola(para[0], para[1]);
			glRotated(90, 0, 0, 1);
			glPopMatrix();
			break;

		case RECTANGLE_NAME: //para[0] 为 Width  para[1] 为 Depth  
			drawRectangle(para[0], para[1]);
			break;
		default:
			break;
		}
		glPopMatrix();
	};

	void save(ofstream & savefile) const
	{
		vector<float>::const_iterator itpara;
		savefile << para.size() << endl;

		for (itpara = para.begin(); itpara != para.end(); itpara++)
			savefile << *itpara << endl;
		//savefile << endl;

		graphTransPara.save(savefile);
		savefile << paraModleID.ID << " " << paraModleID.name << " " << name.toStdString()
			<< endl;
	}

	void open(ifstream & openfile)
	{
		int NumPara;
		string tempString;
		openfile >> NumPara;
		for (int i = 0; i < NumPara; i++)
		{
			float tempfloat;
			openfile >> tempfloat;
			para.push_back(tempfloat);
		}
		graphTransPara.open(openfile);

		string tempName;
		openfile >> paraModleID.ID >> paraModleID.name >> tempName;
		name.clear();
		for (int i = 0; i < tempName.size(); i++)
			name.push_back(tempName[i]);
	}

private:
	void initial()
	{
		n = 40;
		m = 20;
		Pi = 3.1415926;
	}

	// 画椭圆 当a=b 是为圆
	void drawEllipse(float a, float b) const
	{
		float theta = 2 * Pi / n;
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < n; i++)
		{
			glVertex3f(a * sin(i * theta), b * cos(i * theta), 0);
		}
		glEnd();
	}

	void drawParabola(float f, float r) const
	{
		float tempL = r / (m - 1);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < m; i++)
		{
			//glVertex3f(tempL * i * sin(0), tempL * i * cos(0), tempL * tempL * i * i / 4 / f);
			glVertex3f(0, tempL * i, tempL * tempL * i * i / 4 / f);
		}
		glEnd();
	}

	void drawRectangle(float w, float d) const
	{
		glBegin(GL_LINE_LOOP);
		glVertex3f(w / 2, d / 2, 0);
		glVertex3f(-w / 2, d / 2, 0);
		glVertex3f(-w / 2, -d / 2, 0);
		glVertex3f(w / 2, -d / 2, 0);
		glEnd();
	}
private:
	QString name;
private:
	//int paraNum;
	vector<float> para; // 模型参数
	GraphTrans graphTransPara; // 旋转参数
	modleID paraModleID; // 模型的ID

	//颜色

	int n, m; // 画圆的精度
	float Pi;

};

#endif // SURFACE_H
