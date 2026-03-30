#ifndef __BITVECTOR_H__
#define __BITVECTOR_H__
#pragma once

/* ---------- headers */

#include <cstdint>
#include "cseries/types.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

// IDA: ?bit_vector_not_and@@YAXJPEBK0PEAK@Z - per word: acc_out[i] = acc_in[i] & ~mask_bits[i].
// bit_count is a multiple of 32 in all H3EK call sites (32 * tag_block.count).
void bit_vector_not_and(uns32 bit_count, uns32 const* mask_bits, uns32 const* acc_in, uns32* acc_out);

/* ---------- globals */

/* ---------- public code */

#endif // __BITVECTOR_H__
