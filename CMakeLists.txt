cmake_minimum_required(VERSION 3.10)

project(MyVTKQtProject LANGUAGES CXX)


find_package(Qt5 REQUIRED COMPONENTS Widgets)

find_package(VTK REQUIRED COMPONENTS
  CommonCore          # 基础核心模块
  InteractionStyle    # 交互支持
  RenderingOpenGL2    # OpenGL渲染
  ImagingCore         # 图像处理核心
  IOImage             # 图像IO支持
  RenderingFreeType
  GUISupportQt
  InteractionImage
)

include_directories(${VTK_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})
link_directories(${VTK_LIBRARY_DIRS} ${Qt5Widgets_LIBRARY_DIRS}${PROJECT_SOURCE_DIR})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SOURCES
    main.cpp
    mainwindow.cpp
    point.cpp
    rt_3.cpp
    save.cpp
)

set(HEADERS
    mainwindow.h
    point.h
    rt_3.h
    save.h
)

set(FORMS
    mainwindow.ui
    point.ui
    rt_3.ui
    save.ui
)

add_executable(MyVTKQtApp ${SOURCES} ${HEADERS} ${FORMS})


message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Generator: ${CMAKE_GENERATOR}")
message(STATUS "CXX compiler: ${CMAKE_CXX_COMPILER}")
# 打印版本信息
message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "VTK version: ${VTK_VERSION}")
message(STATUS "Qt5 version: ${Qt5Widgets_VERSION}")

target_link_libraries(MyVTKQtApp
    ${VTK_LIBRARIES}
    Qt5::Widgets
)

add_custom_command(TARGET MyVTKQtApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:MyVTKQtApp>/qss
)

# 复制 QSS 文件到构建目录
add_custom_command(TARGET MyVTKQtApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/qss/myQSS.qss
        $<TARGET_FILE_DIR:MyVTKQtApp>/qss/
)
