set(SOURCES
    main.cpp
)

set(HEADERS
)

# Demo 可执行程序
add_executable(ThemeSvg ${SOURCES} ${HEADERS})

target_link_libraries(ThemeSvg
    PRIVATE
        common
        themesvg   # ✅ 直接链接库，而不是重复源文件
)

# 复制 qss/svg 到构建目录
configure_file(style.qss style.qss COPYONLY)
configure_file(tip_charge.svg tip_charge.svg COPYONLY)


# -------------------------------
# 封装的 themesvg 库
# -------------------------------
set(COMMON_SOURCES
    themedsvgrenderer.cpp
    themesvgwgt.cpp
)

set(COMMON_HEADERS
    themedsvgrenderer.h
    themesvgwgt.h
)

add_library(themesvg STATIC ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(themesvg
    PRIVATE
        common
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
)

target_link_libraries(themesvg PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)

target_include_directories(themesvg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
