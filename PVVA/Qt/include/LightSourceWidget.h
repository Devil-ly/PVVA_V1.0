#ifndef LIGHTSOURCEWIDGET_H
#define LIGHTSOURCEWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <list>

#include "Qt/include/GraphTransWidget.h"
#include "openGL/include/LightSource.h"
#include "tool/ModleID.h"
using namespace std;

class LightSourceWidget : public GraphTransWidget
{
	Q_OBJECT
public:
	LightSourceWidget(QWidget *parent = 0, int wayButton = 0);
	~LightSourceWidget();

	//set and get Data
	void setWidgetData(const LightSource  &_LightSource);
	bool getWidgetData(LightSource & _LightSource);

	void addAllSurface(list<modleID> _allSurfaceID, QStringList _allSurfaceName);

	//set and get 选择面的数据
	void getSelSurfaces(int &_NumSel, list<modleID> &_SelSurf);
	void setSelSurfaces(int _NumSel);

private slots:
    void comboChange(int index);
	void selectLineEidtChange(const QString & text);


private:

	//page1
	QTabWidget * tabWidget;
	QGroupBox * defGroupBox;
	QGroupBox * baseGroupBox;
	QGroupBox * dimGroupBox;

	QComboBox * combobox;

	QLabel * imgLlabel;

	QLabel * radiuslabel;
	QLabel * depthlabel;

	QLineEdit * widthLineEidt;
	QLineEdit * depthLineEidt;

	//page3
	QGroupBox * img2GroupBox;
	QGroupBox * LightNumGroupBox;
	QGroupBox * lengthGroupBox;

	QLabel * imgLlabel2;

	QLabel * var1label;
	QLabel * var2label;

	QLineEdit * var1LineEidt;
	QLineEdit * var2LineEidt;

	QLabel * lengthlabel;
	QLineEdit * lengthLineEidt;

	//page4
	QGroupBox * img3GroupBox;
	QGroupBox * surfaceNumGroupBox;
	QLabel *  surfaceNumlabel;

	QGroupBox * selectGroupBox;
	QGridLayout * selectlayout;

	QLabel *  selectlabel;
	QLineEdit * selectLineEidt;
	QComboBox * selectcombobox;

	QVBoxLayout * selectTabLayout;
	QWidget * selectWidget;
	//QComboBox * combobox;

	list<modleID> allSurfaceID; // 所有面数据的代号
	QStringList allSurfaceName; // 所有面数据的代号

	int NumSel;
};

#endif // LightSourceWidget_H
