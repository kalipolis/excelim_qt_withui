#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageViewer2.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <QVTKOpenGLNativeWidget.h>
#include <QColorDialog>
#include <QDebug>
#include <QDir>

#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->colorButton, &QPushButton::clicked, this, &MainWindow::onColorButtonClicked);
    
    // 检查路径是否存在
    QDir dir("../../images/basic_sequence/01");
    if (!dir.exists()) {
        qDebug() << "Directory does not exist:" << dir.absolutePath();
        return;
    } else {
        qDebug() << "Directory exists:" << dir.absolutePath();
    }

    // 创建DICOM图像读取器
    vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetDirectoryName("../../images/basic_sequence/01"); // 设置DICOM图像所在的目录
    reader->SetDataExtent(0, 255, 0, 255, 0, 3); // 设置读取的图像范围，0~3张
    reader->SetDataSpacing(1.0, 1.0, 1.0); // 设置图像间距
    reader->SetDataOrigin(0.0, 0.0, 0.0); // 设置图像原点
    reader->Update();

    // 检查是否成功读取
    if (reader->GetErrorCode() != 0) {
        qDebug() << "Failed to read DICOM images. Error code:" << reader->GetErrorCode();
        return;
    }

    // 创建图像查看器
    vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
    imageViewer->SetInputConnection(reader->GetOutputPort());

/*
    // 设置渲染窗口
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    imageViewer->SetRenderWindow(renderWindow);

    // 将渲染窗口设置到QVTKOpenGLNativeWidget中
    ui->openGLWidget->SetRenderWindow(renderWindow);
*/


    // 将渲染窗口设置到 QVTKOpenGLNativeWidget 控件上
    imageViewer->SetRenderWindow(ui->openGLWidget->renderWindow());

    // 设置交互器（可选）
    imageViewer->SetupInteractor(ui->openGLWidget->interactor());

    // 设置初始显示的切片
    imageViewer->SetSlice(0);
    imageViewer->Render();
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

void MainWindow::on_lastpage_clicked()
{
    // 获取当前tabWidget
    QTabWidget* currentTabWidget = ui->tabWidget;
    
    // 获取当前tab中的widget
    QWidget* currentTab = currentTabWidget->currentWidget();
    
    // 查找当前tab中的stackedWidget
    QStackedWidget* stackedWidget = currentTab->findChild<QStackedWidget*>();
    
    if(stackedWidget) {
        int currentIndex = stackedWidget->currentIndex();
        qDebug() << "当前页面索引: " << currentIndex; // 使用英文双引号
        if(currentIndex > 0) {
            stackedWidget->setCurrentIndex(currentIndex - 1);
            qDebug() << "切换到上一页，新索引: " << currentIndex - 1;
        } else {
            qDebug() << "已经是第一页，无法切换";
        }
    } else {
        qDebug() << "未找到stackedWidget";
    }
}
void MainWindow::on_nextpage_clicked()
{
    // 获取当前tabWidget
    QTabWidget* currentTabWidget = ui->tabWidget;
    
    // 获取当前tab中的widget
    QWidget* currentTab = currentTabWidget->currentWidget();
    
    // 查找当前tab中的stackedWidget
    QStackedWidget* stackedWidget = currentTab->findChild<QStackedWidget*>();
    
    if(stackedWidget) {
        int currentIndex = stackedWidget->currentIndex();
        qDebug() << "当前页面索引: " << currentIndex; // 使用英文双引号
        if(currentIndex < stackedWidget->count() - 1) {
            stackedWidget->setCurrentIndex(currentIndex + 1);
            qDebug() << "切换到下一页，新索引: " << currentIndex + 1;
        } else {
            qDebug() << "已经是最后一页，无法切换";
        }
    } else {
        qDebug() << "未找到stackedWidget";
    }
}
