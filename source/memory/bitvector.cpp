/* ---------- headers */

#include "memory/bitvector.h"

#include "cseries/cseries.h"
#include "memory/module.h"

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- hooks*/

HOOK_DECLARE(0x1408BFB00ull, bit_vector_not_and);

/* ---------- globals */

/* ---------- public code */

void bit_vector_not_and(uns32 bit_count, uns32 const* mask_bits, uns32 const* acc_in, uns32* acc_out)
{
	HOOK_INVOKE_VOID(bit_vector_not_and, bit_count, mask_bits, acc_in, acc_out);
}

/* ---------- private code */

/* ---------- reverse engineering */
