set(SOURCES
    main.cpp
)

set(HEADERS
)

# Demo 可执行程序
add_executable(BatteryPackWidget ${SOURCES} ${HEADERS})

target_link_libraries(BatteryPackWidget
    PRIVATE
        batterypackwidget
)

# 复制 qss/svg 到构建目录
# configure_file(style.qss style.qss COPYONLY)
configure_file(package.svg package.svg COPYONLY)
configure_file(package_open.svg package_open.svg COPYONLY)

# -------------------------------
# 封装的库
# -------------------------------
set(COMMON_SOURCES
    batterypackwidget.cpp
)

set(COMMON_HEADERS
    batterypackwidget.h
)

add_library(batterypackwidget SHARED ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(batterypackwidget
    PUBLIC
        templatesvg
        
)

target_include_directories(batterypackwidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
