#include "Qt/include/LightSourceWidget.h"
#include <QMessageBox>

LightSourceWidget::LightSourceWidget(QWidget *parent, int wayButton)

{
	setWindowTitle(tr("Create LightSource"));

	// page1
	//defGroupBox
	combobox = new QComboBox;
	combobox->addItem(tr("Rectangle"));
	combobox->addItem(tr("Circle"));
	combobox->addItem(tr("Point"));
	
	connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboChange(int)));

	imgLlabel = new QLabel;
	imgLlabel->setPixmap(QPixmap("Qt/images/rectangle_coor.png"));

	QGridLayout * layout1 = new QGridLayout;
	layout1->addWidget(combobox);
	layout1->addWidget(imgLlabel);

	defGroupBox = new QGroupBox;
	defGroupBox->setLayout(layout1);
	defGroupBox->setTitle(tr("Definition methods"));

	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);
	
	//dimGroupBox
	radiuslabel = new QLabel(tr("Width(W)"));
	depthlabel = new QLabel(tr("Depth(D)"));

	widthLineEidt = new QLineEdit;
	depthLineEidt = new QLineEdit;

	widthLineEidt->setText(tr("50"));
	depthLineEidt->setText(tr("50"));

	QGridLayout * layout3= new QGridLayout;
	layout3->addWidget(radiuslabel, 0, 0);
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
	//imgGroupBox
	QWidget * widget2;
	widget2 = new QWidget;
	addRotateWidget(widget2, "Qt/images/RotateWidget.png");

	//page3
	//img2GroupBox
	img2GroupBox = new QGroupBox;
	addDefGroupBox(img2GroupBox, "Qt/images/rectangle_coor.png");

	//LightNumGroupBox
	var1label = new QLabel(tr("Num of points in a"));
	var2label = new QLabel(tr("Num of points in b"));

	var1LineEidt = new QLineEdit;
	var2LineEidt = new QLineEdit;

	var1LineEidt->setText(tr("10"));
	var2LineEidt->setText(tr("10"));

	QGridLayout * layout9 = new QGridLayout;
	layout9->addWidget(var1label, 0, 0);
	layout9->addWidget(var2label, 1, 0);
	layout9->addWidget(var1LineEidt, 0, 1);
	layout9->addWidget(var2LineEidt, 1, 1);

	LightNumGroupBox = new QGroupBox;
	LightNumGroupBox->setTitle(tr("Ray numbers"));
	LightNumGroupBox->setLayout(layout9);

	// lengthGroupBox
	lengthlabel = new QLabel(tr("length"));

	lengthLineEidt = new QLineEdit;
	lengthLineEidt->setText(tr("100"));

	QGridLayout * layoutlength = new QGridLayout;
	layoutlength->addWidget(lengthlabel, 0, 0);
	layoutlength->addWidget(lengthLineEidt, 0, 1);
	
	lengthGroupBox = new QGroupBox;
	lengthGroupBox->setTitle(tr("Length of ray after reflection"));
	lengthGroupBox->setLayout(layoutlength);

	QVBoxLayout * tabLayout3;
	tabLayout3 = new QVBoxLayout;
	tabLayout3->addWidget(img2GroupBox);
	tabLayout3->addWidget(LightNumGroupBox);
	tabLayout3->addWidget(lengthGroupBox);

	QWidget * widget3;
	widget3 = new QWidget;
	widget3->setLayout(tabLayout3);

	//page4
	//img3GroupBox
	img3GroupBox = new QGroupBox;
	addDefGroupBox(img3GroupBox, "Qt/images/LightSource_SelSurface.png");

	//surfaceNumGroupBox
	surfaceNumlabel = new QLabel(tr("Num of Surfaces"));
	selectLineEidt = new QLineEdit;
	selectLineEidt->setText(tr("3"));

	connect(selectLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(selectLineEidtChange(QString)));

	QGridLayout * layout10 = new QGridLayout;
	layout10->addWidget(surfaceNumlabel, 0, 0);
	layout10->addWidget(selectLineEidt, 0, 1);

	surfaceNumGroupBox = new QGroupBox;
	surfaceNumGroupBox->setTitle(tr("Add surfaces to trace"));
	surfaceNumGroupBox->setLayout(layout10);
	
	//selectGroupBox
	NumSel = 3;
	selectlayout = new QGridLayout;
	selectcombobox = new QComboBox[NumSel];
	selectlabel = new QLabel[NumSel];
	for (int i = 0; i < NumSel; i++)
	{
		selectlabel[i].setText(tr("Surface ") + QString::number(i + 1));
	
		selectcombobox[i].addItems(allSurfaceName);
		selectlayout->addWidget(&selectlabel[i], i, 0);
		selectlayout->addWidget(&selectcombobox[i], i, 1);
	}

	selectGroupBox = new QGroupBox;
	selectGroupBox->setTitle(tr("Select surfaces"));
	selectGroupBox->setLayout(selectlayout);

	selectTabLayout = new QVBoxLayout;
	selectTabLayout->addWidget(img3GroupBox);
	selectTabLayout->addWidget(surfaceNumGroupBox);
	selectTabLayout->addWidget(selectGroupBox);

	selectWidget = new QWidget;
	selectWidget->setLayout(selectTabLayout);

	tabWidget = new QTabWidget;
	tabWidget->addTab(widget1, tr("Geometry"));
	tabWidget->addTab(widget2, tr("Workplane"));
	tabWidget->addTab(widget3, tr("Rays"));
	tabWidget->addTab(selectWidget, tr("Surfaces"));

	QGridLayout * layoutbt = new QGridLayout;
	addBtn(layoutbt, wayButton);

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layoutbt); 

}

