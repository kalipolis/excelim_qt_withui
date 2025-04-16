#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vtkRendererCollection.h> 
#include <vtkRenderer.h>       
#include <vtkImageViewer2.h>     
#include <vtkImageData.h>
#include <vtkRenderWindow.h>      // 明确包含渲染窗口头文件
#include <vtkRenderWindowInteractor.h> // 明确包含交互器头文件
#include <vtkDICOMImageReader.h>  // DICOM读取器头文件
#include <QVTKOpenGLNativeWidget.h>
#include <QColorDialog>
#include <QDebug>
#include <QDir>
#include <QMessageBox> 
#include <vtkGenericOpenGLRenderWindow.h>
#include <algorithm>

#include "vtkAutoInit.h"
// 确保初始化必要的VTK模块
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType); // 增加字体渲染模块

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    displayMode = 1; // 默认显示模式为1*1
    reader = vtkSmartPointer<vtkDICOMImageReader>::New(); // 初始化reader
    ui->base_series->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->stacked_widget_series->setCurrentIndex(0);
    connect(ui->pushButton_1v1, &QPushButton::clicked, this, &MainWindow::on1v1Clicked);
    connect(ui->pushButton_2v2, &QPushButton::clicked, this, &MainWindow::on2v2Clicked);
    connect(ui->pushButton_3v3, &QPushButton::clicked, this, &MainWindow::on3v3Clicked);
    connect(ui->pushButton_4v4, &QPushButton::clicked, this, &MainWindow::on4v4Clicked);
    connect(ui->exit_to_choose_1, &QPushButton::clicked, this, [this]() {
        ui->stacked_widget_series->setCurrentIndex(0);
    });


    // 添加三个文件夹（01, 02, 03）
    QStringList folders = {"01", "02", "03"};
    for (const QString &folder : folders) {
        int row = ui->base_series->rowCount();
        ui->base_series->insertRow(row);
        ui->base_series->setItem(row, 0, new QTableWidgetItem(folder)); // 姓名
        ui->base_series->setItem(row, 1, new QTableWidgetItem("DICOM")); // 种类
        // 统计文件夹中的切片数量
        QDir imageDir("../../images/basic_sequence/" + folder);
        QStringList imageFiles = imageDir.entryList(QDir::Files);
        int sliceCount = imageFiles.size();

        // 更新"数量"列
        ui->base_series->setItem(row, 2, new QTableWidgetItem(QString::number(sliceCount)));
    }

    // 连接read_in_1按钮的点击信号到槽函数
    connect(ui->read_in_1, &QPushButton::clicked, this, &MainWindow::onReadIn1Clicked);
    // 初始化时隐藏QVTKOpenGLNativeWidget1
    //ui->QVTKOpenGLNativeWidget1->setVisible(false);
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


    // 设置初始切片（如果是多层DICOM）
    imageViewer->SetSlice(50); // 显示第一张切片

    vtkWidget->renderWindow()->GetInteractor()->SetInteractorStyle(nullptr);

    // 渲染
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

