#ifndef HUFFPRESS_EXPORT_H
#define HUFFPRESS_EXPORT_H

#if defined(_WIN32) || defined(_WIN64)
#ifdef HUFFPRESS_LIBRARY_BUILD
#define HUFFPRESS_API __declspec(dllexport)
#else
#define HUFFPRESS_API __declspec(dllimport)
#endif
#else
#ifdef HUFFPRESS_LIBRARY_BUILD
#define HUFFPRESS_API  __attribute__((visibility("default")))
#else
#define HUFFPRESS_API
#endif
#endif

#endif // HUFFPRESS_EXPORT_H