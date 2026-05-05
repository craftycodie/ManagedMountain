/* ---------- headers */

#include "cache/tag_resource_streaming_output_stream.h"
#include "cache/physical_memory_map.h"

#include "cseries/cseries_asserts.h"
#include "cseries/cseries_macros.h"

/* ---------- definitions */

c_output_stream<s_tag_resource_streaming_sublocation>::c_output_stream(s_tag_block* const tag_block)
	: m_tag_block(tag_block)
{
	ASSERT(m_tag_block != nullptr);
}

c_output_stream<s_tag_resource_streaming_sublocation>::c_output_stream(s_tag_block* const tag_block, int)
	: m_tag_block(tag_block)
{}

bool c_output_stream<s_tag_resource_streaming_sublocation>::streaming_output_capacity_allows_adding(int const element_count) const
{
	s_tag_block const* const block = m_tag_block;
	int const definition_index = block->definition;
	char const* definition_entry = nullptr;
	if (definition_index != 0)
	{
		if (definition_index == -1)
		{
			ASSERT(false);
			return false;
		}
		definition_entry = reinterpret_cast<char const*>(virtual_to_physical_base_offset() + static_cast<uns64>(definition_index) * 4ull);
	}

	int const current_count = block->count;
	if (definition_entry == nullptr)
	{
		return true;
	}

	int const max_count = *reinterpret_cast<int const*>(definition_entry + 4);
	return (current_count + element_count) <= max_count;
}

void* c_output_stream<s_tag_resource_streaming_sublocation>::streaming_output_append_row_from_qword(__int64 const* const row_bits)
{
	s_tag_block* const block = m_tag_block;
	__int64 const row_value = *row_bits;
	int const new_index = tag_block_add_element(block);
	int const count = block->count;
	ASSERT(VALID_INDEX(new_index, count));

	void* const range = tag_block_get_range_with_size(block, 0, count, 8);
	reinterpret_cast<__int64*>(range)[static_cast<size_t>(static_cast<unsigned>(new_index))] = row_value;
	return range;
}

c_null_output_stream<s_tag_resource_streaming_sublocation>::c_null_output_stream()
	: c_output_stream<s_tag_resource_streaming_sublocation>(nullptr, 0)
{}

bool c_null_output_stream<s_tag_resource_streaming_sublocation>::streaming_output_capacity_allows_adding(int const element_count) const
{
	(void)element_count;
	return true;
}

void* c_null_output_stream<s_tag_resource_streaming_sublocation>::streaming_output_append_row_from_qword(__int64 const* const row_bits)
{
	(void)row_bits;
	return nullptr;
}
