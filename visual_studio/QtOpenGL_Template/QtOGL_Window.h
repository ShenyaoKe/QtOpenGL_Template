#ifndef QTOGL_WINDOW_H
#define QTOGL_WINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_QtOGL_Window.h"

class QtOGL_Window : public QMainWindow
{
	Q_OBJECT

public:
	QtOGL_Window(QWidget *parent = 0);
	~QtOGL_Window();

private:
	Ui::QtOGL_WindowClass ui;
};

#endif // QTOGL_WINDOW_H
