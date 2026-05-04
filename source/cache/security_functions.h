#ifndef __SECURITY_FUNCTIONS_H__
#define __SECURITY_FUNCTIONS_H__
#pragma once

/* ---------- headers */

/* ---------- constants */

enum e_secure_memory_region
{
    _secure_memory_region_cache_files = 0,
    _secure_memory_region_signature_manifest,
    k_secure_memory_region_count,
};

/* ---------- definitions */

struct s_network_http_request_hash
{
    unsigned char hash[20]; // 0x0
};
static_assert(sizeof(s_network_http_request_hash) == 20);

struct s_rsa_signature
{
    unsigned char signature[256]; // 0x0
};
static_assert(sizeof(s_rsa_signature) == 256);

struct s_simple_security_hash_workspace
{
    char working_memory[88]; // 0x0
};
static_assert(sizeof(s_simple_security_hash_workspace) == 88);

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

/* ---------- private code */

#endif // __SECURITY_FUNCTIONS_H__
