#include <widgettoolkit/listbox.h>
#include <widgettoolkit/legacytools.h>
#include <widgettoolkit/resources.h>
#include <widgettoolkit/window.h>
#include <SDL_image.h>

/* *********************************
 * ListBox Object Oriented Wrapper *
 ***********************************/

akListBox::akListBox(akRect rect, akView* parent)
    : akControl(rect, parent)
{
    SetClassName("ListBox");

    akRect viewRect = GetRect();

    SDL_Color bgColor;
    bgColor.r = bgColor.g = bgColor.b = 255;

    SDL_Color selectionColor;
    selectionColor.r = 49;
    selectionColor.g = 106;
    selectionColor.b = 197;

    mListBox = ListBox_Init(viewRect.location.x, viewRect.location.y, viewRect.size.width, viewRect.size.height,
        Resources::GetFontResource(DEFAULT_FONT), 10, bgColor, selectionColor, this);

    AddPainter(this);
    AddMouseEventReceiver(this);
    AddKeyEventReceiver(this);
    AddViewNotificationReceiver(this);
}

void akListBox::AddItem(string text)
{
    ListBox_AddItem(mListBox, text.c_str());
}

void akListBox::RemoveItem(int index)
{
    ListBox_RemoveItem(mListBox, index);
}

string akListBox::GetItemText(int index)
{
    string text(ListBox_GetItem(mListBox, index));
    return text;
}

void akListBox::SetItemText(int index, string text)
{
    ListBox_SetItem(mListBox, index, (char*)text.c_str());
}

int akListBox::GetSelectionIndex()
{
    return ListBox_GetSelectionIndex(mListBox);
}

/* akPainter Override */
void akListBox::Paint(akView* view, SDL_Surface* destination)
{
    SDL_Rect p, ps;
    int i, l, scale;
    SDL_Surface* scrollbar = NULL;
    ListBox* listbox = mListBox;

    ListBox_DrawContent(listbox);
    LegacyTools::Blit(listbox->bg, 0, 0, listbox->width, listbox->height, listbox->destSurface, 0, 0);
    LegacyTools::Blit(listbox->surface, 0, listbox->k, listbox->width, listbox->height, listbox->destSurface, 0, 0);

    if (listbox->of) {
        p.x = 0 + listbox->width - listbox->data[0]->w;
        p.y = 0;
        if (listbox->upScrollBtnState) {
            SDL_BlitSurface(listbox->data[1], NULL, listbox->destSurface, &p);
        }
        else {
            SDL_BlitSurface(listbox->data[0], NULL, listbox->destSurface, &p);
        }

        p.x = 0 + listbox->width - listbox->data[2]->w;
        p.y = 0 + listbox->height - listbox->data[2]->h;
        if (listbox->downScrollBtnState) {
            SDL_BlitSurface(listbox->data[3], NULL, listbox->destSurface, &p);
        }
        else {

            SDL_BlitSurface(listbox->data[2], NULL, listbox->destSurface, &p);
        }

        for (i = 0; i < (listbox->height - listbox->data[0]->h - listbox->data[2]->h); i++) {
            p.x = 0 + listbox->width - listbox->data[4]->w;
            p.y = 0 + listbox->data[0]->h + i;
            ps.x = 0;
            ps.y = 1;
            ps.w = listbox->data[4]->w;
            ps.h = 1;
            SDL_BlitSurface(listbox->data[4], &ps, listbox->destSurface, &p);
        }

        listbox->nbScroll = ListBox_GetNbScroll(listbox);

        l = listbox->height - (listbox->data[0]->h + listbox->data[2]->h);
        if (listbox->nbScroll == 0) {
            scrollbar = SDL_CreateRGBSurface(0, listbox->data[0]->w - 2, 1, 32, 0, 0, 0, 0);
        }
        else {
            if ((l / listbox->nbScroll) < 0) {
                scrollbar = SDL_CreateRGBSurface(0, listbox->data[0]->w - 2, 1, 32, 0, 0, 0, 0);
            }
            else {
                scrollbar = SDL_CreateRGBSurface(0, listbox->data[0]->w - 2, l / listbox->nbScroll, 32, 0, 0, 0, 0);
            }
        }

        SDL_FillRect(scrollbar, NULL, SDL_MapRGB(listbox->destSurface->format, listbox->bgColor.r - 100, listbox->bgColor.g - 100, listbox->bgColor.b - 100));
        if (listbox->nbScroll == 0) {
            scale = (l)*listbox->currentNbScroll;
        }
        else
            scale = (l / listbox->nbScroll) * listbox->currentNbScroll;

        p.y = 0 + listbox->data[0]->h + scale;

        if (listbox->currentNbScroll == listbox->nbScroll) {
            if (listbox->nbScroll == 0)
                p.y = 0 + listbox->height - listbox->data[2]->h - l;
            else
                p.y = 0 + listbox->height - listbox->data[2]->h - l / listbox->nbScroll;
        }
        p.x = 0 + listbox->width - scrollbar->w - 1;
        SDL_BlitSurface(scrollbar, NULL, listbox->destSurface, &p);
    }

    /* To be updated */
    SDL_Rect dstrect;
    dstrect.x = listbox->x - 2;
    dstrect.y = listbox->y - 2;
    SDL_BlitSurface(listbox->mbg, NULL, destination, &dstrect);

    dstrect.x = listbox->x;
    dstrect.y = listbox->y;
    SDL_BlitSurface(listbox->destSurface, NULL, destination, &dstrect);

    SDL_FreeSurface(scrollbar);
}

/* akMouseEvenReceiver Overrides */
void akListBox::MousePress(akView* sender, akMouseEvent* event)
{
    this->InvokeActionReceivers(this);
    GetWindow()->SetFirstResponder(this);
    ListBox_OnMouseButtonDown(mListBox, event->GetLocationInWindow().x, event->GetLocationInWindow().y);
    GetWindow()->Repaint();
}

