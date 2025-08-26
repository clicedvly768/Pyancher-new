cmake_minimum_required(VERSION 3.10)
project(MinecraftLauncher)

set(CMAKE_CXX_STANDARD 17)

add_executable(MinecraftLauncher main.cpp)

if(WIN32)
elseif(UNIX AND NOT APPLE)
elseif(APPLE)
endif()