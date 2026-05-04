#ifndef __TAG_GROUPS_H__
#define __TAG_GROUPS_H__
#pragma once

/* ---------- headers */

#include "cseries/types.h"

/* ---------- constants */

/* ---------- definitions */

struct s_data_iterator
{
	uns64 opaque[12];
};

struct s_tag_block
{
	int count;
	int address;
	int definition;
};

struct s_tag_data_definition;

struct s_tag_data
{
	int size;
	int internal_flags;
	int file_offset;
	int address;
	int definition;
};
static_assert(sizeof(s_tag_data) == 20);

/* ---------- prototypes */

bool __fastcall tag_block_resize(s_tag_block* tag_block, int count);
void* __fastcall tag_block_get_range_with_size(s_tag_block* tag_block, int index, int count, int element_size);
int __fastcall tag_block_add_element(s_tag_block* tag_block);
void __fastcall data_iterator_begin_sub_1407BC980(s_data_iterator* a1, void* unk1);
int __fastcall data_iterator_next_sub_1407BC9C0(s_data_iterator* iter);

/* ---------- globals */

#endif // __TAG_GROUPS_H__