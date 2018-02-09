#include <widgettoolkit/basewindow.h>
#include <widgettoolkit/utility.h>
#include <widgettoolkit/resources.h>
#include <widgettoolkit/windowmanager.h>
#include <widgettoolkit/window.h>
#include <string.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

extern WindowManager* TheWindowManager;

BaseWindow::BaseWindow(akRect contentRect, const string& title, long style = 0)
{
    mNeedsToBeClosed = false;

    this->Window = NULL;
    mSurface = NULL;
    mContentSurface = NULL;
    mMaximized = false;
    mMinimized = false;
    mStyle = style;
    mTitle = title;

    CalculateMargins();

    SetTitle(title);

    /* Set the content rect */
    mContentRect.location.x = mLeftMargin;
    mContentRect.location.y = mTopMargin;
    mContentRect.size.width = contentRect.size.width;
    mContentRect.size.height = contentRect.size.height;

    /* Set the rect */
    mRect.location.x = contentRect.location.x - mLeftMargin;
    mRect.location.y = contentRect.location.y - mTopMargin;
    mRect.size.width = contentRect.size.width + mLeftMargin + mRightMargin;
    mRect.size.height = contentRect.size.height + mTopMargin + mBottomMargin;

    ParseStyle();
    CalculateDecorationsMetrics();

    CreateSurface();
    DrawContent(true);
}

void BaseWindow::CreateSurface()
{
    if (mSurface)
        SDL_FreeSurface(mSurface);
    if (mContentSurface)
        SDL_FreeSurface(mContentSurface);

    mSurface = SDL_CreateRGBSurface(0, mRect.size.width, mRect.size.height, 32, 0, 0, 0, 0);
    mContentSurface = SDL_CreateRGBSurface(0, mContentRect.size.width, mContentRect.size.height, 32, 0, 0, 0, 0);
}

void BaseWindow::ParseStyle()
{
    mVisible = false;
    mMaximizable = false;
    mMinimizable = false;
    mClosable = false;
    mActive = false;

    if (Utility::BitTest(mStyle, 0)) { // akWS_VISIBLE
        mVisible = true;
    }
    if (Utility::BitTest(mStyle, 1)) { // akWS_MAXIMIZABLE
        mMaximizable = true;
    }
    if (Utility::BitTest(mStyle, 2)) { // akWS_MINIMIZABLE
        mMinimizable = true;
    }
    if (Utility::BitTest(mStyle, 3)) { // akWS_CLOSABLE
        mClosable = true;
    }
    if (Utility::BitTest(mStyle, 4)) { // akWS_ACTIVE
        mActive = true;
    }
}

void BaseWindow::CalculateMargins()
{
    SDL_Surface* titleBarCenter = Resources::GetImageResource(WIN_TITLE_BAR_CENTER);
    SDL_Surface* leftBorder = Resources::GetImageResource(LEFT_BORDER);
    SDL_Surface* rightBorder = Resources::GetImageResource(RIGHT_BORDER);
    SDL_Surface* bottomBorder = Resources::GetImageResource(BOTTOM_BORDER);

    mRightMargin = rightBorder->w;
    mLeftMargin = leftBorder->w;
    mTopMargin = titleBarCenter->h;
    mBottomMargin = bottomBorder->h;
}

void BaseWindow::SetActive(bool active)
{
    if (!active) {
        mStyle = mStyle ^ akWS_ACTIVE;
        mActive = false;
    }
    else {
        mStyle = mStyle | akWS_ACTIVE;
        mActive = true;
    }

    if (mSurface) {
        DrawContent(true);
        TheWindowManager->ComposeWindows();
    }
}

void BaseWindow::SetVisible(bool visible)
{
    if (!visible) {
        mStyle = mStyle ^ akWS_VISIBLE;
        mVisible = false;
    }
    else {
        mStyle = mStyle | akWS_VISIBLE;
        mVisible = true;
    }

    if (mSurface) {
        TheWindowManager->ComposeWindows();
    }
}

void BaseWindow::SetMaximizable(bool maximizable)
{
    if (!maximizable) {
        mStyle = mStyle ^ akWS_MAXIMIZABLE;
        mMaximizable = false;
    }
    else {
        mStyle = mStyle | akWS_MAXIMIZABLE;
        mMaximizable = true;
    }

    if (mSurface) {
        DrawContent(true);
        TheWindowManager->ComposeWindows();
    }
}

void BaseWindow::SetMinimizable(bool minimizable)
{
    if (!minimizable) {
        mStyle = mStyle ^ akWS_MINIMIZABLE;
        mMinimizable = false;
    }
    else {
        mStyle = mStyle | akWS_MINIMIZABLE;
        mMinimizable = true;
    }

    if (mSurface) {
        DrawContent(true);
        TheWindowManager->ComposeWindows();
    }
}

