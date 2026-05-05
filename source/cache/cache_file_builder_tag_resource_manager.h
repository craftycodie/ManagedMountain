#ifndef __CACHE_FILE_BUILDER_TAG_RESOURCE_MANAGER_H__
#define __CACHE_FILE_BUILDER_TAG_RESOURCE_MANAGER_H__
#pragma once

/* ---------- headers */

#include <cstddef>
#include <map>

#include "cache/cache_file_builder_writer.h"
#include "cache/cache_file_uncompressed_codec.h"
#include "cseries/types.h"
#include "tag_files/tag_groups.h"

/* ---------- constants */

constexpr uns64 k_build_cache_file_add_tag_resources_address = 0x1408EA960ull;
constexpr uns64 k_sub_1408F5560_address = 0x1408F5560ull;
constexpr uns64 k_sub_1408F7750_address = 0x1408F7750ull;
constexpr uns64 k_add_or_get_file_location_address = 0x1408F3350ull;
constexpr uns64 k_get_or_create_shared_file_index_address = 0x1408F3610ull;
constexpr uns64 k_get_or_create_codec_definition_index_address = 0x1408F2E30ull;
constexpr uns64 k_build_resource_streaming_sublocation_table_address = 0x1408F7B30ull;
constexpr uns64 k_build_and_assign_pageable_resource_location_address = 0x1408F78F0ull;

constexpr int k_cache_file_tag_zone_manifest_element_bytes = 120;

/* ---------- definitions */

namespace i343
{
struct HaloMapId;
}

struct c_cache_file_global_tag_registry;
struct c_cache_file_builder_tag_resource_manager;
struct c_cache_file_builder_tag_resource_runtime_allocator;
struct c_cache_file_builder_per_bsp_game_resource_collection;
struct c_cache_file_resource_library;
struct c_cache_file_tracer;
struct c_cache_file_builder_tag_resource_output;
struct c_allocation_base;
struct dynamic_array;
struct s_cache_file_zone_manifest;
class c_wrapped_flags;

struct s_cache_file_zone_resource_visit_node;
struct s_cache_file_zone_resource_visit_node_block_struct;
struct s_cache_file_zone_resource_visit_node_link_block;

struct s_cache_file_resource_gestalt;
struct s_cache_file_resource_file_location;
struct s_cache_file_resource_checksum;
struct s_cache_file_tag_resource_data;
struct s_tag_resource_definition;
struct c_tag_file_backend_resource_cache_file_datum_handler_simple_vtbl;

struct alignas(8) s_tag_resource_cache_file_config
{
	char path[256];
	uns32 io_offset;
};
static_assert(sizeof(s_tag_resource_cache_file_config) == 264);

struct alignas(8) s_indirect_file
{
	int32 file_index;
};
static_assert(sizeof(s_indirect_file) == 8);

struct alignas(8) s_tag_resource_cache_file_location
{
	s_tag_resource_cache_file_config io_config;
	
	s_indirect_file indirect_file;
	int32 file_offset;
	int32 location_size;
	int32 unknown;
	c_cache_file_builder_codec* codec;
};
static_assert(sizeof(s_tag_resource_cache_file_location) == 296);

struct s_unknown_struct_shared_file_cache
{
	std::map<uns32, int32> m_shared_file_index_cache;
	int m_zone_tag_index;
	c_cache_file_builder_writer_default* m_writer;
};

struct c_tag_file_backend_resource_cache_file_datum_handler_simple
{
	c_tag_file_backend_resource_cache_file_datum_handler_simple_vtbl const* vftable;
};

