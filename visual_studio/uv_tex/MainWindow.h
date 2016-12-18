#pragma once
#ifndef QTOGL_WINDOW_H
#define QTOGL_WINDOW_H

#include "OGLViewer.h"
#include <QMainWindow>
//#include <QtGui/QWindow>
#include <QDialog>
#include <QtGui/QOpenGLFunctions>
#include "ui_MainWindow.h"


class MainWindow : public QMainWindow, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	bool m_update_pending;
	bool m_animating;

	OGLViewer* m_oglviewer;
	Ui::MainWindowClass ui;
};

#endif // QTOGL_WINDOW_H
