/* ---------- headers */

#include "cache/cache_file_builder_tag_resource_manager.h"
#include "cache/cache_file_tag_resource_definitions.h"
#include "cseries/cseries.h"
#include "cseries/cseries_asserts.h"
#include "cseries/cseries_macros.h"

#include "common/logging.h"
#include "memory/array.h"
#include "memory/wrapped_arrays.h"
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

bool __fastcall add_resource_usage_to_zone_manifest(
	dynamic_array* builder_manifests_array,
	int builder_manifest_index,
	s_cache_file_tag_zone_manifest* zone_manifest,
	int resources_count,
	unsigned int maximum_tag_instances_count);

/* ---------- hooks */

HOOK_DECLARE(0x1408EA960ull, build_cache_file_add_tag_resources);
HOOK_DECLARE(0x1408F5560ull, add_resource_usage_to_zone_manifest);
HOOK_DECLARE(0x1408F7750ull, build_zone_manifest_resource_usage);

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
	c_tag_block_bit_vector required_resource_bitvector;
	c_tag_block_bit_vector deferred_resource_bitvector;
	c_tag_block_bit_vector optional_resource_bitvector;
	c_tag_block_bit_vector streamed_resource_bitvector;
	int required_resource_size;
	int deferred_required_size;
	int optional_resource_size;
	int streamed_resource_size;
	int dvd_in_memory_resource_size;
	int name;
	s_tag_block resource_usage;
	c_tag_block_bit_vector active_resource_owners;
	c_tag_block_bit_vector top_level_resource_owners;
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

static_assert(sizeof(s_cache_file_tag_zone_manifest) == k_cache_file_tag_zone_manifest_element_bytes);
static_assert(sizeof(s_in_out_used_resources) == 2 + 4 + (sizeof(std::uint32_t) * k_in_out_used_resources_dword_count));

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

// Adds resource usage to a zone manifest block (bitvectors and attachment hierarchy) and returns a bitvector of used resources.
// located at 1408F7750 in h3ek
bool __fastcall build_zone_manifest_resource_usage(
	c_wrapped_flags* in_out_used_resources,
	dynamic_array* builder_manifests_array,
	s_tag_block* manifests_tag_block,
	int resources_count,
	unsigned int maximum_tag_instances_count)
{
	std::ostringstream before_stream;
	before_stream
		<< "sub_1408F7750 begin"
		<< " in_out_used_resources=" << static_cast<void*>(in_out_used_resources)
		<< " builder_manifests_array=" << builder_manifests_array
		<< " manifests_tag_block=" << manifests_tag_block
		<< " resources_count=" << resources_count
		<< " maximum_tag_instances_count=" << maximum_tag_instances_count;
	logging::Log(before_stream.str());

	bool success = true;

	tag_block_resize(manifests_tag_block, builder_manifests_array->count);

	s_cache_file_tag_zone_manifest* const zone_manifests = reinterpret_cast<s_cache_file_tag_zone_manifest*>(
		tag_block_get_range_with_size(
			manifests_tag_block,
			0,
			manifests_tag_block->count,
			sizeof(s_cache_file_tag_zone_manifest)));

	for (int builder_manifest_index = 0; builder_manifest_index < builder_manifests_array->count; ++builder_manifest_index)
	{
		int const manifests_count = manifests_tag_block->count;
		ASSERT(VALID_INDEX(builder_manifest_index, manifests_count));

		s_cache_file_tag_zone_manifest* cache_file_zone = &zone_manifests[builder_manifest_index];

		bool const zone_manifest_populated = add_resource_usage_to_zone_manifest(
			builder_manifests_array,
			builder_manifest_index,
			cache_file_zone,
			resources_count,
			maximum_tag_instances_count);
            
		if (!zone_manifest_populated)
		{
			success = false;
		}

		// not entirely sure why we only assert the bit vector is large enough ONLY for required resources.
		// maybe because optional & streamed are ok if not populated.
		ASSERT(in_out_used_resources->bit_count() >= cache_file_zone->required_resource_bitvector.bit_count());

		bit_vector_not_and(
			cache_file_zone->required_resource_bitvector.bit_count(),
			cache_file_zone->required_resource_bitvector.get_bits(),
			in_out_used_resources->get_bits_unsafe(),
			in_out_used_resources->get_bits_unsafe());

		bit_vector_not_and(
			cache_file_zone->optional_resource_bitvector.bit_count(),
			cache_file_zone->optional_resource_bitvector.get_bits(),
			in_out_used_resources->get_bits_unsafe(),
			in_out_used_resources->get_bits_unsafe());

		bit_vector_not_and(
			cache_file_zone->streamed_resource_bitvector.bit_count(),
			cache_file_zone->streamed_resource_bitvector.get_bits(),
			in_out_used_resources->get_bits_unsafe(),
			in_out_used_resources->get_bits_unsafe());
	}

	std::ostringstream after_stream;
	after_stream << "sub_1408F7750 end"
				 << " result=" << static_cast<int>(success);
	logging::Log(after_stream.str());

	return success;
}

