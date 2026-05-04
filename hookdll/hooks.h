#pragma once

#include <Windows.h>

#include "cseries/types.h"

#include <string_view>

namespace hooks
{
bool InstallAll();
void RemoveAll();

constexpr uns64 kToolImageBase = 0x140000000ull;

constexpr uns64 AddressToRva(uns64 address)
{
	return address >= kToolImageBase ? address - kToolImageBase : address;
}

void* ResolveTargetByRva(HMODULE module, uns64 rva);
void* ResolveTargetByPattern(HMODULE module, std::string_view pattern);
}
