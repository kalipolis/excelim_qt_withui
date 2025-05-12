cmake_minimum_required(VERSION 3.10)

project(MyVTKQtProject LANGUAGES CXX)


set(USER_CONFIG_FILE "${CMAKE_SOURCE_DIR}/config/UserConfig.cmake")
if(EXISTS ${USER_CONFIG_FILE})
    message(STATUS "Including user config: ${USER_CONFIG_FILE}")
    include(${USER_CONFIG_FILE})
endif()

find_package(VTK REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

include_directories(${VTK_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})
link_directories(${VTK_LIBRARY_DIRS} ${Qt5Widgets_LIBRARY_DIRS}${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 打印编译器信息
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")
file(GLOB FORMS "*.ui")

file(GLOB_RECURSE QSS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/qss/*")

add_executable(MyVTKQtApp ${SOURCES} ${HEADERS} ${FORMS} ${QSS_FILES})


message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Generator: ${CMAKE_GENERATOR}")
message(STATUS "CXX compiler: ${CMAKE_CXX_COMPILER}")
# 打印版本信息
message(STATUS "Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "VTK version: ${VTK_VERSION}")
message(STATUS "Qt5 version: ${Qt5Widgets_VERSION}")

target_link_libraries(MyVTKQtApp ${VTK_LIBRARIES} Qt5::Widgets)

add_custom_command(TARGET MyVTKQtApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:MyVTKQtApp>/qss
)

# 复制 QSS 文件到构建目录
add_custom_command(TARGET MyVTKQtApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/qss/myQSS.qss
        $<TARGET_FILE_DIR:MyVTKQtApp>/qss/
)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
