#include "point.h"
#include "ui_point.h"

point::point(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::point)
{
    ui->setupUi(this);
}

point::~point()
{
    delete ui;
}