void akListBox::MouseRelease(akView* sender, akMouseEvent* event)
{
    ListBox_OnMouseButtonUp(mListBox, event->GetLocationInWindow().x, event->GetLocationInWindow().y);
    GetWindow()->Repaint();
}

void akListBox::MouseMove(akView* sender, akMouseEvent* event)
{
}

void akListBox::MouseDrag(akView* sender, akMouseEvent* event)
{
    ListBox_OnMouseMotion(mListBox, event->GetLocationInWindow().x, event->GetLocationInWindow().y);
    GetWindow()->Repaint();
}

void akListBox::MouseWheelUp(akView* sender, akMouseEvent* event)
{
    ListBox_OnWheelUp(mListBox, event->GetLocationInWindow().x, event->GetLocationInWindow().y);
    GetWindow()->Repaint();
}

void akListBox::MouseWheelDown(akView* sender, akMouseEvent* event)
{
    ListBox_OnWheelDown(mListBox, event->GetLocationInWindow().x, event->GetLocationInWindow().y);
    GetWindow()->Repaint();
}

/* akKeyEventReceiver Overrides */
void akListBox::KeyPress(akView* sender, akKeyEvent* event)
{
    SDL_Event* sdlEvent = (SDL_Event*)event->Reserved;
    ListBox_OnKeyDown(mListBox, sdlEvent->key.keysym.sym);
    GetWindow()->Repaint();
}

void akListBox::KeyRelease(akView* sender, akKeyEvent* event)
{
    ListBox_OnKeyUp(mListBox);
    GetWindow()->Repaint();
}

void akListBox::TextInput(akView* sender, akKeyEvent* event)
{
}

/* akViewNotification receiver overrides */
void akListBox::ViewWillResignFirstResponder()
{
    ListBox_DrawBorder(mListBox);
}

void akListBox::ViewWillBecameFirstResponder()
{
    akColor focusColor = GetFocusColor();
    SDL_FillRect(mListBox->mbg, NULL, SDL_MapRGB(mListBox->mbg->format, focusColor.r, focusColor.g, focusColor.b));
}

/* ******************************************************
 * LEGACY CODE (To be upgraded later in later versions) *
 ********************************************************/

int akListBox::GetItemCount()
{
    return ListBox_GetNbItems(mListBox);
}

void ListBox_DrawBorder(ListBox* w)
{
    SDL_Surface* s;
    SDL_Rect p;
    int i;

    w->mbg = SDL_CreateRGBSurface(0, w->width + 4, w->height + 4, 32, 0, 0, 0, 0);
    SDL_FillRect(w->mbg, NULL, SDL_MapRGB(w->mbg->format, 255, 255, 255));

    s = SDL_CreateRGBSurface(0, 1, 1, 32, 0, 0, 0, 0);
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 171, 173, 179));
    for (i = 2; i < w->mbg->w - 2; i++) {
        p.x = i;
        p.y = 0;
        SDL_BlitSurface(s, NULL, w->mbg, &p);
    }
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 227, 233, 239));
    for (i = 1; i < w->mbg->w - 1; i++) {
        p.x = i;
        p.y = w->mbg->h - 1;
        SDL_BlitSurface(s, NULL, w->mbg, &p);
    }
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 226, 227, 234));
    for (i = 1; i < w->mbg->h - 1; i++) {
        p.x = 0;
        p.y = i;
        SDL_BlitSurface(s, NULL, w->mbg, &p);
    }
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 219, 223, 230));
    for (i = 1; i < w->mbg->h - 1; i++) {
        p.x = w->mbg->w - 1;
        p.y = i;
        SDL_BlitSurface(s, NULL, w->mbg, &p);
    }

    // 1/4
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 219, 223, 230));
    p.x = 0;
    p.y = 0;
    SDL_BlitSurface(s, NULL, w->mbg, &p);
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 187, 189, 194));
    p.x = 1;
    p.y = 0;
    SDL_BlitSurface(s, NULL, w->mbg, &p);
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 233, 236, 240));
    p.x = 1;
    p.y = 1;
    SDL_BlitSurface(s, NULL, w->mbg, &p);

    // 2/4
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 235, 235, 238));
    p.x = 0;
    p.y = w->mbg->h - 1;
    SDL_BlitSurface(s, NULL, w->mbg, &p);
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 233, 236, 240));
    p.x = 1;
    p.y = w->mbg->h - 2;
    SDL_BlitSurface(s, NULL, w->mbg, &p);

    // 3/4
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 187, 189, 194));

    p.x = w->mbg->w - 2;
    p.y = 0;
    SDL_BlitSurface(s, NULL, w->mbg, &p);
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 219, 223, 230));
    p.x = w->mbg->w - 1;
    p.y = 0;
    SDL_BlitSurface(s, NULL, w->mbg, &p);
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 233, 236, 240));
    p.x = w->mbg->w - 2;
    p.y = 1;
    SDL_BlitSurface(s, NULL, w->mbg, &p);

    // 4/4
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 235, 235, 238));
    p.x = w->mbg->w - 1;
    p.y = w->mbg->h - 1;
    SDL_BlitSurface(s, NULL, w->mbg, &p);
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 233, 236, 240));
    p.x = w->mbg->w - 2;
    p.y = w->mbg->h - 2;
    SDL_BlitSurface(s, NULL, w->mbg, &p);
}

