#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QWidget>
#include <QDockWidget>
#include <QTreeWidget>
#include <QLabel>
#include <QComBobox>
#include <QstatusBar>
#include <QPushButton>

#include "Qt/include/GLMainWidget.h"
#include "tool/ModleID.h"
#include "tool/GEMS_Memory.h"

#include "openGL/include/GraphTrans.h"
#include "openGL/include/Surface.h"
#include "openGL/include/SurfaceList.h"
#include "openGL/include/LightSource.h"
#include "openGL/include/LightSourceList.h"
#include "openGL/include/ApertureField.h"
#include "openGL/include/Gaussian.h"
#include "openGL/include/Field.h"

//#include"calculation/include/pvva.h"


using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();


private slots:
    void saveFile();  // �����ļ�
	void openFile();  // ���ļ�
	void newFile();   // �½��ļ�

	void viewInitFile(); // ��ʼ���ӽ�
	void setView(int NumView); // �����ӽ�

    void createRectangle();   // ����
	void createParaboloid();    // ������
	void createLightSource();   // ��Դ
	void createApertureField(); // ���泡
	void createGaussian();      // ��˹

	void createCalculation();  // ����

	void on_treeWidget_ContextMenuRequested(QPoint pos);// �Ҽ��˵�
	void DeleteGeometry(); // ɾ��ģ��
	void SetGeometryPara();  //�޸Ĳ���

	void changeUnit();  // �޸ĵ�λ

private:
	void init();

	void createActions();
	void createMenus(); // �˵�
	void createToolBars(); 
	void createStatusBar();

	void createTreeWidgetItem(); // ����tree
	void updateTreeWidgetItem(); // ������ԭ����tree �ڴ���һ���µ�tree

	void createRightMenu(); // �Ҽ��˵�

	void updateLightSource(); // ���¹�Դ

	//list<Paraboloid>::iterator getParaboloid(int Num);

	// �������
	void addTreeChild(QTreeWidgetItem * treeFather, QString treeName,
		QString iconFile, modleID paraModleID);

private:
	GLMainWidget * glMainWidget; // opengl����

	QDockWidget * leftWidget; //����treeWidget�Ĵ���
	QTreeWidget * treeWidget;

	QLabel * locationLabel;  // ״̬����ǩ
	QPushButton * unitBtn;   // ��λ��ť

	//******TreeWidgetItem***
	QTreeWidgetItem * definitionsTreeItem;
	QTreeWidgetItem * variablesTreeItem;
	QTreeWidgetItem * modelTreeItem;
	QTreeWidgetItem * geometryTreeItem;
	QTreeWidgetItem * sourceTreeItem;

	QTreeWidgetItem * SelectItem; // �Ҽ�ѡ�еĽڵ�

	QVariant ItemVar1, ItemVar2;   //�ڵ�����

	//**********Menu**********
	QMenu * fileMenu;  //�˵���

	//�Ҽ��˵�
	QMenu *R_Tree_compenents_childMenu;
	QMenu *R_BlankMenu;

	//**********ToolBar*******
	QToolBar * fileTool;   //������
	QToolBar * constructTool;  //ģ����

	//**********Action********
	//�ļ��˵���
	QAction * saveFileAction;
	QAction * openFileAction; 
	QAction * newFileAction;
	QAction * viewAction;  // �ӽ�
	QLabel * viewLabel; 
	QComboBox * viewComboBox;

	//ģ��
	QAction * rectangleAction; //����
	QAction * polygonAction; // ����͹�����
	QAction * paraboloidAction; // ������

	//�Ҽ��˵�
	QAction * SetGeometryAction;
	QAction * DeleteGeometryAction;

	//Դ
	QAction * LightSourceAction;  //����׷��Դ
	QAction * ApertureFieldAction;//���泡Դ
	QAction * GaussianAction;     //��˹��Դ

	//����
	QAction * CalculationAction;     //����

	//******��¼����ģ�͵ĸ���********
	int paraboloidNum;
	int LightSourceNum;
	int rectangleNum;
	int FieldNum;
	int isSet; // �Ƿ����޸� 0Ϊ���� 1Ϊ��

	//******Modle Date********
	SurfaceList SurfaceList_Data;

	//******Source Date********
	LightSourceList LightSource_Data; // ��ӹ�Դ
	ApertureField * ApertureField_Data; // ���泡
	Gaussian * Gaussian_Data;

	list<Field> FieldList;

	//PVVA * pvva; // �����

	double Unit_factor;
};

#endif // MainWindow_H
