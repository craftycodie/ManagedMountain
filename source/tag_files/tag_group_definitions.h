#ifndef __TAG_GROUP_DEFINITIONS_H__
#define __TAG_GROUP_DEFINITIONS_H__
#pragma once

/* ---------- headers */

#include "memory/wrapped_arrays.h"

/* ---------- definitions */

struct s_tag_struct_definition;
struct s_cache_file_builder_stream_context;
class c_tag_allocator;

enum class e_runtime_platform_type : int;
class c_cache_file_scratch_allocator;
struct s_string_list_definition;
class c_tag_resource_reporter;
struct s_tag_resource_streaming_sublocation;

template<typename t_element>
class c_output_stream;

#include "cache/tag_resource_streaming_output_stream.h"

class c_tag_resource_vtable_interface
{
public:
	virtual bool has_on_load_resource_proc(void) const;
	virtual bool on_load_resource_thunk(long, void*) const;
	virtual bool has_on_dispose_resource_proc(void) const;
	virtual void on_dispose_resource_thunk(long, void*) const;
	virtual bool has_resource_is_locked_proc(void) const;
	virtual bool resource_is_locked_thunk(void*) const;
	virtual bool has_write_resource_to_cache_file_proc(void) const;
	virtual bool write_resource_to_cache_file_thunk(long, void*, e_runtime_platform_type, c_cache_file_scratch_allocator*) const;
	virtual bool has_get_xsync_version_proc(void) const;
	virtual unsigned long get_xsync_version_thunk(void) const;
	virtual bool has_resource_report_categories_proc(void) const;
	virtual s_string_list_definition const* get_resource_report_categories_thunk(void) const;
	virtual bool has_resource_report_proc(void) const;
	virtual void resource_report_thunk(long, long, void const*, c_tag_resource_reporter*) const;
	virtual bool has_get_resource_streaming_sublocations_proc(void) const;
	virtual void get_resource_streaming_sublocations_thunk(
		long,
		long,
		c_output_stream<s_tag_resource_streaming_sublocation>*,
		c_output_stream<s_tag_resource_streaming_sublocation>*) const;
};

class c_tag_struct_vtable_interface
{
public:
	virtual bool has_byte_swap_proc(void) const;
	virtual void byte_swap_block_proc(void*) const;
	virtual bool has_postprocess_proc(void) const;
	virtual bool postprocess_thunk(long, void*, bool) const;
	virtual bool has_format_proc(void) const;
	virtual char* format_thunk(long, __int64, s_tag_block*, long, char*, long) const;
	virtual bool has_on_delete_proc(void) const;
	virtual void on_delete_thunk(void*) const;
	virtual bool has_on_new_proc(void) const;
	virtual bool on_new_thunk(void*) const;
	virtual bool has_on_copy_proc(void) const;
	virtual bool on_copy_thunk(void const*, void*) const;
	virtual bool has_write_to_cache_file_proc(void) const;
	virtual void write_to_cache_file_thunk(long, s_tag_block*, s_cache_file_builder_stream_context const*, void**, void**) const;
	virtual bool has_block_does_not_exist_in_cache_file_proc(void) const;
	virtual bool block_does_not_exist_in_cache_file_thunk(void) const;
	virtual bool has_fixup_old_struct_definition(void) const;
	virtual s_tag_struct_definition* get_fixup_source_definition_thunk(void) const;
	virtual bool has_fixup_old_struct_proc(void) const;
	virtual bool fixup_old_struct_thunk(
		c_basic_buffer<void>&,
		s_tag_struct_definition const*,
		c_basic_buffer<void>&,
		s_tag_struct_definition const*,
		c_tag_allocator*,
		bool) const;
	virtual bool has_fixup_old_struct_from_disk_proc(void) const;
	virtual bool fixup_old_struct_from_disk_thunk(
		s_tag_struct_definition const*,
		c_basic_buffer<void>&,
		s_tag_struct_definition const*,
		c_basic_buffer<void>&,
		s_tag_struct_definition const*,
		c_tag_allocator*,
		bool) const;
};

#endif // __TAG_GROUP_DEFINITIONS_H__
