#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <AppKit/appkit_export.h>
#include <string>

using namespace std;

class APPKIT_API akObject
{
public:
	akObject();
	void SetClassName(string className);
	string GetClassName();
private:
	string mClassName;
};

#endif
