/**
 * Copyright(C) Stefano Moioli <smxdev4.gmail.com>
 */
#ifndef __PRELOADER_H
#define __PRELOADER_H

#include "config.h"

#define ENTRY_FUNC  __attribute__((__section__(".entry")))

typedef unsigned char uint8_t;
typedef unsigned long uint32_t;
typedef uint32_t word_t;

#endif