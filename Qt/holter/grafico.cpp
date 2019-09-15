#include "grafico.h"
#include "ui_grafico.h"
#include "qmessagebox.h"

grafico::grafico(QWidget *parent, libusb_device_handle** hand) :
    QDialog(parent),
    ui(new Ui::grafico)
{
    ui->setupUi(this);

    grabando = false;

    timerADC = new QTimer(this);
    connect(timerADC, SIGNAL(timeout()), this, SLOT(timerVencio()));

    grafico::InicializoPlot();

    hand2 = hand;
}

grafico::~grafico()
{

}

void grafico::on_BSalir_clicked()
{
    timerADC->stop();
    respuesta = EnvioDato('T'); //Codigo para terminar programa

    // Cierro el puerto
    libusb_close(*hand2);
    libusb_release_interface(*hand2, 0);

    if(archivo.isOpen())
        archivo.close();

    this->destroy();
}

void grafico::on_BEncender_clicked()
{
    if(ui->BEncender->text() == "DISPARO"){
        respuesta = EnvioDato('C'); //Codigo para comenzar a tomar datos del ADC

        if( !respuesta ){
            //Si se envio el paquete correctamente, entra
            ui->BEncender->setText("PAUSA");
            ui->BGrabar->setEnabled(true);

            timerADC->start(50); // Pido dato cada 50mseg
        }
    } else{
        // Se pidio una pausa, freno el timer
        timerADC->stop();
        ui->BEncender->setText("DISPARO");
        ui->BGrabar->setDisabled(true);
    }
}

void grafico::timerVencio(){
    uint8_t paquete[64];
    int recibido;

    EnvioDato('D');

    // Nunca va a tardar mas de 2mseg en responder el uC
    int res = libusb_interrupt_transfer(*hand2, 0x80 | 1, paquete, 64, &recibido, 2);

    if( res == 0 ){
        // ACA recibido DEBERIA SER SIEMPRE 1

        uint8_t i = 0;
        uint8_t pos = 0;
        while( i < 60 ){
            buffer[0][pos] = (paquete[i+1] << 8) | paquete[i];
            pos++;
            i = i + 6;
        }

        i = 2;
        pos = 0;
        while( i < 60 ){
            buffer[1][pos] = (paquete[i+1] << 8) | paquete[i];
            pos++;
            i = i + 6;
        }

        i = 4;
        pos = 0;
        while( i < 60 ){
            buffer[2][pos] = (paquete[i+1] << 8) | paquete[i];
            pos++;
            i = i + 6;
        }

        realtimeDataSlot();
    }
}

bool grafico::EnvioDato ( uint8_t dato )
{
    uint8_t paquete[1];
    int mandado;

    paquete[0] = dato;

    if(libusb_interrupt_transfer(*hand2, 1, paquete, 1, &mandado, 100) == 0)
        return 0;
    else
        return 1;
}


void grafico::InicializoPlot( void )
{
    ui->customPlot->addGraph(); // linea azul
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->customPlot->graph(0)->setAntialiasedFill(false);
    ui->customPlot->addGraph(); // linea roja
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->graph(0)->setChannelFillGraph(ui->customPlot->graph(1));
    ui->customPlot->addGraph(); // linea verde
    ui->customPlot->graph(2)->setPen(QPen(Qt::green));
    ui->customPlot->graph(0)->setChannelFillGraph(ui->customPlot->graph(2));

    ui->customPlot->addGraph(); // punto azul
    ui->customPlot->graph(3)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->customPlot->addGraph(); // punto rojo
    ui->customPlot->graph(4)->setPen(QPen(Qt::red));
    ui->customPlot->graph(4)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(4)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->customPlot->addGraph(); // punto verde
    ui->customPlot->graph(5)->setPen(QPen(Qt::green));
    ui->customPlot->graph(5)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(5)->setScatterStyle(QCPScatterStyle::ssDisc);

    /*ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(2);*/
    ui->customPlot->xAxis->setRange( -2 , 2 );
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange( -80 , 80 );

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
}

