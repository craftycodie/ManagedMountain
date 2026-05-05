#ifndef __CACHE_FILE_TAG_RESOURCE_DEFINITIONS_H__
#define __CACHE_FILE_TAG_RESOURCE_DEFINITIONS_H__
#pragma once

/* ---------- headers */

#include "cseries/platform.h"
#include "cache/cache_file_tag_resource_checksum.h"
#include "tag_files/tag_group_access.h"
#include "tag_files/tag_resource_internals.h"
#include "memory/static_arrays.h"
#include "memory/wrapped_arrays.h"

/* ---------- constants */

enum
{
	CACHE_FILE_RESOURCE_GESTALT_TAG = 'zone', // 0x7A6F6E65
	CACHE_FILE_RESOURCE_LAYOUT_TABLE_TAG = 'play', // 0x706C6179
};

enum
{
	k_maximum_cache_file_tag_resource_types = 16, // 0x0010
	k_maximum_cache_file_codecs = 127, // 0x007F
	k_maximum_cache_file_shared_files = 7, // 0x0007
};

enum e_cache_file_tag_resource_location_bit
{
	_cache_file_location_all_checksums_valid_bit = 0,
	_cache_file_location_shared_required_bit,
	_cache_file_location_shared_required_dvd_only_bit,
	_cache_file_location_required_dvd_only_bit,
	_cache_file_location_referenced_in_header_bit,
	_cache_file_location_full_checksum_valid_only_bit,
	k_number_of_cache_file_tag_resource_location_bits,
};

enum e_cache_file_tag_resource_data_bit
{
	_cache_file_resource_has_pageable_data_bit = 0,
	_cache_file_resource_has_optional_data_bit,
	k_number_of_cache_file_tag_resource_data_bits,
};

enum e_cache_file_resource_shared_file_bit
{
	_cache_file_resource_shared_file_use_header_io_offset_bit = 0,
	_cache_file_resource_shared_file_optional_bit,
	_cache_file_resource_shared_file_use_header_locations_bit,
	k_number_of_cache_file_resource_shared_file_bits,
};

enum e_cache_file_tag_parentage_bit
{
	_cache_file_tag_parentage_global_bit = 0,
	_cache_file_tag_parentage_unloaded_bit,
	k_number_of_cache_file_tag_parentage_bits,
};

enum e_named_value_type
{
	_named_value_unknown = 0,
	_named_value_string,
	_named_value_real,
	_named_value_int,
	k_number_of_named_value_types,
};

FORWARD_DECLARE_ENUM(e_scenario_type);

/* ---------- definitions */

struct s_tag_resource_streaming_sublocation;
struct s_cache_file_resource_gestalt;
struct s_tag_resource_prediction_quantum;
struct s_tag_resource_prediction_atom;
struct s_tag_resource_prediction_molecule;
struct s_tag_resource_prediction_molecule_key;

struct s_cache_file_tag_resource_vtable
{
	s_tag_persistent_identifier resource_identifier; // 0x0
	bool (*on_load_from_cache_file)(long, void*); // 0x10
	void (*on_dispose_from_cache)(long, void*); // 0x18
};
static_assert(sizeof(s_cache_file_tag_resource_vtable) == 32);

struct s_cache_file_resource_file_location
{
	short header_salt_at_runtime; // 0x0
	c_flags_no_init<e_cache_file_tag_resource_location_bit, unsigned char, k_number_of_cache_file_tag_resource_location_bits> flags; // 0x2
	char codec_index; // 0x3
	short shared_file_index; // 0x4
	short shared_file_global_location_index; // 0x6
	unsigned long file_offset; // 0x8
	unsigned long file_size; // 0xC
	unsigned long memory_size; // 0x10
	s_cache_file_resource_checksum checksum; // 0x14
	unsigned short resource_reference_count; // 0x54
	short streaming_sublocation_table_index; // 0x56
};
static_assert(sizeof(s_cache_file_resource_file_location) == 88);

