#include "Qt/include/GLMainWidget.h"
#include <gl/glut.h>
#include"openGL/include/RoamingScenceManager.h"

#include<QMouseEvent>
#include<QWheelEvent>
#include<QDebug>

GLMainWidget::GLMainWidget(QGLWidget * parent) : QGLWidget(parent) {
	this->setFocusPolicy(Qt::StrongFocus);
	manager = new RoamingScenceManager();
	isLightSourceShow = false;
	isSurfaceShow = false;
	isFieldShow = false;
}

GLMainWidget::~GLMainWidget() {
	delete manager;
}

void GLMainWidget::init()
{
	manager->init();
}

void GLMainWidget::setView(int NumView)
{
	manager->setView(NumView);
}

void GLMainWidget::updateLightSource(LightSourceList * _LightSource_Data, bool isShow)
{
	LightSource_Data = _LightSource_Data;
	isLightSourceShow = isShow;
}

void GLMainWidget::updateSurface(SurfaceList  * _SurfaceList_Data, bool isShow)
{
	SurfaceList_Data = _SurfaceList_Data;
	isSurfaceShow = isShow;
}

void GLMainWidget::updateField(list<Field>  * _FieldList, bool isShow)
{
	FieldList = _FieldList;
	isFieldShow = isShow;
}

void GLMainWidget::initializeGL()
{
	manager->init();

}

void GLMainWidget::resizeGL(int width, int height)
{
	glViewport(0, -(width - height) / 2, width, width);

}
void GLMainWidget::paintGL()
{
	manager->render();
	drawGeometry();

}

void GLMainWidget::mousePressEvent(QMouseEvent *e)
{
	manager->getInitPos(e->x(), e->y());
}

void GLMainWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (e->buttons()&Qt::MiddleButton)
	{
		manager->executeTranslateOperation(e->x(), e->y());		

	}
	else if (e->buttons()&Qt::LeftButton)
	{
		manager->executeRotateOperation(e->x(), e->y());
	}
	updateGL();
}

void GLMainWidget::wheelEvent(QWheelEvent *e)
{
	if (e->delta() >= 0)
	{
		manager->executeScaleOperation(-0.1);
	}
	else
	{
		manager->executeScaleOperation(0.1);
	}
	updateGL();
}

void GLMainWidget::drawGeometry()
{

	if (isLightSourceShow)
		LightSource_Data->draw();
	
	if(isSurfaceShow)
		SurfaceList_Data->draw();

	if (isFieldShow)
	{
		list<Field>::const_iterator it;
		for (it = FieldList->begin(); it != FieldList->end(); it++)
			it->draw();
	}
	
}

void GLMainWidget::save(ofstream & saveFile) const
{
	manager->save(saveFile);
}

void GLMainWidget::open(ifstream & openFile)
{
	manager->open(openFile);
}
