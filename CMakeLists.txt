cmake_minimum_required(VERSION 3.10)

project(Blend-Sequencer)

file(GLOB_RECURSE DEMO_SRC_FILES demo/*.cpp)

add_subdirectory(core-lib)
add_subdirectory(gen)

add_custom_command(
    COMMAND code-gen
    DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}/Cube.blend
    OUTPUT generated_code.h
    COMMENT "tesing123..."
)

add_custom_target(code-gen-run 
    DEPENDS generated_code.h
    COMMAND code-gen
)

# demo

add_executable(blend-demo ${DEMO_SRC_FILES})
target_include_directories(blend-demo
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core-lib/include
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
target_link_libraries(blend-demo blend-seq-core)

add_dependencies(blend-demo code-gen-run)