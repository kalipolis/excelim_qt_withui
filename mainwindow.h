#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define vtkRenderingCore_AUTOINIT 2(vtkRenderingOpenGL2, vtkInteractionStyle)

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageViewer2.h>
#include <vtkMarchingCubes.h>
#include <vtkPolyDataMapper.h>
#include <QVTKOpenGLNativeWidget.h>
#include <QTableWidget>
#include <vtkProperty.h>
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
    void on1v1Clicked();
    void on2v2Clicked();
    void on3v3Clicked();
    void on4v4Clicked();
    void resizeColumnsToFit(QTableWidget *tableWidget);

    void onImageChange1Clicked();  // 序列前后交换
    void onImageChange2Clicked();  // 图像左右翻转
    void onImageChange3Clicked();  // 图像上下翻转

    void on3DReconstructionClicked();

    void onEditOrganClicked();
    void applyOrganColor(const QColor &color);

    void onThresholdSliderChanged(int value);

    void onOpacitySliderChanged(int value);  // 添加透明度滑块槽函数

    void on_pushButton_61_clicked();

    void on_tabWidget_tabBarClicked(int index);

private:
    void updateImageViewer();

    vtkSmartPointer<vtkActor> m_3DActor; // 保存三维模型的 Actor

    Ui::MainWindow *ui;
    QString currentImagePath;
    bool isSequenceSwapped;  // 是否交换序列
    bool isHorizontalFlip;  // 是否水平翻转
    bool isVerticalFlip;    // 是否垂直翻转
    int startSlice;
    int endSlice;
    int displayMode;
    vtkSmartPointer<vtkDICOMImageReader> reader;
    double opacity;  // 添加透明度成员变量
};
#endif // MAINWINDOW_H
