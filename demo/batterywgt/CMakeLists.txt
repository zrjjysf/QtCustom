set(SOURCES
    batterywgt.cpp
    main.cpp
)

set(HEADERS
    batterywgt.h
)

add_executable(BatteryWidget ${SOURCES} ${HEADERS})

target_link_libraries(BatteryWidget
    PRIVATE
        common       # 公共库
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
)

# 复制专属 style.qss 到构建目录
configure_file(style.qss style.qss COPYONLY)