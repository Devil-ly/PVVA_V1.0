#include "Qt/include/MainWindow.h"
#include "Qt/include/rectanglewidget.h"
#include "Qt/include/ParaboloidWidget.h"
#include "Qt/include/LightSourceWidget.h"
#include "Qt/include/ApertureFieldWidget.h"
#include "Qt/include/GaussianWidget.h"
#include "Qt/include/UnitWidget.h"
#include "Qt/include/CalculationWidget.h"

#include "calculation/include/PVVA.h"

#include <QMessageBox>
#include <QFileDialog>  
#include <fstream>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle(tr("MainWindow-V1.0"));

	resize(1200, 800);

	glMainWidget = new GLMainWidget;
	setCentralWidget(glMainWidget);

	// ����dock����
	leftWidget = new QDockWidget("Navigation", this);
	leftWidget->setFeatures(QDockWidget::DockWidgetMovable);
	leftWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	//leftWidget->setWindowFlags(Qt::FramelessWindowHint);
	addDockWidget(Qt::LeftDockWidgetArea, leftWidget);

	// treeWidget
	treeWidget = new QTreeWidget;
	treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(treeWidget, SIGNAL(customContextMenuRequested(QPoint)),
		this, SLOT(on_treeWidget_ContextMenuRequested(QPoint)));
	treeWidget->setHeaderHidden(true);  // ���ر�ͷ
	leftWidget->setWidget(treeWidget);

	//treeWidget->setStyleSheet("QTreeView::branch {image:none;}"); 
	//QTreeWidgetItem *imageItem1 = new QTreeWidgetItem(treeWidget, QStringList(QString("test1")));
	//QTreeWidgetItem *imageItem2 = new QTreeWidgetItem(imageItem1, QStringList(QString("test2")));

	createTreeWidgetItem();
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	createRightMenu();
	init();

}

MainWindow::~MainWindow()
{
	
}

void MainWindow::init()
{
	glMainWidget->init();
   
	paraboloidNum = 0;
	LightSourceNum = 0;
	rectangleNum = 0;
	FieldNum = 0;

	SurfaceList_Data.clear();
	LightSource_Data.clear();

	isSet = 0; // �����޸�

	updateTreeWidgetItem();

	Unit_factor = 0.001;
}

