#include "rt_3.h"
#include "ui_rt_3.h"

rt_3::rt_3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rt_3)
{
    ui->setupUi(this);
}

rt_3::~rt_3()
{
    delete ui;
}
