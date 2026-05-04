#ifndef __CACHE_FILE_TAG_RESOURCE_CHECKSUM_H__
#define __CACHE_FILE_TAG_RESOURCE_CHECKSUM_H__
#pragma once

/* ---------- headers */

#include "cache/security_functions.h"

/* ---------- constants */

enum e_cache_file_resource_hash
{
    _cache_file_resource_hash_entire = 0,
    _cache_file_resource_hash_head_chunk,
    _cache_file_resource_hash_tail_chunk,
    k_cache_file_resource_hash_count,
};

/* ---------- definitions */

struct s_cache_file_resource_checksum
{
    unsigned long crc_value; // 0x0
    s_network_http_request_hash hashes[3]; // 0x4
};
static_assert(sizeof(s_cache_file_resource_checksum) == 64);

class c_cache_file_resource_checksum_incremental
{
public:
    c_cache_file_resource_checksum_incremental();
    bool begin();
    void update(void const *buffer, unsigned long buffer_size);
    bool finish(s_cache_file_resource_checksum *checksum);
private:
    static long const k_working_memory_size;
    bool m_active; // 0x0
    unsigned long long m_bytes_processed; // 0x8
    unsigned long m_crc_value; // 0x10
    unsigned char m_hash_working_memory[88]; // 0x14
    unsigned char m_head_chunk[1024]; // 0x6C
    unsigned char m_tail_chunk[1024]; // 0x46C
};
static_assert(sizeof(c_cache_file_resource_checksum_incremental) == 2160);

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __CACHE_FILE_TAG_RESOURCE_CHECKSUM_H__
