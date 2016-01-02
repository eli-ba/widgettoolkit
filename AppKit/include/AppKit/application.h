#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <AppKit/appkit_export.h>
#include <AppKit/geometry.h>
#include <AppKit/inputevent.h>
#include <AppKit/object.h>
#include <string>

using namespace std;

class APPKIT_API akApplication : public akObject
{
public:
	akApplication(const string &name);
	akApplication(akSize size, const string &name);
	int Run();
private:
	void DispatchEvent(akInputEvent *evt);
};

#endif