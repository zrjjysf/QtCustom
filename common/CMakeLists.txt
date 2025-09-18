set(COMMON_SOURCES
    tool.cpp
)

set(COMMON_HEADERS
    tool.h
)

add_library(common STATIC ${COMMON_SOURCES} ${COMMON_HEADERS})
target_link_libraries(common PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})