void MainWindow::createActions()
{
	//**************�˵�*****************
	//����
	saveFileAction = new QAction(QIcon(tr("Qt/images/save.png")), tr("Save"), this);
	saveFileAction->setShortcut(tr("Ctrl +��s"));
	saveFileAction->setStatusTip(tr("Save a file"));
	connect(saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()));

	//��
	openFileAction = new QAction(QIcon(tr("Qt/images/open.png")), tr("Open"), this);
	openFileAction->setShortcut(tr("Ctrl +��O"));
	openFileAction->setStatusTip(tr("Open a file"));
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));

	//�½�
	newFileAction = new QAction(QIcon(tr("Qt/images/new.png")), tr("New"), this);
	newFileAction->setShortcut(tr("Ctrl +��N"));
	newFileAction->setStatusTip(tr("New a file"));
	connect(newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));

	//��ʼ���ӽ�
	viewAction = new QAction(QIcon(tr("Qt/images/view.png")), tr("View"), this);
	viewAction->setStatusTip(tr("Initialize the view"));
	connect(viewAction, SIGNAL(triggered()), this, SLOT(viewInitFile()));

	//�޸��ӽ�
	viewLabel = new QLabel(tr("View"));
	viewComboBox = new QComboBox;
	viewComboBox->addItem("View YZ plane(original)");
	viewComboBox->addItem("View XZ plane");
	viewComboBox->addItem("View XY plane");
	viewComboBox->addItem("View -YZ plane");
	viewComboBox->addItem("View -XZ plane");
	viewComboBox->addItem("View -XY plane");
	connect(viewComboBox, SIGNAL(activated(int)), this, SLOT(setView(int)));

	//**************ģ��*****************
	//����
	rectangleAction = new QAction(QIcon(tr("Qt/images/rectangle.png")), tr("Rectangle"), this);
	rectangleAction->setStatusTip(tr("Create a rectangle"));
	connect(rectangleAction, SIGNAL(triggered()), this, SLOT(createRectangle()));

	//�����
	polygonAction = new QAction(QIcon(tr("Qt/images/polygon.png")), tr("Polygon"), this);
	polygonAction->setStatusTip(tr("Create a polygon"));

	//������
	paraboloidAction = new QAction(QIcon(tr("Qt/images/paraboloid.png")), tr("Paraboloid"), this);
	paraboloidAction->setStatusTip(tr("Create a paraboloid"));
	connect(paraboloidAction, SIGNAL(triggered()), this, SLOT(createParaboloid()));

	//����׷��Դ
	LightSourceAction = new QAction(QIcon(tr("Qt/images/LightSource.png")), tr("LightSource"), this);
	LightSourceAction->setStatusTip(tr("Create a LightSource"));
	connect(LightSourceAction, SIGNAL(triggered()), this, SLOT(createLightSource()));

	//���泡Դ
	ApertureFieldAction = new QAction(QIcon(tr("Qt/images/ApertureField.png")), tr("Aperture-Field"), this);
	ApertureFieldAction->setStatusTip(tr("Create a Aperture-Field source"));
	connect(ApertureFieldAction, SIGNAL(triggered()), this, SLOT(createApertureField()));

	//��˹Դ
	GaussianAction = new QAction(QIcon(tr("Qt/images/Gaussian.png")), tr("Gaussian"), this);
	GaussianAction->setStatusTip(tr("Create a Gaussian source"));
	connect(GaussianAction, SIGNAL(triggered()), this, SLOT(createGaussian()));

	//����
	CalculationAction = new QAction(QIcon(tr("Qt/images/calculation.png")), tr("Calculate"), this);
	CalculationAction->setStatusTip(tr("Calculate"));
	connect(CalculationAction, SIGNAL(triggered()), this, SLOT(createCalculation()));

}

void MainWindow::createStatusBar()
{
	locationLabel = new QLabel("    ");
	locationLabel->setAlignment(Qt::AlignHCenter);
	locationLabel->setMinimumSize(locationLabel->sizeHint());
	this->statusBar()->addWidget(locationLabel,90);

	unitBtn = new QPushButton(tr("Unit: mm"));
	connect(unitBtn, SIGNAL(clicked()), this, SLOT(changeUnit()));
	//connect(unitBtn, SIGNAL(clicked()), this, SLOT(changeUnit()));
	this->statusBar()->addWidget(unitBtn);
}

void MainWindow::createMenus()
{
	// �ļ��˵�
	//this->menuBar()
	fileMenu = this->menuBar()->addMenu(tr("Files"));
	
	fileMenu->addAction(saveFileAction);
	fileMenu->addAction(openFileAction);
	fileMenu->addAction(newFileAction);
	fileMenu->addSeparator();
	//fileMenu->addAction(LightSourceAction);

}

void MainWindow::createToolBars()
{
	//file ��
	fileTool = addToolBar("Files");
	fileTool->addAction(saveFileAction);
	fileTool->addAction(openFileAction);
	fileTool->addAction(newFileAction);
	fileTool->addSeparator();
	fileTool->addAction(viewAction);
	fileTool->addSeparator();
	fileTool->addWidget(viewLabel);
	fileTool->addWidget(viewComboBox);
	//fileTool->addAction(LightSourceAction);

	// ģ����
	constructTool = addToolBar("Construct");
	constructTool->addAction(rectangleAction);
	constructTool->addAction(polygonAction);
	constructTool->addAction(paraboloidAction);
	constructTool->addSeparator();
	constructTool->addAction(LightSourceAction);
	constructTool->addAction(ApertureFieldAction);
	constructTool->addAction(GaussianAction);
	constructTool->addSeparator();
	constructTool->addAction(CalculationAction);
}

