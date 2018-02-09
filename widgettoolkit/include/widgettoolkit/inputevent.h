#pragma once

#include <widgettoolkit/export.h>
#include <widgettoolkit/constants.h>
#include <widgettoolkit/object.h>

class akWindow;

class APPKIT_API akInputEvent : public akObject {
public:
    akInputEvent();
    void SetWindow(akWindow* wnd);
    akWindow* GetWindow();
    void SetType(int type);
    int GetType();

private:
    akWindow* mWindow;
    int mType;

public:
    void* Reserved;
};
