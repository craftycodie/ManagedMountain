#ifndef __TAG_RESOURCE_INTERNALS_H__
#define __TAG_RESOURCE_INTERNALS_H__
#pragma once

/* ---------- headers */

#include <cstdint>

/* ---------- constants */

/* ---------- definitions */

struct s_tag_resource
{
  int resource_handle;
  int definition_ptr;
};

struct s_tag_resource_definition
{
  std::uint64_t unknown0;
  int flags;
  int unknownC;
  void* resource_struct_definition;

  static bool optional(const s_tag_resource_definition* tag_resource_definition);
  static bool streamed(const s_tag_resource_definition* tag_resource_definition);
  static bool required(const s_tag_resource_definition* tag_resource_definition);
  static bool is_required_in_manifest(const s_tag_resource_definition* tag_resource_definition);

};


/* ---------- prototypes */

bool __fastcall tag_resource_not_empty(const s_tag_resource* tag_resource);
bool __fastcall tag_resource_definition_optional(const s_tag_resource_definition* tag_resource_definition);
bool __fastcall tag_resource_definition_streamed(const s_tag_resource_definition* tag_resource_definition);
bool __fastcall tag_resource_definition_required(const s_tag_resource_definition* tag_resource_definition);
bool __fastcall tag_resource_definition_is_required_in_manifest(const s_tag_resource_definition* tag_resource_definition);

/* ---------- globals */

#endif // __TAG_RESOURCE_INTERNALS_H__