/*******************************************************************************
 mp4_fragment.h - A library for reading and writing Fragmented MPEG4.

 Copyright (C) 2009 CodeShop B.V.
 http://www.code-shop.com

 For licensing see the LICENSE file
******************************************************************************/ 

#ifndef MP4_FRAGMENT_H_AKW
#define MP4_FRAGMENT_H_AKW

#include "mod_streaming_export.h"

#ifndef _MSC_VER
#include <inttypes.h>
#else
#include "inttypes.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct mp4_context_t;
struct bucket_t;

// Fragment a complete file

MOD_STREAMING_DLL_LOCAL extern
int mp4_fragment_file(struct mp4_context_t const* mp4_context,
                      struct bucket_t** buckets);


#ifdef __cplusplus
} /* extern C definitions */
#endif

#endif // MP4_FRAGMENT_H_AKW

// End Of File

