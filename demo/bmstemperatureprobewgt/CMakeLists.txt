set(SOURCES
    main.cpp
)

set(HEADERS
)

# Demo 可执行程序
add_executable(BMSTemperatureProbeWgt ${SOURCES} ${HEADERS})

target_link_libraries(BMSTemperatureProbeWgt
    PRIVATE
        bmstemperatureprobewgt
)

# 复制 qss/svg 到构建目录
# configure_file(style.qss style.qss COPYONLY)
configure_file(package.svg package.svg COPYONLY)
configure_file(package_open.svg package_open.svg COPYONLY)

# -------------------------------
# 封装的库
# -------------------------------
set(COMMON_SOURCES
    bmstemperatureprobewgt.cpp
)

set(COMMON_HEADERS
    bmstemperatureprobewgt.h
)

add_library(bmstemperatureprobewgt SHARED ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(bmstemperatureprobewgt
    PUBLIC
        batterypackwidget
        flowlayout
        customscrollbar
        temperaturemodulewidget
)

target_include_directories(bmstemperatureprobewgt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
