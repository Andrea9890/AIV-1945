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
add_custom_command(TARGET game1945 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/lib/raylib.dll $<TARGET_FILE_DIR:game1945>
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:game1945>/assets
)
