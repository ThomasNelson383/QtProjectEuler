cmake_minimum_required(VERSION 3.5.2 FATAL_ERROR)

project(QtProjectEuler)

# Setting up Qt related stuff
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 REQUIRED COMPONENTS Core)

function(add_qt_executable TARGET_NAME FILE_LIST)
    add_executable(${TARGET_NAME} ${FILE_LIST})
    target_link_libraries(${TARGET_NAME} Qt5::Core)
endfunction()

add_subdirectory(src)
