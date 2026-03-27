#ifndef __TAG_GROUPS_H__
#define __TAG_GROUPS_H__
#pragma once

/* ---------- headers */

#include <cstdint>

/* ---------- constants */

/* ---------- definitions */

struct s_data_iterator;
struct s_tag_block;

/* ---------- prototypes */

bool __fastcall tag_block_resize(s_tag_block* tag_block, int count);
void* __fastcall tag_block_get_range_with_size(int* tag_block, int index, int count, int element_size);
int __fastcall tag_block_add_element(s_tag_block* tag_block);
void __fastcall data_iterator_begin_sub_1407BC980(s_data_iterator* a1, void* unk1);
int __fastcall data_iterator_next_sub_1407BC9C0(s_data_iterator* iter);

/* ---------- globals */

#endif // __TAG_GROUPS_H__