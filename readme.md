### 环境要求
1. 操作系统：Windows 10 64位
2. 编译器：Visual Studio 2017
3. CMake版本：3.26.0
4. Qt版本：5.14.2 (msvc2017_64)
5. VTK版本：8.2.0

### 构建步骤
1. 确保已安装Visual Studio 2022，并包含C++开发工具
2. 安装CMake 3.26.0，并将其添加到系统PATH
3. 安装Qt 5.14.2，配置好环境变量
4. 安装VTK 8.2.0，配置好环境变量

### 项目配置
1. 打开命令行，进入项目目录
2. 创建build目录并进入：
```bash
mkdir build
cd build
```
3. 使用CMake生成项目：
```bash
cmake ..
cmake --build .
```

### 运行程序
1. 编译完成后，在build/Debug目录下会生成MyVTKQtApp.exe
2. 双击MyVTKQtApp.exe即可运行程序

### 项目结构
- main.cpp：程序入口
- mainwindow.h/.cpp：主窗口实现
- 有待补充。。。

### 注意事项
1. 程序运行需要Qt和VTK的运行时库，确保它们位于系统PATH中


