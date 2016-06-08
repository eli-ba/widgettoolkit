project (demo)

include_directories ("${PROJECT_SOURCE_DIR}")

add_executable (demo widgets_example.cpp)
#add_executable (demo demo.h demo.cpp)

target_link_libraries (demo appkit ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARY} ${SDL_TTF_LIBRARY} ${SDLMAIN_LIBRARY})