void BaseWindow::SetClosable(bool closable)
{
    if (!closable) {
        mStyle = mStyle ^ akWS_CLOSABLE;
        mClosable = false;
    }
    else {
        mStyle = mStyle | akWS_CLOSABLE;
        mClosable = true;
    }

    if (mSurface) {
        DrawContent(true);
        TheWindowManager->ComposeWindows();
    }
}

bool BaseWindow::IsActive()
{
    return mActive;
}

bool BaseWindow::IsVisible()
{
    return mVisible;
}

bool BaseWindow::IsMaximizable()
{
    return mMaximizable;
}

bool BaseWindow::IsMinimizable()
{
    return mMinimizable;
}

bool BaseWindow::IsClosable()
{
    return mClosable;
}

/*
 * Draws the window frame decorations
 * based on the style bitwise values
 */
void BaseWindow::DrawDecoration()
{
    if (!mSurface)
        return;

    /* Title bar */
    SDL_Surface *titleBarRight = NULL, *titleBarLeft = NULL, *titleBarCenter = NULL,
                *leftBorder = NULL, *rightBorder = NULL, *leftCorner, *rightCorner,
                *bottomBorder = NULL, *titleSurface = NULL, *closeButton = NULL, *maximizeButton = NULL, *minimizeButton = NULL,
                *closeButtonInactive = NULL, *maximizeButtonInactive = NULL, *minimizeButtonInactive = NULL;

    titleBarRight = Resources::GetImageResource(WIN_TITLE_BAR_RIGHT);
    titleBarLeft = Resources::GetImageResource(WIN_TITLE_BAR_LEFT);
    titleBarCenter = Resources::GetImageResource(WIN_TITLE_BAR_CENTER);
    leftBorder = Resources::GetImageResource(LEFT_BORDER);
    rightBorder = Resources::GetImageResource(RIGHT_BORDER);
    bottomBorder = Resources::GetImageResource(BOTTOM_BORDER);
    leftCorner = Resources::GetImageResource(LEFT_CORNER);
    rightCorner = Resources::GetImageResource(RIGHT_CORNER);
    closeButton = Resources::GetImageResource(CLS_BTN);
    maximizeButton = Resources::GetImageResource(MX_BTN);
    minimizeButton = Resources::GetImageResource(RDC_BTN);
    closeButtonInactive = Resources::GetImageResource(CLS_BTN_INACTIVE);
    maximizeButtonInactive = Resources::GetImageResource(MX_BTN_INACTIVE);
    minimizeButtonInactive = Resources::GetImageResource(RDC_BTN_INACTIVE);

    // Left
    SDL_Rect srcrect;
    SDL_Rect dstrect;
    dstrect.x = 0;
    dstrect.y = 0;
    SDL_BlitSurface(titleBarLeft, NULL, mSurface, &dstrect);
    // Center fill
    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = 1;
    srcrect.h = titleBarCenter->h;
    for (int i = titleBarLeft->w; i <= (mRect.size.width - titleBarLeft->w); i++) {
        dstrect.x = i;
        dstrect.y = 0;
        SDL_BlitSurface(titleBarCenter, &srcrect, mSurface, &dstrect);
    }
    // Right
    dstrect.x = mRect.size.width - titleBarRight->w;
    dstrect.y = 0;
    SDL_BlitSurface(titleBarRight, NULL, mSurface, &dstrect);

    /* Left border */
    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = leftBorder->w;
    srcrect.h = 1;
    for (int i = titleBarCenter->h; i <= (mRect.size.height - bottomBorder->h); i++) {
        dstrect.x = 0;
        dstrect.y = i;
        SDL_BlitSurface(leftBorder, &srcrect, mSurface, &dstrect);
    }

    /* Right border */
    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = rightBorder->w;
    srcrect.h = 1;
    for (int i = titleBarCenter->h; i <= (mRect.size.height - bottomBorder->h); i++) {
        dstrect.x = mRect.size.width - rightCorner->w;
        dstrect.y = i;
        SDL_BlitSurface(rightBorder, &srcrect, mSurface, &dstrect);
    }

    /* Left corner */
    dstrect.x = 0;
    dstrect.y = mRect.size.height - leftCorner->h;
    SDL_BlitSurface(leftCorner, NULL, mSurface, &dstrect);

    /* Right corner */
    dstrect.x = mRect.size.width - rightCorner->w;
    dstrect.y = mRect.size.height - rightCorner->h;
    SDL_BlitSurface(rightCorner, NULL, mSurface, &dstrect);

    /* Bottom border */
    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = 1;
    srcrect.h = bottomBorder->h;
    for (int i = leftCorner->w; i < mRect.size.width - rightCorner->w; i++) {
        dstrect.x = i;
        dstrect.y = mRect.size.height - bottomBorder->h;
        SDL_BlitSurface(bottomBorder, &srcrect, mSurface, &dstrect);
    }

    /* Render the title */
    TTF_Font* font = Resources::GetFontResource(DEFAULT_FONT);
    SDL_Color blackColor;
    blackColor.r = 0;
    blackColor.g = 0;
    blackColor.b = 0;
    titleSurface = TTF_RenderText_Blended(font, mTitle.c_str(), blackColor);
    dstrect.x = 12;
    dstrect.y = 5;
    SDL_BlitSurface(titleSurface, NULL, mSurface, &dstrect);

    /* Buttons */
    akRect firstRect, secondRect;
    int topSpacing = 4, rightSpacing = 7, horizontalSpacing = 2;
    SDL_Surface* currentSurface = NULL;

    /* Close button */
    if (IsClosable())
        currentSurface = closeButton;
    else
        currentSurface = closeButtonInactive;

    dstrect.x = mRect.size.width - currentSurface->w - rightSpacing;
    dstrect.y = topSpacing;

    SDL_BlitSurface(currentSurface, NULL, mSurface, &dstrect);

    /* Maximize button */
    if (IsMaximizable())
        currentSurface = maximizeButton;
    else
        currentSurface = maximizeButtonInactive;

    dstrect.x = dstrect.x - currentSurface->w - horizontalSpacing;
    dstrect.y = topSpacing;

    SDL_BlitSurface(currentSurface, NULL, mSurface, &dstrect);

    /* Minimize button */
    if (IsMinimizable())
        currentSurface = minimizeButton;
    else
        currentSurface = minimizeButtonInactive;

    dstrect.x = dstrect.x - currentSurface->w - horizontalSpacing;
    ;
    dstrect.y = topSpacing;

    SDL_BlitSurface(currentSurface, NULL, mSurface, &dstrect);
}

