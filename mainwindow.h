#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define vtkRenderingCore_AUTOINIT 2(vtkRenderingOpenGL2, vtkInteractionStyle)

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageViewer2.h>
#include <QVTKOpenGLNativeWidget.h>

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

    void on_lastpage_clicked();
    void on_nextpage_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
