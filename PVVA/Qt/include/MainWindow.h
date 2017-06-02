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
    void saveFile();  // 保存文件
	void openFile();  // 打开文件
	void newFile();   // 新建文件

	void viewInitFile(); // 初始化视角
	void setView(int NumView); // 设置视角

    void createRectangle();   // 矩形
	void createParaboloid();    // 抛物面
	void createLightSource();   // 光源
	void createApertureField(); // 口面场
	void createGaussian();      // 高斯

	void createCalculation();  // 计算

	void on_treeWidget_ContextMenuRequested(QPoint pos);// 右键菜单
	void DeleteGeometry(); // 删除模型
	void SetGeometryPara();  //修改参数

	void changeUnit();  // 修改单位

private:
	void init();

	void createActions();
	void createMenus(); // 菜单
	void createToolBars(); 
	void createStatusBar();

	void createTreeWidgetItem(); // 创建tree
	void updateTreeWidgetItem(); // 先销毁原来的tree 在创建一个新的tree

	void createRightMenu(); // 右键菜单

	void updateLightSource(); // 更新光源

	//list<Paraboloid>::iterator getParaboloid(int Num);

	// 添加子树
	void addTreeChild(QTreeWidgetItem * treeFather, QString treeName,
		QString iconFile, modleID paraModleID);

private:
	GLMainWidget * glMainWidget; // opengl窗口

	QDockWidget * leftWidget; //放置treeWidget的窗口
	QTreeWidget * treeWidget;

	QLabel * locationLabel;  // 状态栏标签
	QPushButton * unitBtn;   // 单位按钮

	//******TreeWidgetItem***
	QTreeWidgetItem * definitionsTreeItem;
	QTreeWidgetItem * variablesTreeItem;
	QTreeWidgetItem * modelTreeItem;
	QTreeWidgetItem * geometryTreeItem;
	QTreeWidgetItem * sourceTreeItem;

	QTreeWidgetItem * SelectItem; // 右键选中的节点

	QVariant ItemVar1, ItemVar2;   //节点属性

	//**********Menu**********
	QMenu * fileMenu;  //菜单栏

	//右键菜单
	QMenu *R_Tree_compenents_childMenu;
	QMenu *R_BlankMenu;

	//**********ToolBar*******
	QToolBar * fileTool;   //工具栏
	QToolBar * constructTool;  //模型栏

	//**********Action********
	//文件菜单项
	QAction * saveFileAction;
	QAction * openFileAction; 
	QAction * newFileAction;
	QAction * viewAction;  // 视角
	QLabel * viewLabel; 
	QComboBox * viewComboBox;

	//模型
	QAction * rectangleAction; //矩形
	QAction * polygonAction; // 任意凸多边形
	QAction * paraboloidAction; // 抛物面

	//右键菜单
	QAction * SetGeometryAction;
	QAction * DeleteGeometryAction;

	//源
	QAction * LightSourceAction;  //光线追踪源
	QAction * ApertureFieldAction;//口面场源
	QAction * GaussianAction;     //高斯场源

	//仿真
	QAction * CalculationAction;     //计算

	//******记录各个模型的个数********
	int paraboloidNum;
	int LightSourceNum;
	int rectangleNum;
	int FieldNum;
	int isSet; // 是否是修改 0为不是 1为是

	//******Modle Date********
	SurfaceList SurfaceList_Data;

	//******Source Date********
	LightSourceList LightSource_Data; // 添加光源
	ApertureField * ApertureField_Data; // 口面场
	Gaussian * Gaussian_Data;

	list<Field> FieldList;

	//PVVA * pvva; // 计算核

	double Unit_factor;
};

#endif // MainWindow_H
