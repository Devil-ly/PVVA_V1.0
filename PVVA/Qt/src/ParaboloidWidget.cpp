#include "Qt/include/ParaboloidWidget.h"
#include <vector>

ParaboloidWidget::ParaboloidWidget(QWidget *parent, int wayButton)
{
	setWindowTitle(tr("Create paraboloid"));

	defGroupBox = new QGroupBox;
	addDefGroupBox(defGroupBox, "Qt/images/Paraboloid_coor.png");

	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);

	//dimGroupBox
	radiuslabel = new QLabel(tr("Radius(R)"));
	depthlabel = new QLabel(tr("Focal depth(F)"));

	radiusLineEidt = new QLineEdit;
	depthLineEidt = new QLineEdit;

	radiusLineEidt->setText(tr("1000.0"));
	depthLineEidt->setText(tr("500.0"));

	QGridLayout * layout3 = new QGridLayout;
	layout3->addWidget(radiuslabel, 0, 0);
	layout3->addWidget(depthlabel, 1, 0);
	layout3->addWidget(radiusLineEidt, 0, 1);
	layout3->addWidget(depthLineEidt, 1, 1);

	dimGroupBox = new QGroupBox;
	dimGroupBox->setTitle(tr("Dimensions"));
	dimGroupBox->setLayout(layout3);

	// label
	QGridLayout * layout4 = new QGridLayout;
	addNameLabel(layout4);


	//tabLayout1
	QVBoxLayout * tabLayout1; // page1
	tabLayout1 = new QVBoxLayout;
	tabLayout1->addWidget(defGroupBox);
	tabLayout1->addWidget(baseGroupBox);
	tabLayout1->addWidget(dimGroupBox);
	tabLayout1->addLayout(layout4);

	QWidget * widget1 = new QWidget;
	widget1->setLayout(tabLayout1);

	// page2
	//imgGroupBox
	QWidget * widget2;
	widget2 = new QWidget;
	addRotateWidget(widget2, "Paraboloid_coor.png");

	tabWidget = new QTabWidget;
	tabWidget->addTab(widget1, tr("Geometry"));
	tabWidget->addTab(widget2, tr("Workplane"));
	QGridLayout * layoutbt = new QGridLayout;

	addBtn(layoutbt, wayButton);

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layoutbt);

}

ParaboloidWidget::~ParaboloidWidget()
{

}

void ParaboloidWidget::setWidgetData(Surface const *  _surfacePtr)
{
	radiusLineEidt->setText(QString::number(_surfacePtr->getPara()[1]));
	depthLineEidt->setText(QString::number(_surfacePtr->getPara()[0]));
	setGraphTransData(_surfacePtr->getGraphTrans());
	setGraphTransName(_surfacePtr->getName());
}

bool ParaboloidWidget::getWidgetData(Surface & _surface) const
{
	bool ok, ok_back;
	vector <float> para;
	para.push_back(depthLineEidt->text().toFloat(&ok_back));
	para.push_back(radiusLineEidt->text().toFloat(&ok));

	ok = ok && ok_back;
	GraphTrans graphTransPara = getGraphTransData(ok_back);
	ok = ok && ok_back;
	QString name = getGraphTransName();
	_surface.setSurfaceData(para, graphTransPara, name);
	return ok;
}

