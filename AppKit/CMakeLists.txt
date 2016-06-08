project (appkit)

set(INC_DIR include/AppKit)
set(SRC_DIR src)

if (WIN32)
    set(WIN32_SRCS ${SRC_DIR}/dllmain.cpp)
endif()

set (SOURCES
    ${INC_DIR}/actionreceiver.h
    ${INC_DIR}/aktypes.h
    ${INC_DIR}/appkit_export.h
    ${INC_DIR}/AppKit.h
    ${INC_DIR}/application.h
    ${INC_DIR}/basewindow.h
    ${INC_DIR}/constants.h
    ${INC_DIR}/control.h
    ${INC_DIR}/geometry.h
    ${INC_DIR}/inputevent.h
    ${INC_DIR}/keyevent.h
    ${INC_DIR}/keyeventreceiver.h
    ${INC_DIR}/label.h
    ${INC_DIR}/legacytools.h
    ${INC_DIR}/listbox.h
    ${INC_DIR}/messagebox.h
    ${INC_DIR}/mouseevent.h
    ${INC_DIR}/mouseeventreceiver.h
    ${INC_DIR}/object.h
    ${INC_DIR}/painter.h
    ${INC_DIR}/pushbutton.h
    ${INC_DIR}/radiobutton.h
    ${INC_DIR}/resources.h
    ${INC_DIR}/targetver.h
    ${INC_DIR}/textbox.h
    ${INC_DIR}/utility.h
    ${INC_DIR}/view.h
    ${INC_DIR}/viewnotificationreceiver.h
    ${INC_DIR}/window.h
    ${INC_DIR}/windowinteractions.h
    ${INC_DIR}/windowmanager.h
    ${SRC_DIR}/aktypes.cpp
    ${SRC_DIR}/application.cpp
    ${SRC_DIR}/basewindow.cpp
    ${SRC_DIR}/control.cpp
    ${SRC_DIR}/geometry.cpp
    ${SRC_DIR}/inputevent.cpp
    ${SRC_DIR}/keyevent.cpp
    ${SRC_DIR}/label.cpp
    ${SRC_DIR}/legacytools.cpp
    ${SRC_DIR}/listbox.cpp
    ${SRC_DIR}/messagebox.cpp
    ${SRC_DIR}/mouseevent.cpp
    ${SRC_DIR}/object.cpp
    ${SRC_DIR}/pushbutton.cpp
    ${SRC_DIR}/radiobutton.cpp
    ${SRC_DIR}/resources.cpp
    ${SRC_DIR}/textbox.cpp
    ${SRC_DIR}/utility.cpp
    ${SRC_DIR}/view.cpp
    ${SRC_DIR}/window.cpp
    ${SRC_DIR}/windowinteractions.cpp
    ${SRC_DIR}/windowmanager.cpp
    ${WIN32_SRCS}
    )
add_library (appkit SHARED ${SOURCES})

target_link_libraries (appkit ${EXTRA_LIBS} ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARY} ${SDL_TTF_LIBRARY} ${SDLMAIN_LIBRARY})
#set_property(TARGET appkit PROPERTY CXX_STANDARD 11)
