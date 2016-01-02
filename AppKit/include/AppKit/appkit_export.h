#ifndef _APPKIT_EXPORT_H
#define _APPKIT_EXPORT_H

#ifdef APPKIT_EXPORTS
#define APPKIT_API __declspec(dllexport)
#else
#define APPKIT_API __declspec(dllimport)
#endif

#endif