// prepares zone manifest block except sizes and resource usage.
// located at 1408F5560 in h3ek
bool __fastcall add_resource_usage_to_zone_manifest(
	dynamic_array* builder_manifests_array,
	int builder_manifest_index,
	s_cache_file_tag_zone_manifest* cache_file_zone,
	int resources_count,
	unsigned int maximum_tag_instances_count)
{
	std::ostringstream before_stream;
	before_stream
		<< "sub_1408F5560 begin"
		<< " builder_manifests_array=" << builder_manifests_array
		<< " builder_manifest_index=" << builder_manifest_index
		<< " zone_manifest=" << cache_file_zone
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

	cache_file_zone->required_resource_bitvector.resize_in_bits(resources_count);
	cache_file_zone->optional_resource_bitvector.resize_in_bits(resources_count);
	cache_file_zone->streamed_resource_bitvector.resize_in_bits(resources_count);

	cache_file_zone->required_resource_bitvector.clear();
	cache_file_zone->optional_resource_bitvector.clear();
	cache_file_zone->streamed_resource_bitvector.clear();

    cache_file_zone->name = builder_manifest->name;

    for (int32 tag_resource_index = 0; tag_resource_index < builder_manifest->tag_resources.count; tag_resource_index++)
    {
        s_tag_resource* tag_resource = static_cast<s_tag_resource*>(
            dynamic_array_get_element(&builder_manifest->tag_resources, tag_resource_index, sizeof(s_tag_resource)));
        
        if (!tag_resource_not_empty(tag_resource))
            continue;

        const uns32 resource_handle = tag_resource->resource_handle;
        const uns16 resource_index = static_cast<uns16>(resource_handle);
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
			cache_file_zone->required_resource_bitvector.set(resource_index, true);
        }

        if (resource_definition->optional(resource_definition))
        {
			cache_file_zone->optional_resource_bitvector.set(resource_index, true);
        }

        if (resource_definition->streamed(resource_definition))
        {
			cache_file_zone->streamed_resource_bitvector.set(resource_index, true);
        }  
    }

    for (int cleared_resource_index = 0; cleared_resource_index < builder_manifest->cleared_resources.count; cleared_resource_index++)
    {
        s_tag_resource* cleared_resource = static_cast<s_tag_resource*>(
            dynamic_array_get_element(&builder_manifest->cleared_resources, cleared_resource_index, sizeof(s_tag_resource)));

		if (!tag_resource_not_empty(cleared_resource))
		{
			continue;
		}

        const uns32 resource_handle = cleared_resource->resource_handle;
        const uns16 resource_index = static_cast<uns16>(resource_handle);

		cache_file_zone->required_resource_bitvector.set(resource_index, false);
		cache_file_zone->optional_resource_bitvector.set(resource_index, false);
		cache_file_zone->streamed_resource_bitvector.set(resource_index, false);
    }

	cache_file_zone->active_resource_owners.resize_in_bits(maximum_tag_instances_count);
	cache_file_zone->top_level_resource_owners.resize_in_bits(maximum_tag_instances_count);

	cache_file_zone->active_resource_owners.clear();
	cache_file_zone->top_level_resource_owners.clear();

    for (uns32 active_resource_owner_index = 0; active_resource_owner_index < builder_manifest->active_resource_owners.count; active_resource_owner_index++)
    {
        uns32 owner_handle = *static_cast<uns32*>(
            dynamic_array_get_element(&builder_manifest->active_resource_owners, active_resource_owner_index, sizeof(uns32)));
        uns16 owner_index = static_cast<uns16>(owner_handle);

		cache_file_zone->active_resource_owners.set(owner_index, true);
    }

    for (uns32 top_level_resource_owner_index = 0; top_level_resource_owner_index < builder_manifest->top_level_resource_owners.count; top_level_resource_owner_index++)
    {
        uns32 owner_handle = *static_cast<uns32*>(
            dynamic_array_get_element(&builder_manifest->top_level_resource_owners, top_level_resource_owner_index, sizeof(uns32)));
        uns16 owner_index = static_cast<uns16>(owner_handle);

		cache_file_zone->top_level_resource_owners.set(owner_index, true);
    }

    auto& attachment_hierarchy_tagblock = cache_file_zone->attachment_hierarchy;
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
				 << " zone_manifest=" << cache_file_zone;
	logging::Log(after_stream.str());

	return result;
}

bool c_tag_block_bit_vector::resize_in_bits(unsigned long new_size)
{
    return tag_block_resize(&m_bits, (new_size + 31) >> 5);
}

long c_tag_block_bit_vector::bit_count() const
{
	return 32L * m_bits.count();
}

/* ---------- private code */

unsigned long const* c_tag_block_bit_vector::get_bit_storage() const
{
	s_tag_block const* tag_block = static_cast<s_tag_block const*>(&m_bits);

	return static_cast<unsigned long const*>(tag_block_get_range_with_size(
		const_cast<s_tag_block*>(tag_block),
		0,
		tag_block->count,
		4));
}

unsigned long* c_tag_block_bit_vector::get_bit_storage()
{
	s_tag_block* const tag_block = static_cast<s_tag_block*>(&m_bits);
	return static_cast<unsigned long*>(tag_block_get_range_with_size(tag_block, 0, tag_block->count, 4));
}

/* ---------- reverse engineering */
