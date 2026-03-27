file(GLOB_RECURSE CORE_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_library(blend-seq-core STATIC ${CORE_SRC_FILES})
target_compile_options(blend-seq-core PRIVATE -Wall)
target_include_directories(blend-seq-core PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)