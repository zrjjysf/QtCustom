set(SOURCES
    main.cpp
)

set(HEADERS
)

# Demo 可执行程序
add_executable(BatteryModuleWidget ${SOURCES} ${HEADERS})

target_link_libraries(BatteryModuleWidget
    PRIVATE
        batterymodulewidget
)

# 复制 qss/svg 到构建目录
# configure_file(style.qss style.qss COPYONLY)
# configure_file(lightning.svg lightning.svg COPYONLY)

# -------------------------------
# 封装的库
# -------------------------------
set(COMMON_SOURCES
    batterymodulewidget.cpp
)

set(COMMON_HEADERS
    batterymodulewidget.h
)

add_library(batterymodulewidget SHARED ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(batterymodulewidget
    PUBLIC
        batterycellwidget
)

target_include_directories(batterymodulewidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