ListBox* ListBox_Init(int x, int y, int width, int height, TTF_Font* font, int scrollScale,
    SDL_Color bgColor, SDL_Color selectionColor, akListBox* listBoxObject)
{
    ListBox* w;

    w = (ListBox*)malloc(sizeof(ListBox));
    //w->akListBoxObject = NULL;
    w->font = font;
    w->x = x;
    w->y = y;
    w->width = width;
    w->height = height;
    w->invMultiSel = 0;
    w->ItemList = NULL;
    w->mouseHold = 0;
    w->of = 0;
    w->selStartItem = NULL;
    w->selEndItem = NULL;
    w->currentNbScroll = 0;
    w->nbScroll = 0;
    w->stopHScroll = 0;
    w->mouseDown_scrollDown = 0;
    w->mouseDown_scrollUp = 0;
    w->mouseDown_scrollBar = 0;
    w->destSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
    w->stop = 0;
    w->tik = 0;
    w->cTime = 0;
    w->pTime = 0;
    w->i = 0;
    w->upScrollBtnState = 0;
    w->downScrollBtnState = 0;
    w->selectedItem = NULL;
    w->surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
    w->bg = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
    w->shiftHold = 0;

    ListBox_DrawBorder(w);

    w->bgColor.r = bgColor.r;
    w->bgColor.g = bgColor.g;
    w->bgColor.b = bgColor.b;
    SDL_FillRect(w->bg, NULL, SDL_MapRGB(w->bg->format, w->bgColor.r, w->bgColor.g, w->bgColor.b));
    w->selectionColor.r = selectionColor.r;
    w->selectionColor.g = selectionColor.g;
    w->selectionColor.b = selectionColor.b;
    w->k = 0;
    w->scrollScale = scrollScale;
    w->data = (SDL_Surface**)malloc(sizeof(SDL_Surface) * 5);
    w->data[0] = Resources::GetImageResource(SCROLLBUTTON_UP);
    w->data[1] = Resources::GetImageResource(SCROLLBUTTON_UP_PRESSED);
    w->data[2] = Resources::GetImageResource(SCROLLBUTTON_DOWN);
    w->data[3] = Resources::GetImageResource(SCROLLBUTTON_DOWN_PRESSED);
    w->data[4] = Resources::GetImageResource(SCROLLSURFACE);
    w->itemsCount = 0;
    w->multiSelect = 0;
    return w;
}

int ListBox_GetNbScroll(ListBox* listbox)
{
    int i = 0, j = 0;
    while ((listbox->height + i) <= (listbox->surface->h)) {
        i += listbox->scrollScale;
        j++;
    }
    return (j - 1);
}

void ListBox_DrawItem(ListBox_ItemList* listitem, ListBox* listbox, int selected)
{
    SDL_Rect p;

    p.x = listitem->info.x;
    p.y = listitem->info.y;
    if (selected) {
        SDL_BlitSurface(listitem->info.surface_selected, NULL, listbox->surface, &p);
    }
    else {
        SDL_BlitSurface(listitem->info.surface, NULL, listbox->surface, &p);
        SDL_BlitSurface(listitem->info.textsurf, NULL, listbox->surface, &p);
    }
}

void ListBox_DrawContent(ListBox* listbox)
{
    ListBox_ItemList* l;
    int s = 0, w;

    l = listbox->ItemList;
    while (l != NULL) {
        s += l->info.surface->h;
        l = l->next;
    }

    w = listbox->surface->w;
    if (listbox->surface != NULL)
        SDL_FreeSurface(listbox->surface);
    listbox->surface = SDL_CreateRGBSurface(0, w, s, 32, 0, 0, 0, 0);
    SDL_FillRect(listbox->surface, NULL, SDL_MapRGB(listbox->surface->format, listbox->bgColor.r, listbox->bgColor.g, listbox->bgColor.b));

    l = listbox->ItemList;
    while (l != NULL) {
        if (l->info.select)
            ListBox_DrawItem(l, listbox, 1);
        else
            ListBox_DrawItem(l, listbox, 0);
        l = l->next;
    }
    if (listbox->surface->h > listbox->height)
        listbox->of = 1;
    else {
        listbox->of = 0;
        listbox->currentNbScroll = 0;
        listbox->nbScroll = 0;
        listbox->stopHScroll = 0;
        listbox->mouseDown_scrollDown = 0;
        listbox->mouseDown_scrollUp = 0;
        listbox->mouseDown_scrollBar = 0;
        listbox->stop = 0;
        listbox->tik = 0;
        listbox->cTime = 0;
        listbox->pTime = 0;
        listbox->i = 0;
        listbox->upScrollBtnState = 0;
        listbox->downScrollBtnState = 0;
        listbox->k = 0;
    }
}

void ListBox_Draw(ListBox* listbox)
{
    SDL_Rect p, ps;
    int i, l, scale;
    SDL_Surface* scrollbar = NULL;

    ListBox_DrawContent(listbox);
    LegacyTools::Blit(listbox->bg, 0, 0, listbox->width, listbox->height, listbox->destSurface, 0, 0);
    LegacyTools::Blit(listbox->surface, 0, listbox->k, listbox->width, listbox->height, listbox->destSurface, 0, 0);

    if (listbox->of) {
        p.x = 0 + listbox->width - listbox->data[0]->w;
        p.y = 0;
        if (listbox->upScrollBtnState) {
            SDL_BlitSurface(listbox->data[1], NULL, listbox->destSurface, &p);
        }
        else {
            SDL_BlitSurface(listbox->data[0], NULL, listbox->destSurface, &p);
        }

        p.x = 0 + listbox->width - listbox->data[2]->w;
        p.y = 0 + listbox->height - listbox->data[2]->h;
        if (listbox->downScrollBtnState) {
            SDL_BlitSurface(listbox->data[3], NULL, listbox->destSurface, &p);
        }
        else {

            SDL_BlitSurface(listbox->data[2], NULL, listbox->destSurface, &p);
        }

        for (i = 0; i < (listbox->height - listbox->data[0]->h - listbox->data[2]->h); i++) {
            p.x = 0 + listbox->width - listbox->data[4]->w;
            p.y = 0 + listbox->data[0]->h + i;
            ps.x = 0;
            ps.y = 1;
            ps.w = listbox->data[4]->w;
            ps.h = 1;
            SDL_BlitSurface(listbox->data[4], &ps, listbox->destSurface, &p);
        }

        listbox->nbScroll = ListBox_GetNbScroll(listbox);

        l = listbox->height - (listbox->data[0]->h + listbox->data[2]->h);
        if (listbox->nbScroll == 0) {
            scrollbar = SDL_CreateRGBSurface(0, listbox->data[0]->w - 2, 1, 32, 0, 0, 0, 0);
        }
        else {
            if ((l / listbox->nbScroll) < 0) {
                scrollbar = SDL_CreateRGBSurface(0, listbox->data[0]->w - 2, 1, 32, 0, 0, 0, 0);
            }
            else {
                scrollbar = SDL_CreateRGBSurface(0, listbox->data[0]->w - 2, l / listbox->nbScroll, 32, 0, 0, 0, 0);
            }
        }

        SDL_FillRect(scrollbar, NULL, SDL_MapRGB(listbox->destSurface->format, listbox->bgColor.r - 100, listbox->bgColor.g - 100, listbox->bgColor.b - 100));
        if (listbox->nbScroll == 0) {
            scale = (l)*listbox->currentNbScroll;
        }
        else
            scale = (l / listbox->nbScroll) * listbox->currentNbScroll;

        p.y = 0 + listbox->data[0]->h + scale;

        if (listbox->currentNbScroll == listbox->nbScroll) {
            if (listbox->nbScroll == 0)
                p.y = 0 + listbox->height - listbox->data[2]->h - l;
            else
                p.y = 0 + listbox->height - listbox->data[2]->h - l / listbox->nbScroll;
        }
        p.x = 0 + listbox->width - scrollbar->w - 1;
        SDL_BlitSurface(scrollbar, NULL, listbox->destSurface, &p);
    }

    /* To be updated */
    //MDDrawSurface(listbox->mbg,listbox->destWindow,listbox->x-2,listbox->y-2,0);
    //MDDrawSurface(listbox->destSurface,listbox->destWindow,listbox->x,listbox->y,0);
    SDL_FreeSurface(scrollbar);
}

