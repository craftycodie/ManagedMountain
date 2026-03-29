/* ---------- headers */

#include "memory/array.h"

#include "common/logging.h"
#include "memory/module.h"

#include "cseries/cseries.h"

#include <sstream>

/* ---------- constants */

/* ---------- prototypes */

void* __fastcall dynamic_array_get_element(dynamic_array* array, int index, int element_size);

/* ---------- hooks — direct callee of sub_1408F5560 (IDA 0x1407A7FA0) */

HOOK_DECLARE(0x1407A7FA0ull, dynamic_array_get_element);

/* ---------- definitions */

/* ---------- globals */

/* ---------- public code */

void* __fastcall dynamic_array_get_element(dynamic_array* array, int index, int element_size)
{
	// std::ostringstream stream;
	// stream << "dynamic_array_get_element array=" << array << " index=" << index
	// 	   << " element_size=" << element_size;
	// logging::Log(stream.str());

	void* result = nullptr;
	HOOK_INVOKE(result, dynamic_array_get_element, array, index, element_size);
	return result;
}

/* ---------- private code */

/* ---------- reverse engineering */