void BaseWindow::DrawContent(bool drawDecoration)
{
    if (drawDecoration)
        DrawDecoration();
    DrawContentSurface();
}

void BaseWindow::DrawContentSurface()
{
    SDL_Rect dstrect;
    dstrect.x = mContentRect.location.x;
    dstrect.y = mContentRect.location.y;
    dstrect.w = mContentSurface->w;
    dstrect.h = mContentSurface->h;
    SDL_BlitSurface(mContentSurface, NULL, mSurface, &dstrect);
}

akRect BaseWindow::GetCloseButtonRect()
{
    return mCloseButtonRect;
}

akRect BaseWindow::GetMinimizeButtonRect()
{
    return mMinimizeButtonRect;
}

akRect BaseWindow::GetMaximizeButtonRect()
{
    return mMaximizeButtonRect;
}

akRect BaseWindow::GetTitleBarRect()
{
    return mTitleBarRect;
}

void BaseWindow::SetTitle(const string& title)
{
    mTitle = title;

    if (mSurface) {
        DrawContent(true);
        TheWindowManager->ComposeWindows();
    }
}

void BaseWindow::CalculateDecorationsMetrics()
{
    mTitleBarRect.location.x = mRect.location.x;
    mTitleBarRect.location.y = mRect.location.y;
    mTitleBarRect.size.width = mRect.size.width;
    mTitleBarRect.size.height = mTopMargin;

    int topSpacing = 4, rightSpacing = 7, horizontalSpacing = 2;

    /* Close button */
    SDL_Surface* closeButton = Resources::GetImageResource(CLS_BTN);

    mCloseButtonRect.location.x = mRect.location.x + mRect.size.width - closeButton->w - rightSpacing;
    mCloseButtonRect.location.y = mRect.location.y + topSpacing;
    mCloseButtonRect.size.width = closeButton->w;
    mCloseButtonRect.size.height = closeButton->h;

    /* Maximize button */
    SDL_Surface* maximizeButton = Resources::GetImageResource(MX_BTN);

    mMaximizeButtonRect.location.x = mCloseButtonRect.location.x - maximizeButton->w - horizontalSpacing;
    mMaximizeButtonRect.location.y = mRect.location.y + topSpacing;
    mMaximizeButtonRect.size.width = maximizeButton->w;
    mMaximizeButtonRect.size.height = maximizeButton->h;

    /* Minimize button */
    SDL_Surface* minimizeButton = Resources::GetImageResource(RDC_BTN);

    mMinimizeButtonRect.location.x = mMaximizeButtonRect.location.x - minimizeButton->w - horizontalSpacing;
    mMinimizeButtonRect.location.y = mRect.location.y + topSpacing;
    mMinimizeButtonRect.size.width = minimizeButton->w;
    mMinimizeButtonRect.size.height = minimizeButton->h;
}

