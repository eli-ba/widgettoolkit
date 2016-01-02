#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <AppKit/appkit_export.h>
#include <AppKit/geometry.h>
#include <AppKit/basewindow.h>
#include <AppKit/aktypes.h>
#include <AppKit/view.h>
#include <AppKit/object.h>
#include <string>

using namespace std;

class APPKIT_API akWindow : public akObject, akPainter
{
public:
	akWindow(akRect contentRect, string title, long style);
	string GetTitle();
	akRect GetRect();
	akRect GetContentRect();
	void SetRect(akRect rect);
	void SetContentRect(akRect rect);
	bool IsVisible();
	void SetVisible(bool visible);
	void SetMaximizable(bool maximizable);
	void SetMinimizable(bool minimizable);
	void SetClosable(bool closable);
	bool IsMaximizable();
	bool IsMinimizable();
	bool IsClosable();
	void SetBackgroundColor(akColor color);
	akColor GetBackgroundColor();
	void Repaint();
	void AddView(akView *view);
	void RemoveView(akView *view);
	virtual void Paint(akView *view, SDL_Surface *destination);
	akView* GetFirstResponder();
	void SetFirstResponder(akView *view);
	void DispatchInputEvent(akInputEvent *evt);
	void SetCanReceiveMouseMoveEvents(bool receive);
	bool CanReceiveMouseMotionEvents();
	void Close();
	~akWindow();
public:
	BaseWindow *mBaseWindow;
	akView *mContentView;
	akColor mBgColor;
	akView *mFirstResponder;
	bool mCanReceiveMouseMotionEvents;
};

#endif