struct c_tag_file_backend_resource_cache_file_datum_handler_simple_vtbl
{
	// Full layout from `.rdata:8231E7F8` vftable for
	// `c_tag_file_backend_resource_cache_file_datum_handler_simple`.
	void* (*get_control_data)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index, bool); // +0x00
	void* (*get_root_address_fixup)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0x08
	bool (*is_streamed)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0x10
	void* (*get_permanent_resource_root_address)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index, bool); // +0x18
	s_tag_resource_definition const* (*get_resource_definition)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0x20
	long (*get_resource_owner)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0x28
	void* (*get_control_fixups)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0x30
	long (*get_control_data_alignment)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0x38
	bool (*has_valid_interop_locations)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0x40
	bool (*describe_resource_cache_location)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index, int section, void* out_location); // +0x48
	void* (*get_interop_buffer)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index, bool); // +0x50
	void* (*get_interop_locations)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0x58
	bool (*describe_resource)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index, void* out_chars); // +0x60
	bool (*any_pageable_data)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0x68
	bool (*any_optional_data)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0x70
	void* (*get_pageable_location_handle)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0x78
	void* (*get_optional_location_handle)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0x80
	unsigned long (*get_pageable_location_offset)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0x88
	unsigned long (*get_optional_location_offset)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0x90
	bool (*get_resource_location)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, unsigned __int64 location_handle, void* out_location); // +0x98
	void* (*get_decompressor_service)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, unsigned __int64 location_handle, long resource_index); // +0xA0
	bool (*get_pageable_sub_location)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index, void* out_sub_location); // +0xA8
	unsigned long (*get_pageable_memory_size)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0xB0
	bool (*try_to_get_pageable_checksum)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index, s_cache_file_resource_checksum* out_checksum); // +0xB8
	bool (*get_optional_sub_location)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index, void* out_sub_location); // +0xC0
	unsigned long (*get_optional_memory_size)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0xC8
	bool (*try_to_get_optional_checksum)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index, s_cache_file_resource_checksum* out_checksum); // +0xD0
	unsigned __int64 (*get_cache_location_identifier)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index, int section); // +0xD8
	unsigned long (*get_resource_file_sort_key)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index, int section); // +0xE0
	unsigned long (*get_pageable_location_size)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0xE8
	unsigned long (*get_optional_location_size)(c_tag_file_backend_resource_cache_file_datum_handler_simple*, long resource_index); // +0xF0
};

/* ---------- prototypes */

using build_cache_file_add_tag_resources_fn = bool(__fastcall*)(
	int cache_file_version,
	const i343::HaloMapId* map_id,
	c_cache_file_global_tag_registry* tag_registry,
	c_cache_file_builder_tag_resource_runtime_allocator* resource_runtime_allocator,
	c_cache_file_builder_per_bsp_game_resource_collection* per_bsp_resources,
	c_cache_file_resource_library* resource_library,
	c_cache_file_tracer* tracer,
	c_cache_file_builder_writer* writer,
	c_cache_file_builder_tag_resource_output* output,
	c_allocation_base* allocation,
	char optimize);

// Name and signature confirmed.
bool __fastcall build_cache_file_add_tag_resources(
	int cache_file_version,
	const i343::HaloMapId* map_id,
	c_cache_file_global_tag_registry* tag_registry,
	c_cache_file_builder_tag_resource_runtime_allocator* resource_runtime_allocator,
	c_cache_file_builder_per_bsp_game_resource_collection* per_bsp_resources,
	c_cache_file_resource_library* resource_library,
	c_cache_file_tracer* tracer,
	c_cache_file_builder_writer* writer,
	c_cache_file_builder_tag_resource_output* output,
	c_allocation_base* allocation,
	char optimize);

bool __fastcall add_resource_usage_to_zone_manifest(
	dynamic_array* builder_manifests_array,
	int builder_manifest_index,
	s_cache_file_zone_manifest* zone_manifest,
	int resources_count,
	uns32 maximum_tag_instances_count);

bool __fastcall build_zone_manifest_resource_usage(
	c_wrapped_flags* in_out_used_resources,
	dynamic_array* builder_manifests_array,
	s_tag_block* manifests_tag_block,
	int resources_count,
	uns32 maximum_tag_instances_count);

// H3EK 0x1408F3350: append/reuse file_location row and increment resource_reference_count.
int32 __fastcall add_or_get_file_location(
	s_cache_file_resource_gestalt* resource_gestalt,
	s_cache_file_resource_file_location* runtime_salt);

int32 __fastcall get_or_create_shared_file_index(
	s_unknown_struct_shared_file_cache& context,
	s_tag_resource_cache_file_location* location,
	uns32 key);

// H3EK 0x1408F2E30: look up 16-byte codec id in gestalt->codec_definitions, append if missing; else out = -1.
void __fastcall get_or_create_codec_definition_index(
	s_cache_file_resource_gestalt* resource_gestalt,
	c_cache_file_builder_codec* cache_file_builder_codec,
	int8* out_codec_index);

// H3EK 0x1408F7B30: append streaming sublocation table row, emit rows from definition, sort by memory_offset, sum sizes.
uns32 __fastcall build_resource_streaming_sublocation_table(
	s_cache_file_resource_gestalt* resource_gestalt,
	uns32 owner_tag_index,
	uns32 resource_index,
	s_tag_resource_definition* resource_definition);

char __fastcall build_and_assign_pageable_resource_location(
	s_unknown_struct_shared_file_cache* context,
	s_cache_file_resource_gestalt* resource_gestalt,
	int owner_tag_index,
	uns32 resource_index,
	s_tag_resource_definition* resource_definition,
	c_tag_file_backend_resource_cache_file_datum_handler_simple* cache_datum_handler,
	s_cache_file_tag_resource_data* resource_data);

/* ---------- globals */

/* ---------- public code */

#endif // __CACHE_FILE_BUILDER_TAG_RESOURCE_MANAGER_H__
