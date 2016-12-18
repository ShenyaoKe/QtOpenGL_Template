#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, mUpdatePending(false), mAnimating(false)
{
	mOGLViewer = new OGLViewer;

	mUI.setupUi(this);
	mUI.ogl_layout->addWidget(mOGLViewer);
	//setWindowTitle(tr("OpenGL Qt Template"));

	mOGLViewer->setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
{
	delete mOGLViewer;
}

void MainWindow::triggerAboutWindow()
{
	mAbout = new QDialog(0,0);
	Ui::about_dialog about_ui;
	about_ui.setupUi(mAbout);
	mAbout->show();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
	Q_UNUSED(e);

	foreach(QWidget *widget, QApplication::topLevelWidgets())
	{
		widget->close();
	}
}
