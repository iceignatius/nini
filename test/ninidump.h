/*
 * Dump NINI node data for debug purpose.
 */
#ifndef _NINIDUMP_H_
#define _NINIDUMP_H_

#include "nini_root.h"

#ifdef __cplusplus
extern "C" {
#endif

void ninidump(const nini_root_t *root, char delimiter, const char *filename);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
