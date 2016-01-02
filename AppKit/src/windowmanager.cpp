#include <stdlib.h>
#include <AppKit/windowmanager.h>
#include <AppKit/resources.h>
#include <AppKit/constants.h>
#include <AppKit/mouseevent.h>
#include <AppKit/keyevent.h>
#include <AppKit/window.h>
#include <AppKit/utility.h>

#define APPKIT_DEFAULT_APPLICATION_NAME "AppKit Application"

WindowManager::WindowManager(akSize resolution, const string &name)
{
	mResolution = resolution;
	mApplicationName = name;

	TTF_Init();
	Resources::Load();

	SDL_Init(SDL_INIT_VIDEO);
	mScreen = SDL_SetVideoMode(mResolution.width, mResolution.height, 32, SDL_HWSURFACE);
	if (!mScreen) {
		exit(EXIT_FAILURE);
	}
	SDL_EnableUNICODE(1);
	SDL_EnableKeyRepeat(500,30);
	mDefaultBgColor = SDL_MapRGB(mScreen->format, 128, 128, 150);
	SDL_FillRect(mScreen, NULL, mDefaultBgColor);
	SDL_WM_SetCaption(name.c_str(), NULL);
	SDL_Flip(mScreen);

	mWindowInteractions = new WindowInteractions(this);
}

void WindowManager::Close()
{
	mRunning = false;
}

