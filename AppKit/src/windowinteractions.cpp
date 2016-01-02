#include <AppKit/windowinteractions.h>
#include <AppKit/windowmanager.h>
#include <AppKit/utility.h>
#include <AppKit/mouseevent.h>
#include <AppKit/keyevent.h>
#include <AppKit/window.h>
#include <iostream>
#include <vector>

using namespace std;

WindowInteractions::WindowInteractions(WindowManager *wm)
{
	mWindowManager = wm;
	mCapturedWindow = NULL;
}

void WindowInteractions::OnInputEvent(akInputEvent *evt)
{
	if (!evt) return;

	static akPoint cursorDifference;
	static bool mustRecalculateCursorDifference = true;
	akMouseEvent *mouseEvent = NULL;
	akKeyEvent *keyEvent = NULL;

	if (evt->GetType() == akINPUT_EVENT_MOUSE)
	{
		mouseEvent = (akMouseEvent*)evt;

		BaseWindow *baseWnd = this->GetWindowThatContainsPoint(mouseEvent->GetLocation());
		if (baseWnd) {
			if (mouseEvent->GetMouseEvent() == akMOUSE_EVENT_DRAG)
			{
				if (!mCapturedWindow) {
					mCapturedWindow = baseWnd;
				}
				
				if (!mCapturedWindow->IsMaximized()) {
					if (Utility::RectContainsPoint(mCapturedWindow->GetTitleBarRect(), mouseEvent->GetLocation()))
					{
						akRect wndRect = mCapturedWindow->GetRect();
						if (mustRecalculateCursorDifference) {
							cursorDifference.x = mouseEvent->GetLocation().x - wndRect.location.x;
							cursorDifference.y = mouseEvent->GetLocation().y - wndRect.location.y;
							mustRecalculateCursorDifference = false;
						}
						akPoint newLocation;
						newLocation.x = mouseEvent->GetLocation().x - cursorDifference.x;
						newLocation.y = mouseEvent->GetLocation().y - cursorDifference.y;
						mCapturedWindow->SetRect(akRect(newLocation, wndRect.size));
						return;
					}
				}
			}
			else if(mouseEvent->GetMouseEvent() == akMOUSE_EVENT_PRESS)
			{
				if (baseWnd != GetTopLevelWindow()) {
					mWindowManager->RemoveWindow(baseWnd);
					mWindowManager->AddWindow(baseWnd);
					mWindowManager->ComposeWindows();
				}
			}
			else if(mouseEvent->GetMouseEvent() == akMOUSE_EVENT_RELEASE)
			{
				if (mCapturedWindow) mCapturedWindow = NULL;

				mustRecalculateCursorDifference = true;
			
				if (Utility::RectContainsPoint(baseWnd->GetCloseButtonRect(), mouseEvent->GetLocation()))
				{
					if (baseWnd->IsClosable()) {
						mWindowManager->RemoveWindow(baseWnd);
						delete baseWnd;
						mWindowManager->ComposeWindows();
					}
				}
				else if (Utility::RectContainsPoint(baseWnd->GetMaximizeButtonRect(), mouseEvent->GetLocation()))
				{
					if (!baseWnd->IsMinimized()) {
						if (baseWnd->IsMaximizable()) {
							if (baseWnd->IsMaximized())
								baseWnd->SetMaximized(false);
							else
								baseWnd->SetMaximized(true);
						}
					}
				}
				else if (Utility::RectContainsPoint(baseWnd->GetMinimizeButtonRect(), mouseEvent->GetLocation()))
				{
					if (!baseWnd->IsMaximized()) {
						if (baseWnd->IsMinimizable()) {
							if (baseWnd->IsMinimized())
								baseWnd->SetMinimized(false);
							else
								baseWnd->SetMinimized(true);
						}
					}
				}
			}
		}
	}
}

BaseWindow* WindowInteractions::GetTopLevelWindow()
{
	return mWindowManager->GetWindows().back();
}

BaseWindow* WindowInteractions::GetWindowThatContainsPoint(akPoint point)
{
	if (mWindowManager->GetWindows().size() == 0) return NULL;

	vector<BaseWindow*> windows = mWindowManager->GetWindows();
	int windowCount = windows.size();
	for (int i= windowCount-1; i>=0; i--) {
		BaseWindow *wnd = windows.at(i);
		akRect wndRect = wnd->GetRect();
		if (Utility::RectContainsPoint(wndRect, point))
			return wnd;
	}
	return NULL;
}