struct s_cache_file_resource_patch_lookup
{
	unsigned long patch_index; // 0x0
	unsigned long pageable_file_location_index; // 0x4
	unsigned long resource_index; // 0x8
};
static_assert(sizeof(s_cache_file_resource_patch_lookup) == 12);

struct s_cache_file_resource_sections_location
{
	short pageable_file_location_index; // 0x0
	short optional_file_location_index; // 0x2
	unsigned long pageable_location_offset; // 0x4
	unsigned long optional_location_offset; // 0x8
	short pageable_sublocation_table_index; // 0xC
	short optional_sublocation_table_index; // 0xE
};
static_assert(sizeof(s_cache_file_resource_sections_location) == 16);

// H3EK gestalt streaming table row (IDB `s_cache_file_resource_streaming_sublocation`); tag_block uses element_size 8 when sorting.
struct s_cache_file_resource_streaming_sublocation
{
	int memory_offset;
	int memory_size;
};
static_assert(sizeof(s_cache_file_resource_streaming_sublocation) == 8);

struct s_cache_file_resource_streaming_sublocation_table
{
	unsigned long total_memory_size; // 0x0
	c_typed_tag_block<s_tag_resource_streaming_sublocation> stream_locations; // 0x4
};
static_assert(sizeof(s_cache_file_resource_streaming_sublocation_table) == 16);

struct s_cache_file_resource_memory_layout
{
	c_tag_resource_fixup root_address_fixup; // 0x0
	c_typed_tag_block<s_tag_resource_fixup_location> control_fixups; // 0x4
	c_typed_tag_block<s_tag_resource_interop_location> interops; // 0x10
};
static_assert(sizeof(s_cache_file_resource_memory_layout) == 28);

struct s_cache_file_tag_resource_data
{
	s_tag_reference owner_tag; // 0x0
	short resource_handle_salt; // 0x10
	char cache_file_build_time_type_index; // 0x12
	char control_alignment_bits; // 0x13
	unsigned long control_offset_offset; // 0x14
	unsigned long control_size; // 0x18
	unsigned long optional_control_offset_offset; // 0x1C
	c_flags_no_init<e_cache_file_tag_resource_data_bit, unsigned short, k_number_of_cache_file_tag_resource_data_bits> flags; // 0x20
	short location_index; // 0x22
	s_cache_file_resource_memory_layout memory_layout; // 0x24
};
static_assert(sizeof(s_cache_file_tag_resource_data) == 64);

struct s_cache_file_tag_resource_named_value
{
	long name; // 0x0
	e_named_value_type type; // 0x4
	long string_value; // 0x8
	float real_value; // 0xC
	long int_value; // 0x10
};
static_assert(sizeof(s_cache_file_tag_resource_named_value) == 20);

struct s_cache_file_tag_resource_properties
{
	c_typed_tag_block<s_cache_file_tag_resource_named_value> named_values; // 0x0
};
static_assert(sizeof(s_cache_file_tag_resource_properties) == 12);

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

struct s_cache_file_resource_shared_file
{
	c_static_string<256> file_name; // 0x0
	short global_location_index_offset; // 0x100
	c_flags_no_init<e_cache_file_resource_shared_file_bit, unsigned short, k_number_of_cache_file_resource_shared_file_bits> flags; // 0x102
	unsigned long io_offset; // 0x104
};
static_assert(sizeof(s_cache_file_resource_shared_file) == 264);

struct s_cache_file_model_variant_usage
{
	short resource_owner_index; // 0x0
	short pad; // 0x2
	long variant_name; // 0x4
	c_typed_tag_block<short> used_material_resource_owners; // 0x8
};
static_assert(sizeof(s_cache_file_model_variant_usage) == 20);

struct s_cache_file_character_usage
{
	short resource_owner_index; // 0x0
	short pad; // 0x2
	c_typed_tag_block<short> used_model_variants; // 0x4
};
static_assert(sizeof(s_cache_file_character_usage) == 16);

