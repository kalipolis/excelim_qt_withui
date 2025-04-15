#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vtkRenderer.h>       
#include <vtkImageViewer2.h>     
#include <vtkRenderWindow.h>      // 明确包含渲染窗口头文件
#include <vtkRenderWindowInteractor.h> // 明确包含交互器头文件
#include <vtkDICOMImageReader.h>  // DICOM读取器头文件
#include <QVTKOpenGLNativeWidget.h>
#include <QColorDialog>
#include <QDebug>
#include <QDir>
#include <vtkGenericOpenGLRenderWindow.h>

#include "vtkAutoInit.h"
// 确保初始化必要的VTK模块
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType); // 增加字体渲染模块

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化VTK Widget
    auto vtkWidget = ui->QVTKOpenGLNativeWidget1; // 确保UI中控件名称正确
    vtkNew<vtkRenderer> renderer;
    vtkWidget->renderWindow()->AddRenderer(renderer);

    //读取DICOM序列
    vtkNew<vtkDICOMImageReader> reader;
    reader->SetDirectoryName("../../images/basic_sequence/01"); // 直接指定文件夹
    reader->Update();

    // // 配置图像显示
    vtkNew<vtkImageViewer2> imageViewer;
    imageViewer->SetInputConnection(reader->GetOutputPort());
    imageViewer->SetRenderWindow(vtkWidget->renderWindow());
    imageViewer->SetColorWindow(2000); // 调整窗宽
    imageViewer->SetColorLevel(500);    // 调整窗位


    // // 渲染
    imageViewer->GetRenderer()->ResetCamera();
    vtkWidget->renderWindow()->Render();
}
    // // 验证DICOM路径
    // QDir dir("../../images/basic_sequence/01");
    // if (!dir.exists()) {
    //     qDebug() << "Directory does not exist:" << dir.absolutePath();
    //     return;
    // }

    // // 创建DICOM读取器
    // vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    // reader->SetDirectoryName("../../images/basic_sequence/01");
    // reader->Update();

    // // 检查数据有效性
    // if (reader->GetErrorCode() != 0 || !reader->GetOutput()) {
    //     qDebug() << "Failed to read DICOM images";
    //     return;
    // }

    // // 获取控件关联的渲染窗口
    // vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow = 
    //     vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    // ui->openGLWidget->setRenderWindow(renderWindow);

    // // 创建图像查看器并关联到渲染窗口
    // vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
    // imageViewer->SetInputConnection(reader->GetOutputPort());
    // imageViewer->SetRenderWindow(renderWindow);  // 直接使用控件关联的窗口
    
    // // 显式创建交互器并关联
    // renderWindow->GetInteractor()->Initialize();
    // imageViewer->SetupInteractor(renderWindow->GetInteractor());

    // // 初始化显示参数
    // imageViewer->SetSlice(0);
    // imageViewer->GetRenderer()->ResetCamera();
    // renderWindow->Render();


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
