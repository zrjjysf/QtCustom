set(SOURCES
    main.cpp
)

set(HEADERS
)

# Demo 可执行程序
add_executable(TemplateSvg ${SOURCES} ${HEADERS})

target_link_libraries(TemplateSvg
    PRIVATE
        templatesvg
)

# 复制 qss/svg 到构建目录
configure_file(style.qss style.qss COPYONLY)
configure_file(package.svg package.svg COPYONLY)
configure_file(package_open.svg package_open.svg COPYONLY)


# -------------------------------
# 封装的库
# -------------------------------
set(COMMON_SOURCES
    templatesvgrenderer.cpp
    templatesvgwidget.cpp
)

set(COMMON_HEADERS
    templatesvgrenderer.h
    templatesvgwidget.h
)

add_library(templatesvg SHARED ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(templatesvg
    PUBLIC
        common
        Qt5::Svg
)

target_include_directories(templatesvg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
