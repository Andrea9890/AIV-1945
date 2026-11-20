cmake_minimum_required(VERSION 3.20)
project(game1945 C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Every .c under src\
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c)
add_executable(game1945 ${SOURCES})

target_include_directories(game1945 PRIVATE include)

# Import libraries
target_link_libraries(game1945 PRIVATE ${CMAKE_SOURCE_DIR}/lib/raylibdll.lib)

# After build, copy raylib.dll and assets\ next to the exe
add_custom_target(copy_runtime_files
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/lib/raylib.dll ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMENT "Copying raylib.dll and assets"
)
add_dependencies(game1945 copy_runtime_files)
