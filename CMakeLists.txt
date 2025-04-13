cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER "clang++")

project(DFT_Project)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(RAYLIB_DIR ${CMAKE_SOURCE_DIR}/third_party/raylib-5.5)

add_subdirectory(${RAYLIB_DIR})

add_custom_target(run
	COMMAND dft
	DEPENDS dft
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_executable(dft 
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/dft.cpp
    ${SOURCE_DIR}/epicycloid.cpp
    ${SOURCE_DIR}/input.cpp
    ${SOURCE_DIR}/visualizer.cpp
)

target_include_directories(dft PRIVATE
    ${SOURCE_DIR}
    ${RAYLIB_DIR}/src 
)

target_link_libraries(dft PRIVATE
    raylib
    GL
    m
    pthread
    dl
    X11
)

target_compile_options(dft PRIVATE -Wall)
