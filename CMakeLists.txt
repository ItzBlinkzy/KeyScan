cmake_minimum_required(VERSION 3.16)
project(KeyScan VERSION 0.5.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)


set(CMAKE_AUTOMOC ON)
# cant use autouic because it doesnt seem to work with this build
#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# executable target
add_executable(KeyScan)

# source files
file(GLOB_RECURSE SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

# ui files
set(UI_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/KeyScan.ui)

# UI headers
qt6_wrap_ui(UI_HEADERS ${UI_FILES})

# add sources to target
target_sources(KeyScan PRIVATE
    ${SRC_FILES}
    ${UI_HEADERS}
)


qt_add_resources(KeyScan "resources"
    PREFIX "/"
    FILES
        resources/KeyScan.ico
        resources/KeyScan.png
)

target_link_libraries(KeyScan PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)

# Set Windows executable properties (if on Windows)
if(WIN32)
    target_sources(KeyScan PRIVATE resources/KeyScan.rc)
endif()

# directories for source and generated files
target_include_directories(KeyScan PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
)

set_target_properties(KeyScan PROPERTIES
    WIN32_EXECUTABLE ON
)

#debug
message(STATUS "UI files: ${UI_FILES}")
message(STATUS "Generated UI headers: ${UI_HEADERS}")
