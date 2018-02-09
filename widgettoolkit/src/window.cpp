#include <widgettoolkit/window.h>
#include <widgettoolkit/windowmanager.h>
#include <widgettoolkit/basewindow.h>
#include <iostream>

extern WindowManager* TheWindowManager;

akWindow::akWindow(akRect rect, string title, long style)
{
    mFirstResponder = NULL;
    mCanReceiveMouseMotionEvents = false;

    mBaseWindow = new BaseWindow(rect, title, style);
    mBaseWindow->Window = this;
    TheWindowManager->AddWindow(mBaseWindow);

    mContentView = new akView(GetContentRect());
    mContentView->SetWindow(this);
    mContentView->AddPainter(this);
    mContentView->SetTag(0);

    mBgColor.r = 240;
    mBgColor.g = 240;
    mBgColor.b = 240;

    if (IsVisible())
        Repaint();
}

void akWindow::Repaint()
{
    mContentView->Repaint();
    mBaseWindow->DrawContentSurface();
    TheWindowManager->ComposeWindows();
}

void akWindow::Paint(akView* view, SDL_Surface* destination)
{
    SDL_FillRect(destination, NULL, SDL_MapRGB(destination->format, mBgColor.r, mBgColor.g, mBgColor.b));
}

void akWindow::DispatchInputEvent(akInputEvent* evt)
{
    mContentView->SendInputEvent(evt);
}

akView* akWindow::GetFirstResponder()
{
    return mFirstResponder;
}

void akWindow::SetFirstResponder(akView* view)
{
    if (mFirstResponder)
        mFirstResponder->InvokeViewNotificationEventReceivers(akVIEW_WILL_RESIGN_FIRSTRESPONDER);
    mFirstResponder = view;
    view->InvokeViewNotificationEventReceivers(akVIEW_WILL_BECAME_FIRSTRESPONDER);
}

void akWindow::AddView(akView* view)
{
    mContentView->AddChild(view);
    view->SetWindow(this);
    this->Repaint();
}

void akWindow::RemoveView(akView* view)
{
    mContentView->RemoveChild(view);
    view->SetWindow(NULL);
    this->Repaint();
}

string akWindow::GetTitle()
{
    return mBaseWindow->GetTitle();
}

akRect akWindow::GetRect()
{
    return mBaseWindow->GetRect();
}

akRect akWindow::GetContentRect()
{
    return mBaseWindow->GetContentRect();
}

void akWindow::SetRect(akRect rect)
{
    mBaseWindow->SetRect(rect);
    mContentView->SetRect(GetContentRect());
    Repaint();
}

void akWindow::SetContentRect(akRect rect)
{
    mBaseWindow->SetContentRect(rect);
    mContentView->SetRect(GetContentRect());
    Repaint();
}

bool akWindow::IsVisible()
{
    return mBaseWindow->IsVisible();
}

void akWindow::SetVisible(bool visible)
{
    mBaseWindow->SetVisible(visible);
    if (visible) {
        Repaint();
    }
}

void akWindow::SetMaximizable(bool maximizable)
{
    mBaseWindow->SetMaximizable(maximizable);
}

void akWindow::SetMinimizable(bool minimizable)
{
    mBaseWindow->SetMinimizable(minimizable);
}

void akWindow::SetClosable(bool closable)
{
    mBaseWindow->SetClosable(closable);
}

bool akWindow::IsMaximizable()
{
    return mBaseWindow->IsMaximizable();
}

bool akWindow::IsMinimizable()
{
    return mBaseWindow->IsMinimizable();
}

bool akWindow::IsClosable()
{
    return mBaseWindow->IsClosable();
}

void akWindow::SetBackgroundColor(akColor color)
{
    mBgColor = color;
    Repaint();
}

akColor akWindow::GetBackgroundColor()
{
    return mBgColor;
}

void akWindow::SetCanReceiveMouseMoveEvents(bool receive)
{
    mCanReceiveMouseMotionEvents = receive;
}

bool akWindow::CanReceiveMouseMotionEvents()
{
    return mCanReceiveMouseMotionEvents;
}

void akWindow::Close()
{
    mBaseWindow->SetNeedsToBeClosed();
}

akWindow::~akWindow()
{
    delete mContentView;
}
