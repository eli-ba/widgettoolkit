#pragma once

#ifdef _WIN32
#define APPKIT_API __declspec(dllexport)
#else
#define APPKIT_API __attribute__((visibility("default")))
#endif
