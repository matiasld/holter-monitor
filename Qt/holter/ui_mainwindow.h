/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *BIniciar;
    QPushButton *BSalir;
    QLabel *TEstadoCon;
    QPushButton *BArchivo;
    QToolBar *mainToolBar;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(650, 240);
        MainWindow->setMaximumSize(QSize(675, 240));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        BIniciar = new QPushButton(centralWidget);
        BIniciar->setObjectName(QStringLiteral("BIniciar"));
        BIniciar->setGeometry(QRect(50, 100, 150, 41));
        BSalir = new QPushButton(centralWidget);
        BSalir->setObjectName(QStringLiteral("BSalir"));
        BSalir->setGeometry(QRect(450, 100, 150, 41));
        TEstadoCon = new QLabel(centralWidget);
        TEstadoCon->setObjectName(QStringLiteral("TEstadoCon"));
        TEstadoCon->setGeometry(QRect(0, 20, 671, 51));
        QFont font;
        font.setPointSize(10);
        font.setBold(false);
        font.setItalic(true);
        font.setUnderline(false);
        font.setWeight(50);
        TEstadoCon->setFont(font);
        TEstadoCon->setAlignment(Qt::AlignCenter);
        BArchivo = new QPushButton(centralWidget);
        BArchivo->setObjectName(QStringLiteral("BArchivo"));
        BArchivo->setGeometry(QRect(250, 100, 150, 41));
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 650, 21));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        BIniciar->setText(QApplication::translate("MainWindow", "Tiempo real", 0));
        BSalir->setText(QApplication::translate("MainWindow", "Salir", 0));
        TEstadoCon->setText(QApplication::translate("MainWindow", "MEDIDOR DE PULSOS CARDIACOS", 0));
        BArchivo->setText(QApplication::translate("MainWindow", "Lectura de archivo", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
