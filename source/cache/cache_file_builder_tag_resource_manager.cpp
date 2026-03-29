/* ---------- headers */

#include "cache/cache_file_builder_tag_resource_manager.h"
#include "cseries/cseries.h"
#include "cseries/cseries_asserts.h"
#include "cseries/cseries_macros.h"

#include "common/logging.h"
#include "memory/array.h"
#include "memory/bitvector.h"
#include "memory/module.h"
#include "tag_files/tag_groups.h"
#include "tag_files/tag_resource_internals.h"

#include <bit>
#include <cstddef>
#include <cstdint>
#include <sstream>
#include <map>

/* ---------- constants */

struct s_cache_file_tag_zone_manifest;
constexpr uns32 k_tag_resource_definition_table_rva =
	static_cast<uns32>(0x15877A5D0ull - 0x140000000ull);

static uns64* resource_definitions_base = reinterpret_cast<uns64*>(global_address_get(k_tag_resource_definition_table_rva));

/* ---------- prototypes */

bool __fastcall prepare_zone_manifest(
	dynamic_array* builder_manifests_array,
	int builder_manifest_index,
	s_cache_file_tag_zone_manifest* zone_manifest,
	int resources_count,
	unsigned int maximum_tag_instances_count);

/* ---------- hooks */

HOOK_DECLARE(0x1408EA960ull, build_cache_file_add_tag_resources);
HOOK_DECLARE(0x1408F5560ull, prepare_zone_manifest);

/* ---------- definitions */

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

// Makes the map the correct size.
struct s_builder_manifest
{
	std::uint32_t name;
	std::uint32_t unknown4;
	dynamic_array active_resource_owners;
	dynamic_array tag_resources;
	dynamic_array cleared_resources;
	dynamic_array top_level_resource_owners;
    std::multimap<uns32, uns16> attachment_heirarchy_tree;
};

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

