/* ---------- headers */

#include "cache/cache_file_builder_tag_resource_manager.h"
#include "cseries/cseries.h"

#include "common/logging.h"
#include "memory/array.h"
#include "memory/module.h"
#include "tag_files/tag_groups.h"
#include "tag_files/tag_resource_internals.h"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <sstream>

/* ---------- constants */

struct s_cache_file_tag_zone_manifest;

/* ---------- prototypes */

char __fastcall sub_1408F5560(
	dynamic_array* builder_manifests_array,
	int builder_manifest_index,
	s_cache_file_tag_zone_manifest* zone_manifest,
	int resources_count,
	unsigned int maximum_tag_instances_count);

/* ---------- hooks */

HOOK_DECLARE(0x1408EA960ull, build_cache_file_add_tag_resources);
HOOK_DECLARE(0x1408F5560ull, sub_1408F5560);

/* ---------- definitions */

struct s_tag_block
{
	int count;
	int address;
	int definition;
};

struct s_cache_file_tag_resources_bitvector
{
	std::uint32_t bits;
};

struct s_attachment_hierarchy_tree_node
{
	s_attachment_hierarchy_tree_node* left;
	s_attachment_hierarchy_tree_node* parent;
	s_attachment_hierarchy_tree_node* right;
	std::uint8_t unknown18;
	std::uint8_t is_nil;
	std::uint8_t unknown1A[2];
	int key;
	std::uint16_t value;
};

struct s_attachment_hierarchy_entry
{
	std::uint16_t index;
	std::uint16_t unknown2;
	s_tag_block children;
};

struct s_cache_file_tag_zone_manifest
{
	s_tag_block required_resource_bitvector;
	s_tag_block deferred_resource_bitvector;
	s_tag_block optional_resource_bitvector;
	s_tag_block streamed_resource_bitvector;
	int required_resource_size;
	int deferred_required_size;
	int optional_resource_size;
	int streamed_resource_size;
	int dvd_in_memory_resource_size;
	int name;
	s_tag_block resource_usage;
	s_tag_block active_resource_owners;
	s_tag_block top_level_resource_owners;
	s_tag_block attachment_hierarchy;
};

struct s_builder_manifest
{
	std::uint32_t name;
	std::uint32_t unknown4;
	dynamic_array active_resource_owners;
	dynamic_array tag_resources;
	dynamic_array cleared_resources;
	dynamic_array top_level_resource_owners;
	s_attachment_hierarchy_tree_node* attachment_hierarchy_tree;
	std::byte unknown50[0x08];
};

static_assert(sizeof(s_tag_block) == 0x0C);
static_assert(sizeof(s_attachment_hierarchy_entry) == 0x10);
static_assert(offsetof(s_builder_manifest, active_resource_owners) == 0x08);
static_assert(offsetof(s_builder_manifest, tag_resources) == 0x18);
static_assert(offsetof(s_builder_manifest, cleared_resources) == 0x28);
static_assert(offsetof(s_builder_manifest, top_level_resource_owners) == 0x38);
static_assert(offsetof(s_builder_manifest, attachment_hierarchy_tree) == 0x48);
static_assert(sizeof(s_builder_manifest) == 0x58);
static_assert(offsetof(s_cache_file_tag_zone_manifest, required_resource_bitvector) == 0x00);
static_assert(offsetof(s_cache_file_tag_zone_manifest, deferred_resource_bitvector) == 0x0C);
static_assert(offsetof(s_cache_file_tag_zone_manifest, optional_resource_bitvector) == 0x18);
static_assert(offsetof(s_cache_file_tag_zone_manifest, streamed_resource_bitvector) == 0x24);
static_assert(offsetof(s_cache_file_tag_zone_manifest, required_resource_size) == 0x30);
static_assert(offsetof(s_cache_file_tag_zone_manifest, deferred_required_size) == 0x34);
static_assert(offsetof(s_cache_file_tag_zone_manifest, optional_resource_size) == 0x38);
static_assert(offsetof(s_cache_file_tag_zone_manifest, streamed_resource_size) == 0x3C);
static_assert(offsetof(s_cache_file_tag_zone_manifest, dvd_in_memory_resource_size) == 0x40);
static_assert(offsetof(s_cache_file_tag_zone_manifest, name) == 0x44);
static_assert(offsetof(s_cache_file_tag_zone_manifest, resource_usage) == 0x48);
static_assert(offsetof(s_cache_file_tag_zone_manifest, active_resource_owners) == 0x54);
static_assert(offsetof(s_cache_file_tag_zone_manifest, top_level_resource_owners) == 0x60);
static_assert(offsetof(s_cache_file_tag_zone_manifest, attachment_hierarchy) == 0x6C);

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

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
	char optimize)
{
	std::ostringstream before_stream;
	before_stream
		<< "build_cache_file_add_tag_resources begin"
		<< " cache_file_version=" << cache_file_version
		<< " map_id=" << map_id
		<< " tag_registry=" << tag_registry
		<< " resource_runtime_allocator=" << resource_runtime_allocator
		<< " per_bsp_resources=" << per_bsp_resources
		<< " resource_library=" << resource_library
		<< " tracer=" << tracer
		<< " writer=" << writer
		<< " output=" << output
		<< " allocation=" << allocation
		<< " optimize=" << static_cast<int>(optimize);
	logging::Log(before_stream.str());

	bool result = false;
	HOOK_INVOKE(
		result,
		build_cache_file_add_tag_resources,
		cache_file_version,
		map_id,
		tag_registry,
		resource_runtime_allocator,
		per_bsp_resources,
		resource_library,
		tracer,
		writer,
		output,
		allocation,
		optimize);

	std::ostringstream after_stream;
	after_stream
		<< "build_cache_file_add_tag_resources end"
		<< " result=" << result
		<< " output=" << output;
	logging::Log(after_stream.str());

	return result;
}

char __fastcall sub_1408F5560(
	dynamic_array* builder_manifests_array,
	int builder_manifest_index,
	s_cache_file_tag_zone_manifest* zone_manifest,
	int resources_count,
	unsigned int maximum_tag_instances_count)
{
	std::ostringstream before_stream;
	before_stream
		<< "sub_1408F5560 begin"
		<< " builder_manifests_array=" << builder_manifests_array
		<< " builder_manifest_index=" << builder_manifest_index
		<< " zone_manifest=" << zone_manifest
		<< " resources_count=" << resources_count
		<< " maximum_tag_instances_count=" << maximum_tag_instances_count;
	logging::Log(before_stream.str());

	char result = 0;
	HOOK_INVOKE(
		result,
		sub_1408F5560,
		builder_manifests_array,
		builder_manifest_index,
		zone_manifest,
		resources_count,
		maximum_tag_instances_count);

	std::ostringstream after_stream;
	after_stream << "sub_1408F5560 end"
				 << " result=" << static_cast<int>(result)
				 << " zone_manifest=" << zone_manifest;
	logging::Log(after_stream.str());

	return result;
}

/* ---------- private code */

/* ---------- reverse engineering */
