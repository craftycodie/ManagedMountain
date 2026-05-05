#ifndef __TAG_RESOURCE_STREAMING_OUTPUT_STREAM_H__
#define __TAG_RESOURCE_STREAMING_OUTPUT_STREAM_H__
#pragma once

/* ---------- headers */

#include "tag_files/tag_groups.h"

/* ---------- definitions */

struct s_tag_resource_streaming_sublocation;

template<typename t_element>
class c_output_stream;

template<typename t_element>
class c_null_output_stream;

// H3EK `c_output_stream<s_tag_resource_streaming_sublocation>`: MSVC layout vptr + `m_tag_block` (+0x8).
// Vtable slots [0],[1] match IDA `sub_1408EE590` / `sub_1408F54F0` on `??_7?$c_output_stream@Us_tag_resource_streaming_sublocation@@@@6B@`.
template<>
class c_output_stream<s_tag_resource_streaming_sublocation>
{
	friend class c_null_output_stream<s_tag_resource_streaming_sublocation>;

public:
	explicit c_output_stream(s_tag_block* tag_block);

	c_output_stream(c_output_stream const&) = delete;
	c_output_stream& operator=(c_output_stream const&) = delete;

	~c_output_stream() = default;

	// Declared first so vtable slots [0],[1] match H3EK `sub_1408EE590` / `sub_1408F54F0` (no leading virtual dtor).
	virtual bool streaming_output_capacity_allows_adding(int element_count) const;
	virtual void* streaming_output_append_row_from_qword(__int64 const* row_bits);

protected:
	s_tag_block* m_tag_block;

private:
	// `c_null_output_stream` only — no assert on `tag_block`.
	c_output_stream(s_tag_block* tag_block, int /*null_stream_ctor_tag*/);
};

// H3EK `c_null_output_stream<s_tag_resource_streaming_sublocation>` — `??_7?$c_null_output_stream@Us_tag_resource_streaming_sublocation@@@@6B@`.
template<>
class c_null_output_stream<s_tag_resource_streaming_sublocation> : public c_output_stream<s_tag_resource_streaming_sublocation>
{
public:
	c_null_output_stream();

	c_null_output_stream(c_null_output_stream const&) = delete;
	c_null_output_stream& operator=(c_null_output_stream const&) = delete;

	bool streaming_output_capacity_allows_adding(int element_count) const override;
	void* streaming_output_append_row_from_qword(__int64 const* row_bits) override;
};

#endif // __TAG_RESOURCE_STREAMING_OUTPUT_STREAM_H__
