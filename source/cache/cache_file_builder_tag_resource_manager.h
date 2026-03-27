#ifndef __CACHE_FILE_BUILDER_TAG_RESOURCE_MANAGER_H__
#define __CACHE_FILE_BUILDER_TAG_RESOURCE_MANAGER_H__
#pragma once

/* ---------- headers */

#include <cstdint>

/* ---------- constants */

constexpr std::uintptr_t k_build_cache_file_add_tag_resources_address = 0x1408EA960ull;
constexpr std::uintptr_t k_sub_1408F5560_address = 0x1408F5560ull;

/* ---------- definitions */

namespace i343
{
struct HaloMapId;
}

struct c_cache_file_global_tag_registry;
struct c_cache_file_builder_tag_resource_runtime_allocator;
struct c_cache_file_builder_per_bsp_game_resource_collection;
struct c_cache_file_resource_library;
struct c_cache_file_tracer;
struct c_cache_file_builder_writer;
struct c_cache_file_builder_tag_resource_output;
struct c_allocation_base;
struct dynamic_array;
struct s_cache_file_tag_zone_manifest;

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

char __fastcall sub_1408F5560(
	dynamic_array* builder_manifests_array,
	int builder_manifest_index,
	s_cache_file_tag_zone_manifest* zone_manifest,
	int resources_count,
	unsigned int maximum_tag_instances_count);

/* ---------- globals */

/* ---------- public code */

#endif // __NEW_HEADER_H__
