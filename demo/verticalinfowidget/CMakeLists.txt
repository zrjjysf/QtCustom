set(SOURCES
    main.cpp
    verticalinfowidget.cpp
)

set(HEADERS
    verticalinfowidget.h
    justifylabelstyle.h
)

add_executable(VerticalInfoWidget ${SOURCES} ${HEADERS})

target_link_libraries(VerticalInfoWidget
    PRIVATE
        common       # 公共库
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
)

# 复制专属 style.qss 到构建目录
configure_file(style.qss style.qss COPYONLY)