/* ---------- headers */

#include "tag_files/tag_groups.h"

#include "common/logging.h"
#include "memory/module.h"

#include "cseries/cseries.h"

#include <sstream>

/* ---------- constants */

/* ---------- prototypes (before HOOK_DECLARE) */

struct s_data_iterator;

bool __fastcall tag_block_resize(s_tag_block* tag_block, int count);
void* __fastcall tag_block_get_range_with_size(s_tag_block* tag_block, int index, int count, int element_size);
int __fastcall tag_block_add_element(s_tag_block* tag_block);
void __fastcall data_iterator_begin_sub_1407BC980(s_data_iterator* iterator, void* unk);
int __fastcall data_iterator_next_sub_1407BC9C0(s_data_iterator* iterator);

/* ---------- hooks — direct callees of sub_1408F5560 (IDA) */

HOOK_DECLARE(0x1407B8C00ull, tag_block_resize);
HOOK_DECLARE(0x1407B8370ull, tag_block_get_range_with_size);
HOOK_DECLARE(0x1407B7970ull, tag_block_add_element);
HOOK_DECLARE(0x1407BC980ull, data_iterator_begin_sub_1407BC980);
HOOK_DECLARE(0x1407BC9C0ull, data_iterator_next_sub_1407BC9C0);

/* ---------- definitions */

/* ---------- globals */

/* ---------- public code */

bool __fastcall tag_block_resize(s_tag_block* tag_block, int count)
{
	// std::ostringstream stream;
	// stream << "tag_block_resize tag_block=" << tag_block << " count=" << count;
	// logging::Log(stream.str());

	bool result = false;
	HOOK_INVOKE(result, tag_block_resize, tag_block, count);
	return result;
}

void* __fastcall tag_block_get_range_with_size(s_tag_block* tag_block, int index, int count, int element_size)
{
	// std::ostringstream stream;
	// stream << "tag_block_get_range_with_size tag_block=" << tag_block << " index=" << index
	// 	   << " count=" << count << " element_size=" << element_size;
	// logging::Log(stream.str());

	void* result = nullptr;
	HOOK_INVOKE(result, tag_block_get_range_with_size, tag_block, index, count, element_size);
	return result;
}

int __fastcall tag_block_add_element(s_tag_block* tag_block)
{
	// std::ostringstream stream;
	// stream << "tag_block_add_element tag_block=" << tag_block;
	// logging::Log(stream.str());

	int result = 0;
	HOOK_INVOKE(result, tag_block_add_element, tag_block);
	return result;
}

// maybe a tag resource data iterator?
void __fastcall data_iterator_begin_sub_1407BC980(s_data_iterator* iterator, void* unk)
{
	// std::ostringstream stream;
	// stream << "data_iterator_begin_sub_1407BC980 iterator=" << iterator << " unk=" << unk;
	// logging::Log(stream.str());

	HOOK_INVOKE_VOID(data_iterator_begin_sub_1407BC980, iterator, unk);
}

int __fastcall data_iterator_next_sub_1407BC9C0(s_data_iterator* iterator)
{
	// std::ostringstream stream;
	// stream << "data_iterator_next_sub_1407BC9C0 iterator=" << iterator;
	// logging::Log(stream.str());

	int result = 0;
	HOOK_INVOKE(result, data_iterator_next_sub_1407BC9C0, iterator);
	return result;
}

/* ---------- private code */

/* ---------- reverse engineering */
