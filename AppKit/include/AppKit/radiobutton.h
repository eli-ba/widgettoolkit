#ifndef _RADIOBUTTON_H_
#define _RADIOBUTTON_H_

#include <AppKit/appkit_export.h>
#include <AppKit/object.h>
#include <AppKit/view.h>
#include <AppKit/geometry.h>
#include <AppKit/mouseevent.h>
#include <AppKit/mouseeventreceiver.h>
#include <AppKit/keyevent.h>
#include <AppKit/keyeventreceiver.h>
#include <AppKit/control.h>
#include <string>
#include <vector>

using namespace std;

class akRadioButton;

class APPKIT_API akRadioButtonGroup : public akObject
{
public:
	akRadioButtonGroup();
	void Add(akRadioButton *radioButton);
	void Remove(akRadioButton *radioButton);
	vector<akRadioButton*> GetButtons();
private:
	vector<akRadioButton*> mButtons;
};

class APPKIT_API akRadioButton : public akControl, akPainter, akMouseEventReceiver, akKeyEventReceiver
{
public:
	akRadioButton(akRect rect, string text, akView *parent = NULL);
	void SetText(string text);
	string GetText();
	void SetSelected(bool selected);
	bool IsSelected();
	void SetGroup(akRadioButtonGroup *group);
private:
	/* akMouseEventReceiver overrides */
	virtual void MousePress(akView* sender, akMouseEvent *event);
    virtual void MouseRelease(akView* sender, akMouseEvent *event);
    virtual void MouseMove(akView* sender, akMouseEvent *event);
    virtual void MouseDrag(akView* sender, akMouseEvent *event);
	virtual void MouseWheelUp(akView *sender, akMouseEvent *event);
	virtual void MouseWheelDown(akView *sender, akMouseEvent *event);
	/* akKeyEventReceiver overrides */
	virtual void KeyPress(akView* sender, akKeyEvent *event);
    virtual void KeyRelease(akView* sender, akKeyEvent *event);
	/* akPainter overrides */
	virtual void Paint(akView *view, SDL_Surface *destination);
private:
	string mText;
	int mButtonState;
	akRadioButtonGroup *mGroup;
};

#endif
