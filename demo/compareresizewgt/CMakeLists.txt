set(SOURCES
    heightforwidthwidget.cpp
    resizeeventwidget.cpp
    main.cpp
)

set(HEADERS
    heightforwidthwidget.h
    resizeeventwidget.h
)

add_executable(CompareResizeWgt ${SOURCES} ${HEADERS})

target_link_libraries(CompareResizeWgt
    PRIVATE
        common       # 公共库
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
)

# 复制专属 style.qss 到构建目录
# configure_file(style.qss style.qss COPYONLY)