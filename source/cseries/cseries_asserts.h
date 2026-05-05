#ifndef __CSERIES_ASSERTS_H__
#define __CSERIES_ASSERTS_H__
#pragma once

/* ---------- headers */

#include <cassert>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

/* ---------- constants */

enum e_event_level
{
	_event_verbose = 0,
	_event_status,
	_event_message,
	_event_warning,
	_event_error,
	_event_critical,
	k_event_level_count,
	k_event_level_none = -1,
};

/* ---------- definitions */

#define __CSERIES_ASSERT_WARNINGS_PUSH()
#define __CSERIES_ASSERT_WARNINGS_POP()

// Layout matches Ares `s_slim_assert_info` size (0x18); flat fields for MSVC v141 `dhalt` macro init.
struct s_slim_assert_info
{
	char const* expression;
	char const* file;
	unsigned line;
	unsigned char version;
	unsigned char level;
	unsigned char padding[2];
};
static_assert(sizeof(s_slim_assert_info) == 24);

#define ASSERT(expression) assert((expression))

#define dhalt(_diagnostic) \
	do \
	{ \
		__CSERIES_ASSERT_WARNINGS_PUSH(); \
		static s_slim_assert_info const __info = { \
			__FUNCTION__ " : halt", \
			__FILE__, \
			static_cast<unsigned>(__LINE__), \
			1, \
			static_cast<unsigned char>(_event_critical), \
			{0, 0}, \
		}; \
		__debugbreak(); \
		handle_slim_assert(&__info, _diagnostic); \
		__assume(0); \
		__CSERIES_ASSERT_WARNINGS_POP(); \
	} while (false)

/* ---------- prototypes */

void handle_slim_assert(s_slim_assert_info const* info);
void handle_slim_assert(s_slim_assert_info const* info, char const* diagnostic);

/* ---------- globals */

/* ---------- public code */

#endif // __CSERIES_ASSERTS_H__