void __ListBox_Draw(ListBox* listbox, int updateWnd)
{
    SDL_Rect p, ps;
    int i, l, scale;
    SDL_Surface* scrollbar = NULL;

    ListBox_DrawContent(listbox);
    LegacyTools::Blit(listbox->bg, 0, 0, listbox->width, listbox->height, listbox->destSurface, 0, 0);
    LegacyTools::Blit(listbox->surface, 0, listbox->k, listbox->width, listbox->height, listbox->destSurface, 0, 0);

    if (listbox->of) {
        p.x = 0 + listbox->width - listbox->data[0]->w;
        p.y = 0;
        if (listbox->upScrollBtnState) {
            SDL_BlitSurface(listbox->data[1], NULL, listbox->destSurface, &p);
        }
        else {
            SDL_BlitSurface(listbox->data[0], NULL, listbox->destSurface, &p);
        }

        p.x = 0 + listbox->width - listbox->data[2]->w;
        p.y = 0 + listbox->height - listbox->data[2]->h;
        if (listbox->downScrollBtnState) {
            SDL_BlitSurface(listbox->data[3], NULL, listbox->destSurface, &p);
        }
        else {

            SDL_BlitSurface(listbox->data[2], NULL, listbox->destSurface, &p);
        }

        for (i = 0; i < (listbox->height - listbox->data[0]->h - listbox->data[2]->h); i++) {
            p.x = 0 + listbox->width - listbox->data[4]->w;
            p.y = 0 + listbox->data[0]->h + i;
            ps.x = 0;
            ps.y = 1;
            ps.w = listbox->data[4]->w;
            ps.h = 1;
            SDL_BlitSurface(listbox->data[4], &ps, listbox->destSurface, &p);
        }

        listbox->nbScroll = ListBox_GetNbScroll(listbox);

        l = listbox->height - (listbox->data[0]->h + listbox->data[2]->h);
        if (listbox->nbScroll == 0) {
            scrollbar = SDL_CreateRGBSurface(0, listbox->data[0]->w - 2, 1, 32, 0, 0, 0, 0);
        }
        else {
            if ((l / listbox->nbScroll) < 0) {
                scrollbar = SDL_CreateRGBSurface(0, listbox->data[0]->w - 2, 1, 32, 0, 0, 0, 0);
            }
            else {
                scrollbar = SDL_CreateRGBSurface(0, listbox->data[0]->w - 2, l / listbox->nbScroll, 32, 0, 0, 0, 0);
            }
        }

        SDL_FillRect(scrollbar, NULL, SDL_MapRGB(listbox->destSurface->format, listbox->bgColor.r - 100, listbox->bgColor.g - 100, listbox->bgColor.b - 100));
        if (listbox->nbScroll == 0) {
            scale = (l)*listbox->currentNbScroll;
        }
        else
            scale = (l / listbox->nbScroll) * listbox->currentNbScroll;

        p.y = 0 + listbox->data[0]->h + scale;

        if (listbox->currentNbScroll == listbox->nbScroll) {
            if (listbox->nbScroll == 0)
                p.y = 0 + listbox->height - listbox->data[2]->h - l;
            else
                p.y = 0 + listbox->height - listbox->data[2]->h - l / listbox->nbScroll;
        }
        p.x = 0 + listbox->width - scrollbar->w - 1;
        SDL_BlitSurface(scrollbar, NULL, listbox->destSurface, &p);
    }

    /* To be updated */
    //MDDrawSurface(listbox->mbg,listbox->destWindow,listbox->x-2,listbox->y-2,0);
    //MDDrawSurface(listbox->destSurface,listbox->destWindow,listbox->x,listbox->y,updateWnd);
    SDL_FreeSurface(scrollbar);
}

/*
 * Needs Redisplay() after calling this
 */
