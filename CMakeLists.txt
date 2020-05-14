cmake_minimum_required(VERSION 3.5.2 FATAL_ERROR)

project(QtProjectEuler)

# Setting up Qt related stuff
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 REQUIRED COMPONENTS Core)
find_package(Qt5 REQUIRED COMPONENTS Concurrent)

macro(add_qt_executable TARGET_NAME SRCS)
    message(${SRCS})
    add_executable(${TARGET_NAME} ${SRCS})
    target_link_libraries(${TARGET_NAME} Qt5::Core)
    target_link_libraries(${TARGET_NAME} Qt5::Concurrent)
endmacro()

add_subdirectory(src)
