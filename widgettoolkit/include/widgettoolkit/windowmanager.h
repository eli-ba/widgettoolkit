#pragma once

#include <SDL.h>
#include <widgettoolkit/export.h>
#include <widgettoolkit/geometry.h>
#include <widgettoolkit/basewindow.h>
#include <widgettoolkit/windowmanager.h>
#include <widgettoolkit/inputevent.h>
#include <widgettoolkit/windowinteractions.h>
#include <vector>
#include <string>

using namespace std;

class WindowManager {
public:
    WindowManager(akSize resolution, const string& name);
    void MakeTopLevel(BaseWindow* wnd);
    void AddWindow(BaseWindow* wnd);
    BaseWindow* RemoveWindow(BaseWindow* wd);
    void ProcessEvents();
    void ComposeWindows();
    void Close();
    vector<BaseWindow*> GetWindows();
    akSize GetResolution();
    ~WindowManager();

private:
    akInputEvent* ParseSDLEvent(SDL_Event* evt);

private:
    //SDL_Surface *mScreen;
    akSize mResolution;
    vector<BaseWindow*> mWindows;
    WindowInteractions* mWindowInteractions;
    string mApplicationName;
    bool mRunning;
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    akPoint mMouseMotion;
};
