project (sample1)

include_directories ("${PROJECT_SOURCE_DIR}")

if (APPLE)
    set (OSX_SOURCES mac/SDLMain.h mac/SDLMain.m)
    find_library (COCOA_LIBRARY Cocoa)
    mark_as_advanced (COCOA_LIBRARY)
    set (EXTRA_LIBS ${COCOA_LIBRARY})
endif()

add_executable (sample1 sample1.h sample1.cpp)

target_link_libraries (sample1 widgettoolkit ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${COCOA_LIBRARY})
set_property(TARGET sample1 PROPERTY CXX_STANDARD 11)
