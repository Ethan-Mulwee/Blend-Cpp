cmake_minimum_required(VERSION 3.10)

project(Blend-Sequencer)

file(GLOB_RECURSE DEMO_SRC_FILES demo/*.cpp)

add_subdirectory(core-lib)
add_subdirectory(gen)

add_custom_command(
    COMMAND code-gen
    DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}/Cube.blend
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/full-lib/include/generated_code.h
    COMMENT "tesing123..."
)

add_custom_target(code-gen-run 
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/full-lib/include/generated_code.h
    COMMAND code-gen
)

# add_subdirectory(full-lib)
file(GLOB_RECURSE ALL_LIBRARY_HEADERS
"${CMAKE_CURRENT_SOURCE_DIR}/full-lib/*.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/full-lib/*.h"
"${CMAKE_CURRENT_SOURCE_DIR}/core-lib/*.hpp"
)

make_directory(${CMAKE_CURRENT_BINARY_DIR}/include)
file(COPY ${ALL_LIBRARY_HEADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include)
# demo

add_executable(blend-demo ${DEMO_SRC_FILES})
target_include_directories(blend-demo
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core-lib/include
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/full-lib/include
    # PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
target_link_libraries(blend-demo blend-seq-core)

add_dependencies(blend-demo code-gen-run)