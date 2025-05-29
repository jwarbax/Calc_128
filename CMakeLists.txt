cmake_minimum_required(VERSION 3.31)
project(Calc)

set(CMAKE_CXX_STANDARD 20)

add_library(core
        src/tokenize.cpp
        src/validate.cpp
        src/calculate.cpp
        src/input.cpp
        src/output.cpp
        src/context.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(Calc src/main.cpp)

target_link_libraries(Calc PRIVATE core quadmath)