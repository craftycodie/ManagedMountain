#include "common/logging.h"

#include <Windows.h>

#include <array>
#include <filesystem>
#include <mutex>
#include <string>
#include <string_view>

namespace
{
std::mutex g_log_mutex;
HANDLE g_log_handle = INVALID_HANDLE_VALUE;
std::filesystem::path g_log_path;
std::wstring g_session_name;

std::string WideToUtf8(std::wstring_view input)
{
    if (input.empty())
    {
        return {};
    }

    const int required_size = WideCharToMultiByte(
        CP_UTF8,
        0,
        input.data(),
        static_cast<int>(input.size()),
        nullptr,
        0,
        nullptr,
        nullptr);

    if (required_size <= 0)
    {
        return {};
    }

    std::string output(static_cast<std::size_t>(required_size), '\0');
    WideCharToMultiByte(
        CP_UTF8,
        0,
        input.data(),
        static_cast<int>(input.size()),
        output.data(),
        required_size,
        nullptr,
        nullptr);
    return output;
}

std::string MakeTimestampPrefix()
{
    SYSTEMTIME system_time{};
    GetLocalTime(&system_time);

    char buffer[128]{};
    wsprintfA(
        buffer,
        "[%04u-%02u-%02u %02u:%02u:%02u.%03u][%ls] ",
        system_time.wYear,
        system_time.wMonth,
        system_time.wDay,
        system_time.wHour,
        system_time.wMinute,
        system_time.wSecond,
        system_time.wMilliseconds,
        g_session_name.c_str());
    return buffer;
}

void WriteRaw(std::string_view line)
{
    std::lock_guard<std::mutex> lock(g_log_mutex);

    const std::string debug_line(line);
    OutputDebugStringA(debug_line.c_str());

    HANDLE stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
    if (stderr_handle != nullptr && stderr_handle != INVALID_HANDLE_VALUE)
    {
        DWORD console_bytes_written = 0;
        WriteFile(
            stderr_handle,
            line.data(),
            static_cast<DWORD>(line.size()),
            &console_bytes_written,
            nullptr);
    }

    if (g_log_handle == INVALID_HANDLE_VALUE)
    {
        return;
    }

    DWORD bytes_written = 0;
    WriteFile(
        g_log_handle,
        line.data(),
        static_cast<DWORD>(line.size()),
        &bytes_written,
        nullptr);
}
} // namespace

namespace logging
{
bool Initialize(std::wstring_view session_name)
{
    std::lock_guard<std::mutex> lock(g_log_mutex);

    g_session_name.assign(session_name.begin(), session_name.end());
    g_log_path = GetLogPath();

    if (g_log_handle == INVALID_HANDLE_VALUE)
    {
        g_log_handle = CreateFileW(
            g_log_path.c_str(),
            FILE_APPEND_DATA,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);
    }

    if (g_log_handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    const std::string banner = MakeTimestampPrefix() + "logging initialized\r\n";
    DWORD bytes_written = 0;
    WriteFile(
        g_log_handle,
        banner.data(),
        static_cast<DWORD>(banner.size()),
        &bytes_written,
        nullptr);
    OutputDebugStringA(banner.c_str());

    return true;
}

void Shutdown()
{
    std::lock_guard<std::mutex> lock(g_log_mutex);

    if (g_log_handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(g_log_handle);
        g_log_handle = INVALID_HANDLE_VALUE;
    }
}

void Log(std::string_view message)
{
    const std::string line = MakeTimestampPrefix() + std::string(message) + "\r\n";
    WriteRaw(line);
}

void Log(std::wstring_view message)
{
    Log(WideToUtf8(message));
}

std::filesystem::path GetLogPath()
{
    std::array<wchar_t, MAX_PATH> temp_path{};
    const DWORD path_length = GetTempPathW(static_cast<DWORD>(temp_path.size()), temp_path.data());
    if (path_length == 0 || path_length > temp_path.size())
    {
        return std::filesystem::temp_directory_path() / "tool_hook_harness.log";
    }

    return std::filesystem::path(temp_path.data()) / "tool_hook_harness.log";
}
} // namespace logging
