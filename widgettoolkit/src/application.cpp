#include <widgettoolkit/application.h>
#include <widgettoolkit/windowmanager.h>

WindowManager* TheWindowManager = NULL;

akApplication::akApplication(const string& name)
{
    SetClassName("akApplication");
    TheWindowManager = new WindowManager(akSize(800, 600), name);
}

akApplication::akApplication(akSize size, const string& name)
{
    SetClassName("akApplication");
    TheWindowManager = new WindowManager(size, name);
}

int akApplication::Run()
{
    bool run = true;

    do {
        TheWindowManager->ProcessEvents();
    } while (run);

    return 0;
}
