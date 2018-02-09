#pragma once

#define akVIEW_WILL_RESIGN_FIRSTRESPONDER 1
#define akVIEW_WILL_BECAME_FIRSTRESPONDER 2

class akViewNotificationReceiver {
public:
    virtual void ViewWillResignFirstResponder() = 0;
    virtual void ViewWillBecameFirstResponder() = 0;
};
