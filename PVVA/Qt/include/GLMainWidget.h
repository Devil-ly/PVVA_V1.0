#pragma once
#ifndef GLMAINWIDGET_H
#define GLMAINWIDGET_H

#include <QGLWidget>
#include <vector>
#include <list>
#include "opengl/include/RoamingScenceManager.h"
#include "opengl/include/LightSourceList.h"
#include "openGL/include/SurfaceList.h"
#include "openGL/include/Field.h"

using namespace std;

class GLMainWidget : public QGLWidget {
	Q_OBJECT

public:
	GLMainWidget(QGLWidget * parent = Q_NULLPTR);
	virtual ~GLMainWidget();

	void init();

	void setView(int NumView); // 设置视角

	void updateLightSource(LightSourceList * _LightSource_Data, bool isShow = true);
	void updateSurface(SurfaceList * _SurfaceList_Data , bool isShow = true);
	void updateField(list<Field>  * _FieldList, bool isShow = true);

	void save(ofstream & saveFile) const;
	void open(ifstream & openFile);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void mousePressEvent(QMouseEvent*e);
	void mouseMoveEvent(QMouseEvent*e);
	void wheelEvent(QWheelEvent *e);

private:

	void drawGeometry();
	RoamingScenceManager * manager;

	//******Surface Date********
	SurfaceList * SurfaceList_Data;
	bool isSurfaceShow;

	//******SourecLight Date********
	bool isLightSourceShow;
	LightSourceList * LightSource_Data;

	bool isFieldShow;
	list<Field> * FieldList;
};

#endif // GLMAINWIDGET_H

