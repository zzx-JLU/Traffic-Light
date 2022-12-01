#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    thread = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
    if (thread)
    {
        delete thread;
    }
}


void MainWindow::on_startButton_clicked()
{
    int delay = ui->spinBox->value();
    thread = new Thread(delay);
    thread->start();

    ui->spinBox->setDisabled(true);
    ui->startButton->setDisabled(true);
    ui->stopButton->setDisabled(false);
    ui->switchButton->setDisabled(false);
}

void MainWindow::on_stopButton_clicked()
{
    thread->stop();
    thread->reset();
    delete thread;
	thread = NULL;

    ui->spinBox->setDisabled(false);
    ui->startButton->setDisabled(false);
    ui->stopButton->setDisabled(true);
    ui->switchButton->setDisabled(true);
}

void MainWindow::on_switchButton_clicked()
{
    thread->pause();
    thread->switchState();
    thread->sleepByDelay();
    thread->resume();
}
