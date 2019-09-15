/********************************************************************************
** Form generated from reading UI file 'grafico.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAFICO_H
#define UI_GRAFICO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_grafico
{
public:
    QCustomPlot *customPlot;
    QPushButton *BEncender;
    QPushButton *BGrabar;
    QPushButton *BAnterior;
    QPushButton *BSalir;
    QLineEdit *LRuta;
    QPushButton *BBuscar;

    void setupUi(QDialog *grafico)
    {
        if (grafico->objectName().isEmpty())
            grafico->setObjectName(QStringLiteral("grafico"));
        grafico->setEnabled(true);
        grafico->resize(656, 378);
        grafico->setMinimumSize(QSize(100, 10));
        grafico->setMaximumSize(QSize(2500, 1500));
        customPlot = new QCustomPlot(grafico);
        customPlot->setObjectName(QStringLiteral("customPlot"));
        customPlot->setGeometry(QRect(20, 90, 611, 241));
        BEncender = new QPushButton(grafico);
        BEncender->setObjectName(QStringLiteral("BEncender"));
        BEncender->setGeometry(QRect(20, 10, 291, 31));
        BGrabar = new QPushButton(grafico);
        BGrabar->setObjectName(QStringLiteral("BGrabar"));
        BGrabar->setEnabled(false);
        BGrabar->setGeometry(QRect(540, 61, 91, 20));
        BAnterior = new QPushButton(grafico);
        BAnterior->setObjectName(QStringLiteral("BAnterior"));
        BAnterior->setEnabled(true);
        BAnterior->setGeometry(QRect(340, 10, 291, 31));
        BSalir = new QPushButton(grafico);
        BSalir->setObjectName(QStringLiteral("BSalir"));
        BSalir->setGeometry(QRect(280, 340, 101, 31));
        LRuta = new QLineEdit(grafico);
        LRuta->setObjectName(QStringLiteral("LRuta"));
        LRuta->setGeometry(QRect(20, 60, 421, 20));
        BBuscar = new QPushButton(grafico);
        BBuscar->setObjectName(QStringLiteral("BBuscar"));
        BBuscar->setGeometry(QRect(450, 60, 75, 21));

        retranslateUi(grafico);

        QMetaObject::connectSlotsByName(grafico);
    } // setupUi

    void retranslateUi(QDialog *grafico)
    {
        grafico->setWindowTitle(QApplication::translate("grafico", "Grafico", 0));
        BEncender->setText(QApplication::translate("grafico", "DISPARO", 0));
        BGrabar->setText(QApplication::translate("grafico", "GRABAR", 0));
        BAnterior->setText(QApplication::translate("grafico", "MENU ANTERIOR", 0));
        BSalir->setText(QApplication::translate("grafico", "SALIR", 0));
        BBuscar->setText(QApplication::translate("grafico", "BUSCAR", 0));
    } // retranslateUi

};

namespace Ui {
    class grafico: public Ui_grafico {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAFICO_H
