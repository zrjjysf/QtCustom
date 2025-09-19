cmake_minimum_required(VERSION 3.5)

cmake_minimum_required(VERSION 3.16)
project(QtCustom LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(Qt5_DIR "/opt/Qt5.14.2/5.14.2/gcc_64/lib/cmake/Qt5")
# 设置 Qt
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Svg)

# 公共库
add_subdirectory(common)

# 各控件子项目
add_subdirectory(controls/batterywgt)
add_subdirectory(controls/customscrollbar)
add_subdirectory(controls/aspectratiowidget)
add_subdirectory(controls/compareresizewgt)
add_subdirectory(controls/aspectratiolayout)
add_subdirectory(controls/testlayout)
