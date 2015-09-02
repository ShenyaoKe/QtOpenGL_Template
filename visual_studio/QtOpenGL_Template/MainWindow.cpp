#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_update_pending(false), m_animating(false)
	, m_context(0), m_device(0)
{
	ui.setupUi(this);
	setWindowTitle(tr("OpenGL Qt Template"));
}

MainWindow::~MainWindow()
{

}
