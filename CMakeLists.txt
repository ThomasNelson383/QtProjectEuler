cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(QtProjectEuler)

# Setting up Qt related stuff
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Concurrent)
qt_standard_project_setup()

macro(add_qt_executable TARGET_NAME SRCS)
    message(${SRCS})
    add_executable(${TARGET_NAME} ${SRCS})
    target_link_libraries(${TARGET_NAME} Qt6::Core)
    target_link_libraries(${TARGET_NAME} Qt6::Concurrent)
endmacro()

add_subdirectory(src)