void MainWindow::createTreeWidgetItem()
{
	definitionsTreeItem = new QTreeWidgetItem(treeWidget, QStringList(QString("Definitions")));
	modelTreeItem = new QTreeWidgetItem(treeWidget, QStringList(QString("Model")));

	variablesTreeItem = new QTreeWidgetItem(QStringList(QString("Variables")));
	definitionsTreeItem->addChild(variablesTreeItem);

	geometryTreeItem = new QTreeWidgetItem(QStringList(QString("Geometry")));
	modelTreeItem->addChild(geometryTreeItem);

	sourceTreeItem = new QTreeWidgetItem(QStringList(QString("Source")));
	modelTreeItem->addChild(sourceTreeItem);

}

void MainWindow::updateTreeWidgetItem()
{
	delete modelTreeItem;
	delete definitionsTreeItem;
	createTreeWidgetItem(); // ��ɾ��֮ǰ��tree �������µ�tree
	QStringList namelist;
	list<modleID> modleIDList;
	// ����paraboldoid
	SurfaceList_Data.getAllSurface(modleIDList, namelist);
	list<modleID>::const_iterator itID;
	QStringList::const_iterator itName;
	itName = namelist.begin();
	for (itID = modleIDList.begin(); itID != modleIDList.end(); itID++)
	{
		addTreeChild(geometryTreeItem, *itName,
			"Qt/images/paraboloid.png", *itID);  // �������
		itName++;
	}
	
	// ����lightsource
	modleIDList.clear();
	namelist.clear();
	LightSource_Data.getAllLightSource(modleIDList, namelist);

	itName = namelist.begin();
	for (itID = modleIDList.begin(); itID != modleIDList.end(); itID++)
	{
		addTreeChild(sourceTreeItem, *itName,
			"Qt/images/LightSource.png", *itID);  // �������
		itName++;
	}
}

void MainWindow::changeUnit()
{
	UnitWidget * dialog;
	if(unitBtn->text() == "Unit: mm")
		dialog = new UnitWidget(this, 0);
	else if (unitBtn->text() == "Unit: cm")
		dialog = new UnitWidget(this, 1);
	else if (unitBtn->text() == "Unit: m")
		dialog = new UnitWidget(this, 2);
	dialog->exec();
	if (dialog->Num() != -1)
	{
		Unit_factor = dialog->getUnitFactor();
		switch (dialog->Num())
		{
		case 1:
			unitBtn->setText(tr("Unit: mm"));
			break;
		case 2:
			unitBtn->setText(tr("Unit: cm"));
			break;
		case 3:
			unitBtn->setText(tr("Unit: m"));
			break;
		default:
			break;
		}
		
	}
}

void MainWindow::saveFile()
{
	//QString s = QFileDialog::getOpenFileName(this, "Open file dialog", "/", tr("MainWindow Files(*.MainWindow)"));

	QString filename = QFileDialog::getSaveFileName(this,
		tr("Save the file"),
		"",
		tr("*.pvva"));
	if (!filename.isEmpty())
	{
		ofstream savefile(filename.toStdString());

		savefile << "Interface" << endl;  //������������
		savefile << paraboloidNum << " " << LightSourceNum << " " 
			<< rectangleNum << endl;
		glMainWidget->save(savefile); // �����������ѡ��ƽ��

		savefile << "modle" << endl;  //����ģ��
		SurfaceList_Data.save(savefile);

		savefile << "Source" << endl; // ����Դ
		LightSource_Data.save(savefile);
		
		savefile.close();

	}
}

void MainWindow::openFile()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open the file"),
		"",
		tr("*.pvva"));
	if (!filename.isEmpty())
	{
		ifstream openfile(filename.toStdString());
		string temp;
		openfile >> temp;   // ����������
		openfile >> paraboloidNum >> LightSourceNum >> rectangleNum;
		glMainWidget->open(openfile);

		openfile >> temp;
		SurfaceList_Data.open(openfile); // ��surface

		openfile >> temp;
		LightSource_Data.open(openfile);
		
		glMainWidget->updateSurface(&SurfaceList_Data);
		updateLightSource();

		updateTreeWidgetItem();
	}
}

void MainWindow::newFile()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, tr("Warning"),
		"Save changes to this project?",
		QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	if (reply == QMessageBox::Yes)
		openFile();
	else if (reply == QMessageBox::No)
		init();
}

void MainWindow::viewInitFile()
{
	glMainWidget->init();
	glMainWidget->updateGL();
}

