/* ---------- headers */

#include "memory/bitvector.h"

#include "cseries/cseries.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

void bitvector_set_bit(uns32* words, uns32 index)
{
	// 32 bits in a word
	// Shifting going on here for efficiency
	// 5 = (32 / 8) + 1
	// 31 = 32 - 1
	words[index >> 5] |= (1u << (index & 31));
}

void bitvector_clear_bit(uns32* words, uns32 index)
{
	words[index >> 5] &= ~(1u << (index & 31));
}

/* ---------- private code */

/* ---------- reverse engineering */
