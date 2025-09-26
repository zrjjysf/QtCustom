set(SOURCES
    iconlabel.cpp
    main.cpp
)

set(HEADERS
    iconlabel.h
)

add_executable(IconLabel ${SOURCES} ${HEADERS})

target_link_libraries(IconLabel
    PRIVATE
        common       # 公共库
        themesvg       # 公共库
)

# 复制专属 style.qss 到构建目录
configure_file(style.qss style.qss COPYONLY)
configure_file(tip_charge.svg tip_charge.svg COPYONLY)
