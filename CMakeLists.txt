cmake_minimum_required (VERSION 3.2.2)
project (appkit_workspace)

set(SDL_INCLUDE_DIR "" CACHE PATH "")
set(SDL_LIBRARY "" CACHE FILEPATH "")
set(SDL_IMAGE_LIBRARY "" CACHE FILEPATH "")
set(SDL_TTF_LIBRARY "" CACHE FILEPATH "")
if (!APPLE)
    set(SDLMAIN_LIBRARY "" CACHE FILEPATH "")
endif()

include_directories (${SDL_INCLUDE_DIR})
include_directories ("${PROJECT_SOURCE_DIR}/AppKit/include")

add_subdirectory (AppKit)
add_subdirectory (Demo)
