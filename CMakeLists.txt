cmake_minimum_required(VERSION 3.12)

project(
        labwork3
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 Labwork3 Abelian sandpile model"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(utils)
add_subdirectory(bin)
