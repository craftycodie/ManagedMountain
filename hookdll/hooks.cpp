#include "hookdll/hooks.h"

#include "common/logging.h"
#include "memory/module.h"

#include <Windows.h>
#include <detours.h>

#include <Psapi.h>

#include <cstdint>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace
{
using CreateFileWFn = decltype(&::CreateFileW);

CreateFileWFn g_original_create_file_w = nullptr;
PVOID g_create_file_detour_target = nullptr;

std::string PointerToString(const void* pointer)
{
	std::ostringstream stream;
	stream << pointer;
	return stream.str();
}

std::wstring SafeWide(std::wstring_view value)
{
	return value.empty() ? L"<empty>" : std::wstring(value);
}

std::vector<int> ParsePattern(std::string_view pattern)
{
	std::vector<int> bytes;
	std::istringstream stream{std::string(pattern)};
	std::string token;

	while (stream >> token)
	{
		if (token == "?" || token == "??")
		{
			bytes.push_back(-1);
			continue;
		}

		bytes.push_back(std::stoi(token, nullptr, 16));
	}

	return bytes;
}

void* ResolveExportTarget(const wchar_t* module_name, const char* export_name)
{
	HMODULE module = GetModuleHandleW(module_name);
	if (module == nullptr)
	{
		module = LoadLibraryW(module_name);
	}

	return module == nullptr ? nullptr : reinterpret_cast<void*>(GetProcAddress(module, export_name));
}

HANDLE WINAPI DetourCreateFileW(
	LPCWSTR file_name,
	DWORD desired_access,
	DWORD share_mode,
	LPSECURITY_ATTRIBUTES security_attributes,
	DWORD creation_disposition,
	DWORD flags_and_attributes,
	HANDLE template_file)
{
	if (g_original_create_file_w == nullptr)
	{
		return INVALID_HANDLE_VALUE;
	}

	std::wstringstream stream;
	stream << L"CreateFileW(\"" << SafeWide(file_name == nullptr ? L"<null>" : std::wstring_view(file_name))
		   << L"\", access=0x" << std::hex << desired_access << L", disposition=0x" << creation_disposition
		   << L")";
	logging::Log(stream.str());

	return g_original_create_file_w(
		file_name,
		desired_access,
		share_mode,
		security_attributes,
		creation_disposition,
		flags_and_attributes,
		template_file);
}
} // namespace

namespace hooks
{
bool InstallAll()
{
	set_mountain_module(GetModuleHandleW(nullptr));

	void* create_file_export = ResolveExportTarget(L"KernelBase.dll", "CreateFileW");
	if (create_file_export == nullptr)
	{
		create_file_export = ResolveExportTarget(L"kernel32.dll", "CreateFileW");
	}

	if (create_file_export == nullptr)
	{
		logging::Log("Failed to resolve CreateFileW for proof hook");
		return false;
	}

	g_create_file_detour_target = create_file_export;

	if (DetourTransactionBegin() != NO_ERROR)
	{
		logging::Log("DetourTransactionBegin failed (CreateFileW)");
		return false;
	}

	if (DetourUpdateThread(GetCurrentThread()) != NO_ERROR)
	{
		DetourTransactionAbort();
		logging::Log("DetourUpdateThread failed (CreateFileW)");
		return false;
	}

	if (DetourAttach(
			&g_create_file_detour_target,
			reinterpret_cast<PVOID>(static_cast<void*>(&DetourCreateFileW)))
		!= NO_ERROR)
	{
		DetourTransactionAbort();
		logging::Log("DetourAttach(CreateFileW) failed");
		return false;
	}

	if (DetourTransactionCommit() != NO_ERROR)
	{
		logging::Log("DetourTransactionCommit failed (CreateFileW)");
		return false;
	}

	g_original_create_file_w = reinterpret_cast<CreateFileWFn>(g_create_file_detour_target);
	logging::Log(std::string("CreateFileW hooked, trampoline at ") + PointerToString(g_original_create_file_w));

	HMODULE main_module = GetModuleHandleW(nullptr);
	logging::Log("Main module base: " + PointerToString(main_module));
	logging::Log("Sample RVA 0x1000 resolves to " + PointerToString(ResolveTargetByRva(main_module, 0x1000)));

	apply_all_hooks(false);
	apply_all_patches(false);

	return true;
}

void RemoveAll()
{
	if (g_create_file_detour_target != nullptr && g_original_create_file_w != nullptr)
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(
			&g_create_file_detour_target,
			reinterpret_cast<PVOID>(static_cast<void*>(&DetourCreateFileW)));
		DetourTransactionCommit();
	}

	g_original_create_file_w = nullptr;
	g_create_file_detour_target = nullptr;

	apply_all_hooks(true);
	apply_all_patches(true);
}

void* ResolveTargetByRva(HMODULE module, std::uintptr_t rva)
{
	if (module == nullptr)
	{
		return nullptr;
	}

	return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(module) + rva);
}

void* ResolveTargetByPattern(HMODULE module, std::string_view pattern)
{
	if (module == nullptr || pattern.empty())
	{
		return nullptr;
	}

	const auto parsed_pattern = ParsePattern(pattern);
	if (parsed_pattern.empty())
	{
		return nullptr;
	}

	MODULEINFO module_info{};
	if (!GetModuleInformation(GetCurrentProcess(), module, &module_info, sizeof(module_info)))
	{
		return nullptr;
	}

	const auto* module_bytes = static_cast<const std::uint8_t*>(module_info.lpBaseOfDll);
	const std::size_t image_size = static_cast<std::size_t>(module_info.SizeOfImage);
	const std::size_t pattern_size = parsed_pattern.size();

	if (image_size < pattern_size)
	{
		return nullptr;
	}

	for (std::size_t i = 0; i <= image_size - pattern_size; ++i)
	{
		bool matched = true;
		for (std::size_t j = 0; j < pattern_size; ++j)
		{
			const int wanted = parsed_pattern[j];
			if (wanted != -1 && module_bytes[i + j] != static_cast<std::uint8_t>(wanted))
			{
				matched = false;
				break;
			}
		}

		if (matched)
		{
			return const_cast<std::uint8_t*>(module_bytes + i);
		}
	}

	return nullptr;
}
} // namespace hooks