LightSourceWidget::~LightSourceWidget()
{
	delete[]selectcombobox;
	delete[]selectlabel;
}

void LightSourceWidget::comboChange(int index)
{
	if (index == 1)
	{
		radiuslabel->setText(tr("Radius(R)"));
		depthlabel->hide();
		depthLineEidt->hide();
	}
	else if (index == 0)
	{
		radiuslabel->setText(tr("Width(W)"));
		depthlabel->setText(tr("Depth(D)"));
		depthlabel->show();
		depthLineEidt->show();
	}
	else
	{
		radiuslabel->setText(tr("Angle(A)"));
		widthLineEidt->setText(tr("30"));
		depthlabel->hide();
		depthLineEidt->hide();
	}
}

void LightSourceWidget::setWidgetData(const LightSource & _LightSource)
{
	float a; float b; int Num_a; int Num_b; GraphTrans graphTransPara;
	float finalLength; int judgeNum; QString name; int _NumSel;
	_LightSource.getData(a, b, Num_a, Num_b, graphTransPara, finalLength, judgeNum, name, _NumSel);
	combobox->setCurrentIndex(judgeNum);

	widthLineEidt->setText(QString::number(a));
	depthLineEidt->setText(QString::number(b));

	var1LineEidt->setText(QString::number(Num_a));
	var2LineEidt->setText(QString::number(Num_b));

	lengthLineEidt->setText(QString::number(finalLength));

	setGraphTransData(graphTransPara);
	setGraphTransName(name);
	setSelSurfaces(_NumSel);
}

bool LightSourceWidget::getWidgetData(LightSource & _LightSource)
{
	float a; float b; int Num_a; int Num_b; GraphTrans graphTransPara;
	float finalLength; int judgeNum; QString name;
	bool ok, ok_back;

	a = widthLineEidt->text().toFloat(&ok);
	b = depthLineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;

	Num_a = var1LineEidt->text().toInt(&ok_back);
	ok = ok && ok_back;
	Num_b = var2LineEidt->text().toInt(&ok_back);
	ok = ok && ok_back;

	graphTransPara = getGraphTransData(ok_back);
	ok = ok && ok_back;

	judgeNum = combobox->currentIndex();

	finalLength = lengthLineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;

	name = getGraphTransName();

	list<modleID> _SelSurf;
	for (int i = 0; i < NumSel; i++)
	{
		int tempNumSel = selectcombobox[i].currentIndex();
		list<modleID>::const_iterator iter = allSurfaceID.begin();
		for (int j = 0; j < tempNumSel; j++)
			iter++;
		_SelSurf.push_back(*iter);
	}

	_LightSource.setData(a, b, Num_a, Num_b, graphTransPara, judgeNum, name, finalLength);
	_LightSource.setSelSurfaces(_SelSurf);
	return ok;
}

void LightSourceWidget::addAllSurface(list<modleID> _allSurfaceID, QStringList _allSurfaceName)
{
	allSurfaceID = _allSurfaceID;
	allSurfaceName = _allSurfaceName;
	for (int i = 0; i < NumSel; i++)
	{
		selectcombobox[i].addItems(allSurfaceName);
	}
}

void LightSourceWidget::getSelSurfaces(int &_NumSel, list<modleID> &_SelSurf)
{
	_NumSel = NumSel;
	for (int i = 0; i < NumSel; i++)
	{
		int tempNumSel = selectcombobox[i].currentIndex();
		list<modleID>::const_iterator iter = allSurfaceID.begin();
		for (int j = 0; j < tempNumSel; j++)
			iter++;
		_SelSurf.push_back(*iter);
	}
	
}

void LightSourceWidget::setSelSurfaces(int _NumSel)
{
	NumSel = _NumSel;
	delete[]selectcombobox;
	delete[]selectlabel;
	selectcombobox = new QComboBox[_NumSel];
	selectlabel = new QLabel[_NumSel];
	for (int i = 0; i < _NumSel; i++)
	{
		selectlabel[i].setText(tr("Surface ") + QString::number(i + 1));
		selectcombobox[i].addItems(allSurfaceName);
		selectlayout->addWidget(&selectlabel[i], i, 0);
		selectlayout->addWidget(&selectcombobox[i], i, 1);
	}
	selectLineEidt->setText(QString::number(_NumSel));
}

void LightSourceWidget::selectLineEidtChange(const QString & text)
{
	bool ok;
	NumSel = text.toInt(&ok);
	if (ok)
	{
		delete[]selectcombobox;
		delete[]selectlabel;
		selectcombobox = new QComboBox[NumSel];
		selectlabel = new QLabel[NumSel];
		for (int i = 0; i < NumSel; i++)
		{
			selectlabel[i].setText(tr("Surface ") + QString::number(i + 1));
			selectcombobox[i].addItems(allSurfaceName);
			selectlayout->addWidget(&selectlabel[i], i, 0);
			selectlayout->addWidget(&selectcombobox[i], i, 1);
		}
	}
	else
	{
		QMessageBox::about(NULL, "Warning", "Invalid data, please enter number!");
	}	
}
