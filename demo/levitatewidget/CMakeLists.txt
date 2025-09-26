set(SOURCES
    main.cpp
    mainwindow.cpp
    gunidbadge.cpp
)

set(HEADERS
    mainwindow.h
    gunidbadge.h
)

add_executable(LevitateWidget ${SOURCES} ${HEADERS})

target_link_libraries(LevitateWidget
    PRIVATE
        common       # 公共库
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
)

# 复制专属 style.qss 到构建目录
# configure_file(style.qss style.qss COPYONLY)