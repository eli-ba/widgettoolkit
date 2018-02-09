#include <stdlib.h>
#include <widgettoolkit/windowmanager.h>
#include <widgettoolkit/resources.h>
#include <widgettoolkit/constants.h>
#include <widgettoolkit/mouseevent.h>
#include <widgettoolkit/keyevent.h>
#include <widgettoolkit/window.h>
#include <widgettoolkit/utility.h>

#define APPKIT_DEFAULT_APPLICATION_NAME "AppKit Application"

WindowManager::WindowManager(akSize resolution, const string& name)
{
    mResolution = resolution;
    mApplicationName = name;
    mMouseMotion = akPoint(0, 0);

    TTF_Init();
    Resources::Load();

    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        _window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mResolution.width, mResolution.height, SDL_WINDOW_SHOWN);
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
    }
    else {
        exit(EXIT_FAILURE);
    }

    SDL_SetRenderDrawColor(_renderer, 128, 128, 150, 255);
    SDL_RenderClear(_renderer);

    mWindowInteractions = new WindowInteractions(this);
}

void WindowManager::Close()
{
    mRunning = false;
}

akInputEvent* WindowManager::ParseSDLEvent(SDL_Event* evt)
{
    static bool mouseButtonPressed = false;
    akMouseEvent* mouseEvent = NULL;
    akKeyEvent* keyEvent = NULL;

    if (evt->type == SDL_KEYDOWN) {
        keyEvent = new akKeyEvent();
        keyEvent->SetKeycode(evt->key.keysym.sym);
        keyEvent->SetKeymod(evt->key.keysym.mod);
        keyEvent->SetScancode(evt->key.keysym.scancode);
        keyEvent->SetKeyEvent(akKEY_EVENT_PRESS);
        keyEvent->SetWindow((akWindow*)(mWindowInteractions->GetTopLevelWindow()->Window));
        return keyEvent;
    }
    else if (evt->type == SDL_KEYUP) {
        keyEvent = new akKeyEvent();
        keyEvent->SetKeycode(evt->key.keysym.sym);
        keyEvent->SetKeymod(evt->key.keysym.mod);
        keyEvent->SetScancode(evt->key.keysym.scancode);
        keyEvent->SetKeyEvent(akKEY_EVENT_RELEASE);
        keyEvent->SetWindow((akWindow*)(mWindowInteractions->GetTopLevelWindow()->Window));
        return keyEvent;
    }
    else if (evt->type == SDL_TEXTINPUT) {
        keyEvent = new akKeyEvent();
        keyEvent->SetKeyEvent(akKEY_EVENT_TEXT);
        keyEvent->SetText(evt->text.text);
        keyEvent->SetWindow((akWindow*)(mWindowInteractions->GetTopLevelWindow()->Window));
        return keyEvent;
    }
    else if (evt->type == SDL_MOUSEWHEEL) {
        mouseEvent = new akMouseEvent();

        BaseWindow* wnd = mWindowInteractions->GetWindowThatContainsPoint(mMouseMotion);
        if (!wnd) {
            return NULL;
        }
        else {
            mouseEvent->SetLocation(mMouseMotion);
            mouseEvent->SetWindow((akWindow*)wnd->Window);

            /* Calculating the location in window */
            akPoint locationInWindow;
            locationInWindow.x = mMouseMotion.x - wnd->GetRect().location.x - wnd->GetLeftMargin();
            locationInWindow.y = mMouseMotion.y - wnd->GetRect().location.y - wnd->GetTopMargin();
            mouseEvent->SetLocationInWindow(locationInWindow);

            if (evt->wheel.y > 0) {
                mouseEvent->SetMouseEvent(akMOUSE_EVENT_WHEEL_UP);
            }
            else {
                mouseEvent->SetMouseEvent(akMOUSE_EVENT_WHEEL_DOWN);
            }

            return mouseEvent;
        }

        return NULL;
    }
    else if (evt->type == SDL_MOUSEBUTTONDOWN) {
        mouseButtonPressed = true;

        mouseEvent = new akMouseEvent();
        akPoint location = akPoint(evt->motion.x, evt->motion.y);

        BaseWindow* wnd = mWindowInteractions->GetWindowThatContainsPoint(location);
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

            mouseEvent->SetMouseEvent(akMOUSE_EVENT_PRESS);

            if (evt->button.button == SDL_BUTTON_LEFT)
                mouseEvent->SetButton(akMOUSE_BUTTON_LEFT);
            else if (evt->button.button == SDL_BUTTON_RIGHT)
                mouseEvent->SetButton(akMOUSE_BUTTON_RIGHT);

            return mouseEvent;
        }
        return NULL;
    }
    else if (evt->type == SDL_MOUSEBUTTONUP) {
        mouseButtonPressed = false;

        mouseEvent = new akMouseEvent();
        akPoint location = akPoint(evt->motion.x, evt->motion.y);

        BaseWindow* wnd = mWindowInteractions->GetWindowThatContainsPoint(location);
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
    else if (evt->type == SDL_MOUSEMOTION) {
        mouseEvent = new akMouseEvent();
        akPoint location = akPoint(evt->motion.x, evt->motion.y);
        mMouseMotion = location;

        BaseWindow* wnd = mWindowInteractions->GetWindowThatContainsPoint(location);
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

void WindowManager::MakeTopLevel(BaseWindow* wnd)
{
    if (!wnd)
        return;

    BaseWindow* wndToMakeTopLevel = NULL;
    for (unsigned int i = 0; i < mWindows.size(); i++) {
        wndToMakeTopLevel = mWindows.at(i);
        if (wndToMakeTopLevel == wnd) {
            mWindows.erase(mWindows.begin() + i);
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

    switch (evt.type) {
    case SDL_QUIT:
        exit(0);
        break;
    }

    if (mWindows.empty())
        return;

    akInputEvent* inputEvent = ParseSDLEvent(&evt);

    if (inputEvent) {
        inputEvent->Reserved = &evt;

        if (inputEvent->GetType() == akINPUT_EVENT_MOUSE) // All mouse events
        {
            akMouseEvent* mouseEvent = (akMouseEvent*)inputEvent;

            akPoint locationInWindow = mouseEvent->GetLocationInWindow();
            akRect contentRect = mouseEvent->GetWindow()->GetContentRect();
            if (Utility::RectContainsPoint(contentRect, locationInWindow)) {
                if (mouseEvent->GetMouseEvent() == akMOUSE_EVENT_MOVE) // Only mouse move events
                {
                    if (mWindows.size() > 0) {
                        BaseWindow* topLevelWindow = mWindowInteractions->GetTopLevelWindow();
                        akWindow* wnd = (akWindow*)topLevelWindow->Window;
                        if (wnd->CanReceiveMouseMotionEvents())
                            wnd->DispatchInputEvent(mouseEvent);

                        akWindow* intersectingWindow = mouseEvent->GetWindow();
                        if (intersectingWindow) {
                            if (intersectingWindow->CanReceiveMouseMotionEvents())
                                intersectingWindow->DispatchInputEvent(mouseEvent);
                        }
                    }
                }
                else if (mouseEvent->GetMouseEvent() == akMOUSE_EVENT_DRAG) {
                    if (mWindows.size() > 0) {
                        BaseWindow* topLevelWindow = mWindowInteractions->GetTopLevelWindow();
                        akWindow* wnd = (akWindow*)topLevelWindow->Window;
                        wnd->DispatchInputEvent(mouseEvent);
                    }
                }
                else { // Any other mouse event
                    akWindow* intersectingWindow = mouseEvent->GetWindow();
                    if (intersectingWindow) {
                        //printf("another mouse event will be dispatched...\n");
                        intersectingWindow->DispatchInputEvent(mouseEvent);
                    }
                }
            }
        }
        else if (inputEvent->GetType() == akINPUT_EVENT_KEY) // All key events
        {
            if (mWindows.size() == 0)
                return;

            akKeyEvent* keyEvent = (akKeyEvent*)inputEvent;
            BaseWindow* topLevelWindow = mWindowInteractions->GetTopLevelWindow();
            if (topLevelWindow) {
                akWindow* wnd = (akWindow*)topLevelWindow->Window;
                wnd->DispatchInputEvent(keyEvent);
                //printf("a key event was dispatched to '%s'\n", wnd->GetTitle().c_str());
            }
        }

        BaseWindow* wnd = inputEvent->GetWindow()->mBaseWindow;
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

void WindowManager::AddWindow(BaseWindow* wnd)
{
    mWindows.push_back(wnd);
}

BaseWindow* WindowManager::RemoveWindow(BaseWindow* wnd)
{
    for (unsigned int i = 0; i < mWindows.size(); i++) {
        BaseWindow* bw = mWindows.at(i);
        if (bw == wnd) {
            mWindows.erase(mWindows.begin() + i);
            return wnd;
        }
    }
    return NULL;
}

void WindowManager::ComposeWindows()
{
    SDL_SetRenderDrawColor(_renderer, 65, 105, 170, 255);
    SDL_RenderClear(_renderer);

    for (unsigned int i = 0; i < mWindows.size(); i++) {
        BaseWindow* wnd = mWindows.at(i);
        if (wnd->IsVisible()) {
            SDL_Rect rect;
            akRect wndRect = wnd->GetRect();
            rect.x = wndRect.location.x;
            rect.y = wndRect.location.y;
            rect.w = wndRect.size.width;
            rect.h = wndRect.size.height;

            SDL_Texture* tex = SDL_CreateTextureFromSurface(_renderer, wnd->GetSurface());
            SDL_RenderCopy(_renderer, tex, NULL, &rect);
            SDL_DestroyTexture(tex);
        }
    }

    SDL_RenderPresent(_renderer);
}

vector<BaseWindow*> WindowManager::GetWindows()
{
    return mWindows;
}

akSize WindowManager::GetResolution()
{
    return mResolution;
}

WindowManager::~WindowManager()
{
    SDL_Quit();
}
