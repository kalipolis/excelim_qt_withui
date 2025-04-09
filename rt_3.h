#ifndef RT_3_H
#define RT_3_H

#include <QWidget>

namespace Ui {
class rt_3;
}

class rt_3 : public QWidget
{
    Q_OBJECT

public:
    explicit rt_3(QWidget *parent = nullptr);
    ~rt_3();

private:
    Ui::rt_3 *ui;
};

#endif // RT_3_H
