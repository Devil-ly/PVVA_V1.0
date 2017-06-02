#include "Qt/include/CalculationWidget.h"
#include <QMessageBox>
#include <QFileDialog>

CalculationWidget::CalculationWidget(QWidget *parent, int wayButton)
	: QDialog(parent)
{
	setWindowTitle(tr("Calculation"));
	
	//paraGroupBox
	freLabel = new QLabel(tr("Frequency(GHz)"));
	freLineEdit = new QLineEdit;
	freLineEdit->setText(tr("10"));

	QGridLayout * layoutpara = new QGridLayout;
	layoutpara->addWidget(freLabel, 0, 0);
	layoutpara->addWidget(freLineEdit, 0, 1);

	paraGroupBox = new QGroupBox;
	paraGroupBox->setTitle(tr("Parameters"));
	paraGroupBox->setLayout(layoutpara);

	//surfaceNumGroupBox
	surfaceLabel = new QLabel(tr("Num of Surfaces"));
	surfaceLineEdit = new QLineEdit;
	surfaceLineEdit->setText(tr("3"));

	connect(surfaceLineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(selectLineEditChange(QString)));

	QGridLayout * layoutSel = new QGridLayout;
	layoutSel->addWidget(surfaceLabel, 0, 0);
	layoutSel->addWidget(surfaceLineEdit, 0, 1);
	
	surfaceNumGroupBox = new QGroupBox;
	surfaceNumGroupBox->setTitle(tr("Add surfaces to calculate"));
	surfaceNumGroupBox->setLayout(layoutSel);

	//selectGroupBox
	NumSel = 3;
	selectlayout = new QGridLayout;
	selectCombobox = new QComboBox[NumSel];
	selectLabel = new QLabel[NumSel];
	for (int i = 0; i < NumSel; i++)
	{
		selectLabel[i].setText(tr("Surface ") + QString::number(i + 1));

		selectCombobox[i].addItems(allSurfaceName);
		selectlayout->addWidget(&selectLabel[i], i, 0);
		selectlayout->addWidget(&selectCombobox[i], i, 1);
	}

	selectGroupBox = new QGroupBox;
	selectGroupBox->setTitle(tr("Select surfaces"));
	selectGroupBox->setLayout(selectlayout);

	//resultGroupBox
	resultLabel = new QLabel(tr("Num of Surfaces"));
	resultCombobox = new QComboBox;
	resultCombobox->addItem(tr("Default"));
	resultCombobox->addItem(tr("Custom"));
	connect(resultCombobox, SIGNAL(activated(int)), this, SLOT(resultComboboxChange(int)));

	rectLabel = new QLabel(tr("Chose a Surface"));
	rectCombobox = new QComboBox;
	rectCombobox->setEnabled(false);

	QGridLayout * layoutRect = new QGridLayout;
	layoutRect->addWidget(resultLabel, 0, 0);
	layoutRect->addWidget(resultCombobox, 0, 1);
	layoutRect->addWidget(rectLabel, 1, 0);
	layoutRect->addWidget(rectCombobox, 1, 1);

	saveLabel = new QLabel(tr("Output address"));
	saveLineEdit = new QLineEdit;
	saveBtn = new QPushButton(tr("Browse..."));
	connect(saveBtn, SIGNAL(clicked()), this, SLOT(buttonSave()));

	QGridLayout * layoutSave = new QGridLayout;
	layoutSave->addWidget(saveLabel, 0, 0);
	layoutSave->addWidget(saveLineEdit, 0, 1);
	layoutSave->addWidget(saveBtn, 0, 2);

	QVBoxLayout * resultLayout = new QVBoxLayout;
	resultLayout->addLayout(layoutRect);
	resultLayout->addLayout(layoutSave);

	resultGroupBox = new QGroupBox;
	resultGroupBox->setTitle(tr("Result"));
	resultGroupBox->setLayout(resultLayout);

	QVBoxLayout * selectTabLayout = new QVBoxLayout;
	selectTabLayout->addWidget(paraGroupBox);
	selectTabLayout->addWidget(surfaceNumGroupBox);
	selectTabLayout->addWidget(selectGroupBox);
	selectTabLayout->addWidget(resultGroupBox);

	QWidget * selectWidget = new QWidget;
	selectWidget->setLayout(selectTabLayout);

	// page 2
	//surfaceNumGroupBox
	monitorNumLabel = new QLabel(tr("Num of Rectangles"));
	monitorNumLineEdit = new QLineEdit;
	monitorNumLineEdit->setText(tr("3"));

	connect(monitorNumLineEdit, SIGNAL(textChanged(QString)),
		this, SLOT(monitorNumLineEditChange(QString)));

	QGridLayout * layoutMonitorNum = new QGridLayout;
	layoutMonitorNum->addWidget(monitorNumLabel, 0, 0);
	layoutMonitorNum->addWidget(monitorNumLineEdit, 0, 1);

	monitorNumGroupBox = new QGroupBox;
	monitorNumGroupBox->setTitle(tr("Add rectangle to watch (only rectangle)"));
	monitorNumGroupBox->setLayout(layoutMonitorNum);

	//selectGroupBox
	NumMonitor = 3;
	monitorlayout = new QGridLayout;
	monitorCombobox = new QComboBox[NumSel];
	monitorLabel = new QLabel[NumSel];
	for (int i = 0; i < NumSel; i++)
	{
		monitorLabel[i].setText(tr("Rectangle ") + QString::number(i + 1));
		monitorCombobox[i].addItems(allSurfaceName);
		monitorlayout->addWidget(&monitorLabel[i], i, 0);
		monitorlayout->addWidget(&monitorCombobox[i], i, 1);
	}

	monitorGroupBox = new QGroupBox;
	monitorGroupBox->setTitle(tr("Select rectangles"));
	monitorGroupBox->setLayout(monitorlayout);

	QVBoxLayout * monitorTabLayout = new QVBoxLayout;
	monitorTabLayout->addWidget(monitorNumGroupBox);
	monitorTabLayout->addWidget(monitorGroupBox);

	QWidget * monitorWidget = new QWidget;
	monitorWidget->setLayout(monitorTabLayout);

	tabWidget = new QTabWidget;
	tabWidget->addTab(selectWidget, tr("Surfaces"));
	tabWidget->addTab(monitorWidget, tr("Monitors"));

	okBtn = new QPushButton(tr("OK"));
	cancelBtn = new QPushButton(tr("Cancel"));
	connect(okBtn, SIGNAL(clicked()), this, SLOT(buttonOk()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(buttonCancel()));
	QHBoxLayout * layoutbtn = new QHBoxLayout;
	layoutbtn->addSpacing(100);
	layoutbtn->addWidget(okBtn, 0);
	layoutbtn->addWidget(cancelBtn, 0);

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layoutbtn);

	returnNum = -1;
}

