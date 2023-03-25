option(LIBGCL_SHARED "Should libgcl be shared" ON)

set(LIBGCL_SOURCES ${PROJECT_SOURCE_DIR}/src/libgcl/file.cc)
if(LIBGCL_SHARED)
    add_library(libgcl SHARED ${LIBGCL_SOURCES})
else()
    add_library(libgcl STATIC ${LIBGCL_SOURCES})
endif()
set_target_properties(libgcl PROPERTIES OUTPUT_NAME "libgcl")

target_include_directories(libgcl PUBLIC ${PROJECT_SOURCE_DIR}/Include)