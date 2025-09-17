cmake_minimum_required(VERSION 3.5)

project(QtCustom LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(Qt5_DIR "/opt/Qt5.14.2/5.14.2/gcc_64/lib/cmake/Qt5")
# 查找 Qt 包
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

# 添加资源文件
qt5_add_resources(RESOURCE_FILES resources.qrc)

# 添加源文件和头文件
set(SOURCES
    batterywgt/batterywgt.cpp
    batterywgt/demonstrate.cpp
    customscrollbar/customscrollbar.cpp
    customscrollbar/demonstrate.cpp
    main.cpp
    ${RESOURCE_FILES}
)

set(HEADERS
    batterywgt/batterywgt.h
    customscrollbar/customscrollbar.h
    demonstrate.h
)

# 添加可执行文件
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

include_directories(
    ${CMAKE_SOURCE_DIR}/batterywgt
    ${CMAKE_SOURCE_DIR}/customscrollbar
)

# 链接 Qt 库
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

# 安装额外的样式文件（可选）
install(FILES style.qss DESTINATION ${CMAKE_INSTALL_PREFIX}/styles)
