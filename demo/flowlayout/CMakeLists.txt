set(SOURCES
    main.cpp
    window.cpp
)

set(HEADERS
    window.h
)

# Demo 可执行程序
add_executable(FlowLayout ${SOURCES} ${HEADERS})

target_link_libraries(FlowLayout
    PRIVATE
        flowlayout
)

# -------------------------------
# 封装的库
# -------------------------------
set(COMMON_SOURCES
    flowlayout.cpp
)

set(COMMON_HEADERS
    flowlayout.h
)

add_library(flowlayout SHARED ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(flowlayout
    PUBLIC
        Qt5::Core Qt5::Gui Qt5::Widgets
)

target_include_directories(flowlayout PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
