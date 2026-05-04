#pragma once

#include "cseries/types.h"
#include "cseries/constants.h"

#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstring>

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define NUMBEROF(array_) (sizeof(array_) / sizeof((array_)[0]))
#define VALID_COUNT(index_, maximum_) ((index_) < (maximum_))

#define csmemcpy std::memcpy
#define csmemset std::memset

inline void csstrnzcpy(char* const dest, char const* const src, uns64 const dest_size_bytes)
{
	if (dest_size_bytes == 0)
	{
		return;
	}
	if (src == nullptr)
	{
		dest[0] = '\0';
		return;
	}
	std::strncpy(dest, src, dest_size_bytes - 1);
	dest[dest_size_bytes - 1] = '\0';
}

inline int csnzprintf(char* buffer, int buffer_size, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	const int result = std::vsnprintf(buffer, static_cast<uns64>(buffer_size), format, args);
	va_end(args);
	return result;
}

#include "memory/static_arrays.h"