void MainWindow::setView(int NumView)
{
	glMainWidget->setView(NumView);
	glMainWidget->updateGL();
}

void MainWindow::createRectangle() // ���޸�
{
	RectangleWidget dialog(this, isSet);
	modleID rectModleID;
	rectModleID.name = RECTANGLE_NAME;

	if (isSet == 1) // �ж��Ƿ����½������޸� 1Ϊ�޸�
	{
		// �ҵ���Ҫ�޸ĵ�ֵ
		rectModleID.ID = ItemVar2.toInt();

		// ���������ó�ѡ�е�ֵ
		dialog.setWidgetData(SurfaceList_Data.getSurfaceData(rectModleID));

		isSet = 0; // ��Ϊ�½�

	}
	else
		dialog.setNameLineEdit("rectangle" + QString::number(rectangleNum + 1));

	dialog.exec();

	if (dialog.Num() == 1)
	{
		bool ok;
		Surface AddSurface;
		ok = dialog.getWidgetData(AddSurface);
		if (!ok) // �ж��Ƿ��ȡ�ɹ�
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		rectModleID.ID = rectangleNum;
		AddSurface.setModleID(rectModleID); // �����µ�surface�����µ�ID

		SurfaceList_Data.push_back(AddSurface);

		glMainWidget->updateSurface(&SurfaceList_Data); // ����glMainWidget�еĵĻ�ͼָ��

		addTreeChild(geometryTreeItem, AddSurface.getName(),
			"Qt/images/rectangle.png", rectModleID);  // �������

		rectangleNum++;
	}
	else if (dialog.Num() == 3)
	{
		bool ok;
		Surface AddSurface;
		ok = dialog.getWidgetData(AddSurface);
		if (!ok)
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}

		AddSurface.setModleID(rectModleID); // ID ����
		glMainWidget->updateSurface(NULL, false);
		SurfaceList_Data.ModifySurfaceData(rectModleID, AddSurface);

		glMainWidget->updateSurface(&SurfaceList_Data);

		SelectItem->setText(0, AddSurface.getName()); // ��������������
	}
	glMainWidget->updateGL(); // ����opengl
}

void MainWindow::createParaboloid()
{
	ParaboloidWidget dialog(this, isSet);
	
	modleID paraModleID;
	paraModleID.name = PARABOLOID_NAME;

	if (isSet == 1) // �ж��Ƿ����½������޸� 1Ϊ�޸�
	{
		// �ҵ���Ҫ�޸ĵ�ֵ
		paraModleID.ID = ItemVar2.toInt();
			
		// ���������ó�ѡ�е�ֵ
		dialog.setWidgetData(SurfaceList_Data.getSurfaceData(paraModleID));

		isSet = 0; // ��Ϊ�½�
		
	}
	else
		dialog.setNameLineEdit("paraboloid" + QString::number(paraboloidNum + 1));

	dialog.exec(); 
	
	if (dialog.Num() == 1)
	{
		bool ok;
		Surface AddSurface;
		ok = dialog.getWidgetData(AddSurface);
		if (!ok) // �ж��Ƿ��ȡ�ɹ�
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		
		paraModleID.ID = paraboloidNum;
		AddSurface.setModleID(paraModleID); // �����µ�surface�����µ�ID

		SurfaceList_Data.push_back(AddSurface);

		glMainWidget->updateSurface(&SurfaceList_Data); // ����glMainWidget�еĵĻ�ͼָ��

		addTreeChild(geometryTreeItem, AddSurface.getName(), 
			"Qt/images/paraboloid.png", paraModleID);  // �������
		
		paraboloidNum++;
	}
	else if (dialog.Num() == 3)
	{
		bool ok;
		Surface AddSurface;
		ok = dialog.getWidgetData(AddSurface);
		if (!ok)
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}

		AddSurface.setModleID(paraModleID); // ID ����
		glMainWidget->updateSurface(NULL, false);
		SurfaceList_Data.ModifySurfaceData(paraModleID, AddSurface);

		glMainWidget->updateSurface(&SurfaceList_Data);

		SelectItem->setText(0, AddSurface.getName()); // ��������������
	}

	glMainWidget->updateGL(); // ����opengl
}