void ListBox_AddItem(ListBox* listbox, const char* itemtext)
{
    ListBox_ItemList *p, *r;
    SDL_Color black = { 0, 0, 0 }, white = { 255, 255, 255 };
    SDL_Rect position;
    SDL_Surface* buffer;

    if (strlen(itemtext) <= 0)
        return;

    p = (ListBox_ItemList*)malloc(sizeof(ListBox_ItemList));
    p->info.text = (char*)malloc(sizeof(char) * (strlen(itemtext) + 1));
    p->info.index = listbox->itemsCount;
    listbox->itemsCount++;
    p->info.select = 0;
    strcpy(p->info.text, itemtext);
    p->info.textsurf = TTF_RenderText_Blended(listbox->font, itemtext, black);
    p->info.surface = SDL_CreateRGBSurface(0, listbox->width, p->info.textsurf->h, 32, 0, 0, 0, 0);
    SDL_FillRect(p->info.surface, NULL, SDL_MapRGB(p->info.surface->format, 255, 255, 255));
    p->info.surface_selected = SDL_CreateRGBSurface(SDL_SWSURFACE, listbox->width, p->info.surface->h, 32, 0, 0, 0, 0);
    SDL_FillRect(p->info.surface_selected, NULL,
        SDL_MapRGB(p->info.surface_selected->format, listbox->selectionColor.r, listbox->selectionColor.g, listbox->selectionColor.b));
    buffer = TTF_RenderText_Blended(listbox->font, itemtext, white);
    position.x = 0;
    position.y = 0;
    SDL_BlitSurface(buffer, NULL, p->info.surface_selected, &position);

    if (listbox->ItemList == NULL) {
        p->info.x = 0;
        p->info.y = 0;
    }
    else {
        p->info.x = 0;
        r = listbox->ItemList;
        while (r->next != NULL) {
            r = r->next;
        }
        p->info.y = r->info.y + r->info.surface->h;
    }
    p->next = NULL;

    if (listbox->ItemList == NULL) {
        listbox->ItemList = (ListBox_ItemList*)malloc(sizeof(ListBox_ItemList));
        listbox->ItemList = p;
        listbox->ItemList->next = NULL;
    }
    else {
        r = listbox->ItemList;
        while (r->next != NULL) {
            r = r->next;
        }
        r->next = p;
    }
    /* Canceled */
    //ListBox_Draw(listbox);
}

void ListBox_Delete(ListBox* listbox)
{
    ListBox_ItemList* p;
    SDL_FreeSurface(listbox->surface);
    p = listbox->ItemList;
    while (p != NULL) {
        free(p->info.text);
        SDL_FreeSurface(p->info.surface);
        SDL_FreeSurface(p->info.surface_selected);
        p = p->next;
    }
    TTF_CloseFont(listbox->font);
}

/*
 * Needs Redisplay() after calling this
 */
void ListBox_ScrollDown(ListBox* listbox)
{
    listbox->currentNbScroll++;
    if (listbox->currentNbScroll > ListBox_GetNbScroll(listbox)) {
        listbox->currentNbScroll--;
    }
    if (listbox->surface->h > listbox->height) {
        listbox->k += listbox->scrollScale;

        if ((listbox->height + listbox->k) > (listbox->surface->h)) {
            listbox->k = listbox->surface->h - listbox->height;
        }

        /* Canceled */
        //ListBox_Draw(listbox);
    }
}

/*
 * Needs Redisplay() after calling this
 */
void ListBox_ScrollUp(ListBox* listbox)
{
    listbox->currentNbScroll--;
    if (listbox->currentNbScroll < 0) {
        listbox->currentNbScroll = 0;
    }
    if (listbox->surface->h > listbox->height) {
        listbox->k -= listbox->scrollScale;
        if (listbox->k < listbox->scrollScale) {
            listbox->k = 0;
        }

        //ListBox_Draw(listbox);
    }
}

/*
 * Needs Redisplay() after calling this
 */
void ListBox_OnMouseMotion(ListBox* listbox, int mouse_x, int mouse_y)
{
    ListBox_ItemList *p, *r;

    if (!listbox->mouseBtnHoldIndicator)
        return;

    p = listbox->ItemList;
    while (p != NULL) {
        if (((mouse_x) >= p->info.x + listbox->x) && ((mouse_y + listbox->k) >= p->info.y + listbox->y) && ((mouse_x) <= p->info.x + p->info.surface_selected->w + listbox->x) && ((mouse_y + listbox->k) <= p->info.y + p->info.surface_selected->h + listbox->y)) {
            listbox->selEndItem = p;
            break;
        }
        p = p->next;
    }

    if (p == NULL)
        return;

    r = listbox->ItemList;
    while (r != NULL) {
        if (r != listbox->selStartItem) {
            r->info.select = 0;
            ListBox_DrawItem(r, listbox, 0);
        }
        r = r->next;
    }
    //ListBox_DrawContent(listbox);

    if (listbox->selStartItem->info.index < p->info.index) {
        listbox->invMultiSel = 0;
        r = listbox->selStartItem;
        while ((r != p) && (r != NULL)) {
            r->info.select = 1;
            listbox->selectedItem = r;
            ListBox_DrawItem(r, listbox, 1);
            //ListBox_DrawContent(listbox);
            r = r->next;
        }

        if (p != NULL) {
            p->info.select = 1;
            listbox->selectedItem = p;
            //ListBox_DrawContent(listbox);
            ListBox_DrawItem(p, listbox, 1);
        }
    }
    else {
        listbox->invMultiSel = 1;
        r = p;
        while ((r != listbox->selStartItem) && (r != NULL)) {
            r->info.select = 1;
            listbox->selectedItem = r;
            ListBox_DrawItem(r, listbox, 1);
            //ListBox_DrawContent(listbox);
            r = r->next;
        }
    }

    /* Canceled */
    //__ListBox_Draw(listbox,1);
}

/*
 * Needs Redisplay() after calling this
 */
