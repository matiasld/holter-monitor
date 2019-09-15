/********************************************************************************
** Form generated from reading UI file 'tiemporeal.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIEMPOREAL_H
#define UI_TIEMPOREAL_H

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

class Ui_TiempoReal
{
public:
    QCustomPlot *customPlot;
    QLineEdit *lE_ruta;
    QPushButton *pB_busqueda;
    QPushButton *pB_play;
    QPushButton *pB_pausa;
    QPushButton *pB_back;

    void setupUi(QDialog *TiempoReal)
    {
        if (TiempoReal->objectName().isEmpty())
            TiempoReal->setObjectName(QStringLiteral("TiempoReal"));
        TiempoReal->resize(650, 400);
        customPlot = new QCustomPlot(TiempoReal);
        customPlot->setObjectName(QStringLiteral("customPlot"));
        customPlot->setGeometry(QRect(10, 80, 630, 300));
        lE_ruta = new QLineEdit(TiempoReal);
        lE_ruta->setObjectName(QStringLiteral("lE_ruta"));
        lE_ruta->setGeometry(QRect(10, 50, 560, 20));
        pB_busqueda = new QPushButton(TiempoReal);
        pB_busqueda->setObjectName(QStringLiteral("pB_busqueda"));
        pB_busqueda->setGeometry(QRect(580, 50, 60, 20));
        pB_play = new QPushButton(TiempoReal);
        pB_play->setObjectName(QStringLiteral("pB_play"));
        pB_play->setGeometry(QRect(10, 10, 150, 30));
        pB_pausa = new QPushButton(TiempoReal);
        pB_pausa->setObjectName(QStringLiteral("pB_pausa"));
        pB_pausa->setEnabled(false);
        pB_pausa->setGeometry(QRect(250, 10, 150, 30));
        pB_back = new QPushButton(TiempoReal);
        pB_back->setObjectName(QStringLiteral("pB_back"));
        pB_back->setGeometry(QRect(490, 10, 150, 30));

        retranslateUi(TiempoReal);

        QMetaObject::connectSlotsByName(TiempoReal);
    } // setupUi

    void retranslateUi(QDialog *TiempoReal)
    {
        TiempoReal->setWindowTitle(QApplication::translate("TiempoReal", "Dialog", 0));
        pB_busqueda->setText(QApplication::translate("TiempoReal", "Buscar", 0));
        pB_play->setText(QApplication::translate("TiempoReal", "PLAY", 0));
        pB_pausa->setText(QApplication::translate("TiempoReal", "PAUSA", 0));
        pB_back->setText(QApplication::translate("TiempoReal", "MENU ANTERIOR", 0));
    } // retranslateUi

};

namespace Ui {
    class TiempoReal: public Ui_TiempoReal {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIEMPOREAL_H
