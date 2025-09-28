set(COMMON_SOURCES
    tool.cpp
    ccapplication.cpp
)

set(COMMON_HEADERS
    tool.h
    ccapplication.h
)

add_library(common SHARED ${COMMON_SOURCES} ${COMMON_HEADERS})
target_link_libraries(common PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})