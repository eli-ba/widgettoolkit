#pragma once

#include <stdlib.h>
#include <widgettoolkit/export.h>
#include <widgettoolkit/geometry.h>
#include <widgettoolkit/painter.h>
#include <widgettoolkit/inputevent.h>
#include <widgettoolkit/mouseevent.h>
#include <widgettoolkit/keyevent.h>
#include <widgettoolkit/keyeventreceiver.h>
#include <widgettoolkit/mouseeventreceiver.h>
#include <widgettoolkit/viewnotificationreceiver.h>
#include <widgettoolkit/object.h>
#include <widgettoolkit/types.h>
#include <vector>

class akWindow;

using namespace std;

class APPKIT_API akView : public akObject {
public:
    akView();
    akView(akRect rect, akView* parent = NULL);
    void SetRect(akRect rect);
    akRect GetRect();
    void AddChild(akView* view);
    akView* RemoveChild(akView* view);
    void Repaint();
    akWindow* GetWindow();
    void SetWindow(akWindow* wnd);
    void AddPainter(akPainter* painter);
    void RemovePainter(akPainter* painter);
    void AddKeyEventReceiver(akKeyEventReceiver* receiver);
    void RemoveKeyEventReceiver(akKeyEventReceiver* receiver);
    void AddMouseEventReceiver(akMouseEventReceiver* receiver);
    void RemoveMouseEventReceiver(akMouseEventReceiver* receiver);
    void AddViewNotificationReceiver(akViewNotificationReceiver* receiver);
    void RemoveViewNotificationReceiver(akViewNotificationReceiver* receiver);
    void InvokeKeyEventReceivers(akKeyEvent* event);
    void InvokeMouseEventReceivers(akMouseEvent* event);
    void InvokeViewNotificationEventReceivers(int notification);
    bool SendInputEvent(akInputEvent* evt);
    void SetTag(int tag);
    int GetTag();
    static akColor GetFocusColor();
    static akColor GetLostFocusColor();
    static void SetFocusColor(akColor c);
    static void SetLostFocusColor(akColor c);
    virtual ~akView();

private:
    akRect mRect;
    vector<akView*> mChildren;
    akWindow* mWindow;
    vector<akKeyEventReceiver*> mKeyEventReceivers;
    vector<akMouseEventReceiver*> mMouseEventReceivers;
    vector<akViewNotificationReceiver*> mViewNotificationReceivers;
    vector<akPainter*> mPainters;
    int mTag;
    akView* mParent;
    static akColor mFocusColor;
    static akColor mLostFocusColor;
};
