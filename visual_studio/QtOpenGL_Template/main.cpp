#include "QtOGL_Window.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtOGL_Window w;
	w.show();
	return a.exec();
}