akInputEvent* WindowManager::ParseSDLEvent(SDL_Event *evt)
{
	static bool mouseButtonPressed = false;
	akMouseEvent *mouseEvent = NULL;
	akKeyEvent *keyEvent = NULL;

	if (evt->type == SDL_KEYDOWN)
	{
		keyEvent = new akKeyEvent();
		keyEvent->SetUnicode(evt->key.keysym.unicode);
		keyEvent->SetKeyEvent(akKEY_EVENT_PRESS);
		keyEvent->SetWindow((akWindow*)(mWindowInteractions->GetTopLevelWindow()->Window));
		return keyEvent;
	}
	else if (evt->type == SDL_KEYUP)
	{
		keyEvent = new akKeyEvent();
		keyEvent->SetUnicode(evt->key.keysym.unicode);
		keyEvent->SetKeyEvent(akKEY_EVENT_RELEASE);
		keyEvent->SetWindow((akWindow*)(mWindowInteractions->GetTopLevelWindow()->Window));
		return keyEvent;
	}
	else if (evt->type == SDL_MOUSEBUTTONDOWN)
	{
		mouseButtonPressed = true;

		mouseEvent = new akMouseEvent();
		akPoint location = akPoint(evt->motion.x, evt->motion.y);
		
		BaseWindow *wnd = mWindowInteractions->GetWindowThatContainsPoint(location);
		if (!wnd) {
			return NULL;
		}
		else {
			mouseEvent->SetLocation(location);
			mouseEvent->SetWindow((akWindow*)wnd->Window);

			/* Calculating the location in window */
			akPoint locationInWindow;
			locationInWindow.x = location.x - wnd->GetRect().location.x - wnd->GetLeftMargin();
			locationInWindow.y = location.y - wnd->GetRect().location.y - wnd->GetTopMargin();
			mouseEvent->SetLocationInWindow(locationInWindow);

			if (evt->button.button == SDL_BUTTON_WHEELUP) {
				mouseEvent->SetMouseEvent(akMOUSE_EVENT_WHEEL_UP);
			}
			else if (evt->button.button == SDL_BUTTON_WHEELDOWN) {
				mouseEvent->SetMouseEvent(akMOUSE_EVENT_WHEEL_DOWN);
			}
			else {
				mouseEvent->SetMouseEvent(akMOUSE_EVENT_PRESS);
		
				if (evt->button.button == SDL_BUTTON_LEFT)
					mouseEvent->SetButton(akMOUSE_BUTTON_LEFT);
				else if (evt->button.button == SDL_BUTTON_RIGHT)
					mouseEvent->SetButton(akMOUSE_BUTTON_RIGHT);
			}
			return mouseEvent;
		}
		return NULL;
	}
	else if (evt->type == SDL_MOUSEBUTTONUP)
	{
		mouseButtonPressed = false;

		mouseEvent = new akMouseEvent();
		akPoint location = akPoint(evt->motion.x, evt->motion.y);
		
		BaseWindow *wnd = mWindowInteractions->GetWindowThatContainsPoint(location);
		if (!wnd) {
			return NULL;
		}
		else {
			mouseEvent->SetLocation(location);
			mouseEvent->SetWindow((akWindow*)wnd->Window);

			/* Calculating the location in window */
			akPoint locationInWindow;
			locationInWindow.x = location.x - wnd->GetRect().location.x - wnd->GetLeftMargin();
			locationInWindow.y = location.y - wnd->GetRect().location.y - wnd->GetTopMargin();
			mouseEvent->SetLocationInWindow(locationInWindow);

			mouseEvent->SetMouseEvent(akMOUSE_EVENT_RELEASE);

			if (evt->button.button == SDL_BUTTON_LEFT)
				mouseEvent->SetButton(akMOUSE_BUTTON_LEFT);
			else if (evt->button.button == SDL_BUTTON_RIGHT)
				mouseEvent->SetButton(akMOUSE_BUTTON_RIGHT);
			return mouseEvent;
		}
		return NULL;
	}
	else if (evt->type == SDL_MOUSEMOTION)
	{
		mouseEvent = new akMouseEvent();
		akPoint location = akPoint(evt->motion.x, evt->motion.y);

		BaseWindow *wnd = mWindowInteractions->GetWindowThatContainsPoint(location);
		if (!wnd){ 
			return NULL;
		}
		else {
			mouseEvent->SetLocation(location);
			mouseEvent->SetWindow((akWindow*)wnd->Window);

			/* Calculating the location in window */
			akPoint locationInWindow;
			locationInWindow.x = location.x - wnd->GetRect().location.x - wnd->GetLeftMargin();
			locationInWindow.y = location.y - wnd->GetRect().location.y - wnd->GetTopMargin();
			mouseEvent->SetLocationInWindow(locationInWindow);

			if (mouseButtonPressed)
				mouseEvent->SetMouseEvent(akMOUSE_EVENT_DRAG);
			else
				mouseEvent->SetMouseEvent(akMOUSE_EVENT_MOVE);
			mouseEvent->SetButton(akMOUSE_BUTTON_NONE);
			return mouseEvent;
		}
		return NULL;
	}
	return NULL;
}

void WindowManager::MakeTopLevel(BaseWindow *wnd)
{
	if (!wnd) return;

	BaseWindow *wndToMakeTopLevel = NULL;
	for (unsigned int i=0; i<mWindows.size(); i++) {
		wndToMakeTopLevel = mWindows.at(i);
		if (wndToMakeTopLevel == wnd) {
			mWindows.erase(mWindows.begin()+i);
			break;
		}
	}
	if (wndToMakeTopLevel) {
		mWindows.push_back(wndToMakeTopLevel);
	}
}

