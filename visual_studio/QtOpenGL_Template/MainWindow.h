#pragma once

#include <QMainWindow>
#include <QDialog>

#include "OGLViewer.h"
#include "ui_MainWindow.h"
#include "ui_About.h"


class MainWindow : public QMainWindow
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
