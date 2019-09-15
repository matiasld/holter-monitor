#ifndef TIEMPOREAL_H
#define TIEMPOREAL_H

#include <QDialog>
#include <QTimer>
#include <QFile>

namespace Ui {
class TiempoReal;
}

class TiempoReal : public QDialog
{
    Q_OBJECT

public:
    explicit TiempoReal(QWidget *parent = 0);
    ~TiempoReal();

private:
    Ui::TiempoReal *ui;
    QString ruta;
    QTimer *timer;
    QFile archivo;
    int16_t buffer[3][10];
    double tiempo;

signals:
    void Cerrada(void);

private slots:
    void on_pB_busqueda_clicked();

    void timerVencio(void);
    void on_pB_play_clicked();
    void realtimeDataSlot(void);
    void on_pB_pausa_clicked();
    void on_pB_back_clicked();
};

#endif // TIEMPOREAL_H
