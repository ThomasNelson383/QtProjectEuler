
add_qt_executable(Problem1 prob_1.cpp)
add_qt_executable(Problem2 prob_2.cpp)
add_qt_executable(Problem3 prob_3.cpp)
add_qt_executable(Problem4 prob_4.cpp)
add_qt_executable(Problem5 prob_5.cpp)
add_qt_executable(Problem6 prob_6.cpp)
list(APPEND Problem96_srcs prob_96.cpp grid.cpp grid.h)
add_qt_executable(Problem96 "${Problem96_srcs}")
