#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_DialValue->hide();

    //QTimer::singleShot(5000, this, SLOT(close())); - закрываем программу по таймеру

    //startTimer(100);
//    int t = startTimer(150);
//    killTimer(t);

    timer = new QTimer(this);
    timer->setInterval(10); // итнервал обновления таймера
    connect (timer, SIGNAL(timeout()), this, SLOT(timerTicked()));
    FirstTimerFlag = false;

    boomTimer = new QTimer(this);
    boomTimer->setInterval(100);
    connect (boomTimer, SIGNAL(timeout()), this, SLOT(boomTimerTicked()));
    SecondTimerFlag = false;

    connect (ui->pushButton_Restart, SIGNAL(clicked()), this, SLOT(changeButtonNameFirstTimer()));

    connect(ui->pushButton_StartStop, SIGNAL(clicked()), this, SLOT(changeButtonNameSecondTimer()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *e)
{
    //ui->pushButton->move(ui->pushButton->x() + 5, ui->pushButton->y()); - передвижение кнопки по таймеру
}

void MainWindow::on_pushButton_Restart_clicked()
{
    if (timer->isActive())
    {
        timer->stop();
        timerElapsed += ElapsedTimer.elapsed();
    }
    else
    {
        timer->start(10);
        timerElapsed = 0;
        ElapsedTimer.start();
        counter = 0;
    }
    FirstTimerFlag = true;
}

// stop button
//    timer->stop();
//    timerElapsed += ElapsedTimer.elapsed();

void MainWindow::on_pushButton_Pause_clicked()
{
    if (timer->isActive())
    {
        timer->stop();
        timerElapsed += ElapsedTimer.elapsed();
    }

    else
    {
        if (FirstTimerFlag)
        {
            timer->start();
            //timerElapsed = 0;
            timerElapsed += ElapsedTimer.restart();
            // доделать чтобы на паузе второй lcd не шел вперед
        }

    }

}

void MainWindow::timerTicked()
{
    counter ++;
    timerElapsed ++;

    int msec = counter * 10;
    QTime time = QTime::fromMSecsSinceStartOfDay(msec);
    QString text = time.toString("HH:mm:ss.zzz");
    ui->lcdNumber_FirstTimer->display(text);

    int msec_2 = ElapsedTimer.elapsed();
    QTime time_2 = QTime::fromMSecsSinceStartOfDay(msec_2);
    QString text_2 = time_2.toString("HH:mm:ss.zzz");
    ui->lcdNumber_FirstTimer2->display(text_2);
}


void MainWindow::on_pushButton_Boom_clicked() // обратный отсчет от 30 до 600 (QDeadlineTimer +10% за первую работу)
{
    // Запускает таймер
    boomTimer->start();
    sec = ui->dial->value();
    ui->pushButton_Reset->blockSignals(true);
    ui->pushButton_Boom->blockSignals(true);
    SecondTimerFlag = true;

}

void MainWindow::on_pushButton_StartStop_clicked()
{
    if(boomTimer->isActive())
    {
        boomTimer->stop();
    }
    else
    {
        if (SecondTimerFlag)
        boomTimer->start();
    }
}


void MainWindow::boomTimerTicked()
{
    --sec;

    if (sec > 0)
    {
        QString time = QString::number(sec);
        ui->lcdNumber_SecondTimer->display(time);
    }
    else
    {
        boomTimer->stop();
        timer->stop();
        ui->lcdNumber_SecondTimer->display("0");
        QMessageBox::warning(this, "Alarm", "Вы взорвались(((", "Смириться");
        BlockAllElements();
        ui->pushButton_Reset->blockSignals(false);

    }
    // отобразить оставшееся время для таймера обратного отсчета

    // когда таймер обратного отсчета дойдет до нуля
    //  + показать АЛАРМ
    //  + заблокировать все элементы на форме
    //  + переделать в две кнопки (название Start/Stop должно меняться)



}

void MainWindow::changeButtonNameFirstTimer() // изменение названия кнопки по нажатию
{
    QString nameStart = "Start";
    QString nameStop = "Stop";

//    QString namePause = "Pause";
//    QString nameUnpause = "Unpause";

    if (ui->pushButton_Restart->text() == nameStart)
        ui->pushButton_Restart->setText(nameStop);
    else
        ui->pushButton_Restart->setText(nameStart);

//    if (ui->pushButton_Pause->text() == namePause)
//        ui->pushButton_Pause->setText(nameUnpause);
//    else
//        ui->pushButton_Pause->setText(namePause);

}

void MainWindow::changeButtonNameSecondTimer() // changing second timer
{
    QString name1 = "Pause";
    QString name2 = "Unpause";

    if ((ui->pushButton_StartStop->text() == name1) and (SecondTimerFlag == true))
        ui->pushButton_StartStop->setText(name2);
    else
        ui->pushButton_StartStop->setText(name1);
}




// ТРЕТИЙ ТАЙМЕР:
//после нажатия кнопки Go, программа ждет такое кол-во секунд, которое указано в первом спинбоксе
//после чего начинается отсчет такого кол-во секунд, которое указано во втором спинбоксе (точность раз в секунду)
//по нажатии конпки Go еще раз - весь процесс начинается заново
//(во втором спинбоксе 0 не должно быть, в первом - может быть)

void MainWindow::on_dial_valueChanged(int value) // changing dial value
{
    ui->label_DialValue->show();
    ui->label_DialValue->setNum(value);
}


void MainWindow::BlockAllElements() // blocking all forms
{
    QPixmap pixmap(":/img/299105_lock_icon.png");
    ui->pushButton_Restart->setIcon(QIcon(pixmap));
    ui->pushButton_Pause->setIcon(QIcon(pixmap));
    ui->pushButton_StartStop->setIcon(QIcon(pixmap));
    ui->pushButton_Boom->setIcon(QIcon(pixmap));
    ui->pushButton_Go->setIcon(QIcon(pixmap));

    ui->pushButton_Restart->blockSignals(true);
    ui->pushButton_Pause->blockSignals(true);
    ui->pushButton_Boom->blockSignals(true);
    ui->pushButton_StartStop->blockSignals(true);
    ui->pushButton_Go->blockSignals(true);

    ui->spinBox->setEnabled(false);
    ui->spinBox_2->setEnabled(false);

    ui->dial->setEnabled(false);
}

void MainWindow::on_pushButton_Reset_clicked() // reset all
{
    ui->pushButton_Restart->setIcon(QIcon());
    ui->pushButton_Pause->setIcon(QIcon());
    ui->pushButton_StartStop->setIcon(QIcon());
    ui->pushButton_Boom->setIcon(QIcon());
    ui->pushButton_Go->setIcon(QIcon());

    ui->pushButton_Restart->blockSignals(false);
    ui->pushButton_Pause->blockSignals(false);
    ui->pushButton_Boom->blockSignals(false);
    ui->pushButton_StartStop->blockSignals(false);
    ui->pushButton_Go->blockSignals(false);

    ui->lcdNumber_FirstTimer->display("0");
    ui->lcdNumber_FirstTimer2->display("0");
    ui->lcdNumber_SecondTimer->display("0");
    ui->lcdNumber_ThirdTimer->display("0");

    ui->spinBox->setValue(0);
    ui->spinBox_2->setValue(0);
    ui->spinBox->setEnabled(true);
    ui->spinBox_2->setEnabled(true);

    ui->dial->setEnabled(true);
    ui->dial->setValue(30);

    ui->pushButton_Restart->setText("Start");

    FirstTimerFlag = false;
    SecondTimerFlag = false;
}
