/* ---------- headers */

#include "cseries/cseries_asserts.h"

#include "common/logging.h"

#include <cstdio>
#include <string>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

/* ---------- constants */

/* ---------- definitions */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

void handle_slim_assert(s_slim_assert_info const* info)
{
	handle_slim_assert(info, nullptr);
}

void handle_slim_assert(s_slim_assert_info const* info, char const* diagnostic)
{
	assert(info != nullptr);

	std::string msg;
	msg.reserve(256);
	msg.append(info->expression != nullptr ? info->expression : "(null)");
	if (diagnostic != nullptr && diagnostic[0] != '\0')
	{
		msg.append(" — ");
		msg.append(diagnostic);
	}
	msg.append(" @ ");
	msg.append(info->file != nullptr ? info->file : "?");
	msg.push_back(':');
	char linebuf[32]{};
	std::snprintf(linebuf, sizeof(linebuf), "%u", static_cast<unsigned int>(info->line));
	msg.append(linebuf);
	logging::Log(std::string_view(msg));
}

/* ---------- private code */
