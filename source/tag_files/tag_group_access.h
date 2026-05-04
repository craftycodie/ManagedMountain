#ifndef __TAG_GROUP_ACCESS_H__
#define __TAG_GROUP_ACCESS_H__
#pragma once

#include "cseries/types.h"

// H3 EK tool.exe: tag_get @ 0x1407BA800 (IDA)
constexpr uns64 k_tag_get_address = 0x1407BA800ull;

void* __fastcall tag_get(void* key, uns32 index);

#define TAG_GET(GROUP, TYPE, INDEX) \
	(reinterpret_cast<TYPE*>(tag_get( \
		reinterpret_cast<void*>(static_cast<uns64>(static_cast<uns32>(GROUP))), \
		(INDEX))))

#endif // __TAG_GROUP_ACCESS_H__
