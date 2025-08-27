cmake_minimum_required(VERSION 3.10)
project(MinecraftLauncher)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

add_executable(MinecraftLauncher
    main.cpp
    mainwindow.cpp
    networkmanager.cpp
    mainwindow.h
    networkmanager.h
    mainwindow.ui
)

target_link_libraries(MinecraftLauncher
    Qt5::Widgets
    Qt5::Network
    curl
    jsoncpp
)
