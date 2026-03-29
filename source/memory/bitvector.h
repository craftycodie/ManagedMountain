#ifndef __BITVECTOR_H__
#define __BITVECTOR_H__
#pragma once

/* ---------- headers */

#include <cstdint>
#include "cseries/types.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

void bitvector_set_bit(uns32* words, uns32 index);
void bitvector_clear_bit(uns32* words, uns32 index);

/* ---------- globals */

/* ---------- public code */

#endif // __BITVECTOR_H__
