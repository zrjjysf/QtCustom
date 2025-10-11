set(SOURCES
    main.cpp
)

set(HEADERS
)

# Demo 可执行程序
add_executable(CustomScrollBar ${SOURCES} ${HEADERS})

target_link_libraries(CustomScrollBar
    PRIVATE
        customscrollbar
)

# 复制 qss/svg 到构建目录
configure_file(style.qss style.qss COPYONLY)
# configure_file(lightning.svg lightning.svg COPYONLY)

# -------------------------------
# 封装的库
# -------------------------------
set(COMMON_SOURCES
    customscrollbar.cpp
)

set(COMMON_HEADERS
    customscrollbar.h
)

add_library(customscrollbar SHARED ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(customscrollbar
    PUBLIC
        common
        Qt5::Svg
)

target_include_directories(customscrollbar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
