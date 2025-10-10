set(SOURCES
    cardframe.cpp
    main.cpp
)

set(HEADERS
    cardframe.h
)

add_executable(cardframe ${SOURCES} ${HEADERS})

target_link_libraries(cardframe
    PRIVATE
        common       # 公共库
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
)

# 复制 qss/svg 到构建目录
# configure_file(normal.svg normal.svg COPYONLY)
# configure_file(pressed.svg pressed.svg COPYONLY)
