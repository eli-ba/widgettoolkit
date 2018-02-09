#pragma once

#include <widgettoolkit/export.h>
#include <widgettoolkit/inputevent.h>
#include <string>

class APPKIT_API akKeyEvent : public akInputEvent {
public:
    akKeyEvent();
    //int GetUnicode();
    int GetKeyEvent();
    //void SetUnicode(int unicode);
    void SetKeyEvent(int keyEvent);
    void SetText(std::string text);
    std::string const& GetText();
    int32_t GetScancode() const { return mScancode; }
    void SetScancode(int32_t scancode) { mScancode = scancode; }
    int32_t GetKeycode() const { return mKeycode; }
    void SetKeycode(int32_t keycode) { mKeycode = keycode; }
    int32_t GetKeymod() const { return mKeymod; }
    void SetKeymod(int32_t keymod) { mKeymod = keymod; }
private:
    //int mUnicode;
    int mKeyEvent;
    std::string mText;
    int32_t mScancode = 0;
    int32_t mKeycode = 0;
    int32_t mKeymod = 0;
};

