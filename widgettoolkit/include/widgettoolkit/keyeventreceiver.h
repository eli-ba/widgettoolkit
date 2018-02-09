#pragma once

#include <widgettoolkit/export.h>
#include <widgettoolkit/keyevent.h>

class akView;

class APPKIT_API akKeyEventReceiver {
public:
    virtual void KeyPress(akView* sender, akKeyEvent* event) = 0;
    virtual void KeyRelease(akView* sender, akKeyEvent* event) = 0;
    virtual void TextInput(akView* sender, akKeyEvent* event) = 0;
};
