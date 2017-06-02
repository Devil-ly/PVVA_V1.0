#ifndef ParaboloidWidget_H
#define ParaboloidWidget_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>

#include "Qt/include/GraphTransWidget.h"
#include "openGL/include/Surface.h"

class ParaboloidWidget : public GraphTransWidget
{
public:
	ParaboloidWidget(QWidget *parent = 0, int wayButton = 0);
	~ParaboloidWidget();

	void setWidgetData(Surface const * _surfacePtr);
	bool getWidgetData(Surface & _surface) const;

private:
	//page1
	QTabWidget * tabWidget;
	QGroupBox * defGroupBox;
	QGroupBox * baseGroupBox;
	QGroupBox * dimGroupBox;

	QLabel * radiuslabel;
	QLabel * depthlabel;

	QLineEdit * radiusLineEidt;
	QLineEdit * depthLineEidt;

	QLabel * label;
	QLineEdit * nameLineEidt;
};

#endif // ParaboloidWidget_H
