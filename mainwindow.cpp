#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vtkRendererCollection.h> 
#include <vtkRenderer.h>   
#include <vtkCamera.h>       
#include <vtkImageActor.h>
#include <vtkImageViewer2.h>  
#include <vtkTransform.h>   
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
#pragma execution_character_set("utf-8")
#include "vtkAutoInit.h"
#include <QScreen>
#include <QTimer>
// 确保初始化必要的VTK模块
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType); // 增加字体渲染模块

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(this->width(), QApplication::primaryScreen()->availableGeometry().height());
    displayMode = 1; // 默认显示模式为1*1
    reader = vtkSmartPointer<vtkDICOMImageReader>::New(); // 初始化reader
    isSequenceSwapped = false; // 默认不交换序列
    isHorizontalFlip = false; // 默认不水平翻转
    isVerticalFlip = false;   // 默认不垂直翻转
    ui->base_series->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->stacked_widget_series->setCurrentIndex(0);
    connect(ui->pushButton_1v1, &QPushButton::clicked, this, &MainWindow::on1v1Clicked);
    connect(ui->pushButton_2v2, &QPushButton::clicked, this, &MainWindow::on2v2Clicked);
    connect(ui->pushButton_3v3, &QPushButton::clicked, this, &MainWindow::on3v3Clicked);
    connect(ui->pushButton_4v4, &QPushButton::clicked, this, &MainWindow::on4v4Clicked);
    connect(ui->exit_to_choose_1, &QPushButton::clicked, this, [this]() {
        ui->stacked_widget_series->setCurrentIndex(0);
    });
    connect(ui->image_change_1, &QPushButton::clicked, this, &MainWindow::onImageChange1Clicked);
    connect(ui->image_change_2, &QPushButton::clicked, this, &MainWindow::onImageChange2Clicked);
    connect(ui->image_change_3, &QPushButton::clicked, this, &MainWindow::onImageChange3Clicked);

    // 添加三个文件夹（01, 02, 03）
    QStringList folders = {"01", "02", "03"};
    for (const QString &folder : folders) {
        int row = ui->base_series->rowCount();
        ui->base_series->insertRow(row);
        
        // 创建表格项并设置居中
        QTableWidgetItem* nameItem = new QTableWidgetItem(folder);
        nameItem->setTextAlignment(Qt::AlignCenter);
        ui->base_series->setItem(row, 0, nameItem); // 姓名
        
        QTableWidgetItem* typeItem = new QTableWidgetItem("DICOM");
        typeItem->setTextAlignment(Qt::AlignCenter);
        ui->base_series->setItem(row, 1, typeItem); // 种类
        
        // 统计文件夹中的切片数量
        QDir imageDir("../../images/basic_sequence/" + folder);
        QStringList imageFiles = imageDir.entryList(QDir::Files);
        int sliceCount = imageFiles.size();

        // 更新"数量"列并设置居中
        QTableWidgetItem* countItem = new QTableWidgetItem(QString::number(sliceCount));
        countItem->setTextAlignment(Qt::AlignCenter);
        ui->base_series->setItem(row, 2, countItem);
    }

    // 延迟调用resizeColumnsToFit
    QTimer::singleShot(0, this, [this]() {
        resizeColumnsToFit(ui->base_series);
    });

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

    int extent[6];
    vtkImageData* imageData = reader->GetOutput();
    imageData->GetExtent(extent);
    double origin[3];
    imageData->GetOrigin(origin);
    double spacing[3];
    imageData->GetSpacing(spacing);
    // int* extent2 = imageData->GetExtent();
    // int realSliceMin = extent2[4];
    // int realSliceMax = extent2[5];
    //qDebug() << "Real slice range:" << realSliceMin << "-" << realSliceMax;

    // 设置相机
    vtkCamera* camera = renderer->GetActiveCamera();
    camera->ParallelProjectionOn();  // 启用平行投影

    // 计算图像中心点
    float xc = origin[0] + 0.5 * (extent[0] + extent[1]) * spacing[0];
    float yc = origin[1] + 0.5 * (extent[2] + extent[3]) * spacing[1];
    float yd = (extent[3] - extent[2] + 1) * spacing[1];

    // 设置相机参数
    float d = camera->GetDistance();
    camera->SetParallelScale(0.5f * static_cast<float>(yd));  // 设置缩放比例
    camera->SetFocalPoint(xc, yc, 0.0);  // 设置焦点
    camera->SetPosition(xc, yc, +d);  // 设置相机位置


    // 设置初始切片（多层DICOM）
    imageViewer->SetSlice(20); // 显示第一张切片

    //vtkWidget->renderWindow()->GetInteractor()->SetInteractorStyle(nullptr);

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
    // 获取当前选中的行，如果未选中则默认第一行
    int row = ui->base_series->currentRow() == -1 ? 0 : ui->base_series->currentRow();

    // 获取选中的文件夹名称（01, 02, 03）
    QString folderName = ui->base_series->item(row, 0)->text();

    // 更新currentImagePath
    currentImagePath = "../../images/basic_sequence/" + folderName;
    qDebug() << "currentImagePath: " << currentImagePath;

    // 获取文件夹中的切片数量
    QDir imageDir(currentImagePath);
    QStringList imageFiles = imageDir.entryList(QDir::Files);
    int sliceCount = imageFiles.size();

    // 获取用户输入的范围值，如果未输入则使用默认值
    startSlice = ui->lineEdit->text().isEmpty() ? 0 : ui->lineEdit->text().toInt();
    endSlice = ui->lineEdit_2->text().isEmpty() ? sliceCount : ui->lineEdit_2->text().toInt();
    qDebug() << "startSlice: " << startSlice;
    qDebug() << "endSlice: " << endSlice;

    // 检查输入范围是否有效
    if (startSlice < 0 || endSlice > sliceCount || startSlice > endSlice) {
        return;
    }

    // 更新SetDirectoryName并设置切片范围
    reader->SetDirectoryName(currentImagePath.toStdString().c_str());
    reader->SetDataExtent(0, 511, 0, 511, startSlice, endSlice); // 假设图像尺寸为512x512
    reader->SetDataScalarTypeToUnsignedShort();
    reader->Update();

    // 检查读取是否成功
    if (reader->GetErrorCode() != 0) {
        return;
    }

    // 更新图像显示
    updateImageViewer();

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
    qDebug() << "Starting updateImageViewer()";
    
    QDir imageDir(currentImagePath);
    if (!imageDir.exists()) {
        qDebug() << "Error: Directory does not exist:" << currentImagePath;
        return;
    }

    auto vtkWidget = ui->QVTKOpenGLNativeWidget1;
    if (!vtkWidget) {
        qDebug() << "Error: QVTKOpenGLNativeWidget1 is null";
        return;
    }

    auto renderWindow = vtkWidget->renderWindow();
    if (!renderWindow) {
        qDebug() << "Error: Failed to get render window";
        return;
    }

    qDebug() << "Clearing old renderers";
    renderWindow->GetRenderers()->RemoveAllItems();

    // 获取实际加载的切片范围
    vtkImageData* imageData = reader->GetOutput();
    int* extent = imageData->GetExtent();
    int zMin = extent[4];
    int zMax = extent[5];
    int totalSlices = zMax - zMin + 1;
    qDebug() << "Loaded slices:" << totalSlices;

    // 计算布局
    int rows = 1, cols = 1;
    switch (displayMode) {
    case 1: rows = cols = 1; break;
    case 2: rows = cols = 2; break;
    case 3: rows = cols = 3; break;
    case 4: rows = cols = 4; break;
    default:
        qDebug() << "Warning: Unknown display mode, using default 1x1";
        break;
    }

    qDebug() << "Calculating layout with rows:" << rows << "cols:" << cols;

    const int totalViews = rows * cols;
    const double vpWidth = 1.0 / cols;
    const double vpHeight = 1.0 / rows;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int index = startSlice + (isSequenceSwapped ? totalViews - 1 - (i * cols + j) : i * cols + j);
            
            if (index > endSlice) {
                qDebug() << "Warning: Index out of bounds, breaking loop";
            }

            //计算切片位置（索引自增）
            int slice = index;
            qDebug() << "Displaying slice:" << slice << "/" << zMax;
            slice = std::clamp(slice, startSlice, endSlice);  //保证不超过序列

            //计算切片位置（均匀分布）
            // double sliceStep = static_cast<double>(totalSlices - 1) / (totalViews - 1);
            // int slice = zMin + round(index * sliceStep);
            // slice = std::clamp(slice, zMin, zMax);
            // qDebug() << "Displaying slice:" << slice << "/" << zMax;

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
            
            
            // 调整窗口/级别
            imageViewer->SetColorWindow(2000);
            imageViewer->SetColorLevel(500);
            imageViewer->SetSlice(slice);

            
            // 获取图像数据
            if (!imageData) {
                qDebug() << "Error: Failed to get image data";
                continue;
            }

            // 获取当前切片的Z坐标（关键修正）
            double sliceZ = imageData->GetOrigin()[2] + slice * imageData->GetSpacing()[2];

            // 重新计算焦点和相机位置（包含Z轴信息）
            vtkCamera* camera = renderer->GetActiveCamera();
            camera->ParallelProjectionOn();

            // 计算XY中心（保持不变）
            float xc = imageData->GetOrigin()[0] + 0.5 * (extent[0] + extent[1]) * imageData->GetSpacing()[0];
            float yc = imageData->GetOrigin()[1] + 0.5 * (extent[2] + extent[3]) * imageData->GetSpacing()[1];
            float yd = (extent[3] - extent[2] + 1) * imageData->GetSpacing()[1];

            // 设置焦点到当前切片的Z平面
            camera->SetFocalPoint(xc, yc, sliceZ); // Z=当前切片位置

            // 调整相机位置到切片的正面（保持与焦点的相对距离）
            float d = camera->GetDistance();
            camera->SetPosition(xc, yc, sliceZ + d); // 保持d的初始计算，但叠加sliceZ

            // 设置平行投影缩放（保持不变）
            camera->SetParallelScale(0.5f * yd);

            vtkNew<vtkTransform> transform;

            if (isHorizontalFlip) {
                qDebug() << "Applying horizontal flip";
                transform->Scale(-1, 1, 1);  // X轴取反
                transform->Translate(-(extent[1] - extent[0] + 1) * imageData->GetSpacing()[0], 0, 0);  // 补偿位置偏移
            }

            if (isVerticalFlip) {
                qDebug() << "Applying vertical flip";
                transform->Scale(1, -1, 1);  // Y轴取反
                transform->Translate(0, -(extent[3] - extent[2] + 1) * imageData->GetSpacing()[1], 0);  // 补偿位置偏移
            }

            // 应用到ImageActor
            imageViewer->GetImageActor()->SetUserTransform(transform);

            // 添加到渲染窗口
            renderWindow->AddRenderer(renderer);
            qDebug() << "Added renderer at position:" << i << j;
        }
    }

    // 禁用交互
    renderWindow->GetInteractor()->SetInteractorStyle(nullptr);
    
    // 强制刷新
    qDebug() << "Rendering window";
    renderWindow->Render();
    
    qDebug() << "Finished updateImageViewer()";
}

