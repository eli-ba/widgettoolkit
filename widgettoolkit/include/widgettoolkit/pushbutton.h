#pragma once

#include <widgettoolkit/export.h>
#include <widgettoolkit/view.h>
#include <widgettoolkit/geometry.h>
#include <widgettoolkit/mouseevent.h>
#include <widgettoolkit/mouseeventreceiver.h>
#include <widgettoolkit/actionreceiver.h>
#include <widgettoolkit/painter.h>
#include <widgettoolkit/control.h>
#include <string>

using namespace std;

class APPKIT_API akPushButton : public akControl, akMouseEventReceiver, akPainter {
public:
    akPushButton(akRect rect, string text, akView* parent = NULL);
    string GetText();
    void SetText(string text);

private:
    virtual void MousePress(akView* sender, akMouseEvent* event);
    virtual void MouseRelease(akView* sender, akMouseEvent* event);
    virtual void MouseMove(akView* sender, akMouseEvent* event);
    virtual void MouseDrag(akView* sender, akMouseEvent* event);
    virtual void MouseWheelUp(akView* sender, akMouseEvent* event);
    virtual void MouseWheelDown(akView* sender, akMouseEvent* event);
    virtual void Paint(akView* view, SDL_Surface* destination);

private:
    string mText;
    int mButtonState;
};
