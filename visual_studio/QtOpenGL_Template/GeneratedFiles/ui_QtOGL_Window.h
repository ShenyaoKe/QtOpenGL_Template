/********************************************************************************
** Form generated from reading UI file 'QtOGL_Window.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTOGL_WINDOW_H
#define UI_QTOGL_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtOGL_WindowClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtOGL_WindowClass)
    {
        if (QtOGL_WindowClass->objectName().isEmpty())
            QtOGL_WindowClass->setObjectName(QStringLiteral("QtOGL_WindowClass"));
        QtOGL_WindowClass->resize(600, 400);
        menuBar = new QMenuBar(QtOGL_WindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        QtOGL_WindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtOGL_WindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QtOGL_WindowClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(QtOGL_WindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QtOGL_WindowClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QtOGL_WindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtOGL_WindowClass->setStatusBar(statusBar);

        retranslateUi(QtOGL_WindowClass);

        QMetaObject::connectSlotsByName(QtOGL_WindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtOGL_WindowClass)
    {
        QtOGL_WindowClass->setWindowTitle(QApplication::translate("QtOGL_WindowClass", "QtOGL_Window", 0));
    } // retranslateUi

};

namespace Ui {
    class QtOGL_WindowClass: public Ui_QtOGL_WindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTOGL_WINDOW_H
