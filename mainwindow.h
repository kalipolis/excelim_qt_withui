#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onColorButtonClicked();

    void on_radioButton_surgery_toggled(bool checked);

    void on_radioButton_patient_toggled(bool checked);

    void on_radioButton_logout_toggled(bool checked);

    void on_pushButton_infoSaving_clicked();

    void on_pushButton_imageSaving_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
