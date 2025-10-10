set(SOURCES
    svgframe.cpp
    main.cpp
)

set(HEADERS
    svgframe.h
)

add_executable(svgframe ${SOURCES} ${HEADERS})

target_link_libraries(svgframe
    PRIVATE
        common       # 公共库
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
)

# 复制 qss/svg 到构建目录
configure_file(normal.svg normal.svg COPYONLY)
configure_file(pressed.svg pressed.svg COPYONLY)