void BaseWindow::SetRect(akRect rect)
{
    akRect oldRect = mRect;
    mRect = rect;

    /* Recalculate the new content rect */
    mContentRect.location.x = mLeftMargin;
    mContentRect.location.y = mTopMargin;
    mContentRect.size.width = mRect.size.width - mLeftMargin - mRightMargin;
    mContentRect.size.height = mRect.size.height - mTopMargin - mBottomMargin;

    CalculateDecorationsMetrics();

    /* If the size changed we must recreate the surface */
    if ((oldRect.size.width != mRect.size.width) || (oldRect.size.height != mRect.size.height)) {
        CreateSurface();
        DrawDecoration();
        if (this->Window) {
            akWindow* wnd = (akWindow*)this->Window;
            wnd->Repaint();
        }
    }
    else { // in the case when we just move the window, ComposeWindows must be called explicitely
        TheWindowManager->ComposeWindows();
    }
}

/*
void BaseWindow::DispatchInputEvent(akInputEvent *evt)
{

}
*/

void BaseWindow::SetContentRect(akRect rect)
{
    akRect oldRect = mContentRect;
    mContentRect = rect;
    if (mSurface) {
        TheWindowManager->ComposeWindows();
    }
    /* Recalculate the window rect */
    mRect.location.x = mContentRect.location.x - mLeftMargin;
    mRect.location.y = mContentRect.location.y - mTopMargin;
    mRect.size.width = mContentRect.size.width + mLeftMargin + mRightMargin;
    mRect.size.height = mContentRect.size.height + mTopMargin + mBottomMargin;

    CalculateDecorationsMetrics();

    /* If the size changed we must recreate the surface */
    if ((oldRect.size.width != mContentRect.size.width) || (oldRect.size.height != mContentRect.size.height)) {
        CreateSurface();
        DrawDecoration();
        if (this->Window) {
            akWindow* wnd = (akWindow*)this->Window;
            wnd->Repaint();
        }
    }
}

string BaseWindow::GetTitle()
{
    return mTitle;
}

akRect BaseWindow::GetRect()
{
    return mRect;
}

akRect BaseWindow::GetContentRect()
{
    return mContentRect;
}

SDL_Surface* BaseWindow::GetSurface()
{
    return mSurface;
}

SDL_Surface* BaseWindow::GetContentSurface()
{
    return mContentSurface;
}

int BaseWindow::GetRightMargin()
{
    return mRightMargin;
}

int BaseWindow::GetLeftMargin()
{
    return mLeftMargin;
}

int BaseWindow::GetTopMargin()
{
    return mTopMargin;
}

int BaseWindow::GetBottomMargin()
{
    return mBottomMargin;
}

bool BaseWindow::IsMaximized()
{
    return mMaximized;
}

bool BaseWindow::IsMinimized()
{
    return mMinimized;
}

void BaseWindow::SetMaximized(bool maximize)
{
    if (maximize) {
        if (mMaximized)
            return;
        mRectBeforeMaximization = mRect;
        SetRect(akRect(akPoint(0, 0), TheWindowManager->GetResolution()));
        mMaximized = true;
    }
    else {
        if (!mMaximized)
            return;
        SetRect(mRectBeforeMaximization);
        mMaximized = false;
    }
}

void BaseWindow::SetMinimized(bool minimize)
{
    if (minimize) {
        if (mMinimized)
            return;
        mRectBeforeMinimization = mRect;
        SetRect(akRect(mRectBeforeMinimization.location.x, mRectBeforeMinimization.location.y, 200 + mLeftMargin + mRightMargin, mTopMargin + mBottomMargin));
        mMinimized = true;
    }
    else {
        if (!mMinimized)
            return;
        SetRect(mRectBeforeMinimization);
        mMinimized = false;
    }
}

void BaseWindow::SetNeedsToBeClosed()
{
    mNeedsToBeClosed = true;
}

bool BaseWindow::NeedsToBeClosed()
{
    return mNeedsToBeClosed;
}

BaseWindow::~BaseWindow()
{
    if (mSurface)
        SDL_FreeSurface(mSurface);
    if (mContentSurface)
        SDL_FreeSurface(mContentSurface);
}
