cmake_minimum_required(VERSION 3.10)

project(Blend-Sequencer)

file(GLOB_RECURSE LIBRARY_SRC_FILES src/*.cpp)
file(GLOB_RECURSE DEMO_SRC_FILES demo/*.cpp)

add_library(blend-seq STATIC ${LIBRARY_SRC_FILES})

target_compile_options(blend-seq PRIVATE -Wall)

target_include_directories(blend-seq PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(blend-demo ${DEMO_SRC_FILES})

target_include_directories(blend-demo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(blend-demo blend-seq)