void WindowManager::ProcessEvents()
{
	SDL_Event evt;

	SDL_WaitEvent(&evt);

	switch(evt.type)
	{
	case SDL_QUIT:
		exit(0);
		break;
	}

	if (mWindows.empty()) return;

	akInputEvent *inputEvent = ParseSDLEvent(&evt);

	if (inputEvent)
	{
		inputEvent->Reserved = &evt;
		
		if (inputEvent->GetType() == akINPUT_EVENT_MOUSE) // All mouse events
		{
			akMouseEvent *mouseEvent = (akMouseEvent*)inputEvent;
			
			akPoint locationInWindow = mouseEvent->GetLocationInWindow();
			akRect contentRect = mouseEvent->GetWindow()->GetContentRect();
			if (Utility::RectContainsPoint(contentRect,locationInWindow))
			{
				if (mouseEvent->GetMouseEvent() == akMOUSE_EVENT_MOVE) // Only mouse move events
				{
					if (mWindows.size() > 0) {
						BaseWindow *topLevelWindow = mWindowInteractions->GetTopLevelWindow();
						akWindow *wnd = (akWindow*)topLevelWindow->Window;
						if (wnd->CanReceiveMouseMotionEvents()) wnd->DispatchInputEvent(mouseEvent);

						akWindow *intersectingWindow = mouseEvent->GetWindow();
						if (intersectingWindow) {
							if (intersectingWindow->CanReceiveMouseMotionEvents()) intersectingWindow->DispatchInputEvent(mouseEvent);
						}
					}
				}
				else if (mouseEvent->GetMouseEvent() == akMOUSE_EVENT_DRAG)
				{
					if (mWindows.size() > 0) {
						BaseWindow *topLevelWindow = mWindowInteractions->GetTopLevelWindow();
						akWindow *wnd = (akWindow*)topLevelWindow->Window;
						wnd->DispatchInputEvent(mouseEvent);
					}
				}
				else { // Any other mouse event
					akWindow *intersectingWindow = mouseEvent->GetWindow();
					if (intersectingWindow) {
						//printf("another mouse event will be dispatched...\n");
						intersectingWindow->DispatchInputEvent(mouseEvent);
					}
				}
			}
		}
		else if (inputEvent->GetType() == akINPUT_EVENT_KEY) // All key events
		{
			if (mWindows.size() == 0) return;

			akKeyEvent *keyEvent = (akKeyEvent*)inputEvent;
			BaseWindow *topLevelWindow = mWindowInteractions->GetTopLevelWindow();
			if (topLevelWindow) {
				akWindow *wnd = (akWindow*)topLevelWindow->Window;
				wnd->DispatchInputEvent(keyEvent);
				//printf("a key event was dispatched to '%s'\n", wnd->GetTitle().c_str());
			}
		}

		BaseWindow *wnd = inputEvent->GetWindow()->mBaseWindow;
		if (wnd->NeedsToBeClosed()) {
			RemoveWindow(wnd);
			delete wnd;
			ComposeWindows();
		}
		else
			mWindowInteractions->OnInputEvent(inputEvent);
		
		delete inputEvent;
	}
}

void WindowManager::AddWindow(BaseWindow *wnd)
{
	mWindows.push_back(wnd);
}

BaseWindow* WindowManager::RemoveWindow(BaseWindow *wnd)
{	
	for (unsigned int i=0; i<mWindows.size(); i++) {
		BaseWindow *bw = mWindows.at(i);
		if (bw == wnd) {
			mWindows.erase(mWindows.begin()+i);
			return wnd;
		}
	}
	return NULL;
}

void WindowManager::ComposeWindows()
{
	SDL_FillRect(mScreen, NULL, mDefaultBgColor);
	for (unsigned int i=0; i<mWindows.size(); i++) {
		BaseWindow *wnd = mWindows.at(i);
		if (wnd->IsVisible()) {
			SDL_Rect rect;
			akRect wndRect = wnd->GetRect();
			rect.x = wndRect.location.x;
			rect.y = wndRect.location.y;
			rect.w = wndRect.size.width;
			rect.h = wndRect.size.height;
			SDL_BlitSurface(wnd->GetSurface(), NULL, mScreen, &rect);
		}
	}
	SDL_Flip(mScreen);
}

vector<BaseWindow*> WindowManager::GetWindows()
{
	return mWindows;
}

akSize WindowManager::GetResolution()
{
	return mResolution;
}

SDL_PixelFormat* WindowManager::GetSDLPixelFormat()
{
	return mScreen->format;
}

WindowManager::~WindowManager()
{
	SDL_Quit();
}