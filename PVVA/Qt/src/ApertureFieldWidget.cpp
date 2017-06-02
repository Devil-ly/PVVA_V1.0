#include "Qt/include/ApertureFieldWidget.h"
#include <QFileDialog>

ApertureFieldWidget::ApertureFieldWidget(QWidget *parent, int wayButton)
{
	setWindowTitle(tr("Create Aperture-Field"));

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

	// page3
	imgGroupBox = new QGroupBox;
	addDefGroupBox(imgGroupBox, "Qt/images/ApertureField_def.png");

	scalelabel   = new QLabel(tr("Magnitude scale factor"));
	phaselabel   = new QLabel(tr("Phase of aperture (deg)"));
	datadeflabel = new QLabel(tr("Aperture data definition"));

	scaleLineEidt = new QLineEdit;
	phaseLineEidt = new QLineEdit;

	scaleLineEidt->setText(tr("1"));
	phaseLineEidt->setText(tr("0"));

	datadefComboBox = new QComboBox;
	datadefComboBox->addItem(tr("Electric field"));
	datadefComboBox->addItem(tr("Magnetic field"));
	datadefComboBox->addItem(tr("Electric and magnetic field"));
	connect(datadefComboBox, SIGNAL(activated(int)), this, SLOT(setDataDef(int)));

	QGridLayout * layout5 = new QGridLayout;
	layout5->addWidget(scalelabel, 0, 0);
	layout5->addWidget(phaselabel, 1, 0);
	layout5->addWidget(datadeflabel, 2, 0);
	layout5->addWidget(scaleLineEidt, 0, 1);
	layout5->addWidget(phaseLineEidt, 1, 1);
	layout5->addWidget(datadefComboBox, 2, 1);

	//sourcegroup
	Efieldlabel = new QLabel(tr("E-field file"));
	Hfieldlabel = new QLabel(tr("H-field file"));
	StartNumlabel = new QLabel(tr("Start reading from line number"));

	EfieldLineEidt = new QLineEdit;
	HfieldLineEidt = new QLineEdit;
	StartNumLineEidt = new QLineEdit;

	StartNumLineEidt->setText(tr("1"));

	Efieldbtn = new QPushButton(tr("Browse..."));
	connect(Efieldbtn, SIGNAL(clicked()), this, SLOT(EfileEeven()));
	Hfieldbtn = new QPushButton(tr("Browse..."));
	connect(Hfieldbtn, SIGNAL(clicked()), this, SLOT(HfileEeven()));

	EfieldLineEidt->setEnabled(true);
	HfieldLineEidt->setEnabled(false);
	Efieldbtn->setEnabled(true);
	Hfieldbtn->setEnabled(false);

	QGridLayout * layout6 = new QGridLayout;
	layout6->addWidget(Efieldlabel, 0, 0);
	layout6->addWidget(Hfieldlabel, 1, 0);
	layout6->addWidget(EfieldLineEidt, 0, 1);
	layout6->addWidget(HfieldLineEidt, 1, 1);
	layout6->addWidget(Efieldbtn, 0, 2);
	layout6->addWidget(Hfieldbtn, 1, 2);

	QGridLayout * layout7 = new QGridLayout;
	layout7->addWidget(StartNumlabel, 0, 0);
	layout7->addWidget(StartNumLineEidt, 0, 1);

	QVBoxLayout * tabLayoutSource = new QVBoxLayout;
	tabLayoutSource->addLayout(layout6);
	tabLayoutSource->addLayout(layout7);

	sourceGroupBox = new QGroupBox;
	sourceGroupBox->setTitle(tr("Source"));
	sourceGroupBox->setLayout(tabLayoutSource);

	//destinationGroupBox;
	UNumlabel = new QLabel(tr("Number of points along U"));
	VNumlabel = new QLabel(tr("Number of points along V"));

	UNumLineEidt = new QLineEdit;
	VNumLineEidt = new QLineEdit;

	QGridLayout * layout8 = new QGridLayout;
	layout8->addWidget(UNumlabel, 0, 0);
	layout8->addWidget(VNumlabel, 1, 0);
	layout8->addWidget(UNumLineEidt, 0, 1);
	layout8->addWidget(VNumLineEidt, 1, 1);

	destinationGroupBox = new QGroupBox;
	destinationGroupBox->setTitle(tr("Source destination"));
	destinationGroupBox->setLayout(layout8);

	//tabLayout2
	QVBoxLayout * tabLayout2; // page1
	tabLayout2 = new QVBoxLayout;
	tabLayout2->addWidget(imgGroupBox);
	tabLayout2->addLayout(layout5);
	tabLayout2->addWidget(sourceGroupBox);
	tabLayout2->addWidget(destinationGroupBox);

	QWidget * widget3 = new QWidget;
	widget3->setLayout(tabLayout2);

	tabWidget = new QTabWidget;
	tabWidget->addTab(widget1, tr("Geometry"));
	tabWidget->addTab(widget2, tr("Workplane"));
	tabWidget->addTab(widget3, tr("Excitation"));


	QGridLayout * layoutbtn = new QGridLayout;
	addBtn(layoutbtn, wayButton);

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layoutbtn);

}

