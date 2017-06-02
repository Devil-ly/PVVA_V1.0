#include "Qt/include/rectanglewidget.h"

RectangleWidget::RectangleWidget(QWidget *parent, int wayButton)
{
	setWindowTitle(tr("Create rectangle"));

	// page1
	//defGroupBox
	defGroupBox = new QGroupBox;
	addDefGroupBox(defGroupBox, "Qt/images/rectangle_coor.png");


	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);

	
	//dimGroupBox
	widthlabel = new QLabel(tr("Width(W)"));
	depthlabel = new QLabel(tr("Depth(D)"));

	widthLineEidt = new QLineEdit;
	depthLineEidt = new QLineEdit;

	widthLineEidt->setText(tr("50"));
	depthLineEidt->setText(tr("50"));

	QGridLayout * layout3= new QGridLayout;
	layout3->addWidget(widthlabel, 0, 0);
	layout3->addWidget(depthlabel, 1, 0);
	layout3->addWidget(widthLineEidt, 0, 1);
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
	QWidget * widget2 = new QWidget;
	addRotateWidget(widget2, "Qt/images/RotateWidget.png");


	tabWidget = new QTabWidget;
	tabWidget->addTab(widget1, tr("Geometry"));
	tabWidget->addTab(widget2, tr("Workplane"));


	QGridLayout * layout5 = new QGridLayout;
	addBtn(layout5, wayButton);

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layout5);

}

RectangleWidget::~RectangleWidget()
{
	
}

void RectangleWidget::setWidgetData(Surface const *  _surfacePtr)
{
	widthLineEidt->setText(QString::number(_surfacePtr->getPara()[0]));
	depthLineEidt->setText(QString::number(_surfacePtr->getPara()[1]));
	setGraphTransData(_surfacePtr->getGraphTrans());
	setGraphTransName(_surfacePtr->getName());
}

bool RectangleWidget::getWidgetData(Surface &_surface) const
{
	bool ok, ok_back;
	vector <float> para;
	para.push_back(widthLineEidt->text().toFloat(&ok_back));
	para.push_back(depthLineEidt->text().toFloat(&ok));

	ok = ok && ok_back;
	GraphTrans graphTransPara = getGraphTransData(ok_back);
	ok = ok && ok_back;
	QString name = getGraphTransName();
	_surface.setSurfaceData(para, graphTransPara, name);
	return ok;
}
