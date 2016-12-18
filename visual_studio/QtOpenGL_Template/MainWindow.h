#pragma once
#ifndef QTOGL_WINDOW_H
#define QTOGL_WINDOW_H

#include "OGLViewer.h"
#include <QMainWindow>
//#include <QtGui/QWindow>
#include <QDialog>
#include <QtGui/QOpenGLFunctions>
#include "ui_MainWindow.h"
#include "ui_About.h"


class MainWindow : public QMainWindow, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	~MainWindow();

public:
	void triggerAboutWindow();
	//void aboutwindow();
protected:
	void closeEvent(QCloseEvent* e);
private:
	bool mUpdatePending;
	bool mAnimating;

	OGLViewer* mOGLViewer;
	QDialog* mAbout;
	Ui::MainWindowClass mUI;
};

#endif // QTOGL_WINDOW_H
