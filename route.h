#ifndef ROUTE_H
#define ROUTE_H

#include <QWidget>

namespace Ui {
class Route;
}

class Route : public QWidget
{
    Q_OBJECT

public:
    explicit Route(QWidget *parent = nullptr);
    ~Route();

private:
    Ui::Route *ui;
};

#endif // ROUTE_H
