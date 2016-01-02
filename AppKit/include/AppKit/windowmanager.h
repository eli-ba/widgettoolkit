#ifndef _WINDOWSYSTEM_H_
#define _WINDOWSYSTEM_H_

#include <SDL.h>
#include <AppKit/appkit_export.h>
#include <AppKit/geometry.h>
#include <AppKit/basewindow.h>
#include <AppKit/windowmanager.h>
#include <AppKit/inputevent.h>
#include <AppKit/windowinteractions.h>
#include <vector>
#include <string>

using namespace std;

class WindowManager
{
public:
	WindowManager(akSize resolution, const string &name);
	void MakeTopLevel(BaseWindow *wnd);
	void AddWindow(BaseWindow *wnd);
	BaseWindow* RemoveWindow(BaseWindow *wd);
	void ProcessEvents();
	void ComposeWindows();
	void Close();
	vector<BaseWindow*> GetWindows();
	akSize GetResolution();
	SDL_PixelFormat* GetSDLPixelFormat();
	~WindowManager();
private:
	akInputEvent* ParseSDLEvent(SDL_Event *evt);
	
private:
	SDL_Surface *mScreen;
	akSize mResolution;
	vector<BaseWindow*> mWindows;
	WindowInteractions *mWindowInteractions;
	Uint32 mDefaultBgColor;
	string mApplicationName;
	bool mRunning;
};

#endif