void MainWindow::createLightSource()
{
	LightSourceWidget dialog(this, isSet);
	
	list<modleID> allSurfaceID;
  	QStringList allSurfaceName;

	// �ҵ����е���
	SurfaceList_Data.getAllSurface(allSurfaceID, allSurfaceName);

	if (isSet == 1) // �ж��Ƿ����½������޸� 1Ϊ�޸�
	{
		dialog.setWidgetData(LightSource_Data.getLightSource(ItemVar2.toInt()));
		isSet = 0;
	}
	else
		dialog.setNameLineEdit("LightSource" + QString::number(LightSourceNum + 1));

	dialog.addAllSurface(allSurfaceID, allSurfaceName);
	dialog.exec();

	if (dialog.Num() == 1 )
	{
		LightSource AddLightSource;

		bool ok;
		ok = dialog.getWidgetData(AddLightSource);
		if (!ok)
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}

		AddLightSource.setID(LightSourceNum); // ��new��һ��Ψһ��ID
		AddLightSource.setSurfaceListPtr(&SurfaceList_Data);

		LightSource_Data.addNew(AddLightSource); // �����úõ�Դ���뵽list�в�����
		
		// �������
		modleID paraModleID;
		paraModleID.name = LIGHTSOURCE_NAME;
		paraModleID.ID = LightSourceNum;
		addTreeChild(sourceTreeItem, AddLightSource.getName(), 
			"Qt/images/LightSource.png", paraModleID);

		LightSourceNum++;
		glMainWidget->updateLightSource(&LightSource_Data);

		}
		else if (dialog.Num() == 3) // ��ʾ�޸�
		{
			// ɾ����������
			glMainWidget->updateLightSource(NULL, false); // ���ؿ�ָ�����ͼ����
			bool ok;
			LightSource AddLightSource;
			ok = dialog.getWidgetData(AddLightSource);
			if (!ok)
			{
				QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
				return;
			}

			AddLightSource.setID(ItemVar2.toInt()); // ����ID����
			AddLightSource.setSurfaceListPtr(&SurfaceList_Data);
			LightSource_Data.modify(ItemVar2.toInt(), AddLightSource);

			glMainWidget->updateLightSource(&LightSource_Data);

			SelectItem->setText(0, AddLightSource.getName()); // ��������������
		}
	
	glMainWidget->updateGL(); // ����opengl
}

void MainWindow::createApertureField()
{
	ApertureFieldWidget dialog(this, isSet);
	if (isSet == 1)
	{
		dialog.setWidgetData(ApertureField_Data);
		isSet = 0;
	}
	else
	{
		if (FieldNum == 1)
		{
			QMessageBox::warning(NULL, "Warning", "Program can only exist a source!");
			return;
		}
		dialog.setNameLineEdit("Aperture-Field");
	}
		
	dialog.exec();
	if (dialog.Num() == 1)
	{
		ApertureField_Data = new ApertureField;
		
		bool ok;
		ok = dialog.getWidgetData(ApertureField_Data);
		if (!ok)
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}

		// �������
		modleID aperModleID;
		aperModleID.name = APERTUREFIELD_NAME;
		aperModleID.ID = LightSourceNum;
		addTreeChild(sourceTreeItem, ApertureField_Data->getName(),
			"Qt/images/ApertureField.png", aperModleID);

		FieldNum = 1;
	}
	else if (dialog.Num() == 3) // ��ʾ�޸�
	{
		bool ok;
		ok = dialog.getWidgetData(ApertureField_Data);
		if (!ok)
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		SelectItem->setText(0, ApertureField_Data->getName()); // ��������������
	}
	glMainWidget->updateGL(); // ����opengl

}

