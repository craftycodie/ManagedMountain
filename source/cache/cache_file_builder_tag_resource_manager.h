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
constexpr uns64 k_get_or_create_shared_file_index_address = 0x1408F3610ull;
constexpr uns64 k_get_or_create_codec_definition_index_address = 0x1408F2E30ull;

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
struct s_cache_file_tag_zone_manifest;
class c_wrapped_flags;

struct s_cache_file_zone_resource_visit_node;
struct s_cache_file_zone_resource_visit_node_block_struct;
struct s_cache_file_zone_resource_visit_node_link_block;

struct s_cache_file_resource_gestalt;

struct s_tag_resource_cache_file_location
{
	char dvd_relative_path[256];
	uns16 flags;
	int16 global_shared_location_offset;
	int32 io_offset;
};

struct s_unknown_struct_shared_file_cache
{
	std::map<uns32, int32> m_shared_file_index_cache;
	int m_zone_tag_index;
	c_cache_file_builder_writer_default* m_writer;
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
	s_cache_file_tag_zone_manifest* zone_manifest,
	int resources_count,
	uns32 maximum_tag_instances_count);

bool __fastcall build_zone_manifest_resource_usage(
	c_wrapped_flags* in_out_used_resources,
	dynamic_array* builder_manifests_array,
	s_tag_block* manifests_tag_block,
	int resources_count,
	uns32 maximum_tag_instances_count);

int32 __fastcall get_or_create_shared_file_index(
	s_unknown_struct_shared_file_cache& context,
	s_tag_resource_cache_file_location* location,
	uns32 key);

// H3EK 0x1408F2E30: look up 16-byte codec id in gestalt->codec_definitions, append if missing; else out = -1.
void __fastcall get_or_create_codec_definition_index(
	s_cache_file_resource_gestalt* resource_gestalt,
	c_cache_file_builder_codec* cache_file_builder_codec,
	int8* out_codec_index);

/* ---------- globals */

/* ---------- public code */

#endif // __NEW_HEADER_H__
