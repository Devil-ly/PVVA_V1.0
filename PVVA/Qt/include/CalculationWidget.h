#ifndef CALCULATIONWIDGET_H
#define CALCULATIONWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QWidget>

#include "Qt/include/GraphTransWidget.h"
#include "openGL/include/LightSource.h"
#include "tool/ModleID.h"

class CalculationWidget : public QDialog
{
	Q_OBJECT

public:
	CalculationWidget(QWidget *parent = 0, int wayButton = 0);
	~CalculationWidget();

	void addAllSurface(list<modleID> _allSurfaceID, QStringList _allSurfaceName);
	void addAllRectangles(list<modleID> _allRectangleID, QStringList _allRectangleName);

	void getSelSurfaces(int & _NumSel, list<modleID>& _SelSurf); // 得到需要计算反射的面
	void getResRectangle(bool & Isdefualt, modleID &_SelRect);  // 得到保存结果的面

	double getFre();

	int Num();

private slots:
    void buttonOk();
    void buttonCancel();
	void buttonSave();

	void selectLineEditChange(const QString & text);
	void monitorNumLineEditChange(const QString & text);
	void resultComboboxChange(int Num);
private:

	//page1
	QTabWidget * tabWidget;

	QGroupBox * paraGroupBox;
	QLabel * freLabel;
	QLineEdit * freLineEdit;

	QGroupBox * surfaceNumGroupBox;
	QLabel *  surfaceLabel;
	QLineEdit * surfaceLineEdit;

	QGroupBox * selectGroupBox;
	QLabel *  selectLabel;
	QComboBox * selectCombobox;
	QGridLayout * selectlayout;

	QGroupBox * resultGroupBox;
	QLabel *  resultLabel;
	QComboBox * resultCombobox;
	QLabel *  rectLabel;
	QComboBox * rectCombobox;
	QLabel * saveLabel;
	QLineEdit * saveLineEdit;
	QPushButton * saveBtn;

	//page2
	QGroupBox * monitorNumGroupBox;
	QLabel *  monitorNumLabel;
	QLineEdit * monitorNumLineEdit;

	QGroupBox * monitorGroupBox;
	QLabel *  monitorLabel;
	QComboBox * monitorCombobox;
	QGridLayout * monitorlayout;


	QPushButton * okBtn;
	QPushButton * cancelBtn;

	int returnNum;

	int NumSel; // 选择需要反射的面的数量
	int NumMonitor; // 加入监视面的数量

	list<modleID> allSurfaceID; // 所有面数据的代号
	QStringList allSurfaceName; // 所有面数据的代号

	list<modleID> allRectangleID; // 所有矩形面数据的代号
	QStringList allRectangleName; // 所有矩形面数据的代号

};

#endif // CALCULATIONWIDGET_H
