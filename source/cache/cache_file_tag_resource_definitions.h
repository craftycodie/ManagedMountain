#ifndef __CACHE_FILE_TAG_RESOURCE_DEFINITIONS_H__
#define __CACHE_FILE_TAG_RESOURCE_DEFINITIONS_H__
#pragma once

/* ---------- headers */

#include "memory/bitvector.h"
#include "memory/wrapped_arrays.h"

/* ---------- constants */

/* ---------- definitions */

class c_tag_block_bit_vector :
	public c_bit_vector_operator_interface<c_tag_block_bit_vector>
{
	friend class c_bit_vector_operator_interface<c_tag_block_bit_vector>;

    public:
	bool resize_in_bits(unsigned long);
	_inline long bit_count() const;
private:
	_inline unsigned long const* get_bit_storage() const;
	unsigned long* get_bit_storage();
	c_typed_tag_block<unsigned long> m_bits; // 0x0
};
static_assert(sizeof(c_tag_block_bit_vector) == 12);

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

#endif // __CACHE_FILE_TAG_RESOURCE_DEFINITIONS_H__