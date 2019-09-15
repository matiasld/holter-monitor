#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <libusb.h>
#include "grafico.h"
#include "tiemporeal.h"

#define FULLHD 1

#ifdef FULLHD
#define SIZE_X_GRAFICO 625
#define SIZE_Y_GRAFICO 375
#else
#define SIZE_X_GRAFICO 2500
#define SIZE_Y_GRAFICO 1500
#endif

#define VENDOR_ID   0x1FC9
#define PRODUCT_ID  0x0081
#define SUCESS      0

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_BIniciar_clicked();

    void on_BSalir_clicked();

    void on_BArchivo_clicked();

    void on_Grafico_closed();

    void on_VentanaTR_closed();

    uint8_t InicializoUsb(libusb_device_handle** hand);

private:
    Ui::MainWindow *ui;
    libusb_device_handle *hand;
    grafico *GraficoWidget;
    TiempoReal *VentanaTR;
    libusb_device *my_dev;
    libusb_context *ctx;
};

#endif // MAINWINDOW_H
