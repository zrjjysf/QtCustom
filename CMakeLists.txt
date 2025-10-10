cmake_minimum_required(VERSION 3.5)

cmake_minimum_required(VERSION 3.16)
project(QtCustom LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)   # 自动处理 .ui 文件

set(Qt5_DIR "/opt/Qt5.14.2/5.14.2/gcc_64/lib/cmake/Qt5")
# 设置 Qt
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Svg)

# 公共库
add_subdirectory(common)

set(RESOURCES
    resources.qrc
)

qt5_add_resources(QRC_SOURCES ${RESOURCES})

# 各控件子项目
add_subdirectory(demo/batterywgt)
add_subdirectory(demo/customscrollbar)
add_subdirectory(demo/aspectratiowidget)
add_subdirectory(demo/compareresizewgt)
add_subdirectory(demo/aspectratiolayout)
add_subdirectory(demo/testlayout)
add_subdirectory(demo/customtablewidget)
add_subdirectory(demo/dynamicui)
add_subdirectory(demo/levitatewidget)
add_subdirectory(demo/verticalinfowidget)
add_subdirectory(demo/themesvg)
add_subdirectory(demo/iconlabel)
add_subdirectory(demo/gunboxwgt)
add_subdirectory(demo/svgframe)
add_subdirectory(demo/cardframe)
add_subdirectory(demo/dynamicLayout)
add_subdirectory(demo/verticaltab)