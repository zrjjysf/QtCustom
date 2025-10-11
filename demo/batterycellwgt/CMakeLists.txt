set(SOURCES
    main.cpp
)

set(HEADERS
)

# Demo 可执行程序
add_executable(BatteryCellWidget ${SOURCES} ${HEADERS})

target_link_libraries(BatteryCellWidget
    PRIVATE
        batterycellwidget
)

# 复制 qss/svg 到构建目录
# configure_file(style.qss style.qss COPYONLY)
# configure_file(lightning.svg lightning.svg COPYONLY)

# -------------------------------
# 封装的库
# -------------------------------
set(COMMON_SOURCES
    batterycellwidget.cpp
)

set(COMMON_HEADERS
    batterycellwidget.h
)

add_library(batterycellwidget SHARED ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(batterycellwidget
    PUBLIC
        common
)

target_include_directories(batterycellwidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
