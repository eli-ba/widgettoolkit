#pragma once

#include <widgettoolkit/export.h>
#include <widgettoolkit/object.h>
#include <widgettoolkit/view.h>
#include <widgettoolkit/geometry.h>
#include <widgettoolkit/mouseevent.h>
#include <widgettoolkit/mouseeventreceiver.h>
#include <widgettoolkit/keyevent.h>
#include <widgettoolkit/keyeventreceiver.h>
#include <widgettoolkit/control.h>
#include <string>
#include <vector>

using namespace std;

class akRadioButton;

class APPKIT_API akRadioButtonGroup : public akObject {
public:
    akRadioButtonGroup();
    void Add(akRadioButton* radioButton);
    void Remove(akRadioButton* radioButton);
    vector<akRadioButton*> GetButtons();

private:
    vector<akRadioButton*> mButtons;
};

class APPKIT_API akRadioButton : public akControl, akPainter, akMouseEventReceiver, akKeyEventReceiver {
public:
    akRadioButton(akRect rect, string text, akView* parent = NULL);
    void SetText(string text);
    string GetText();
    void SetSelected(bool selected);
    bool IsSelected();
    void SetGroup(akRadioButtonGroup* group);

private:
    /* akMouseEventReceiver overrides */
    virtual void MousePress(akView* sender, akMouseEvent* event);
    virtual void MouseRelease(akView* sender, akMouseEvent* event);
    virtual void MouseMove(akView* sender, akMouseEvent* event);
    virtual void MouseDrag(akView* sender, akMouseEvent* event);
    virtual void MouseWheelUp(akView* sender, akMouseEvent* event);
    virtual void MouseWheelDown(akView* sender, akMouseEvent* event);
    /* akKeyEventReceiver overrides */
    virtual void KeyPress(akView* sender, akKeyEvent* event);
    virtual void KeyRelease(akView* sender, akKeyEvent* event);
    virtual void TextInput(akView* sender, akKeyEvent* event);
    /* akPainter overrides */
    virtual void Paint(akView* view, SDL_Surface* destination);

private:
    string mText;
    int mButtonState;
    akRadioButtonGroup* mGroup;
};
