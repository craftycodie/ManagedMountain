#ifndef __CACHE_FILE_UNCOMPRESSED_CODEC_H__
#define __CACHE_FILE_UNCOMPRESSED_CODEC_H__
#pragma once

#include "cseries/types.h"
#include "tag_files/tag_resource_internals.h"

struct c_cache_file_decompressor_service;
struct c_cache_file_compressor_service;

class c_cache_file_builder_codec
{
public:
	virtual char const* get_name() = 0;
	virtual s_tag_persistent_identifier get_identifier() = 0;
	virtual c_cache_file_decompressor_service* get_decompressor_service() = 0;
	virtual c_cache_file_compressor_service* get_compressor_service() = 0;

protected:
	~c_cache_file_builder_codec() = default;
};

static_assert(sizeof(c_cache_file_builder_codec) == 8);

#endif // __CACHE_FILE_UNCOMPRESSED_CODEC_H__
