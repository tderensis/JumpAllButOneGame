cmake_minimum_required(VERSION 3.20)

project(all-but-one-game
    VERSION 0.0.1
    LANGUAGES CXX
    DESCRIPTION "A solver for the All But One Game"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
