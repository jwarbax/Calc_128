cmake_minimum_required(VERSION 3.31)
project(Calc)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

include(FetchContent)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
        cmake-build-debug/_deps/imgui-src/imgui.cpp
        cmake-build-debug/_deps/imgui-src/imgui_demo.cpp
        cmake-build-debug/_deps/imgui-src/imgui_draw.cpp
        cmake-build-debug/_deps/imgui-src/imgui_tables.cpp
        cmake-build-debug/_deps/imgui-src/imgui_widgets.cpp
        cmake-build-debug/_deps/imgui-src/backends/imgui_impl_sdl3.cpp
        cmake-build-debug/_deps/imgui-src/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(imgui PUBLIC
        cmake-build-debug/_deps/imgui-src
        cmake-build-debug/_deps/imgui-src/backends
)

target_link_libraries(imgui PUBLIC SDL3::SDL3)

add_library(core
        src/functions.cpp
        cmake-build-debug/_deps/imgui-src/misc/cpp/imgui_stdlib.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include cmake-build-debug/_deps/imgui-src cmake-build-debug/_deps/imgui-src/backends)

add_executable(Calc src/main.cpp)

target_link_libraries(Calc
        PRIVATE core imgui quadmath
        SDL3::SDL3 SDL3_ttf::SDL3_ttf)