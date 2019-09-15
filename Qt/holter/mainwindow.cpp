#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "usb.h"
#include "grafico.h"
#include "tiemporeal.h"
#include "qmessagebox.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    my_dev = NULL;
    ctx = NULL;
    hand = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BIniciar_clicked()
{
    uint8_t EstadoConexion = InicializoUsb(&hand);
    if(EstadoConexion == SUCESS){
        //Se establecio la conexion, abro menu principal
        GraficoWidget = new grafico(this, &hand);

        connect(GraficoWidget, SIGNAL(Cerrada()), this, SLOT(on_Grafico_closed()));
        this->hide();

        GraficoWidget->show();
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("ERROR");

        switch (EstadoConexion) {
        case 1:
            msgBox.setInformativeText("Error en la inicializacion USBLib");
            break;

        case 2:
            msgBox.setInformativeText("Error de interface");
            break;

        case 3:
            msgBox.setInformativeText("No se pudo conectar dispositivo USB");
            break;

        case 4:
            msgBox.setInformativeText("No se encontro el dispositivo buscado");

            break;

        case 5:
            msgBox.setInformativeText("No se encontraron dispositivos USB conectados");
            break;

        default:
            msgBox.setText("Error al buscar dispositivos");
            break;
        }
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void MainWindow::on_BSalir_clicked()
{
    this->close();
}

void MainWindow::on_BArchivo_clicked()
{
    VentanaTR = new TiempoReal(this);
    connect(VentanaTR, SIGNAL(Cerrada()), this, SLOT(on_VentanaTR_closed()));

    this->hide();
    VentanaTR->show();
}

void MainWindow::on_Grafico_closed(){
    this->show();
    hand = NULL;
    my_dev = NULL;
    ctx = NULL;
}

void MainWindow::on_VentanaTR_closed(){
    this->show();
}

uint8_t MainWindow::InicializoUsb ( libusb_device_handle** hand)
{
    if(libusb_init(&ctx) != 0){
        return 1;   //Error en la inicializacion de USBLib
    } else{
        //Inicializo bien usblib
    }

    // List storing all detected devices
    libusb_device **devs;

    // Get all devices connected to system
    int res = libusb_get_device_list(ctx, &devs);
    if( res >= 0 ){
        libusb_device *dev;
        int i = 0;

        // Busco el que tiene el mismo id que nuestro producto
        while((dev = devs[i++]) != NULL && my_dev == NULL){
            struct libusb_device_descriptor desc;

            libusb_get_device_descriptor(dev, &desc);
            if((desc.idVendor == VENDOR_ID) && (desc.idProduct == PRODUCT_ID)){
                my_dev = dev;

                if(libusb_open(my_dev, &(*hand)) == 0){
                    if(libusb_claim_interface(*hand, 0) == 0){
                        return 0;   //Conexion exitosa
                    } else{
                        return 2;   //Error de interface
                    }
                } else{
                    return 3;   //Se encontro el dispositivo pero no se pudo abrir
                }
            }
        }

        libusb_free_device_list(devs, 1);

        if(my_dev == NULL){
            return 4; //No se encontro el dispositivo buscado
        }
    } else{
        return 5; //No se encontraron dispositivos USB conectados
    }
}
