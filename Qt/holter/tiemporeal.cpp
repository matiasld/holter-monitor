#include "tiemporeal.h"
#include "ui_tiemporeal.h"
#include <QMessageBox>
#include "grafico.h"

TiempoReal::TiempoReal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TiempoReal)
{
    ui->setupUi(this);

    tiempo = 0;
    ruta.clear();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerVencio()));

    // Inicializacion del grafico
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

    ui->customPlot->xAxis->setRange( -2 , 2 );
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange( -80 , 80 );

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
}

TiempoReal::~TiempoReal()
{
    emit Cerrada();
}

void TiempoReal::on_pB_busqueda_clicked()
{
    timer->stop();
    tiempo = 0;

    for(int h = 0; h < 6; h++){
        ui->customPlot->graph(h)->clearData();
    }

    ui->customPlot->xAxis->setRange(tiempo - 2.0, 4, Qt::AlignLeft);
    ui->customPlot->replot();

    if(archivo.isOpen())
        archivo.close();

    if(ruta.isNull()){
        ruta = QFileDialog::getOpenFileName(this, tr("Abrir archivo"), "C:\\", tr("Archivos de texto(*.txt)"));
    } else{
        ruta = QFileDialog::getOpenFileName(this, tr("Abrir archivo"), ruta, tr("Archivos de texto(*.txt)"));
    }

    // Seteo la ruta en el line edit
    ui->lE_ruta->setText(ruta);
}

void TiempoReal::on_pB_play_clicked()
{
    if(!ruta.isNull()){
        // Intento abrir el archivo especificado por el usuario
        archivo.setFileName(ruta);
        if(!archivo.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox msgBox;

            msgBox.setText("No se encontro el archivo especificado!");

            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();

            return;
        }

        // Pudo abrir el archivo, disparamos el timer correspondiente para graficar de la misma forma que si fuese en tiempo real
        timer->start(50);

        // Deshabilito/Habilito botones correspondientes
        ui->pB_pausa->setEnabled(true);
        ui->pB_play->setDisabled(true);
    }
}

void TiempoReal::timerVencio(){
    // Leo de a 10 lineas (equivalente a 10 datos que llegan por USB)
    // Y cargo los buffers correspondientes
    QList<QByteArray> canales;
    QByteArray linea;
    bool flag_end = false;

    for(int i = 0; i < 10; i++){
        // No se llego al final del archivo
        if(!archivo.atEnd()){
            linea = archivo.readLine();
            linea.chop(1); // Borro el '\n'
            canales = linea.split(',');

            // Canal por canal voy guardando el buffer
            for(int j = 0; j < 3; j++){
                if(canales.at(j).contains('X')){
                    // No estaba activo el canal
                    buffer[j][9-i] = 5000;
                } else{
                    // El canal estaba activo, traduzco a int el valor
                    buffer[j][9-i] = canales.at(j).toInt();
                }
            }
        } else{
            // Si se llego al final del archivo escribo los ultimos datos y paro el timer
            if(i){
                while(i){
                    // Canal por canal voy guardando el buffer
                    for(int j = 0; j < 3; j++){
                        if(canales.at(j).contains('X')){
                            // No estaba activo el canal
                            buffer[j][9-i] = 5000;
                        } else{
                            // El canal estaba activo, traduzco a int el valor
                            buffer[j][9-i] = canales.at(j).toInt();
                        }
                    }

                    i--;
                }

                i = 10;
            } else{
                i = 10;
            }

            timer->stop();
            ui->lE_ruta->clear();
            archivo.close();
            flag_end = true;
            ui->pB_play->setEnabled(true);
            ui->pB_pausa->setDisabled(true);
        }
    }

    // Luego de tener el buffer actualizo el grafico
    realtimeDataSlot();

    if(flag_end){
        for(int h = 0; h < 6; h++){
            ui->customPlot->graph(h)->clearData();
        }
        tiempo = 0;
    }
}

void TiempoReal::realtimeDataSlot()
{
    for(int i = 0; i < 10; i++ ){
        for(int j = 0; j < 3; j++){
            if(buffer[j][9-i] < 5000){
                ui->customPlot->graph(j)->removeDataBefore(tiempo-2.0);
                ui->customPlot->graph(j)->addData(tiempo, buffer[j][9-i]);
                ui->customPlot->graph(j+3)->clearData();
                ui->customPlot->graph(j+3)->addData(tiempo, buffer[j][9-i]);
            } else{
                ui->customPlot->graph(j)->clearData();
                ui->customPlot->graph(j+3)->clearData();
            }
        }

        tiempo += 0.005;
    }

    ui->customPlot->xAxis->setRange(tiempo - 2.0, 4, Qt::AlignLeft);
    ui->customPlot->replot();
}

void TiempoReal::on_pB_pausa_clicked()
{
    if(timer->isActive())
        timer->stop();
    else
        timer->start(50);
}

void TiempoReal::on_pB_back_clicked()
{
    timer->stop();

    if(archivo.isOpen())
        archivo.close();

    this->destroy();
    emit Cerrada();
}
