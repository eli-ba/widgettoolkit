cmake_minimum_required (VERSION 3.2.2)
project (appkit_workspace)

set(SDL2_INCLUDE_DIR "" CACHE PATH "")
set(SDL2_LIBRARY "" CACHE FILEPATH "")
set(SDL2_IMAGE_LIBRARY "" CACHE FILEPATH "")
set(SDL2_TTF_LIBRARY "" CACHE FILEPATH "")

if (APPLE)
    set(SDL2_IMAGE_INCLUDE_DIR "" CACHE FILEPATH "")
    set(SDL2_TTF_INCLUDE_DIR "" CACHE FILEPATH "")
endif()

# Include directories
include_directories (${SDL2_INCLUDE_DIR})
if (APPLE)
    include_directories (${SDL2_IMAGE_INCLUDE_DIR})
    include_directories (${SDL2_TTF_INCLUDE_DIR})
endif()
include_directories ("${PROJECT_SOURCE_DIR}/AppKit/include")

# Subdirectories
add_subdirectory (AppKit)
add_subdirectory (Demo)
