#ifndef _WSWINDOW_H_
#define _WSWINDOW_H_

#include <SDL.h>
#include <AppKit/appkit_export.h>
#include <AppKit/geometry.h>
#include <AppKit/constants.h>
#include <AppKit/aktypes.h>
#include <AppKit/inputevent.h>
#include <string>

using namespace std;

class BaseWindow
{
public:
	BaseWindow(akRect contentRect, const string &title, long style);
	void SetTitle(const string &title);
	void SetRect(akRect rect);
	void SetContentRect(akRect rect);
	string GetTitle();
	akRect GetRect();
	akRect GetContentRect();
	SDL_Surface* GetSurface();
	SDL_Surface* GetContentSurface();
	void SetActive(bool active);
	void SetVisible(bool visible);
	void SetMaximizable(bool maximizable);
	void SetMinimizable(bool minimizable);
	void SetClosable(bool closable);
	bool IsActive();
	bool IsVisible();
	bool IsMaximizable();
	bool IsMinimizable();
	bool IsClosable();
	akRect GetCloseButtonRect();
	akRect GetMinimizeButtonRect();
	akRect GetMaximizeButtonRect();
	akRect GetTitleBarRect();
	int GetRightMargin();
	int GetLeftMargin();
	int GetTopMargin();
	int GetBottomMargin();
	bool IsMaximized();
	bool IsMinimized();
	void SetMaximized(bool miximize);
	void SetMinimized(bool minimize);
	void DrawContent(bool drawDecoration);
	void DrawContentSurface();
	void SetNeedsToBeClosed();
	bool NeedsToBeClosed();
	~BaseWindow();
private:
	void CreateSurface();
	void ParseStyle();
	void CalculateMargins();
	void CalculateDecorationsMetrics();
	void DrawDecoration();
private:
	string mTitle;
	akRect mRect;
	akRect mContentRect;
	int mLeftMargin;
	int mRightMargin;
	int mTopMargin;
	int mBottomMargin;
	SDL_Surface *mSurface;
	SDL_Surface *mContentSurface;
	long mStyle;
	bool mMaximizable;
	bool mMinimizable;
	bool mClosable;
	bool mVisible;
	bool mActive;
	akRect mCloseButtonRect;
	akRect mMinimizeButtonRect;
	akRect mMaximizeButtonRect;
	akRect mTitleBarRect;
	bool mMaximized;
	bool mMinimized;
	akRect mRectBeforeMaximization;
	akRect mRectBeforeMinimization;
	bool mNeedsToBeClosed;
public:
	void *Window;
};

#endif