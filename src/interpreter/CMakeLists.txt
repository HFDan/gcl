find_package(fmt)

add_executable(gcl ${PROJECT_SOURCE_DIR}/src/interpreter/main.cc ${PROJECT_SOURCE_DIR}/src/interpreter/VM/vm.cc ${PROJECT_SOURCE_DIR}/src/interpreter/VM/Instructions/instructions.cc)

target_include_directories(gcl PUBLIC ${PROJECT_SOURCE_DIR}/src/interpreter)
target_include_directories(gcl PUBLIC ${PROJECT_SOURCE_DIR}/Include)

target_link_libraries(gcl fmt::fmt libgcl)
