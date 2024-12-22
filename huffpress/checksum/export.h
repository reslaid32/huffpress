#ifndef CHECKSUM_EXPORT_H
#define CHECKSUM_EXPORT_H

#if defined(_WIN32) || defined(_WIN64)
#ifdef CHECKSUM_LIBRARY_BUILD
#define CHECKSUM_API __declspec(dllexport)
#else
#define CHECKSUM_API  __declspec(dllimport)
#endif
#else
#ifdef CHECKSUM_LIBRARY_BUILD
#define CHECKSUM_API __attribute__((visibility("default")))
#else
#define CHECKSUM_API
#endif
#endif

#endif // CHECKSUM_EXPORT_H