struct s_cache_file_zone_resource_usage
{
	unsigned long required_pageable_size; // 0x0
	unsigned long deferred_required_size; // 0x4
	unsigned long optional_memory_size; // 0x8
	unsigned long streamed_size; // 0xC
	unsigned long disk_size; // 0x10
};
static_assert(sizeof(s_cache_file_zone_resource_usage) == 20);

struct s_cache_file_zone_resource_visit_node
{
	short resource_owner_index; // 0x0
	short pad; // 0x2
	c_typed_tag_block<short> child_node_indices; // 0x4
};
static_assert(sizeof(s_cache_file_zone_resource_visit_node) == 16);

struct s_cache_file_zone_resource_visit_node_block_struct
{
	short parent_tag;
	short pad;
	s_tag_block children;
};
static_assert(sizeof(s_cache_file_zone_resource_visit_node_block_struct) == 16);

struct s_cache_file_zone_resource_visit_node_link_block
{
	short child_tag;
};
static_assert(sizeof(s_cache_file_zone_resource_visit_node_link_block) == 2);

struct s_cache_file_zone_manifest
{
	c_tag_block_bit_vector required_resources_bitvector; // 0x0
	c_tag_block_bit_vector unused_resources_bitvector; // 0xC
	c_tag_block_bit_vector optional_resources_bitvector; // 0x18
	c_tag_block_bit_vector streamed_resources_bitvector; // 0x24
	unsigned long required_pageable_size; // 0x30
	unsigned long deferred_required_size; // 0x34
	unsigned long optional_memory_size; // 0x38
	unsigned long streamed_size; // 0x3C
	unsigned long dvd_memory_size; // 0x40
	long name; // 0x44
	c_typed_tag_block<s_cache_file_zone_resource_usage> resource_usage; // 0x48
	c_tag_block_bit_vector active_resource_owners; // 0x54
	c_tag_block_bit_vector top_level_resource_owners; // 0x60
	c_typed_tag_block<s_cache_file_zone_resource_visit_node> visitation_hierarchy; // 0x6C
	void setup(s_cache_file_resource_gestalt const*, unsigned long);
	bool resource_is_required_somehow(long absolute_resource_index) const;
};
static_assert(sizeof(s_cache_file_zone_manifest) == 120);

struct s_cache_file_zone_set_zone_usage
{
	long name; // 0x0
	unsigned long flags; // 0x4
	unsigned long required_bsp_mask; // 0x8
	unsigned long expected_touched_bsp_mask; // 0xC
	unsigned long required_designer_zone_mask; // 0x10
	unsigned long expected_designer_zone_mask; // 0x14
	unsigned long forbidden_designer_zone_mask; // 0x18
	unsigned long cinematic_zone_mask; // 0x1C
	long hint_previous_zone_set_index; // 0x20
};
static_assert(sizeof(s_cache_file_zone_set_zone_usage) == 36);

struct s_debug_tag_cache_file_zone_manifest :
	public s_cache_file_zone_manifest
{
	unsigned long total_disk_size; // 0x78
	unsigned long unused_size; // 0x7C
	s_tag_reference owner_tag; // 0x80
};
static_assert(sizeof(s_debug_tag_cache_file_zone_manifest) == 144);

struct s_cache_file_tag_resource_definition
{
	s_tag_persistent_identifier identifier; // 0x0
	unsigned long flags; // 0x10
	short pageable_alignment_bits; // 0x14
	short optional_alignment_bits; // 0x16
	long name; // 0x18
};
static_assert(sizeof(s_cache_file_tag_resource_definition) == 28);

struct s_cache_file_codec_definition
{
	s_tag_persistent_identifier identifier; // 0x0
};
static_assert(sizeof(s_cache_file_codec_definition) == 16);

struct s_cache_file_tag_interop_definition
{
	s_tag_persistent_identifier type_identifier; // 0x0
	long name; // 0x10
};
static_assert(sizeof(s_cache_file_tag_interop_definition) == 20);

struct s_cache_file_debug_tag_resource_defintion
{
	c_typed_tag_block<long> report_categories; // 0x0
};
static_assert(sizeof(s_cache_file_debug_tag_resource_defintion) == 12);

