cmake_minimum_required(VERSION 3.15)
project(BlockEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")
endif()

# Dependencies directory
set(DEPS_DIR "${CMAKE_SOURCE_DIR}/dependencies")

# Add luau
if(EXISTS "${DEPS_DIR}/luau/CMakeLists.txt")
    add_subdirectory(${DEPS_DIR}/luau)
    set(LUAU_FOUND TRUE)
else()
    message(FATAL_ERROR "luau not found in dependencies")
endif()

# Add raylib
if(EXISTS "${DEPS_DIR}/raylib/CMakeLists.txt")
    add_subdirectory(${DEPS_DIR}/raylib)
    set(raylib_FOUND TRUE)
else()
    message(FATAL_ERROR "raylib not found in dependencies")
endif()

# Collect source files
file(GLOB_RECURSE SOURCES "src/**/*.cpp")
list(APPEND SOURCES src/main.cpp)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${DEPS_DIR}/luau/Ast/include
    ${DEPS_DIR}/luau/Config/include
    ${DEPS_DIR}/luau/VM/include
    ${DEPS_DIR}/luau/Compiler/include
    ${DEPS_DIR}/luau/Analysis/include
    ${CMAKE_SOURCE_DIR}
)

# Link Luau libraries
if(LUAU_FOUND)
    target_link_libraries(${PROJECT_NAME}
        Luau.Ast
        Luau.Config
        Luau.Analysis
        Luau.Compiler
        Luau.VM
    )
endif()

# Link raylib
if(raylib_FOUND)
    target_link_libraries(${PROJECT_NAME} raylib)
endif()

# Platform-specific libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        winmm
        gdi32
        opengl32
    )
else()
    target_link_libraries(${PROJECT_NAME}
        m
        dl
        pthread
        GL
        rt
        X11
    )
endif()