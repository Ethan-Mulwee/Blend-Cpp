cmake_minimum_required(VERSION 3.10)

project(Blend-Sequencer)

file(GLOB_RECURSE DEMO_SRC_FILES demo/*.cpp)

file(GLOB_RECURSE CORE_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp)

add_library(blend-seq-core STATIC ${CORE_SRC_FILES})
target_compile_options(blend-seq-core PRIVATE -Wall)
target_include_directories(blend-seq-core PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(gen)

make_directory(${CMAKE_CURRENT_BINARY_DIR}/include)

add_custom_command(
    COMMAND code-gen
    DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}/Reference.blend code-gen
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/blend_types/sdna_types_gen.h
    COMMENT "generating SDNA types header..."
)

add_custom_target(code-gen-run 
    # DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/include/blend_types/generated_code.h
    COMMAND code-gen
)

# demo

add_executable(blend-demo ${DEMO_SRC_FILES})
target_include_directories(blend-demo
    # PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core-lib/include
    # PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/full-lib/include
    # PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(blend-demo blend-seq-core)

add_dependencies(blend-demo code-gen-run)