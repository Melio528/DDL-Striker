cmake_minimum_required(VERSION 3.16)
project(Tests LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
enable_testing()
find_package(Qt6 REQUIRED COMPONENTS Core Test)

qt_add_executable(Tests
    main.cpp
    gamelogic.cpp
    gamelogic.h
)

target_link_libraries(Tests PRIVATE
    Qt::Core
    Qt::Test
)