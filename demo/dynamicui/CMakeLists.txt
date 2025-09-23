cmake_minimum_required(VERSION 3.5)
project(DynamicUiDemo)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 REQUIRED COMPONENTS Widgets UiTools)

add_executable(DynamicUiDemo main.cpp)
target_link_libraries(DynamicUiDemo Qt5::Widgets Qt5::UiTools)
configure_file(mainwindow.ui mainwindow.ui COPYONLY)