#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_radioButton_surgery_toggled(bool checked)
{
    if(checked) {
        ui->stackedWidget_6->setCurrentIndex(0);
    }
}


void MainWindow::on_radioButton_patient_toggled(bool checked)
{
    if(checked) {
        ui->stackedWidget_6->setCurrentIndex(1);
    }
}


void MainWindow::on_radioButton_logout_toggled(bool checked)
{
    if(checked) {
        ui->stackedWidget_6->setCurrentIndex(2);
    }
}




void MainWindow::on_pushButton_infoSaving_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(1);
}


void MainWindow::on_pushButton_imageSaving_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(0);
}

