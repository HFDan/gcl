cmake_minimum_required(VERSION 3.16)
project(GCL VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(WRITE ${CMAKE_BINARY_DIR}/.gitignore "*")

# CONAN SETUP
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

# add_executable(gclc src/compiler/main.cpp)

# target_include_directories(gclc PUBLIC ${PROJECT_SOURCE_DIR}/Include)
add_compile_definitions(GCL_VERSION=\"${CMAKE_PROJECT_VERSION}\")
set(GCL_ABI_VERSION v${CMAKE_PROJECT_VERSION})
string(REPLACE "." "_" GCL_ABI_VERSION ${GCL_ABI_VERSION})
add_compile_definitions(GCL_ABI_VERSION=${GCL_ABI_VERSION})

add_subdirectory(${CMAKE_SOURCE_DIR}/src/interpreter)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/libgcl)