struct s_cache_file_tag_resource_layout
{
	unsigned long immediately_required_size; // 0x0
	unsigned long deferred_required_size; // 0x4
	unsigned long optional_memory_size; // 0x8
	unsigned long unused_size; // 0xC
	unsigned long pageable_compressed_size; // 0x10
	unsigned long optional_compressed_size; // 0x14
	unsigned short global_zone_flags; // 0x18
	unsigned short bsp_zone_flags; // 0x1A
	unsigned long designer_zone_flags; // 0x1C
	unsigned long cinematic_zone_flags; // 0x20
};
static_assert(sizeof(s_cache_file_tag_resource_layout) == 36);

struct s_cache_file_tag_parentage
{
	s_tag_reference child_tag; // 0x0
	c_flags_no_init<e_cache_file_tag_parentage_bit, unsigned short, k_number_of_cache_file_tag_parentage_bits> flags; // 0x10
	short resource_owner_index; // 0x12
	c_typed_tag_block<long> parent_indices; // 0x14
	c_typed_tag_block<long> child_indices; // 0x20
};
static_assert(sizeof(s_cache_file_tag_parentage) == 44);

struct s_cache_file_bsp_attachment
{
	s_tag_reference reference; // 0x0
};
static_assert(sizeof(s_cache_file_bsp_attachment) == 16);

struct s_cache_file_bsp_game_attachments
{
	c_typed_tag_block<s_cache_file_bsp_attachment> static_attachments; // 0x0
	c_typed_tag_block<s_cache_file_bsp_attachment> persistent_attachments; // 0xC
	c_typed_tag_block<s_cache_file_bsp_attachment> dynamic_attachments; // 0x18
};
static_assert(sizeof(s_cache_file_bsp_game_attachments) == 36);

struct s_cache_file_resource_prediction_table
{
	c_typed_tag_block<s_tag_resource_prediction_quantum> quanta; // 0x0
	c_typed_tag_block<s_tag_resource_prediction_atom> atoms; // 0xC
	c_typed_tag_block<long> molecule_atoms; // 0x18
	c_typed_tag_block<s_tag_resource_prediction_molecule> molecules; // 0x24
	c_typed_tag_block<s_tag_resource_prediction_molecule_key> molecule_keys; // 0x30
};
static_assert(sizeof(s_cache_file_resource_prediction_table) == 60);

