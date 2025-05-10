cmake_minimum_required(VERSION 3.20)

add_library(glbasimac)

file(GLOB_RECURSE GLBASIMAC_SOURCES CONFIGURE_DEPENDS src/*)

target_sources(glbasimac PRIVATE ${GLBASIMAC_SOURCES})
target_compile_features(glbasimac PUBLIC cxx_std_17)
target_include_directories(glbasimac PUBLIC .)

# ---Add OpenGL---
find_package(OpenGL REQUIRED)
target_link_libraries(glbasimac PUBLIC OpenGL::GL)

# ---Add GLFW---
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

target_link_libraries(glbasimac PUBLIC glfw)

# ---Add glad---
add_library(glad glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

target_link_libraries(glbasimac PUBLIC glad)


# ---Add exe_path---
include(FetchContent)
FetchContent_Declare(
    exe_path
    GIT_REPOSITORY https://github.com/coollibs/exe_path.git
    GIT_TAG 5c0679bca78a4caa38809461622f2dde712470e6
)
FetchContent_MakeAvailable(exe_path)

target_link_libraries(glbasimac PUBLIC exe_path::exe_path)