void ListBox_OnMouseButtonDown(ListBox* listbox, int mouse_x, int mouse_y)
{
    ListBox_ItemList *p, *r;
    SDL_Rect ps;
    int i = 0;

    listbox->focus = 1;
    if (listbox->of) {
        if ((mouse_x >= listbox->x + listbox->width - listbox->data[0]->w) && (mouse_y >= listbox->y) && (mouse_x <= listbox->x + listbox->width) && (mouse_y <= listbox->y + listbox->data[0]->h)) {
            ps.x = 0 + listbox->width - listbox->data[1]->w;
            ps.y = 0;
            SDL_BlitSurface(listbox->data[1], NULL, listbox->destSurface, &ps);
            listbox->mouseDown_scrollUp = 1;
            listbox->upScrollBtnState = 1;
            /* To be updated */
            //MDDrawSurface(listbox->destSurface,listbox->destWindow,listbox->x,listbox->y,1);
            return;
        }

        if ((mouse_x >= listbox->x + listbox->width - listbox->data[2]->w) && (mouse_y >= listbox->y + listbox->height - listbox->data[2]->h) && (mouse_x <= listbox->x + listbox->width) && (mouse_y <= listbox->y + listbox->height - listbox->data[2]->h + listbox->data[2]->h)) {
            ps.x = 0 + listbox->width - listbox->data[3]->w;
            ps.y = 0 + listbox->height - listbox->data[2]->h;
            SDL_BlitSurface(listbox->data[3], NULL, listbox->destSurface, &ps);
            listbox->mouseDown_scrollDown = 1;
            listbox->downScrollBtnState = 1;
            /* To be updated */
            //MDDrawSurface(listbox->destSurface,listbox->destWindow,listbox->x,listbox->y,1);
            return;
        }

        if (mouse_x > (listbox->x + listbox->width - listbox->data[4]->w)) {
            return;
        }
    }

    if (listbox->shiftHold) {
        i = ListBox_GetSelectionIndex(listbox);
        if (i != -1) {
            p = listbox->ItemList;
            while (p != NULL) {
                if (((mouse_x) >= p->info.x + listbox->x) && ((mouse_y + listbox->k) >= p->info.y + listbox->y) && ((mouse_x) <= p->info.x + p->info.surface_selected->w + listbox->x) && ((mouse_y + listbox->k) <= p->info.y + p->info.surface_selected->h + listbox->y)) {
                    listbox->selEndItem = p;
                    break;
                }
                p = p->next;
            }

            if (p == NULL)
                return;

            r = listbox->ItemList;
            while (r != NULL) {
                if (r != listbox->selStartItem) {
                    r->info.select = 0;
                    ListBox_DrawItem(r, listbox, 0);
                }
                r = r->next;
            }
            //ListBox_DrawContent(listbox);

            if (listbox->selStartItem->info.index < p->info.index) {
                listbox->invMultiSel = 0;
                r = listbox->selStartItem;
                while ((r != p) && (r != NULL)) {
                    r->info.select = 1;
                    listbox->selectedItem = r;
                    ListBox_DrawItem(r, listbox, 1);
                    //ListBox_DrawContent(listbox);
                    r = r->next;
                }

                if (p != NULL) {
                    p->info.select = 1;
                    listbox->selectedItem = p;
                    //ListBox_DrawContent(listbox);
                    ListBox_DrawItem(p, listbox, 1);
                }
            }
            else {
                listbox->invMultiSel = 1;
                r = p;
                while ((r != listbox->selStartItem) && (r != NULL)) {
                    r->info.select = 1;
                    listbox->selectedItem = r;
                    ListBox_DrawItem(r, listbox, 1);
                    //ListBox_DrawContent(listbox);
                    r = r->next;
                }
            }
            /* Canceled */
            //__ListBox_Draw(listbox,1);
        }
        else {
            //Copy Paste code
            p = listbox->ItemList;
            while (p != NULL) {
                if (((mouse_x) >= p->info.x + listbox->x) && ((mouse_y + listbox->k) >= p->info.y + listbox->y) && ((mouse_x) <= p->info.x + p->info.surface_selected->w + listbox->x) && ((mouse_y + listbox->k) <= p->info.y + p->info.surface_selected->h + listbox->y)) {
                    r = listbox->ItemList;
                    while (r != NULL) {
                        if (r != p) {
                            r->info.select = 0;
                        }
                        r = r->next;
                    }
                    p->info.select = 1;
                    listbox->selectedItem = p;
                    listbox->mouseHold = 1;
                    //puts("LISTBOX: mouseHold = 1");
                    listbox->mouseBtnHoldIndicator = 1;
                    listbox->selStartItem = p;
                    listbox->selEndItem = p;
                    ListBox_DrawContent(listbox);
                    /* Canceled */
                    //ListBox_Draw(listbox);
                    break;
                }
                p = p->next;
            }
        }
    }
    else {
        //Copy Paste code
        p = listbox->ItemList;
        while (p != NULL) {
            if (((mouse_x) >= p->info.x + listbox->x) && ((mouse_y + listbox->k) >= p->info.y + listbox->y) && ((mouse_x) <= p->info.x + p->info.surface_selected->w + listbox->x) && ((mouse_y + listbox->k) <= p->info.y + p->info.surface_selected->h + listbox->y)) {
                r = listbox->ItemList;
                while (r != NULL) {
                    if (r != p) {
                        r->info.select = 0;
                    }
                    r = r->next;
                }
                p->info.select = 1;
                listbox->selectedItem = p;
                listbox->mouseHold = 1;
                //puts("LISTBOX: mouseHold = 1");
                listbox->mouseBtnHoldIndicator = 1;
                listbox->selStartItem = p;
                listbox->selEndItem = p;
                ListBox_DrawContent(listbox);
                /* Canceled */
                //ListBox_Draw(listbox);
                break;
            }
            p = p->next;
        }
    }

    /* To be updated */
    //MDDrawSurface(listbox->destSurface,listbox->destWindow,listbox->x,listbox->y,1);
}

