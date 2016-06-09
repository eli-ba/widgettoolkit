project (demo)

include_directories ("${PROJECT_SOURCE_DIR}")

if (APPLE)
    set (OSX_SOURCES mac/SDLMain.h mac/SDLMain.m)
    find_library (COCOA_LIBRARY Cocoa)
    mark_as_advanced (COCOA_LIBRARY)
    set (EXTRA_LIBS ${COCOA_LIBRARY})
endif()

add_executable (demo widgets_example.cpp)
#add_executable (demo demo.h demo.cpp ${OSX_SOURCES})

target_link_libraries (demo appkit ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${COCOA_LIBRARY})
