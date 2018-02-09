#pragma once

#include <widgettoolkit/export.h>
#include <widgettoolkit/view.h>
#include <widgettoolkit/geometry.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <widgettoolkit/mouseeventreceiver.h>
#include <widgettoolkit/keyeventreceiver.h>
#include <widgettoolkit/viewnotificationreceiver.h>
#include <widgettoolkit/painter.h>
#include <widgettoolkit/control.h>
#include <string>

using namespace std;

/* ******************************************************
 * LEGACY CODE (To be upgraded later in later versions) *
 ********************************************************/

typedef struct ListBox_Item ListBox_Item;
struct ListBox_Item {
    int x;
    int y;
    char* text;
    SDL_Surface* surface;
    SDL_Surface* surface_selected;
    int select;
    int index;
    SDL_Surface* textsurf;
};

typedef struct ListBox_ItemList ListBox_ItemList;
struct ListBox_ItemList {
    ListBox_Item info;
    ListBox_ItemList* next;
};

class akListBox;

typedef struct ListBox ListBox;
struct ListBox {
    //akListBox *akListBoxObject;
    int x;
    int y;
    int width;
    int height;
    SDL_Surface* surface;
    SDL_Surface* bg;
    TTF_Font* font;
    ListBox_ItemList* ItemList;
    SDL_Color bgColor;
    SDL_Color selectionColor;
    int k;
    int of;
    int scrollScale;
    SDL_Surface** data;
    int mouseDown_scrollUp;
    int mouseDown_scrollDown;
    int mouseDown_scrollBar;
    int currentNbScroll;
    int stopHScroll;
    int nbScroll;
    SDL_Surface* destSurface;
    int stop, tik, cTime, pTime, i;
    int upScrollBtnState, downScrollBtnState;
    ListBox_ItemList* selectedItem;
    ListBox_ItemList* selStartItem;
    ListBox_ItemList* selEndItem;
    int mouseBtnHoldIndicator;
    int itemsCount;
    int focus;
    SDL_Surface* mbg;
    int mouseHold;
    int multiSelect;
    int invMultiSel;
    int shiftHold;
};

void __ListBox_Draw(ListBox* listbox, int updateWnd);
void ListBox_SetItemBg(ListBox* listbox, int index, SDL_Surface* bg, int x, int y);
void ListBox_SetItemBg1(ListBox* listbox, ListBox_ItemList* p, SDL_Surface* bg, int x, int y);
ListBox_ItemList* ListBox_GetSelectedItems(ListBox* listbox);

ListBox* ListBox_Init(int x, int y, int width, int height, TTF_Font* font, int scrollScale,
    SDL_Color bgColor, SDL_Color selectionColor, akListBox* listBoxObject);
void ListBox_Draw(ListBox* listbox);
void ListBox_DrawContent(ListBox* listbox);
void ListBox_DrawBorder(ListBox* w);
void ListBox_AddItem(ListBox* listbox, const char* itemtext);
char* ListBox_GetItem(ListBox* listbox, int index);
void ListBox_SetItem(ListBox* listbox, int index, char* text);
int ListBox_RemoveItem(ListBox* listbox, int index);
int ListBox_GetSelectionIndex(ListBox* listbox);
char* ListBox_GetSelectionText(ListBox* listbox);
void ListBox_ScrollDown(ListBox* listbox);
void ListBox_ScrollUp(ListBox* listbox);
int ListBox_GetNbScroll(ListBox* listbox);
int ListBox_GetNbItems(ListBox* listbox);

void ListBox_OnMouseButtonDown(ListBox* listbox, int mouse_x, int mouse_y);
void ListBox_OnMouseButtonUp(ListBox* listbox, int mouse_x, int mouse_y);
void ListBox_OnMouseMotion(ListBox* listbox, int mouse_x, int mouse_y);
void ListBox_OnKeyDown(ListBox* listbox, int sym);
void ListBox_OnKeyUp(ListBox* listbox);
void ListBox_OnWheelDown(ListBox* listbox, int mouse_x, int mouse_y);
void ListBox_OnWheelUp(ListBox* listbox, int mouse_x, int mouse_y);
void ListBox_OnNoEvent(ListBox* listbox, int mouse_x, int mouse_y);

void ListBox_Delete(ListBox* listbox);

/* *********************************
 * ListBox Object Oriented Wrapper *
 ***********************************/

class APPKIT_API akListBox : public akControl, akMouseEventReceiver, akKeyEventReceiver, akPainter, akViewNotificationReceiver {
public:
    akListBox(akRect rect, akView* parent = NULL);
    void AddItem(string text);
    void RemoveItem(int index);
    string GetItemText(int index);
    void SetItemText(int index, string text);
    int GetSelectionIndex();
    int GetItemCount();

private:
    /* akPainter overrides */
    virtual void Paint(akView* view, SDL_Surface* destination);
    /* akMouseEventReceiver overrides */
    virtual void MousePress(akView* sender, akMouseEvent* event);
    virtual void MouseRelease(akView* sender, akMouseEvent* event);
    virtual void MouseMove(akView* sender, akMouseEvent* event);
    virtual void MouseDrag(akView* sender, akMouseEvent* event);
    virtual void MouseWheelUp(akView* sender, akMouseEvent* event);
    virtual void MouseWheelDown(akView* sender, akMouseEvent* event);
    /* akKeyEventEventReceiver overrides */
    virtual void KeyPress(akView* sender, akKeyEvent* event);
    virtual void KeyRelease(akView* sender, akKeyEvent* event);
    virtual void TextInput(akView* sender, akKeyEvent* event);
    /* akViewNotificationReceiver overrides */
    virtual void ViewWillResignFirstResponder();
    virtual void ViewWillBecameFirstResponder();

private:
    ListBox* mListBox;
};
