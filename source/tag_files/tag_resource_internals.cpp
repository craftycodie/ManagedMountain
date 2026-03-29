/* ---------- headers */

#include "tag_files/tag_resource_internals.h"

#include "common/logging.h"
#include "memory/module.h"

#include "cseries/cseries.h"

#include <cstdint>
#include <sstream>

/* ---------- constants */

/* ---------- prototypes */

bool __fastcall tag_resource_not_empty(const s_tag_resource* tag_resource);
bool __fastcall tag_resource_definition_optional(const s_tag_resource_definition* tag_resource_definition);
bool __fastcall tag_resource_definition_streamed(const s_tag_resource_definition* tag_resource_definition);
bool __fastcall tag_resource_definition_required(const s_tag_resource_definition* tag_resource_definition);
bool __fastcall tag_resource_definition_is_required_in_manifest(const s_tag_resource_definition* tag_resource_definition);

/* ---------- hooks - direct callees of sub_1408F5560 (IDA) */

HOOK_DECLARE(0x140801D70ull, tag_resource_not_empty);
HOOK_DECLARE(0x1407E78E0ull, tag_resource_definition_optional);
HOOK_DECLARE(0x1407E7AC0ull, tag_resource_definition_streamed);
HOOK_DECLARE(0x1407E79F0ull, tag_resource_definition_required);

/* ---------- definitions */

/* ---------- globals */

/* ---------- public code */

bool __fastcall tag_resource_not_empty(const s_tag_resource* tag_resource)
{
	// std::ostringstream stream;
	// stream << "tag_resource_not_empty tag_resource=" << tag_resource;
	// logging::Log(stream.str());

	bool result = false;
	HOOK_INVOKE(result, tag_resource_not_empty, tag_resource);
	return result;
}

bool __fastcall tag_resource_definition_optional(const s_tag_resource_definition* tag_resource_definition)
{
	// std::ostringstream stream;
	// stream << "tag_resource_definition_optional def=" << tag_resource_definition;
	// logging::Log(stream.str());

	bool result = false;
	HOOK_INVOKE(result, tag_resource_definition_optional, tag_resource_definition);
	return result;
}

bool __fastcall tag_resource_definition_streamed(const s_tag_resource_definition* tag_resource_definition)
{
	bool result = false;
	HOOK_INVOKE(result, tag_resource_definition_streamed, tag_resource_definition);
	return result;
}

bool __fastcall tag_resource_definition_required(const s_tag_resource_definition* tag_resource_definition)
{
	bool result = false;
	HOOK_INVOKE(result, tag_resource_definition_required, tag_resource_definition);
	return result;
}

bool __fastcall tag_resource_definition_is_required_in_manifest(const s_tag_resource_definition* tag_resource_definition)
{
	return tag_resource_definition_required(tag_resource_definition);
}

bool s_tag_resource_definition::optional(const s_tag_resource_definition* tag_resource_definition)
{
	return tag_resource_definition_optional(tag_resource_definition);
}

bool s_tag_resource_definition::streamed(const s_tag_resource_definition* tag_resource_definition)
{
	return tag_resource_definition_streamed(tag_resource_definition);
}

bool s_tag_resource_definition::required(const s_tag_resource_definition* tag_resource_definition)
{
	return tag_resource_definition_required(tag_resource_definition);
}

bool s_tag_resource_definition::is_required_in_manifest(const s_tag_resource_definition* tag_resource_definition)
{
	return tag_resource_definition_is_required_in_manifest(tag_resource_definition);
}

/* ---------- private code */

/* ---------- reverse engineering */
