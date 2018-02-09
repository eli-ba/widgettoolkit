#include <stdlib.h>
#include <widgettoolkit/widgettoolkit.h>
#include <iostream>

class MyPainter : public akPainter {
    virtual void Paint(akView* view, SDL_Surface* destination)
    {
        SDL_Rect dstrect;
        akRect rect = view->GetRect();
        dstrect.x = rect.location.x;
        dstrect.y = rect.location.y;
        dstrect.w = rect.size.width;
        dstrect.h = rect.size.height;
        SDL_FillRect(destination, &dstrect, SDL_MapRGB(destination->format, 255, 128, 0));
    }
};

class MyInputEventReceiver : public akMouseEventReceiver {
    virtual void MousePress(akView* sender, akMouseEvent* event)
    {
        printf("(MyView)MousePress\n");
    }

    virtual void MouseRelease(akView* sender, akMouseEvent* event)
    {
        printf("(MyView)MouseRelease\n");
    }

    virtual void MouseMove(akView* sender, akMouseEvent* event)
    {
        printf("(MyView)MouseMove\n");
    }

    virtual void MouseDrag(akView* sender, akMouseEvent* event)
    {
        printf("(MyView)MouseDrag\n");
    }

    virtual void MouseWheelUp(akView* sender, akMouseEvent* event)
    {
        printf("(MyView)MouseWheelUp\n");
    }

    virtual void MouseWheelDown(akView* sender, akMouseEvent* event)
    {
        printf("(MyView)MouseWheelDown\n");
    }
};

class AnotherPainter : public akPainter {
    virtual void Paint(akView* view, SDL_Surface* destination)
    {
        SDL_Rect dstrect;
        akRect rect = view->GetRect();
        dstrect.x = rect.location.x;
        dstrect.y = rect.location.y;
        dstrect.w = rect.size.width;
        dstrect.h = rect.size.height;
        SDL_FillRect(destination, &dstrect, SDL_MapRGB(destination->format, 0, 128, 128));
    }
};

class AnotherInputEventReceiver : public akMouseEventReceiver, public akKeyEventReceiver {
    virtual void MousePress(akView* sender, akMouseEvent* event)
    {
        printf("(AnotherView)MousePress\n");
    }

    virtual void MouseRelease(akView* sender, akMouseEvent* event)
    {
        printf("(AnotherView)MouseRelease\n");
    }

    virtual void MouseMove(akView* sender, akMouseEvent* event)
    {
        printf("(AnotherView)MouseMove\n");
    }

    virtual void MouseDrag(akView* sender, akMouseEvent* event)
    {
        printf("(AnotherView)MouseDrag\n");
    }

    virtual void KeyPress(akView* sender, akKeyEvent* event)
    {
        printf("(AnotherView)KeyPress '%d'\n", event->GetKeycode());
    }

    virtual void TextInput(akView* sender, akKeyEvent* event)
    {
        printf("(AnotherView)TextInput '%s'\n", event->GetText().c_str());
    }

    virtual void KeyRelease(akView* sender, akKeyEvent* event)
    {
        printf("(AnotherView)KeyRelease '%d'\n", event->GetKeycode());
    }

    virtual void MouseWheelUp(akView* sender, akMouseEvent* event)
    {
        printf("(AnotherView)MouseWheelUp\n");
    }

    virtual void MouseWheelDown(akView* sender, akMouseEvent* event)
    {
        printf("(AnotherView)MouseWheelDown\n");
    }
};

class Button1ActionReceiver : public akActionReceiver {
public:
    virtual void ActionPerformed(akControl* sender)
    {
        //akWindow *wnd = new akWindow(akRect(50,50,200,200), "Another window", akWS_VISIBLE|akWS_CLOSABLE);
        akMessageBox("Simle title", "Simple Message but very long", NULL);
    }
};

int main(int argc, char** argv)
{
    akApplication* app = new akApplication("Sample Application");
    akWindow* wnd = new akWindow(akRect(100, 100, 400, 400), "My Window",
        akWS_VISIBLE | akWS_CLOSABLE | akWS_MAXIMIZABLE | akWS_MINIMIZABLE);

    akView* view1 = new akView(akRect(150, 10, 70, 70));
    view1->AddPainter(new MyPainter());
    view1->AddMouseEventReceiver(new MyInputEventReceiver());
    view1->SetTag(1);
    wnd->AddView(view1);

    akView* view2 = new akView(akRect(250, 10, 70, 70));
    view2->AddPainter(new AnotherPainter());
    AnotherInputEventReceiver* view2receiver = new AnotherInputEventReceiver();
    view2->AddMouseEventReceiver(view2receiver);
    view2->AddKeyEventReceiver(view2receiver);
    view2->SetTag(2);
    wnd->AddView(view2);

    akPushButton* button1 = new akPushButton(akRect(30, 30, 80, 35), "Button 1");
    button1->AddActionReceiver(new Button1ActionReceiver());
    wnd->AddView(button1);

    wnd->AddView(new akLabel(akRect(10, 100, 150, 25), "Full Name:"));
    akTextBox* textbox1 = new akTextBox(akRect(75, 100, 200, 25), "");
    wnd->AddView(textbox1);

    wnd->AddView(new akLabel(akRect(10, 140, 150, 25), "E-mail:"));
    akTextBox* textbox2 = new akTextBox(akRect(75, 140, 200, 25), "");
    wnd->AddView(textbox2);

    akListBox* listbox1 = new akListBox(akRect(30, 180, 150, 160));
    listbox1->AddItem("Apple");
    listbox1->AddItem("Bananas");
    listbox1->AddItem("Cocoa");
    listbox1->AddItem("Apple");
    listbox1->AddItem("Bananas");
    listbox1->AddItem("Cocoa");
    listbox1->AddItem("Apple");
    listbox1->AddItem("Bananas");
    listbox1->AddItem("Cocoa");
    listbox1->AddItem("Apple");
    listbox1->AddItem("Bananas");
    listbox1->AddItem("Cocoa");
    listbox1->AddItem("Apple");
    listbox1->AddItem("Bananas");
    listbox1->AddItem("Cocoa");
    listbox1->AddItem("Apple");
    listbox1->AddItem("Bananas");
    listbox1->AddItem("Cocoa");
    wnd->AddView(listbox1);

    akRadioButton* radio1 = new akRadioButton(akRect(250, 150 + 30, 150, 25), "Radio 1");
    akRadioButton* radio2 = new akRadioButton(akRect(250, 150 + 60, 150, 25), "Radio 2");
    akRadioButton* radio3 = new akRadioButton(akRect(250, 170 + 90, 150, 25), "Radio 3");
    akRadioButton* radio4 = new akRadioButton(akRect(250, 170 + 120, 150, 25), "Radio 4");
    wnd->AddView(radio1);
    wnd->AddView(radio2);
    wnd->AddView(radio3);
    wnd->AddView(radio4);

    akRadioButtonGroup* group1 = new akRadioButtonGroup;
    group1->Add(radio1);
    group1->Add(radio2);

    akRadioButtonGroup* group2 = new akRadioButtonGroup;
    group2->Add(radio3);
    group2->Add(radio4);

    return app->Run();
}
