set(SOURCES
    main.cpp
    customscrollbar.cpp
)

set(HEADERS
    customscrollbar.h
)

add_executable(CustomScrollBar ${SOURCES} ${HEADERS})

target_link_libraries(CustomScrollBar
    PRIVATE
        common       # 公共库
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
)

# 复制专属 style.qss 到构建目录
configure_file(style.qss style.qss COPYONLY)