cmake_minimum_required(VERSION 3.12)
project(Chip_8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};D:\\Install\\codeLib\\SDL2-2.30.7\\x86_64-w64-mingw32\\lib\\cmake\\SDL2")

option(USE_SDL2 "Use SDL2" ON)

if(USE_SDL2)
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};D:\\Install\\codeLib\\SDL2-2.30.7\\i686-w64-mingw32\\lib\\cmake\\SDL2")
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
endif()

# Add source files
file (GLOB SRC "*.cpp")

if (USE_SDL2)
    add_executable(Chip_8 ${SRC})
    target_link_libraries(Chip_8 ${SDL2_LIBRARIES})
else ()
    add_executable(Chip_8 main.cpp Chip8.cpp)
endif()


target_compile_options(Chip_8 PRIVATE -Wall)

add_custom_command(TARGET Chip_8 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Chip_8> $<TARGET_FILE_DIR:Chip_8>
        COMMAND_EXPAND_LISTS
)


