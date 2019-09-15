#ifndef GRAFICO_H
#define GRAFICO_H

#include <QDialog>
#include <QTimer>
#include <libusb.h>
#include <QFile>

namespace Ui {
class grafico;
}

class grafico : public QDialog
{
    Q_OBJECT

public:
    explicit grafico(QWidget *parent, libusb_device_handle ** hand);
    ~grafico();    
    void realtimeDataSlot();
    void InicializoPlot();
    bool EnvioDato ( uint8_t );

private slots:
    void on_BSalir_clicked();

    void on_BEncender_clicked();

    void timerVencio();

    void on_BGrabar_clicked();

    void on_BAnterior_clicked();

    void on_BBuscar_clicked();

private:
    Ui::grafico *ui;
    QTimer *timerADC;
    libusb_device_handle **hand2;
    bool respuesta = false;
    bool grabando;
    int16_t buffer[3][10];
    QFile archivo;
    double tiempo = 0;
    QString ruta;

signals:
    void Cerrada(void);
};

#endif // GRAFICO_H
