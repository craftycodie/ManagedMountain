#ifndef __PHYSICAL_MEMORY_MAP_H__
#define __PHYSICAL_MEMORY_MAP_H__
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "memory/module.h"

/* ---------- definitions */

// This is a bit of a guess, I'm not 100% sure what this address is.
// If it is k_virtual_to_physical_base_offset it's behaviour differs to other executables.
// In H3EK tool I believe its never set and stays zero-initialised.
inline constexpr uns32 k_virtual_to_physical_base_offset_rva =
	static_cast<uns32>(0x15877A5D0ull - 0x140000000ull);

inline uns64& virtual_to_physical_base_offset()
{
	return *reinterpret_cast<uns64*>(global_address_get(k_virtual_to_physical_base_offset_rva));
}

#endif /* __PHYSICAL_MEMORY_MAP_H__ */
