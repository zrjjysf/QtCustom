set(SOURCES
    main.cpp
)

set(HEADERS
)

# Demo 可执行程序
add_executable(GunBox ${SOURCES} ${HEADERS})

target_link_libraries(GunBox
    PRIVATE
        gunboxwgt
)

# 复制 qss/svg 到构建目录
configure_file(style.qss style.qss COPYONLY)
configure_file(lightning.svg lightning.svg COPYONLY)

# -------------------------------
# 封装的库
# -------------------------------
set(COMMON_SOURCES
    gunboxwgt.cpp
)

set(COMMON_HEADERS
    gunboxwgt.h
)

add_library(gunboxwgt SHARED ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(gunboxwgt
    PUBLIC
        iconlabel
        verticalinfowidget
)

target_include_directories(gunboxwgt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
