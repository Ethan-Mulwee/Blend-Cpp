project(code-gen)

# code gen
file(GLOB_RECURSE GEN_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_executable(code-gen ${GEN_SRC_FILES})
target_include_directories(code-gen PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../include)
target_link_libraries(code-gen blend-seq-core)