#include "route.h"
#include "ui_route.h"

Route::Route(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Route)
{
    ui->setupUi(this);
}

Route::~Route()
{
    delete ui;
}