void MainWindow::onReadIn1Clicked()
{
    // 获取当前选中的行
    int row = ui->base_series->currentRow();

    // 获取选中的文件夹名称（01, 02, 03）
    QString folderName = ui->base_series->item(row, 0)->text();

    // 更新currentImagePath
    currentImagePath = "../../images/basic_sequence/" + folderName;

    // 获取文件夹中的切片数量
    QDir imageDir(currentImagePath);
    QStringList imageFiles = imageDir.entryList(QDir::Files);
    int sliceCount = imageFiles.size();

    // 获取用户输入的范围值，如果未输入则使用默认值
    startSlice = ui->lineEdit->text().isEmpty() ? 0 : ui->lineEdit->text().toInt();
    endSlice = ui->lineEdit_2->text().isEmpty() ? sliceCount : ui->lineEdit_2->text().toInt();

    // // 检查输入范围是否有效
    // if (startSlice < 0 || endSlice > sliceCount || startSlice > endSlice) {
    //     QMessageBox::warning(this, "错误", "输入的范围无效，请重新输入");
    //     return;
    // }

    // 更新SetDirectoryName
    vtkNew<vtkDICOMImageReader> reader;
    reader->SetDirectoryName(currentImagePath.toStdString().c_str());
    reader->Update();
    updateImageViewer();

    // 初始化VTK Widget
    // auto vtkWidget = ui->QVTKOpenGLNativeWidget1;
    // vtkNew<vtkRenderer> renderer;
    // vtkWidget->renderWindow()->AddRenderer(renderer);

    // // 配置图像显示
    // vtkNew<vtkImageViewer2> imageViewer;
    // imageViewer->SetInputConnection(reader->GetOutputPort());
    // imageViewer->SetRenderWindow(vtkWidget->renderWindow());

    // // 计算要显示的切片
    // int slice = (startSlice + endSlice) / 2;
    // imageViewer->SetSlice(slice); // 显示中间切片

    // vtkWidget->renderWindow()->GetInteractor()->SetInteractorStyle(nullptr);

    // // 渲染
    // imageViewer->Render();

    // 设置QVTKOpenGLNativeWidget1为可见
    //ui->QVTKOpenGLNativeWidget1->setVisible(true);

    // 切换到stackedWidget_series的第二页
    ui->stacked_widget_series->setCurrentIndex(1);
}

void MainWindow::on1v1Clicked()
{
    displayMode = 1;
    updateImageViewer();
}

void MainWindow::on2v2Clicked()
{
    displayMode = 2;
    updateImageViewer();
}

void MainWindow::on3v3Clicked()
{
    displayMode = 3;
    updateImageViewer();
}

void MainWindow::on4v4Clicked()
{
    displayMode = 4;
    updateImageViewer();
}

void MainWindow::updateImageViewer()
{
    QDir imageDir(currentImagePath);
    if (!imageDir.exists()) {
        qDebug() << "Directory does not exist:" << currentImagePath;
        return;
    }

    auto vtkWidget = ui->QVTKOpenGLNativeWidget1;
    auto renderWindow = vtkWidget->renderWindow();
    
    // 清除旧内容
    renderWindow->GetRenderers()->RemoveAllItems();

    // 配置DICOM读取器
    reader->SetDirectoryName(currentImagePath.toStdString().c_str());
    reader->Update();

    // 计算布局
    int rows = 1, cols = 1;
    switch (displayMode) {
    case 1: rows = cols = 1; break;
    case 2: rows = cols = 2; break;
    case 3: rows = cols = 3; break;
    case 4: rows = cols = 4; break;
    }

    const int totalViews = rows * cols;
    const double vpWidth = 1.0 / cols;
    const double vpHeight = 1.0 / rows;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            const int index = i * cols + j;
            if (index >= totalViews) break;

            // 计算切片位置（均匀分布）
            int slice = startSlice + index;
            slice = std::clamp(slice, startSlice, endSlice);

            // 创建视图组件
            vtkNew<vtkRenderer> renderer;
            vtkNew<vtkImageViewer2> imageViewer;
            
            // 配置视口，消除边框
            double viewport[4] = {
                j * vpWidth,
                1.0 - (i + 1) * vpHeight,
                (j + 1) * vpWidth,
                1.0 - i * vpHeight
            };
            renderer->SetViewport(viewport);
            
            // 关联视图组件
            imageViewer->SetInputConnection(reader->GetOutputPort());
            imageViewer->SetRenderWindow(renderWindow);
            imageViewer->SetRenderer(renderer);
            
            // 设置切片
            imageViewer->SetSlice(slice);
            
            // 调整窗口/级别
            imageViewer->SetColorWindow(2000);
            imageViewer->SetColorLevel(500);
            
            // 启用图像自适应
            imageViewer->SetSize(renderWindow->GetSize());
            imageViewer->GetRenderer()->ResetCamera();
            
            // 添加到渲染窗口
            renderWindow->AddRenderer(renderer);
        }
    }

    // 禁用交互
    renderWindow->GetInteractor()->SetInteractorStyle(nullptr);
    
    // 强制刷新
    renderWindow->Render();
}