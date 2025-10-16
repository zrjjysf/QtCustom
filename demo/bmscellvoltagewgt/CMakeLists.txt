set(SOURCES
    main.cpp
)

set(HEADERS
)

# Demo 可执行程序
add_executable(BMSCellVoltageWgt ${SOURCES} ${HEADERS})

target_link_libraries(BMSCellVoltageWgt
    PRIVATE
        bmscellvoltagewgt
)

# 复制 qss/svg 到构建目录
# configure_file(style.qss style.qss COPYONLY)
configure_file(package.svg package.svg COPYONLY)
configure_file(package_open.svg package_open.svg COPYONLY)

# -------------------------------
# 封装的库
# -------------------------------
set(COMMON_SOURCES
    bmscellvoltagewgt.cpp
)

set(COMMON_HEADERS
    bmscellvoltagewgt.h
)

add_library(bmscellvoltagewgt SHARED ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(bmscellvoltagewgt
    PUBLIC
        batterypackwidget
        batterymodulewidget
)

target_include_directories(bmscellvoltagewgt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
