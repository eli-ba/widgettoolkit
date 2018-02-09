#pragma once

#include <widgettoolkit/export.h>
#include <widgettoolkit/constants.h>
#include <widgettoolkit/inputevent.h>
#include <widgettoolkit/geometry.h>

class APPKIT_API akMouseEvent : public akInputEvent {
public:
    akMouseEvent();
    int GetMouseEvent();
    int GetButton();
    akPoint GetLocation();
    akPoint GetLocationInWindow();
    void SetMouseEvent(int mouseEvent);
    void SetButton(int button);
    void SetLocation(akPoint location);
    void SetLocationInWindow(akPoint location);

private:
    int mMouseEvent;
    int mButton;
    akPoint mLocation;
    akPoint mLocationInWindow;
};
