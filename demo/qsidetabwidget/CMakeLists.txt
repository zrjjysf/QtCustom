set(SOURCES
    main.cpp
)

set(HEADERS
)

# Demo 可执行程序
add_executable(QSideTabWidget ${SOURCES} ${HEADERS})

target_link_libraries(QSideTabWidget
    PRIVATE
        qsidetabwidget
)

# 复制 qss/svg 到构建目录
# configure_file(style.qss style.qss COPYONLY)
# configure_file(lightning.svg lightning.svg COPYONLY)

# -------------------------------
# 封装的库
# -------------------------------
set(COMMON_SOURCES
    qsidetabwidget.cpp
)

set(COMMON_HEADERS
    qsidetabwidget.h
)

add_library(qsidetabwidget SHARED ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(qsidetabwidget
    PUBLIC
        common
)

target_include_directories(qsidetabwidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
