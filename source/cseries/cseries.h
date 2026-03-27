#pragma once

#include "cseries/types.h"

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

inline int csnzprintf(char* buffer, int buffer_size, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	const int result = std::vsnprintf(buffer, static_cast<size_t>(buffer_size), format, args);
	va_end(args);
	return result;
}

template<int32 k_maximum_size>
class c_static_string
{
public:
	explicit c_static_string(const char* string)
	{
		if (string == nullptr)
		{
			m_string[0] = '\0';
			return;
		}

		std::strncpy(m_string, string, k_maximum_size - 1);
		m_string[k_maximum_size - 1] = '\0';
	}

	const char* get_string() const
	{
		return m_string;
	}

private:
	char m_string[k_maximum_size]{};
};