void ListBox_OnMouseButtonUp(ListBox* listbox, int mouse_x, int mouse_y)
{
    SDL_Rect ps;
    listbox->mouseHold = 0;
    listbox->mouseBtnHoldIndicator = 0;

    if (listbox->mouseDown_scrollUp) {

        ps.x = 0 + listbox->width - listbox->data[0]->w;
        ps.y = 0;
        SDL_BlitSurface(listbox->data[0], NULL, listbox->destSurface, &ps);
        listbox->mouseDown_scrollUp = 0;
        listbox->upScrollBtnState = 0;
        if ((mouse_x >= listbox->x + listbox->width - listbox->data[0]->w) && (mouse_y >= listbox->y) && (mouse_x <= listbox->x + listbox->width) && (mouse_y <= listbox->y + listbox->data[0]->h)) {
            ListBox_ScrollUp(listbox);
        }

        listbox->stop = 0;
        listbox->tik = 0;
        listbox->cTime = 0;
        listbox->pTime = 0;
        listbox->i = 0;
        listbox->mouseDown_scrollDown = 0;
    }
    if (listbox->mouseDown_scrollDown) {
        ps.x = 0 + listbox->width - listbox->data[2]->w;
        ps.y = 0 + listbox->height - listbox->data[2]->h;
        SDL_BlitSurface(listbox->data[2], NULL, listbox->destSurface, &ps);
        listbox->mouseDown_scrollDown = 0;
        listbox->downScrollBtnState = 0;
        if ((mouse_x >= listbox->x + listbox->width - listbox->data[2]->w) && (mouse_y >= listbox->y + listbox->height - listbox->data[2]->h) && (mouse_x <= listbox->x + listbox->width) && (mouse_y <= listbox->y + listbox->height - listbox->data[2]->h + listbox->data[2]->h)) {
            ListBox_ScrollDown(listbox);
        }

        listbox->stop = 0;
        listbox->tik = 0;
        listbox->cTime = 0;
        listbox->pTime = 0;
        listbox->i = 0;
        listbox->mouseDown_scrollDown = 0;
    }
    /* To be updated */
    //MDDrawSurface(listbox->destSurface,listbox->destWindow,listbox->x,listbox->y,1);
}

void ListBox_OnWheelDown(ListBox* listbox, int mouse_x, int mouse_y)
{
    if ((mouse_x >= listbox->x) && (mouse_y >= listbox->y) && (mouse_x <= listbox->x + listbox->width + listbox->data[4]->w) && (mouse_y <= listbox->y + listbox->height)) {
        ListBox_ScrollDown(listbox);
    }
}

void ListBox_OnWheelUp(ListBox* listbox, int mouse_x, int mouse_y)
{
    if ((mouse_x >= listbox->x) && (mouse_y >= listbox->y) && (mouse_x <= listbox->x + listbox->width + listbox->data[4]->w) && (mouse_y <= listbox->y + listbox->height)) {
        ListBox_ScrollUp(listbox);
    }
}

void ListBox_OnNoEvent(ListBox* listbox, int mouse_x, int mouse_y)
{
    if ((mouse_x >= listbox->x) && (mouse_y >= listbox->y) && (mouse_x <= listbox->x + listbox->width + listbox->data[4]->w) && (mouse_y <= listbox->y + listbox->height)) {
        if (listbox->mouseDown_scrollDown) {
            listbox->cTime = SDL_GetTicks();
            if (listbox->cTime - listbox->pTime > 350) {
                listbox->pTime = listbox->cTime;
                listbox->tik = 1;
                listbox->i++;
            }
            if (((listbox->tik) && (listbox->i > 1)) || listbox->stop) {
                listbox->tik = 0;
                listbox->stop = 1;
                ListBox_ScrollDown(listbox);
                //SDL_Flip(SDL_GetVideoSurface());
                SDL_Delay(50);
            }
        }
        if (listbox->mouseDown_scrollUp) {
            listbox->cTime = SDL_GetTicks();
            if (listbox->cTime - listbox->pTime > 500) {
                listbox->pTime = listbox->cTime;
                listbox->tik = 1;
                listbox->i++;
            }
            if (((listbox->tik) && (listbox->i > 1)) || listbox->stop) {
                listbox->tik = 0;
                listbox->stop = 1;
                ListBox_ScrollUp(listbox);
                //SDL_Flip(SDL_GetVideoSurface());
                SDL_Delay(50);
            }
        }
    }
}

char* ListBox_GetItem(ListBox* listbox, int index)
{
    ListBox_ItemList* p;
    int i = 0;
    p = listbox->ItemList;
    while ((p != NULL) && i < index) {
        p = p->next;
        i++;
    }
    if (p == NULL)
        return NULL;
    else
        return p->info.text;
}

void ListBox_SetItem(ListBox* listbox, int index, char* text)
{
    ListBox_ItemList* p;
    int i = 0;
    SDL_Surface* buffer;
    SDL_Rect position;
    SDL_Color black = { 0, 0, 0 }, white = { 255, 255, 255 };

    p = listbox->ItemList;
    while ((p != NULL) && i < index) {
        p = p->next;
        i++;
    }
    if (p != NULL) {
        strcpy(p->info.text, text);
        p->info.surface = TTF_RenderText_Blended(listbox->font, text, black);
        p->info.surface_selected = SDL_CreateRGBSurface(SDL_SWSURFACE, listbox->width, p->info.surface->h, 32, 0, 0, 0, 0);
        SDL_FillRect(p->info.surface_selected, NULL,
            SDL_MapRGB(p->info.surface_selected->format, listbox->selectionColor.r, listbox->selectionColor.g, listbox->selectionColor.b));
        buffer = TTF_RenderText_Blended(listbox->font, text, white);
        position.x = 0;
        position.y = 0;
        SDL_BlitSurface(buffer, NULL, p->info.surface_selected, &position);
    }
}

int ListBox_GetNbItems(ListBox* listbox)
{
    ListBox_ItemList* p;
    int i = 0;
    p = listbox->ItemList;
    while (p != NULL) {
        p = p->next;
        i++;
    }
    return i;
}

/*
 * Needs Redisplay() after calling this
 */