void grafico::realtimeDataSlot()
{
    for(int i = 0; i < 10; i++ ){
        for(int j = 0; j < 3; j++){
            QByteArray aux;

            if(buffer[j][9-i] < 5000){
                ui->customPlot->graph(j)->removeDataBefore(tiempo-2.0);
                ui->customPlot->graph(j)->addData(tiempo, buffer[j][9-i]);
                ui->customPlot->graph(j+3)->clearData();
                ui->customPlot->graph(j+3)->addData(tiempo, buffer[j][9-i]);

                if(grabando){
                    aux.clear();
                    aux.setNum(buffer[j][9-i], 10);
                    archivo.write(aux);
                    if(j == 2){
                        aux.clear();
                        aux.append('\n');
                        archivo.write(aux);
                    } else{
                        aux.clear();
                        aux.append(',');
                        archivo.write(aux);
                    }
                }
            } else{
                ui->customPlot->graph(j)->clearData();
                ui->customPlot->graph(j+3)->clearData();
                if(grabando){
                    aux.clear();
                    aux.append('X');
                    archivo.write(aux);
                    if(j == 2){
                        aux.clear();
                        aux.append('\n');
                        archivo.write(aux);
                    } else{
                        aux.clear();
                        aux.append(',');
                        archivo.write(aux);
                    }
                }
            }
        }

        tiempo += 0.005;
    }

    ui->customPlot->xAxis->setRange(tiempo - 2.0, 4, Qt::AlignLeft);
    ui->customPlot->replot();
}

void grafico::on_BGrabar_clicked()
{
    if(ui->BGrabar->text() == "GRABAR"){
        // Intento abrir un archivo en donde se especifica
        if(!ruta.isNull()){
            // Fijo nombre del archivo
            archivo.setFileName(ruta);

            if(!archivo.open(QIODevice::WriteOnly | QIODevice::Text)){
                QMessageBox msgBox;

                msgBox.setText("No se pudo crear el archivo en la ruta especificada!");

                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();

                return;
            }

            // En esta instancia tenemos bien creado el archivo para ir escribiendolo, cambio flag de escritura
            grabando = true;
            EnvioDato('G');

            ui->BGrabar->setText("STOP");
            ui->BEncender->setDisabled(true);
            ui->BBuscar->setDisabled(true);
            ui->BAnterior->setDisabled(true);
        } else{
            QMessageBox msgBox;

            msgBox.setText("La ruta esta vacia!");

            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();

            return;
        }
    } else{
        // Cambio flag de escritura, deshabilitandolo
        grabando = false;
        EnvioDato('X');

        // Cierro el archivo creado
        archivo.close();

        // Limpio el line edit
        ui->LRuta->clear();

        ui->BGrabar->setText("GRABAR");
        ui->BEncender->setEnabled(true);
        ui->BBuscar->setEnabled(true);
        ui->BAnterior->setEnabled(true);
    }
}

void grafico::on_BAnterior_clicked()
{
    timerADC->stop();
    respuesta = EnvioDato('T'); //Codigo para terminar programa

    // Cierro el puerto
    libusb_close(*hand2);
    libusb_release_interface(*hand2, 0);

    if(archivo.isOpen())
        archivo.close();

    this->destroy();
    emit Cerrada();
}

void grafico::on_BBuscar_clicked()
{
    bool valida = false;

    do{
        if(ruta.isNull()){
            ruta = QFileDialog::getSaveFileName(this, tr("Guardar archivo"), "C:\\", tr("Archivos de texto(*.txt)"));
        } else{
            ruta = QFileDialog::getSaveFileName(this, tr("Guardar archivo"), ruta, tr("Archivos de texto(*.txt)"));
        }

        // Si el archivo tenia puntos es un archivo invalido
        if(!ruta.endsWith(".txt", Qt::CaseInsensitive)){
            QMessageBox msgBox;

            msgBox.setText("Nombre de archivo/ruta invalido!");

            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        } else{
            // Se agrega el .txt solo
            valida = true;
        }
    }while(!valida);

    ui->LRuta->setText(ruta);
}
