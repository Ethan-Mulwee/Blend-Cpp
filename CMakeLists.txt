cmake_minimum_required(VERSION 3.10)

project(Blend-Sequencer)

file(GLOB_RECURSE DEMO_SRC_FILES demo/*.cpp)

add_subdirectory(core-lib)
add_subdirectory(gen)

# demo

add_executable(blend-demo ${DEMO_SRC_FILES})
target_include_directories(blend-demo
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core-lib/include
)
target_link_libraries(blend-demo blend-seq-core)