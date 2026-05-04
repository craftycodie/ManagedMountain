#ifndef __CACHE_FILE_BUILDER_WRITER_H__
#define __CACHE_FILE_BUILDER_WRITER_H__
#pragma once

#include "cseries/types.h"

class c_cache_file_builder_writer_default;

struct c_cache_file_builder_writer_default_vtbl
{
	void (*scalar_deleting_destructor)(c_cache_file_builder_writer_default*, uns64);
	char const* (*build_path)(c_cache_file_builder_writer_default*, void* buffer, int mode);
	void (*fn_02)(c_cache_file_builder_writer_default*);
	void (*fn_03)(c_cache_file_builder_writer_default*);
	void (*fn_04)(c_cache_file_builder_writer_default*);
	void (*fn_05)(c_cache_file_builder_writer_default*);
	void (*fn_06)(c_cache_file_builder_writer_default*);
	void (*fn_07)(c_cache_file_builder_writer_default*);
	void (*fn_08)(c_cache_file_builder_writer_default*);
	void (*fn_09)(c_cache_file_builder_writer_default*);
	void (*fn_0A)(c_cache_file_builder_writer_default*);
};

class c_cache_file_builder_writer
{
public:
	c_cache_file_builder_writer_default_vtbl const* vftable;
};

class c_cache_file_builder_writer_default :
	public c_cache_file_builder_writer
{
};

#endif // __CACHE_FILE_BUILDER_WRITER_H__
