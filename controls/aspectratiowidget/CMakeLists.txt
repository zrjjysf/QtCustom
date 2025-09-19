set(SOURCES
    aspectratiowidget.cpp
    main.cpp
)

set(HEADERS
    aspectratiowidget.h
)

add_executable(AspectRatioWidget ${SOURCES} ${HEADERS})

target_link_libraries(AspectRatioWidget
    PRIVATE
        common       # 公共库
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
)

# 复制专属 style.qss 到构建目录
configure_file(style.qss style.qss COPYONLY)