cmake_minimum_required(VERSION 3.21)
project(PongFinal)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_OSX_ARCHITECTURES x86_64)

set(SFML_INCLUDE_DIR "/usr/local/include")
set(SFML_LIBRARY_DIR "/usr/local/lib")

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

add_executable(PongFinal pongfinal.cpp pong_defs.h)
target_link_libraries(PongFinal sfml-graphics sfml-window sfml-system)