#include <AppKit/object.h>

akObject::akObject()
{
	mClassName = "akObject";
}

void akObject::SetClassName(string className)
{
	mClassName = className;
}

string akObject::GetClassName()
{
	return mClassName;
}