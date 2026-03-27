#pragma once

#include <filesystem>
#include <string_view>

namespace logging
{
bool Initialize(std::wstring_view session_name);
void Shutdown();

void Log(std::string_view message);
void Log(std::wstring_view message);

std::filesystem::path GetLogPath();
}
