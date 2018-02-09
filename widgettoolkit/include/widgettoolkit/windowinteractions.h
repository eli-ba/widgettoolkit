#pragma once

#include <widgettoolkit/inputevent.h>
#include <widgettoolkit/basewindow.h>

class WindowManager;

class WindowInteractions {
public:
    WindowInteractions(WindowManager* wm);
    void OnInputEvent(akInputEvent* evt);
    BaseWindow* GetTopLevelWindow();
    BaseWindow* GetWindowThatContainsPoint(akPoint point);

public:
    WindowManager* mWindowManager;
    BaseWindow* mTopLevelWindow;
    BaseWindow* mCapturedWindow;
};
