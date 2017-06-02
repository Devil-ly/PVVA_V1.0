#ifndef GRAPHTRANSWIDGET_H
#define GRAPHTRANSWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>

#include "openGL/include/GraphTrans.h"

class GraphTransWidget : public QDialog
{
	Q_OBJECT

public:
	GraphTransWidget(QWidget *parent = 0, int wayButton = 0);
	~GraphTransWidget();

	void addDefGroupBox(QGroupBox * _defGroupBox, QString filename);
	void addBaseGroupBox(QGroupBox * _baseGroupBox);
	void addNameLabel(QGridLayout * layout);
	void addRotateWidget(QWidget * RotateWidget, QString filename);
	void addBtn(QGridLayout * _layoutbt, int wayButton = 0);

	GraphTrans getGraphTransData(bool &ok) const;
	void setGraphTransData(GraphTrans graphTransPara);

	QString getGraphTransName()const;
	void setGraphTransName(QString name);

	void setNameLineEdit(QString name);

	int Num();

	private slots:
	void buttonClose();
	void buttonAdd();
	void buttonCreate();
	void buttonApply();
	void buttonOk();

private:

	//page1
	QLabel * Ulabel;
	QLabel * Vlabel;
	QLabel * Nlabel;

	QLineEdit * ULineEidt;
	QLineEdit * VLineEidt;
	QLineEdit * NLineEidt;

	QLabel * label;
	QLineEdit * nameLineEidt;

	//page2
	QGroupBox * imgGroupBox;
	QGroupBox * RotateGroupBox;
	QGroupBox * thetaGroupBox;

	QLabel * imgLlabel1;

	QLabel * xlabel;
	QLabel * ylabel;
	QLabel * zlabel;

	QLineEdit * xRotateLineEidt;
	QLineEdit * yRotateLineEidt;
	QLineEdit * zRotateLineEidt;

	QLabel * thetalabel;
	QLineEdit * thetaLineEidt;

	QPushButton * createbtn;
	QPushButton * addbtn;
	QPushButton * closebtn;

	int returnNum; // 1��ʾCreate 2��ʾAdd 3��ʾOK 4��ʾapply ������ʾclose
				   //int wayButton;
};

#endif // ParaboloidWidget_H