// prepares zone manifest block except sizes and resource usage.
// located at 1408F5560 in h3ek
bool __fastcall prepare_zone_manifest(
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

	bool result = false;

	// HOOK_INVOKE(
	// 	result,
	// 	sub_1408F5560,
	// 	builder_manifests_array,
	// 	builder_manifest_index,
	// 	zone_manifest,
	// 	resources_count,
	// 	maximum_tag_instances_count);

    // ===============================================

    s_builder_manifest* builder_manifest = static_cast<s_builder_manifest*>(
        dynamic_array_get_element(builder_manifests_array, builder_manifest_index, sizeof(s_builder_manifest)));
    
    // How many 32-bit words we need to store the bitvectors for the provided resource count.
    int resource_bitvector_word_count = (resources_count + 31) / 32;

    // Resize bitvector tag blocks, these each store a 32 bit word bitvector.
    tag_block_resize(&zone_manifest->required_resource_bitvector, resource_bitvector_word_count);
    tag_block_resize(&zone_manifest->optional_resource_bitvector, resource_bitvector_word_count);
    tag_block_resize(&zone_manifest->streamed_resource_bitvector, resource_bitvector_word_count);

    csmemset(tag_block_get_range_with_size(&zone_manifest->required_resource_bitvector, 0, resource_bitvector_word_count, 4), 0, resource_bitvector_word_count * 4);
    csmemset(tag_block_get_range_with_size(&zone_manifest->optional_resource_bitvector, 0, resource_bitvector_word_count, 4), 0, resource_bitvector_word_count * 4);
    csmemset(tag_block_get_range_with_size(&zone_manifest->streamed_resource_bitvector, 0, resource_bitvector_word_count, 4), 0, resource_bitvector_word_count * 4);

    zone_manifest->name = builder_manifest->name;

    for (uns32 tag_resource_index = 0; tag_resource_index < builder_manifest->tag_resources.count; tag_resource_index++)
    {
        s_tag_resource* tag_resource = static_cast<s_tag_resource*>(
            dynamic_array_get_element(&builder_manifest->tag_resources, tag_resource_index, sizeof(s_tag_resource)));
        
        if (!tag_resource_not_empty(tag_resource))
            continue;

        const uns32 resource_handle = tag_resource->resource_handle;
        const uns16 resource_index = static_cast<uns16>(tag_resource->resource_handle);
        const uns32 definition_ptr = tag_resource->definition_ptr;

        ASSERT(definition_ptr != NONE);
        if (definition_ptr == NULL)
        {
            continue;
        }

        // 32 -> 64 bit definition pointer logic may be PC/Durango only
        s_tag_resource_definition* resource_definition = reinterpret_cast<s_tag_resource_definition*>((*resource_definitions_base) + (tag_resource->definition_ptr * 4ull));

        if (resource_definition->required(resource_definition))
        {
            bitvector_set_bit(
                reinterpret_cast<uns32*>(tag_block_get_range_with_size(&zone_manifest->required_resource_bitvector, 0, zone_manifest->required_resource_bitvector.count, 4)),
                resource_index);
        }

        if (resource_definition->optional(resource_definition))
        {
            bitvector_set_bit(
                reinterpret_cast<uns32*>(tag_block_get_range_with_size(&zone_manifest->optional_resource_bitvector, 0, zone_manifest->optional_resource_bitvector.count, 4)),
                resource_index);
        }

        if (resource_definition->streamed(resource_definition))
        {
            bitvector_set_bit(
                reinterpret_cast<uns32*>(tag_block_get_range_with_size(&zone_manifest->streamed_resource_bitvector, 0, zone_manifest->streamed_resource_bitvector.count, 4)),
                resource_index);
        }  
    }

    for (int cleared_resource_index = 0; cleared_resource_index < builder_manifest->cleared_resources.count; cleared_resource_index++)
    {
        s_tag_resource* cleared_resource = static_cast<s_tag_resource*>(
            dynamic_array_get_element(&builder_manifest->cleared_resources, cleared_resource_index, sizeof(s_tag_resource)));

        if (!tag_resource_not_empty(cleared_resource))
            continue;

        const uns32 resource_handle = cleared_resource->resource_handle;
        const uns16 resource_index = static_cast<uns16>(cleared_resource->resource_handle);

        bitvector_clear_bit(
            reinterpret_cast<uns32*>(tag_block_get_range_with_size(&zone_manifest->required_resource_bitvector, 0, zone_manifest->required_resource_bitvector.count, 4)),
            resource_index);
        bitvector_clear_bit(
            reinterpret_cast<uns32*>(tag_block_get_range_with_size(&zone_manifest->optional_resource_bitvector, 0, zone_manifest->optional_resource_bitvector.count, 4)),
            resource_index);
        bitvector_clear_bit(
            reinterpret_cast<uns32*>(tag_block_get_range_with_size(&zone_manifest->streamed_resource_bitvector, 0, zone_manifest->streamed_resource_bitvector.count, 4)),
            resource_index);
    }

    const uns32 owner_word_count = (maximum_tag_instances_count + 31) / 32;

    tag_block_resize(&zone_manifest->active_resource_owners, owner_word_count);
    tag_block_resize(&zone_manifest->top_level_resource_owners, owner_word_count);

    csmemset(reinterpret_cast<uns32*>(tag_block_get_range_with_size(&zone_manifest->active_resource_owners, 0, zone_manifest->active_resource_owners.count, 4)), 0, owner_word_count * 4);
    csmemset(reinterpret_cast<uns32*>(tag_block_get_range_with_size(&zone_manifest->top_level_resource_owners, 0, zone_manifest->top_level_resource_owners.count, 4)), 0, owner_word_count * 4);

    for (int active_resource_owner_index = 0; active_resource_owner_index < builder_manifest->active_resource_owners.count; active_resource_owner_index++)
    {
        uns32 owner_handle = *static_cast<uns32*>(
            dynamic_array_get_element(&builder_manifest->active_resource_owners, active_resource_owner_index, sizeof(uns32)));
        uns16 owner_index = static_cast<uns16>(owner_handle);

        bitvector_set_bit(
            reinterpret_cast<uns32*>(tag_block_get_range_with_size(&zone_manifest->active_resource_owners, 0, zone_manifest->active_resource_owners.count, 4)),
            owner_index);
    }

    for (int top_level_resource_owner_index = 0; top_level_resource_owner_index < builder_manifest->top_level_resource_owners.count; top_level_resource_owner_index++)
    {
        uns32 owner_handle = *static_cast<uns32*>(
            dynamic_array_get_element(&builder_manifest->top_level_resource_owners, top_level_resource_owner_index, sizeof(uns32)));
        uns16 owner_index = static_cast<uns16>(owner_handle);

        bitvector_set_bit(
            reinterpret_cast<uns32*>(tag_block_get_range_with_size(&zone_manifest->top_level_resource_owners, 0, zone_manifest->top_level_resource_owners.count, 4)),
            owner_index);
    }

    auto& attachment_hierarchy_tagblock = zone_manifest->attachment_hierarchy;
    std::multimap<uns32, uns16> const& attachment_hierarchy_tree = builder_manifest->attachment_heirarchy_tree;

    tag_block_resize(&attachment_hierarchy_tagblock, maximum_tag_instances_count);

    s_data_iterator resource_iterator;
    data_iterator_begin_sub_1407BC980(&resource_iterator, (void*)NONE);

    uns32 resource_handle;
    while ((resource_handle = data_iterator_next_sub_1407BC9C0(&resource_iterator)) != NONE)
    {
        uns16 resource_index = static_cast<uns16>(resource_handle);
        assert(VALID_INDEX(resource_index, attachment_hierarchy_tagblock.count));

        s_cache_file_zone_resource_visit_node* visit_node = &reinterpret_cast<s_cache_file_zone_resource_visit_node*>(
            tag_block_get_range_with_size(
                &attachment_hierarchy_tagblock,
                0,
                attachment_hierarchy_tagblock.count,
                static_cast<int>(sizeof(s_cache_file_zone_resource_visit_node))
            ))[resource_index];

        visit_node->resource_owner_index = resource_index;

        auto range = attachment_hierarchy_tree.equal_range(resource_handle);
        for (auto current_node = range.first; current_node != range.second; ++current_node)
        {
            assert(VALID_INDEX(resource_index, attachment_hierarchy_tagblock.count));

            uns32 new_index = static_cast<uns32>(tag_block_add_element(&visit_node->child_node_indices));

            assert(VALID_INDEX(static_cast<int>(new_index), visit_node->child_node_indices.count));

            s_cache_file_zone_resource_visit_node_link_block* node_link = &reinterpret_cast<s_cache_file_zone_resource_visit_node_link_block*>(
                tag_block_get_range_with_size(
                    &visit_node->child_node_indices,
                    static_cast<int>(new_index),
                    1,
                    static_cast<int>(sizeof(s_cache_file_zone_resource_visit_node_link_block))
                ))[0];

            node_link->child_tag = current_node->second;
        }
    }

    result = true;

    // ===============================================

	std::ostringstream after_stream;
	after_stream << "sub_1408F5560 end"
				 << " result=" << static_cast<int>(result)
				 << " zone_manifest=" << zone_manifest;
	logging::Log(after_stream.str());

	return result;
}

/* ---------- private code */

/* ---------- reverse engineering */
