#ifndef __TAG_RESOURCE_INTERNALS_H__
#define __TAG_RESOURCE_INTERNALS_H__
#pragma once

/* ---------- headers */

#include <cstddef>

#include "cseries/port_64.h"
#include "cseries/types.h"
#include "tag_files/tag_group_definitions.h"

/* ---------- constants */

/* ---------- definitions */

struct s_tag_persistent_identifier
{
	unsigned long data[4];
};

struct s_tag_reference
{
	unsigned long opaque[4];
};
static_assert(sizeof(s_tag_reference) == 16);

struct c_tag_resource_fixup
{
	long raw_bits;
};
static_assert(sizeof(c_tag_resource_fixup) == 4);

struct s_tag_resource_fixup_location
{
	unsigned long address_offset;
	c_tag_resource_fixup fixup;
};
static_assert(sizeof(s_tag_resource_fixup_location) == 8);

struct s_tag_resource_interop_location
{
	c_tag_resource_fixup interop_fixup;
	long interop_type_index;
};
static_assert(sizeof(s_tag_resource_interop_location) == 8);

struct s_tag_struct_definition;
struct s_tag_field;

enum e_tag_resource_definition_bit
{
	_tag_resource_is_not_cached_bit = 0,
	_tag_resource_data_is_streamed_bit,
	_tag_resource_not_required_immediately_bit,
	_tag_resource_game_bit,
	k_number_of_tag_resource_flags,
};

struct s_tag_resource_definition
{
	const char* resource_name;
	unsigned long flags;
	s_tag_struct_definition* resource_struct_definition;
	c_tag_resource_vtable_interface* resource_vtable;
	unsigned int unknown20;
	const char* file_path;
	int line_number;
	uns16 pagable_alignment_bits;
  uns16 optional_alignment_bits;

	void validate_vtable(void);
	bool streamed(void) const;
	bool required(void) const;
	bool optional(void) const;
	bool not_required_immediately(void) const;
	void const* get_type_key(void) const;
	c_tag_struct_vtable_interface const* get_struct_vtable(void) const;
	unsigned long get_runtime_flags(void) const;
	long get_resource_size(void) const;
	char const* get_resource_name(void) const;
	char const* get_pretty_name(void) const;
	s_tag_persistent_identifier const& get_persistent_identifier(void) const;
	s_tag_field const* get_fields(void) const;
	char const* get_elements_name(void) const;
	long get_cache_file_resource_size(void) const;
	long get_alignment_bits(void) const;
};


struct s_tag_resource
{
	int resource_handle;
	ptr32_t<s_tag_resource_definition> definition_ptr;
};
static_assert(sizeof(s_tag_resource) == 8);

/* ---------- prototypes */

bool __fastcall tag_resource_not_empty(const s_tag_resource* tag_resource);
bool __fastcall tag_resource_definition_is_required_in_manifest(const s_tag_resource_definition* tag_resource_definition);

/* ---------- globals */

#endif // __TAG_RESOURCE_INTERNALS_H__
