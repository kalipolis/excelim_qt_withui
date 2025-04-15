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

    void onReadIn1Clicked();
    void MainWindow::on1v1Clicked();
    void MainWindow::on2v2Clicked();
    void MainWindow::on3v3Clicked();
    void MainWindow::on4v4Clicked();

private:
    void updateImageViewer();

    Ui::MainWindow *ui;
    QString currentImagePath;
    int startSlice;
    int endSlice;
    int displayMode;
    vtkSmartPointer<vtkDICOMImageReader> reader;
};
#endif // MAINWINDOW_H
