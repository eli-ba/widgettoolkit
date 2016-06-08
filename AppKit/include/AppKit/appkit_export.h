#ifndef _APPKIT_EXPORT_H
#define _APPKIT_EXPORT_H

#ifdef _WIN32
#define APPKIT_API __declspec( dllexport )
#else
#define APPKIT_API __attribute__((visibility("default")))
#endif

#endif
