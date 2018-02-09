#pragma once

#include <widgettoolkit/export.h>
#include <widgettoolkit/view.h>
#include <widgettoolkit/mouseevent.h>
#include <widgettoolkit/keyevent.h>
#include <widgettoolkit/mouseeventreceiver.h>
#include <widgettoolkit/keyeventreceiver.h>
#include <widgettoolkit/viewnotificationreceiver.h>
#include <widgettoolkit/control.h>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

using namespace std;

/* ******************************************************
 * LEGACY CODE (To be upgraded later in later versions) *
 ********************************************************/
typedef struct TextBox TextBox;
struct TextBox {
    SDL_Surface* textSurface;
    SDL_Rect textPosition, textBgPosition, cursorPosition;
    SDL_Surface *bgSurface, *cursorSurface, *bg;
    int blitInitialPosition, count, cursorIndex, dontDeleteChar, width, height, x, y, fontSize;
    char *text, *textSelectionBuffer, *text1, *text2, *text3;
    TTF_Font* font;
    SDL_Color bgColor, textColor;
    SDL_Surface* destSurface;
    int focus;
    long int id;
};

void TextBox_Delete(TextBox* textbox);
TextBox* TextBox_Init(int x, int y, int width, int height,
    TTF_Font* font, SDL_Color bgColor, SDL_Color textColor, SDL_Color cursorColor);
void TextBox_Draw(TextBox* textbox);
void __TextBox_Draw(TextBox* textbox, int updateWnd);
void TextBox_DrawBorder(TextBox* textbox);
int TextBox_OnMouseButtonDown(TextBox* textbox, int mouse_x, int mouse_y);
void TextBox_OnKeyDown(TextBox* textbox, int32_t scancode, int32_t keycode, int32_t keymod);
void TextBox_OnTextInput(TextBox* textbox, std::string text);
void TextBox_AppendText(TextBox* textbox, char* text);
void TextBox_SetText(TextBox* textbox, char* text);
char* TextBox_GetText(TextBox* textbox);
void TextBox_SetCursor(TextBox* textbox, int index);
void TextBox_SetFocus(TextBox* textbox);
void TextBox_RemoveFocus(TextBox* textbox);

/* *********************************
 * TextBox Object Oriented Wrapper *
 ***********************************/

class APPKIT_API akTextBox : public akControl, akKeyEventReceiver, akMouseEventReceiver, akPainter, akViewNotificationReceiver {
public:
    akTextBox(akRect rect, string text, akView* parent = NULL);
    string GetText();
    void SetText(string text);

private:
    virtual void KeyPress(akView* sender, akKeyEvent* event);
    virtual void KeyRelease(akView* sender, akKeyEvent* event);
    virtual void TextInput(akView* sender, akKeyEvent* event);
    virtual void MousePress(akView* sender, akMouseEvent* event);
    virtual void MouseRelease(akView* sender, akMouseEvent* event);
    virtual void MouseMove(akView* sender, akMouseEvent* event);
    virtual void MouseDrag(akView* sender, akMouseEvent* event);
    virtual void MouseWheelUp(akView* sender, akMouseEvent* event);
    virtual void MouseWheelDown(akView* sender, akMouseEvent* event);
    virtual void Paint(akView* view, SDL_Surface* destination);
    virtual void ViewWillResignFirstResponder();
    virtual void ViewWillBecameFirstResponder();

private:
    string mText;
    TextBox* mTextBox;
};