void MainWindow::createGaussian()
{
	GaussianWidget dialog(this, isSet);
	if (isSet == 1)
	{
		dialog.setWidgetData(Gaussian_Data);
		isSet = 0;
	}
	else
	{
		if (FieldNum == 1)
		{
			QMessageBox::warning(NULL, "Warning", "Program can only exist a source!");
			return;
		}
		dialog.setNameLineEdit("Gaussian-Field");
	}
		
	dialog.exec();
	if (dialog.Num() == 1)
	{
		Gaussian_Data = new Gaussian;

		bool ok;
		ok = dialog.getWidgetData(Gaussian_Data);
		if (!ok)
		{
			QMessageBox::about(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}

		Gaussian_Data->setUnit(Unit_factor);
		Field SourceField(Gaussian_Data);
		SourceField.setContent(1);
		
		FieldList.push_back(SourceField);
		glMainWidget->updateField(&FieldList);

		// �������
		modleID gusModleID;
		gusModleID.name = GAUSSIAN_NAME;
		gusModleID.ID = LightSourceNum;
		addTreeChild(sourceTreeItem, Gaussian_Data->getName(),
			"Qt/images/Gaussian.png", gusModleID);

		FieldNum = 1;
	}
	else if (dialog.Num() == 3) // ��ʾ�޸�
	{
		bool ok;
		ok = dialog.getWidgetData(Gaussian_Data);
		if (!ok)
		{
			QMessageBox::about(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		Gaussian_Data->setUnit(Unit_factor);
		Field SourceField(Gaussian_Data);
		SourceField.setContent(1);

		glMainWidget->updateField(NULL,false);
		FieldList.clear();
		FieldList.push_back(SourceField);
		glMainWidget->updateField(&FieldList);

		SelectItem->setText(0, Gaussian_Data->getName()); // ��������������
	}
	glMainWidget->updateGL(); // ����opengl
}

void MainWindow::createCalculation()
{
	CalculationWidget * dialog  = new CalculationWidget(this);
	
	// �ҵ����е���
	list<modleID> allSurfaceID;
	QStringList allSurfaceName;
	SurfaceList_Data.getAllSurface(allSurfaceID, allSurfaceName);
	dialog->addAllSurface(allSurfaceID, allSurfaceName);

	// �ҵ����о�����
	list<modleID> allRectangleID; 
	QStringList allRectangleName;
	SurfaceList_Data.getAllRectangle(allRectangleID, allRectangleName);
	dialog->addAllRectangles(allRectangleID, allRectangleName);

	dialog->exec();

	if (dialog->Num() == 1)
	{
		// ����Ƿ���Լ���
		if (FieldNum == 0)
		{
			QMessageBox::warning(NULL, "Warning", "Please add a source first!");
			return;
		}
		// ��ȡ������Ϣ
		int NumSurface, NumRectangle;
		bool isdefualt;
		modleID ResRectangle;  // �����
		dialog->getSelSurfaces(NumSurface, allSurfaceID);
		dialog->getResRectangle(isdefualt, ResRectangle);
		double fre = dialog->getFre() * 1e9;
		delete dialog; // �ͷ�dialog 

		// **********��ʼ����***********
		glMainWidget->updateField(NULL, false);

		PVVA pvva;
		// ���õ�λ
		pvva.setUnit(Unit_factor);
		// ����Ƶ��
		pvva.setFre(fre);
		// ����Դ�������ڴ�
		pvva.setSource(FieldList.begin());
		// ������Ҫ�����ģ��
		list<modleID>::const_iterator iter;
		for (iter = allSurfaceID.begin(); iter != allSurfaceID.end(); iter++)
		{
			pvva.setModle(SurfaceList_Data.getSurfaceData(*iter));
			pvva.Analytic_Calculate(); // ���㵥��������
		}

		if (isdefualt)
		{
			Field tempField;
			pvva.getField(tempField);
			FieldList.push_back(tempField);
		}

		glMainWidget->updateField(&FieldList);
	}

}

void MainWindow::createRightMenu()
{
	R_Tree_compenents_childMenu = new QMenu(this);
	R_BlankMenu = new QMenu(this);

	SetGeometryAction = new QAction(tr("Properties"), this);
	SetGeometryAction->setStatusTip(tr("Modifying parameters"));
	//SetGeometryAction->setIcon(QIcon("./images/set.png"));
	connect(SetGeometryAction, SIGNAL(triggered()), this, SLOT(SetGeometryPara()));

	DeleteGeometryAction = new QAction(tr("Delete"), this);
	DeleteGeometryAction->setStatusTip(tr("Delete parameters"));
	//DeleteAction->setShortcut(QKeySequence::Delete);
	//DeleteAction->setIcon(QIcon("./images/delete.png"));
	connect(DeleteGeometryAction, SIGNAL(triggered()), this, SLOT(DeleteGeometry()));
}

void MainWindow::updateLightSource()
{
	glMainWidget->updateLightSource(NULL, false); // ���ؿ�ָ�����ͼ����
	LightSource_Data.updateLightSource(&SurfaceList_Data);
	glMainWidget->updateLightSource(&LightSource_Data);
}

void MainWindow::on_treeWidget_ContextMenuRequested(QPoint pos)
{
	SelectItem = treeWidget->itemAt(pos);
	if (SelectItem == NULL)
	{
		return;
	}
	QVariant var = SelectItem->data(0, Qt::UserRole);
	ItemVar1 = SelectItem->data(1, Qt::UserRole);
	ItemVar2 = SelectItem->data(2, Qt::UserRole);
	if (0 == var)      //data(...)���ص�data�Ѿ���֮ǰ�����ڵ�ʱ��setdata()���ú�  
	{
		if (R_BlankMenu->isEmpty())
		{
			R_Tree_compenents_childMenu->addAction(SetGeometryAction);
			R_Tree_compenents_childMenu->addAction(DeleteGeometryAction);

		}
		//�˵����ֵ�λ��Ϊ��ǰ����λ��  
		R_Tree_compenents_childMenu->exec(QCursor::pos());
	}
	else
	{

	}

}

void MainWindow::SetGeometryPara()  //�޸Ĳ���
{
	isSet = 1;
	if (ItemVar1 == PARABOLOID_NAME)   createParaboloid();
	else if (ItemVar1 == LIGHTSOURCE_NAME)   createLightSource();
	else if (ItemVar1 == RECTANGLE_NAME)   createRectangle();
	else if (ItemVar1 == APERTUREFIELD_NAME)   createApertureField();
	else if (ItemVar1 == GAUSSIAN_NAME)   createGaussian();

}

void MainWindow::DeleteGeometry()
{
	modleID selModle;
	selModle.ID = ItemVar2.toInt();
	selModle.name = ItemVar1.toInt();
	if (ItemVar1 == PARABOLOID_NAME || ItemVar1 == RECTANGLE_NAME) // ������
	{
		geometryTreeItem->removeChild(SelectItem);
		// ɾ��ѡ���ֵ   
		SurfaceList_Data.erase(selModle);	
	}

	else if (ItemVar1 == LIGHTSOURCE_NAME) // ��Դ
	{
		sourceTreeItem->removeChild(SelectItem);
		// ɾ��ѡ���ֵ
	
		LightSource_Data.erase(ItemVar2.toInt());
	}

	else if (ItemVar1 == APERTUREFIELD_NAME) // ���泡
	{
		sourceTreeItem->removeChild(SelectItem);
		delete ApertureField_Data;
		FieldNum = 0;
	}

	else if (ItemVar1 == GAUSSIAN_NAME) // ��˹��
	{
		sourceTreeItem->removeChild(SelectItem);
		delete Gaussian_Data;
		glMainWidget->updateField(NULL, false);
		FieldList.clear();
		FieldNum = 0;
	}
	glMainWidget->updateGL();// ����opengl
}

void MainWindow::addTreeChild(QTreeWidgetItem *treeFather, QString treeName, 
	QString iconFile, modleID paraModleID)
{
	QTreeWidgetItem *child;
	child = new QTreeWidgetItem;
	child->setText(0, treeName);
	child->setIcon(0, QIcon(iconFile));

	QVariant var0(0);        //�����Ҽ�����
	QVariant var1(paraModleID.name);
	QVariant var2(paraModleID.ID);
	child->setData(0, Qt::UserRole, var0);
	child->setData(1, Qt::UserRole, var1);
	child->setData(2, Qt::UserRole, var2);
	treeFather->addChild(child);

	modelTreeItem->setExpanded(true);
	treeFather->setExpanded(true);
}




