set(SOURCES
    main.cpp
)

set(HEADERS
)

# Demo 可执行程序
add_executable(VerticalInfoWidget ${SOURCES} ${HEADERS})

target_link_libraries(VerticalInfoWidget
    PRIVATE
        verticalinfowidget
)

# 复制 qss/svg 到构建目录
configure_file(style.qss style.qss COPYONLY)

# -------------------------------
# 封装的库
# -------------------------------
set(COMMON_SOURCES
    verticalinfowidget.cpp
)

set(COMMON_HEADERS
    verticalinfowidget.h
    justifylabelstyle.h
)

add_library(verticalinfowidget SHARED ${COMMON_SOURCES} ${COMMON_HEADERS} verticalinfowidget.ui)

target_link_libraries(verticalinfowidget
    PUBLIC
        common
)

target_include_directories(verticalinfowidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