struct s_cache_file_resource_gestalt
{
	e_scenario_type get_sharing_type() const;
	e_scenario_type get_actual_type() const;
	short internal_scenario_type; // 0x0
	unsigned short internal_scenario_flags; // 0x2
	c_typed_tag_block<s_cache_file_tag_resource_definition> resource_definitions; // 0x4
	c_typed_tag_block<s_cache_file_tag_interop_definition> interop_definitions; // 0x10
	c_typed_tag_block<s_cache_file_codec_definition> codec_definitions; // 0x1C
	c_typed_tag_block<s_cache_file_resource_shared_file> shared_files; // 0x28
	c_typed_tag_block<s_cache_file_resource_file_location> file_locations; // 0x34
	c_typed_tag_block<s_cache_file_resource_streaming_sublocation_table> streaming_sublocation_tables; // 0x40
	c_typed_tag_block<s_cache_file_resource_patch_lookup> file_location_patch_lookup; // 0x4C
	c_typed_tag_block<s_cache_file_resource_sections_location> default_locations; // 0x58
	c_typed_tag_block<s_cache_file_tag_resource_data> resources; // 0x64
	c_typed_tag_block<s_cache_file_zone_manifest> designer_zone_manifests; // 0x70
	c_typed_tag_block<s_cache_file_zone_manifest> global_zone_manifests; // 0x7C
	c_typed_tag_block<s_cache_file_zone_manifest> hs_zone_manifests; // 0x88
	c_typed_tag_block<s_cache_file_zone_manifest> unattached_designer_zone_manifests; // 0x94
	c_typed_tag_block<s_cache_file_zone_manifest> dvd_forbidden_zone_manifests; // 0xA0
	c_typed_tag_block<s_cache_file_zone_manifest> dvd_always_streaming_zone_manifests; // 0xAC
	c_typed_tag_block<s_cache_file_zone_manifest> default_bsp_zone_manifests; // 0xB8
	c_typed_tag_block<s_cache_file_zone_manifest> static_bsp_zone_manifests; // 0xC4
	c_typed_tag_block<s_cache_file_zone_manifest> dynamic_bsp_zone_manifests; // 0xD0
	c_typed_tag_block<s_cache_file_zone_manifest> cinematic_zone_manifests; // 0xDC
	c_typed_tag_block<s_cache_file_zone_manifest> zones_only_zone_set_manifests; // 0xE8
	c_typed_tag_block<s_cache_file_zone_manifest> expected_zone_set_manifests; // 0xF4
	c_typed_tag_block<s_cache_file_zone_manifest> fully_populated_zone_set_manifests; // 0x100
	c_typed_tag_block<s_cache_file_zone_set_zone_usage> zone_set_zone_usages; // 0x10C
	c_typed_tag_block<s_tag_reference> bsp_references; // 0x118
	c_typed_tag_block<s_tag_reference> resource_owners; // 0x124
	c_typed_tag_block<s_cache_file_model_variant_usage> per_model_variant_usages; // 0x130
	c_typed_tag_block<s_cache_file_character_usage> per_character_usages; // 0x13C
	s_tag_data naive_control_data; // 0x148
	unsigned long minimum_complete_pageable_data_size; // 0x15C
	unsigned long minimum_required_pageable_data_size; // 0x160
	unsigned long minimum_required_dvd_data_size; // 0x164
	unsigned long global_pageable_data_size; // 0x168
	unsigned long optional_control_data_size; // 0x16C
	c_typed_tag_block<s_cache_file_zone_resource_usage> global_resource_usage; // 0x170
	c_typed_tag_block<s_cache_file_bsp_game_attachments> bsp_game_attachments; // 0x17C
	c_typed_tag_block<s_debug_tag_cache_file_zone_manifest> debug_resources_per_model_variant; // 0x188
	c_typed_tag_block<s_debug_tag_cache_file_zone_manifest> debug_resources_per_combat_dialogue; // 0x194
	c_typed_tag_block<s_debug_tag_cache_file_zone_manifest> debug_resources_per_tag; // 0x1A0
	c_typed_tag_block<s_cache_file_debug_tag_resource_defintion> resource_debug_definitions; // 0x1AC
	c_typed_tag_block<s_cache_file_tag_resource_layout> resource_layouts; // 0x1B8
	c_typed_tag_block<s_cache_file_tag_resource_properties> resource_properties; // 0x1C4
	c_typed_tag_block<s_cache_file_tag_parentage> parentages; // 0x1D0
	s_cache_file_resource_prediction_table prediction_table; // 0x1DC
	long campaign_id; // 0x218
	long map_id; // 0x21C
	bool resize_resources(long);
};
static_assert(sizeof(s_cache_file_resource_gestalt) == 544);

struct s_cache_file_resource_layout_table
{
	c_typed_tag_block<s_cache_file_codec_definition> codec_definitions; // 0x0
	c_typed_tag_block<s_cache_file_resource_shared_file> shared_files; // 0xC
	c_typed_tag_block<s_cache_file_resource_file_location> file_locations; // 0x18
	c_typed_tag_block<s_cache_file_resource_streaming_sublocation_table> streaming_sublocation_tables; // 0x24
	c_typed_tag_block<s_cache_file_resource_sections_location> default_locations; // 0x30
	long required_location_count; // 0x3C
	long required_dvd_only_location_count; // 0x40
};
static_assert(sizeof(s_cache_file_resource_layout_table) == 68);

class c_datum_index
{
public:
	_inline c_datum_index(long datum_index);
	_inline long get_absolute_index() const;
	_inline long get_identifier() const;
private:
	long m_datum_index; // 0x0
};
static_assert(sizeof(c_datum_index) == 4);

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __CACHE_FILE_TAG_RESOURCE_DEFINITIONS_H__