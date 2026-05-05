/* ---------- headers */

#include "tag_files/tag_resource_internals.h"

#include "common/logging.h"
#include "memory/module.h"

#include "cseries/cseries.h"
#include "cseries/cseries_asserts.h"
#include "cseries/cseries_macros.h"

#include <cstdio>
#include <sstream>

/* ---------- constants */

/* ---------- prototypes */

bool __fastcall tag_resource_not_empty(const s_tag_resource* tag_resource);
bool __fastcall tag_resource_definition_is_required_in_manifest(const s_tag_resource_definition* tag_resource_definition);

/* ---------- hooks - direct callees of sub_1408F5560 (IDA) */

HOOK_DECLARE(0x140801D70ull, tag_resource_not_empty);

/* ---------- definitions */

/* ---------- globals */

/* ---------- public code */

void s_tag_resource_definition::validate_vtable(void)
{
	c_tag_resource_vtable_interface* const vtable = resource_vtable;
	if (vtable != nullptr)
	{
		if (vtable->has_resource_report_proc() != vtable->has_resource_report_categories_proc())
		{
			dhalt("either implement the whole reporting proc interface or none of it");
		}
	}
	
	if (vtable != nullptr
		&& vtable->has_get_resource_streaming_sublocations_proc()
		&& !streamed())
	{
		char buf[512]{};
		char const* const path = file_path != nullptr ? file_path : "";
		std::snprintf(
			buf,
			sizeof(buf),
			"%s(%d): non streaming resources will not be queried for resource streaming sublocations!",
			path,
			line_number);
		logging::Log(std::string_view(buf));
	}
}

bool s_tag_resource_definition::streamed(void) const
{
	ASSERT(resource_struct_definition != nullptr);
	return TEST_BIT(flags, _tag_resource_data_is_streamed_bit);
}

bool s_tag_resource_definition::optional(void) const
{
	ASSERT(resource_struct_definition != nullptr);
	return false;
}

bool s_tag_resource_definition::not_required_immediately(void) const
{
	ASSERT(resource_struct_definition != nullptr);
	if (!TEST_BIT(flags, _tag_resource_not_required_immediately_bit))
		return false;
	if (TEST_BIT(flags, _tag_resource_data_is_streamed_bit))
		return false;
	return true;
}

bool s_tag_resource_definition::required(void) const
{
	ASSERT(resource_struct_definition != nullptr);
	return !TEST_BIT(flags, _tag_resource_data_is_streamed_bit);
}

bool __fastcall tag_resource_not_empty(const s_tag_resource* tag_resource)
{
	// std::ostringstream stream;
	// stream << "tag_resource_not_empty tag_resource=" << tag_resource;
	// logging::Log(stream.str());

	bool result = false;
	HOOK_INVOKE(result, tag_resource_not_empty, tag_resource);
	return result;
}

bool __fastcall tag_resource_definition_is_required_in_manifest(const s_tag_resource_definition* tag_resource_definition)
{
	return tag_resource_definition->required();
}

/* ---------- private code */

/* ---------- reverse engineering */