int ListBox_RemoveItem(ListBox* listbox, int index)
{
    ListBox_ItemList *p, *r;
    int i = 0;
    p = listbox->ItemList;
    while ((p != NULL) && i < index) {
        p = p->next;
        i++;
    }
    if (p == NULL)
        return -1;
    else {
        if (p == listbox->ItemList) {
            if (listbox->ItemList->next == NULL) {
                listbox->ItemList = NULL;
            }
            else {
                listbox->ItemList = listbox->ItemList->next;
                listbox->ItemList->info.y = 0;

                r = listbox->ItemList;
                p = listbox->ItemList->next;
                while (p != NULL) {
                    p->info.y = r->info.y + p->info.surface->h;
                    r = p;
                    p = p->next;
                }
                free(p);

                p = listbox->ItemList;
                while (p != NULL) {
                    p = p->next;
                }
            }
        }
        else if (p->next == NULL) {
            r = listbox->ItemList;
            while (r->next != p)
                r = r->next;
            r->next = NULL;
            free(p);

            p = listbox->ItemList;
            while (p != NULL) {
                p = p->next;
            }
        }
        else {
            r = listbox->ItemList;
            while (r->next != p)
                r = r->next;
            r->next = r->next->next;

            p = r->next;
            while (p != NULL) {
                p->info.y = r->info.y + p->info.surface->h;
                r = p;
                p = p->next;
            }

            free(p);
        }
        p = listbox->ItemList;
        while (p != NULL) {
            p = p->next;
        }
        listbox->selectedItem = NULL;

        //if(!listbox->of)
        //ListBox_DrawContent(listbox);
        //ListBox_Draw(listbox);
        /*j=0;
		p = listbox->ItemList;
		while(p != NULL)
		{
			j++;
			p = p->next;
		}
		for(i=0; i<j; i++)
			ListBox_ScrollUp(listbox);*/
    }
    return 0;
}

int ListBox_GetSelectionIndex(ListBox* listbox)
{
    ListBox_ItemList* p;
    int i = 0;

    if ((listbox->ItemList == NULL) || (listbox->selectedItem == NULL))
        return -1;

    if (listbox->selectedItem != NULL) {
        p = listbox->ItemList;
        while (p != listbox->selectedItem) {
            i++;
            p = p->next;
        }
        return i;
    }
    else {
        return -1;
    }
}

char* ListBox_GetSelectionText(ListBox* listbox)
{
    ListBox_ItemList* p;

    if ((listbox->ItemList == NULL) || (listbox->selectedItem == NULL))
        return NULL;

    if (listbox->selectedItem != NULL) {
        p = listbox->ItemList;
        while (p != listbox->selectedItem) {
            p = p->next;
        }
        return p->info.text;
    }
    else {
        return NULL;
    }
}

void ListBox_SetItemBg(ListBox* listbox, int index, SDL_Surface* bg, int x, int y)
{

    ListBox_ItemList* p;
    SDL_Rect r;
    int i = 0;
    p = listbox->ItemList;
    while ((p != NULL) && i < index) {
        p = p->next;
        i++;
    }
    r.x = x;
    r.y = y;
    SDL_BlitSurface(bg, NULL, p->info.surface, &r);
}

ListBox_ItemList* ListBox_GetSelectedItems(ListBox* listbox)
{
    ListBox_ItemList *p = NULL, *r = NULL, *q = NULL, *w = NULL, *end = NULL;

    if ((listbox->selStartItem == NULL) || (listbox->selEndItem == NULL))
        return NULL;

    if (listbox->selStartItem->info.index < listbox->selEndItem->info.index) {
        p = listbox->selStartItem;
        end = listbox->selEndItem;
    }
    else if (listbox->selStartItem->info.index > listbox->selEndItem->info.index) {
        p = listbox->selEndItem;
        end = listbox->selStartItem;
    }
    else if (listbox->selStartItem == listbox->selEndItem) {
        r = (ListBox_ItemList*)malloc(sizeof(ListBox_ItemList));
        r->info = listbox->selStartItem->info;
        r->next = NULL;
        return r;
    }

    while (p != end) {
        if (r == NULL) {
            r = (ListBox_ItemList*)malloc(sizeof(ListBox_ItemList));
            r->info = p->info;
            r->next = NULL;
        }
        else {
            w = r;
            while (w->next != NULL)
                w = w->next;

            q = (ListBox_ItemList*)malloc(sizeof(ListBox_ItemList));
            q->info = p->info;
            w->next = q;
            q->next = NULL;
        }
        p = p->next;
    }
    if (end != NULL) {
        w = r;
        while (w->next != NULL)
            w = w->next;
        q = (ListBox_ItemList*)malloc(sizeof(ListBox_ItemList));
        q->info = end->info;
        w->next = q;
        q->next = NULL;
    }
    return r;
}

void ListBox_SetItemBg1(ListBox* listbox, ListBox_ItemList* p, SDL_Surface* bg, int x, int y)
{
    SDL_Rect r;

    r.x = x;
    r.y = y;
    SDL_BlitSurface(bg, NULL, p->info.surface, &r);
}

/*
 * Needs Redisplay() after calling this
 */
void ListBox_OnKeyDown(ListBox* listbox, int sym)
{
    ListBox_ItemList *p, *r;
    int i = 0;

    if (!listbox->focus)
        return;

    i = ListBox_GetSelectionIndex(listbox);
    if (sym == SDLK_UP) {
        if (listbox->selectedItem == NULL)
            return;
        if (listbox->selectedItem->info.index > 0) {
            p = listbox->ItemList;
            while (p->next != listbox->selectedItem) {
                i++;
                p = p->next;
            }
            r = listbox->ItemList;
            while (r != NULL) {
                r->info.select = 0;
                r = r->next;
            }
            listbox->selectedItem = p;
            p->info.select = 1;
            /* Canceled */
            //ListBox_Draw(listbox);
        }
    }
    else if (sym == SDLK_DOWN) {
        if (listbox->selectedItem == NULL)
            return;
        if (listbox->selectedItem->next != NULL) {
            p = listbox->ItemList;
            while (p != listbox->selectedItem) {
                i++;
                p = p->next;
            }
            r = listbox->ItemList;
            while (r != NULL) {
                r->info.select = 0;
                r = r->next;
            }
            listbox->selectedItem = p->next;
            p->next->info.select = 1;
            //ListBox_Draw(listbox);
        }
    }
    else if ((sym == SDLK_LSHIFT) || (sym == SDLK_RSHIFT)) {
        listbox->shiftHold = 1;
    }
}

void ListBox_OnKeyUp(ListBox* listbox)
{
    if (listbox->shiftHold)
        listbox->shiftHold = 0;
}
