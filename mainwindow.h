#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimerEvent>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include <QDeadlineTimer>
#include <QDateTime>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void timerEvent(QTimerEvent *e);

private slots:

    //pushButtons
    void on_pushButton_Restart_clicked();

    void on_pushButton_Pause_clicked();

    void on_pushButton_Boom_clicked();

    void on_pushButton_Reset_clicked();

    void on_pushButton_StartStop_clicked();

    //timers
    void timerTicked();

    void boomTimerTicked();

    //different functions
    void changeButtonNameFirstTimer();

    void changeButtonNameSecondTimer();

    void on_dial_valueChanged(int value);

    void BlockAllElements();






private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTimer *boomTimer;
    QElapsedTimer ElapsedTimer;
    int timerElapsed;
    int counter;
    int sec;
    // flags for correct Pause pushbutton`s work and changing their titles
    bool FirstTimerFlag;
    bool SecondTimerFlag;

};
#endif // MAINWINDOW_H
