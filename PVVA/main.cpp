#include "Qt/include/MainWindow.h"
#include <QtWidgets/QApplication>
#include <gl/glut.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	glutInit(&argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
