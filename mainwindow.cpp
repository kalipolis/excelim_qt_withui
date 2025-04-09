#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->colorButton, &QPushButton::clicked, this, &MainWindow::onColorButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onColorButtonClicked()
{
    QColorDialog *cdia = new QColorDialog(this);
    cdia->setWindowTitle("选择颜色");
    cdia->setCurrentColor(QColor("red"));
    if (cdia->exec() == QDialog::Accepted) {
        QColor color = cdia->currentColor();
        ui->colorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
    }
    delete cdia;
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