CalculationWidget::~CalculationWidget()
{
	delete[]selectCombobox;
	delete[]selectLabel;
	delete[]monitorCombobox;
	delete[]monitorLabel;
}

int CalculationWidget::Num()
{
	return returnNum;
}

void CalculationWidget::buttonCancel()
{
	close();
}

void CalculationWidget::buttonSave()
{
	QString filename = QFileDialog::getExistingDirectory(this, 
		"Select the path to save output...", "./");
	if (!filename.isEmpty())
	{
		saveLineEdit->setText(filename);
	}
}

void CalculationWidget::buttonOk()
{
	returnNum = 1;
	close();
}

void CalculationWidget::selectLineEditChange(const QString & text)
{
	bool ok;
	NumSel = text.toInt(&ok);
	if (ok)
	{
		delete[]selectCombobox;
		delete[]selectLabel;
		selectCombobox = new QComboBox[NumSel];
		selectLabel = new QLabel[NumSel];
		for (int i = 0; i < NumSel; i++)
		{
			selectLabel[i].setText(tr("Surface ") + QString::number(i + 1));
			selectCombobox[i].addItems(allSurfaceName);
			selectlayout->addWidget(&selectLabel[i], i, 0);
			selectlayout->addWidget(&selectCombobox[i], i, 1);
		}
	}
	else
	{
		QMessageBox::about(NULL, "Warning", "Invalid data, please enter number!");
		surfaceLineEdit->setText(QString::number(NumSel));
	}
}

void CalculationWidget::monitorNumLineEditChange(const QString & text)
{
	bool ok;
	NumMonitor = text.toInt(&ok);
	if (ok)
	{
		delete[]monitorCombobox;
		delete[]monitorLabel;
		monitorCombobox = new QComboBox[NumMonitor];
		monitorLabel = new QLabel[NumMonitor];
		for (int i = 0; i < NumMonitor; i++)
		{
			monitorLabel[i].setText(tr("Rectangle ") + QString::number(i + 1));
			monitorCombobox[i].addItems(allSurfaceName);
			monitorlayout->addWidget(&monitorLabel[i], i, 0);
			monitorlayout->addWidget(&monitorCombobox[i], i, 1);
		}
	}
	else
	{
		QMessageBox::about(NULL, "Warning", "Invalid data, please enter number!");
		monitorNumLineEdit->setText(QString::number(NumMonitor));
	}
}

void CalculationWidget::resultComboboxChange(int Num)
{
	if (Num == 0) 
	{
		rectCombobox->setEnabled(false);
	}
	else
	{
		rectCombobox->setEnabled(true);
	}
}

void CalculationWidget::addAllSurface(list<modleID> _allSurfaceID, QStringList _allSurfaceName)
{
	allSurfaceID = _allSurfaceID;
	allSurfaceName = _allSurfaceName;
	for (int i = 0; i < NumSel; i++)
	{
		selectCombobox[i].addItems(allSurfaceName);
	}
}

void CalculationWidget::addAllRectangles(list<modleID> _allRectangleID, QStringList _allRectangleName)
{
	allRectangleID = _allRectangleID; 
	allRectangleName = _allRectangleName;
	for (int i = 0; i < NumMonitor; i++)
	{
		monitorCombobox[i].addItems(allRectangleName);
	}
	rectCombobox->addItems(allRectangleName);
}

void CalculationWidget::getSelSurfaces(int &_NumSel, list<modleID> &_SelSurf)
{
	_NumSel = NumSel;
	_SelSurf.clear();
	for (int i = 0; i < NumSel; i++)
	{
		int tempNumSel = selectCombobox[i].currentIndex();
		list<modleID>::const_iterator iter = allSurfaceID.begin();
		for (int j = 0; j < tempNumSel; j++)
			iter++;
		_SelSurf.push_back(*iter);
	}
}

void CalculationWidget::getResRectangle(bool & Isdefualt, modleID &_SelRect)
{
	if (resultCombobox->currentIndex() == 0)
	{
		Isdefualt = true;
		return;
	}
	else
	{
		Isdefualt = false;
		int tempNumSel = rectCombobox->currentIndex();
		list<modleID>::const_iterator iter = allRectangleID.begin();
		for (int j = 0; j < tempNumSel; j++)
			iter++;
		_SelRect= *iter;		
	}
}

double CalculationWidget::getFre()
{
	return freLineEdit->text().toDouble();
}
