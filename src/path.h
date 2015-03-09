#ifndef ASTEROIDS_PATH_H
#define ASTEROIDS_PATH_H

#include <string.h>

#include "types.h"
#include "debug.h"
#include "log.h"

bool path_concat(const char* p1, const char* p2, char* dest, u32 maxLength);

#endif