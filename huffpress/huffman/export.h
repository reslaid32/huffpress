#ifndef HUFFMAN_EXPORT_H
#define HUFFMAN_EXPORT_H

#if defined(_WIN32) || defined(_WIN64)
#ifdef HUFFMAN_LIBRARY_BUILD
#define HUFFMAN_API __declspec(dllexport)
#else
#define HUFFMAN_API __declspec(dllimport)
#endif
#else
#ifdef HUFFMAN_LIBRARY_BUILD
#define HUFFMAN_API __attribute__((visibility("default")))
#else
#define HUFFMAN_API
#endif
#endif

#endif // HUFFMAN_EXPORT_H