void MainWindow::onImageChange1Clicked()
{
    isSequenceSwapped = !isSequenceSwapped;
    updateImageViewer();
}

void MainWindow::onImageChange2Clicked()
{
    // 图像左右翻转
    isHorizontalFlip = !isHorizontalFlip;
    updateImageViewer();
}

void MainWindow::onImageChange3Clicked()
{
    // 图像上下翻转
    isVerticalFlip = !isVerticalFlip;
    updateImageViewer();
}
void MainWindow::resizeColumnsToFit(QTableWidget *tableWidget)
{
    if (!tableWidget) {
        qDebug() << "Invalid table widget pointer";
        return; // 安全检查，确保传入的指针有效
    }
    
    qDebug() << "Resizing columns to contents";
    // 先根据内容调整列宽
    tableWidget->resizeColumnsToContents();
    
    // 获取表格的可用宽度
    int tableWidth = tableWidget->viewport()->width();
    qDebug() << "Table viewport width:" << tableWidth;
    int totalWidth = 0;
    
    // 计算所有列的当前宽度总和
    for (int i = 0; i < tableWidget->columnCount(); ++i) {
        totalWidth += tableWidget->columnWidth(i);
    }
    qDebug() << "Total columns width:" << totalWidth;
    
    // 如果内容宽度小于表格宽度，则平均分配剩余空间
    if (totalWidth < tableWidth) {
        qDebug() << "Distributing extra space between columns";
        int extraWidth = (tableWidth - totalWidth) / tableWidget->columnCount();
        for (int i = 0; i < tableWidget->columnCount(); ++i) {
            tableWidget->setColumnWidth(i, tableWidget->columnWidth(i) + extraWidth);
        }
    }
    qDebug() << "Column resizing completed";
}