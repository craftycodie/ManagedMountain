#pragma once

#include <Windows.h>

#include <cstdint>
#include <string_view>

namespace hooks
{
bool InstallAll();
void RemoveAll();

constexpr std::uintptr_t kToolImageBase = 0x140000000ull;

constexpr std::uintptr_t AddressToRva(std::uintptr_t address)
{
	return address >= kToolImageBase ? address - kToolImageBase : address;
}

void* ResolveTargetByRva(HMODULE module, std::uintptr_t rva);
void* ResolveTargetByPattern(HMODULE module, std::string_view pattern);
}
