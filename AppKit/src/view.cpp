#include <AppKit/view.h>
#include <AppKit/basewindow.h>
#include <AppKit/window.h>
#include <AppKit/utility.h>
#include <iostream>


akColor akView::mFocusColor = akColor(153,217,234,0);
akColor akView::mLostFocusColor = akColor(255,255,255,0);

akView::akView()
{
	SetClassName("akView");
	mRect = akRect(0,0,0,0);
	mParent = NULL;
}

akView::akView(akRect rect, akView *parent/* = NULL*/)
{
	mRect = rect;
	mParent = parent;
	if (mParent != NULL) mParent->AddChild(this);
}

void akView::SetRect(akRect rect)
{
	mRect = rect;
	Repaint();
}

akRect akView::GetRect()
{
	return mRect;
}

akColor akView::GetFocusColor()
{
	return mFocusColor;
}

akColor akView::GetLostFocusColor()
{
	return mLostFocusColor;
}

void akView::SetFocusColor(akColor c)
{
	mFocusColor = c;
}

void akView::SetLostFocusColor(akColor c)
{
	mLostFocusColor = c;
}

void akView::AddChild(akView *view)
{
	mChildren.push_back(view);
	view->SetWindow(mWindow);
}

akView* akView::RemoveChild(akView *view)
{
	for (unsigned int i=0; i<mChildren.size(); i++) {
		akView *viewToRemove = mChildren.at(i);
		if (view == viewToRemove) {
			mChildren.erase(mChildren.begin()+i);
			viewToRemove->SetWindow(NULL);
			return viewToRemove;
		}
	}
	return NULL;
}

akWindow* akView::GetWindow()
{
	return mWindow;
}

void akView::SetWindow(akWindow *wnd)
{
	mWindow = wnd;
}

void akView::Repaint()
{
	if (!mWindow) return;

	BaseWindow *baseWnd = (BaseWindow*)mWindow->mBaseWindow;

	SDL_Surface *destination = baseWnd->GetContentSurface();

	for (unsigned int i=0; i<mPainters.size(); i++) {
		akPainter *painter = mPainters.at(i);
		painter->Paint(this, destination);
	}

	for (unsigned int i=0; i<mChildren.size(); i++) {
		akView *child = mChildren.at(i);
		child->Repaint();
	}
}

bool akView::SendInputEvent(akInputEvent *evt)
{
	if (evt->GetType() == akINPUT_EVENT_KEY)
	{
		akKeyEvent *keyEvent = (akKeyEvent*)evt;

		if (mWindow->GetFirstResponder() == this) {
			InvokeKeyEventReceivers(keyEvent);
		}

		// Send the event to children
		for (unsigned int i=0; i<mChildren.size(); i++) {
			akView *child = mChildren.at(i);
			child->SendInputEvent(evt);
		}

		return true;
	}
	else if (evt->GetType() == akINPUT_EVENT_MOUSE)
	{
		akMouseEvent *mouseEvent = (akMouseEvent*)evt;
		if (Utility::RectContainsPoint(mRect, mouseEvent->GetLocationInWindow()))
		{
			// Call mouse event receivers
			InvokeMouseEventReceivers(mouseEvent);

			// Send the event to children
			for (unsigned int i=0; i<mChildren.size(); i++)
			{
				akView *child = mChildren.at(i);
				child->SendInputEvent(evt);
			}
			return true;
		}
		else return false;
	}
	return false;
}

void akView::AddPainter(akPainter *painter)
{
	mPainters.push_back(painter);
}

void akView::RemovePainter(akPainter *painter)
{
	for (unsigned int i=0; i<mPainters.size(); i++) {
		akPainter *painterToRemove = mPainters.at(i);
		if (painterToRemove == painter) {
			mPainters.erase(mPainters.begin()+i);
			return;
		}
	}
}

void akView::AddKeyEventReceiver(akKeyEventReceiver *receiver)
{
	this->mKeyEventReceivers.push_back(receiver);
}

void akView::RemoveKeyEventReceiver(akKeyEventReceiver *receiver)
{
	for (unsigned int i=0; i<mKeyEventReceivers.size(); i++) {
		akKeyEventReceiver *rcv = mKeyEventReceivers.at(i);
		if (rcv == receiver) {
			mKeyEventReceivers.erase(mKeyEventReceivers.begin()+i);
			return;
		}
	}
}

void akView::AddMouseEventReceiver(akMouseEventReceiver *receiver)
{
	this->mMouseEventReceivers.push_back(receiver);
}

void akView::RemoveMouseEventReceiver(akMouseEventReceiver *receiver)
{
	for (unsigned int i=0; i<mMouseEventReceivers.size(); i++) {
		akMouseEventReceiver *rcv = mMouseEventReceivers.at(i);
		if (rcv == receiver) {
			mMouseEventReceivers.erase(mMouseEventReceivers.begin()+i);
			return;
		}
	}
}

void akView::AddViewNotificationReceiver(akViewNotificationReceiver *receiver)
{
	this->mViewNotificationReceivers.push_back(receiver);
}

void akView::RemoveViewNotificationReceiver(akViewNotificationReceiver *receiver)
{
	for (unsigned int i=0; i<mViewNotificationReceivers.size(); i++) {
		akViewNotificationReceiver *rcv = mViewNotificationReceivers.at(i);
		if (rcv == receiver) {
			mViewNotificationReceivers.erase(mViewNotificationReceivers.begin()+i);
			return;
		}
	}
}

void akView::InvokeKeyEventReceivers(akKeyEvent *event)
{
	for (unsigned int i=0; i<mKeyEventReceivers.size(); i++) {
		akKeyEventReceiver *rcv = mKeyEventReceivers.at(i);
		if (event->GetKeyEvent() == akKEY_EVENT_PRESS) rcv->KeyPress(this, event);
		else if (event->GetKeyEvent() == akKEY_EVENT_RELEASE) rcv->KeyRelease(this, event);
	}
}

void akView::InvokeMouseEventReceivers(akMouseEvent *event)
{
	for (unsigned int i=0; i<mMouseEventReceivers.size(); i++) {
		akMouseEventReceiver *rcv = mMouseEventReceivers.at(i);
		if (event->GetMouseEvent() == akMOUSE_EVENT_PRESS) rcv->MousePress(this, event);
		else if (event->GetMouseEvent() == akMOUSE_EVENT_RELEASE) rcv->MouseRelease(this, event);
		else if (event->GetMouseEvent() == akMOUSE_EVENT_MOVE) rcv->MouseMove(this, event);
		else if (event->GetMouseEvent() == akMOUSE_EVENT_DRAG) rcv->MouseDrag(this, event);
		else if (event->GetMouseEvent() == akMOUSE_EVENT_WHEEL_UP) rcv->MouseWheelUp(this, event);
		else if (event->GetMouseEvent() == akMOUSE_EVENT_WHEEL_DOWN) rcv->MouseWheelDown(this, event);
	}
}

void akView::InvokeViewNotificationEventReceivers(int notification)
{
	for (int i=0; i<mViewNotificationReceivers.size(); i++) {
		akViewNotificationReceiver *rcv = mViewNotificationReceivers.at(i);
		if (notification == akVIEW_WILL_BECAME_FIRSTRESPONDER) rcv->ViewWillBecameFirstResponder();
		else if (notification == akVIEW_WILL_RESIGN_FIRSTRESPONDER) rcv->ViewWillResignFirstResponder();
	}
}

void akView::SetTag(int tag) {
	mTag = tag;
}

int akView::GetTag() {
	return mTag;
}

akView::~akView()
{
	mChildren.clear();
}