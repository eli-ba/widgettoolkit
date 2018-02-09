#pragma once

#include <widgettoolkit/export.h>
#include <widgettoolkit/geometry.h>
#include <widgettoolkit/inputevent.h>
#include <widgettoolkit/object.h>
#include <string>

using namespace std;

class APPKIT_API akApplication : public akObject {
public:
    akApplication(const string& name);
    akApplication(akSize size, const string& name);
    int Run();

private:
    void DispatchEvent(akInputEvent* evt);
};