ApertureFieldWidget::~ApertureFieldWidget()
{
	
}

void ApertureFieldWidget::setWidgetData(ApertureField const * _ApertureFieldPtr)
{
	vector<float> _para; GraphTrans _graphTransPara; float _factor; float _phase;
	QString _EfieldFile; QString _HfieldFile; int _StartNum; int _NumU; int _NumV; 
	int _indexEHfield; QString _name;
	_ApertureFieldPtr->getApertureField(_para, _graphTransPara, _factor, _phase, 
		_EfieldFile, _HfieldFile, _StartNum, _NumU, _NumV, _indexEHfield, _name);
	widthLineEidt->setText(QString::number(_para[0]));
	depthLineEidt->setText(QString::number(_para[1]));
	setGraphTransData(_graphTransPara);
	scaleLineEidt->setText(QString::number(_factor));
	phaseLineEidt->setText(QString::number(_phase));
	EfieldLineEidt->setText(_EfieldFile);
	HfieldLineEidt->setText(_HfieldFile);
	phaseLineEidt->setText(QString::number(_StartNum));
	UNumLineEidt->setText(QString::number(_NumU));
	VNumLineEidt->setText(QString::number(_NumV));
	setGraphTransName(_name);
}

bool ApertureFieldWidget::getWidgetData(ApertureField * _ApertureFieldPtr) const
{
	bool ok, ok_back;
	vector <float> para;
	para.push_back(widthLineEidt->text().toFloat(&ok_back));
	para.push_back(depthLineEidt->text().toFloat(&ok));
	ok = ok && ok_back;
	GraphTrans graphTransPara = getGraphTransData(ok_back);
	ok = ok && ok_back;

	float factor = scaleLineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;

	float phase = phaseLineEidt->text().toFloat(&ok_back);
	ok = ok && ok_back;

	QString EfieldFile = EfieldLineEidt->text();
	QString HfieldFile = HfieldLineEidt->text();

	int StartNum = StartNumLineEidt->text().toInt(&ok_back);
	ok = ok && ok_back;

	int NumU = UNumLineEidt->text().toInt(&ok_back);
	ok = ok && ok_back;

	int NumV = VNumLineEidt->text().toInt(&ok_back);
	ok = ok && ok_back;

	int indexEHfield = datadefComboBox->currentIndex();
	
	QString name = getGraphTransName();
	_ApertureFieldPtr->setApertureField(para, graphTransPara, factor, phase, EfieldFile,
		HfieldFile, StartNum, NumU, NumV, indexEHfield, name);
	return ok;
}

void ApertureFieldWidget::EfileEeven()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open the file"),
		"",
		tr("*.txt"));
	if (!filename.isEmpty())
	{
		EfieldLineEidt->setText(filename);
	}
}

void ApertureFieldWidget::HfileEeven()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open the file"),
		"",
		tr("*.txt"));
	if (!filename.isEmpty())
	{
		HfieldLineEidt->setText(filename);
	}
}

void ApertureFieldWidget::setDataDef(int Num)
{
	if (Num == 0)
	{
		EfieldLineEidt->setEnabled(true);
		HfieldLineEidt->setEnabled(false);
		Efieldbtn->setEnabled(true);
		Hfieldbtn->setEnabled(false);
	}
	else if (Num == 1)
	{
		EfieldLineEidt->setEnabled(false);
		HfieldLineEidt->setEnabled(true);
		Efieldbtn->setEnabled(false);
		Hfieldbtn->setEnabled(true);
	}
	else
	{
		EfieldLineEidt->setEnabled(true);
		HfieldLineEidt->setEnabled(true);
		Efieldbtn->setEnabled(true);
		Hfieldbtn->setEnabled(true);
	}
}
