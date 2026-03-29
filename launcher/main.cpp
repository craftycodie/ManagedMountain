#include "common/logging.h"

#include <Windows.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace
{
std::filesystem::path GetCurrentExecutablePath()
{
    std::wstring path(MAX_PATH, L'\0');
    const DWORD length = GetModuleFileNameW(nullptr, path.data(), static_cast<DWORD>(path.size()));
    path.resize(length);
    return path;
}

std::wstring JoinArgumentsFrom(int argc, wchar_t** argv, int start_index)
{
    std::wstring joined;
    for (int index = start_index; index < argc; ++index)
    {
        if (!joined.empty())
        {
            joined.append(L" ");
        }

        joined.append(argv[index]);
    }

    return joined;
}

bool InjectDll(HANDLE process_handle, const std::filesystem::path& dll_path)
{
    const std::wstring dll_path_string = dll_path.wstring();
    const SIZE_T buffer_size = (dll_path_string.size() + 1) * sizeof(wchar_t);

    LPVOID remote_buffer = VirtualAllocEx(
        process_handle,
        nullptr,
        buffer_size,
        MEM_RESERVE | MEM_COMMIT,
        PAGE_READWRITE);

    if (remote_buffer == nullptr)
    {
        logging::Log("VirtualAllocEx failed");
        return false;
    }

    SIZE_T bytes_written = 0;
    const BOOL write_ok = WriteProcessMemory(
        process_handle,
        remote_buffer,
        dll_path_string.c_str(),
        buffer_size,
        &bytes_written);

    if (!write_ok || bytes_written != buffer_size)
    {
        logging::Log("WriteProcessMemory failed");
        VirtualFreeEx(process_handle, remote_buffer, 0, MEM_RELEASE);
        return false;
    }

    HMODULE kernel32_module = GetModuleHandleW(L"kernel32.dll");
    auto* load_library_w = reinterpret_cast<LPTHREAD_START_ROUTINE>(
        GetProcAddress(kernel32_module, "LoadLibraryW"));

    if (load_library_w == nullptr)
    {
        logging::Log("GetProcAddress(LoadLibraryW) failed");
        VirtualFreeEx(process_handle, remote_buffer, 0, MEM_RELEASE);
        return false;
    }

    HANDLE remote_thread = CreateRemoteThread(
        process_handle,
        nullptr,
        0,
        load_library_w,
        remote_buffer,
        0,
        nullptr);

    if (remote_thread == nullptr)
    {
        logging::Log("CreateRemoteThread failed");
        VirtualFreeEx(process_handle, remote_buffer, 0, MEM_RELEASE);
        return false;
    }

    WaitForSingleObject(remote_thread, INFINITE);

    DWORD remote_module_handle = 0;
    GetExitCodeThread(remote_thread, &remote_module_handle);

    CloseHandle(remote_thread);
    VirtualFreeEx(process_handle, remote_buffer, 0, MEM_RELEASE);

    if (remote_module_handle == 0)
    {
        logging::Log("Remote LoadLibraryW returned null");
        return false;
    }

    logging::Log("Injected hook DLL successfully");
    return true;
}

DWORD RunToolWithForwardedArguments(
    const std::wstring& tool_path,
    const std::wstring& tool_arguments,
    const std::filesystem::path& tool_working_directory,
    const std::filesystem::path& hook_dll_path)
{
    std::wstring command_line = L"\"" + tool_path + L"\"";
    if (!tool_arguments.empty())
    {
        command_line.append(L" ");
        command_line.append(tool_arguments);
    }
    std::vector<wchar_t> mutable_command_line(command_line.begin(), command_line.end());
    mutable_command_line.push_back(L'\0');

    STARTUPINFOW startup_info{};
    startup_info.cb = sizeof(startup_info);

    PROCESS_INFORMATION process_info{};

    logging::Log(L"Starting target: " + tool_path);
    logging::Log(L"Arguments: " + tool_arguments);

    const bool no_suspend_mode = (IsDebuggerPresent() == TRUE);
    const DWORD creation_flags = no_suspend_mode ? 0 : CREATE_SUSPENDED;

    if (no_suspend_mode)
    {
        logging::Log("Debugger detected; creating target in running state");
    }

    const BOOL create_ok = CreateProcessW(
        tool_path.c_str(),
        mutable_command_line.data(),
        nullptr,
        nullptr,
        FALSE,
        creation_flags,
        nullptr,
        tool_working_directory.wstring().c_str(),
        &startup_info,
        &process_info);

    if (!create_ok)
    {
        const DWORD last_error = GetLastError();
        std::wcerr << L"CreateProcessW failed: " << last_error << L"\n";
        logging::Log("CreateProcessW failed");
        return 2;
    }

    if (no_suspend_mode)
    {
        logging::Log("Process created in running state");
    }
    else
    {
        logging::Log("Process created in suspended state");
    }

    if (!InjectDll(process_info.hProcess, hook_dll_path))
    {
        TerminateProcess(process_info.hProcess, 3);
        CloseHandle(process_info.hThread);
        CloseHandle(process_info.hProcess);
        return 3;
    }

    if (!no_suspend_mode)
    {
        ResumeThread(process_info.hThread);
        logging::Log("Resumed target main thread");
    }

    WaitForSingleObject(process_info.hProcess, INFINITE);

    DWORD exit_code = 0;
    GetExitCodeProcess(process_info.hProcess, &exit_code);
    logging::Log("Target exited");

    CloseHandle(process_info.hThread);
    CloseHandle(process_info.hProcess);
    return exit_code;
}
} // namespace

int wmain(int argc, wchar_t** argv)
{
    logging::Initialize(L"launcher");
    logging::Log(L"launcher log path: " + logging::GetLogPath().wstring());

    const std::filesystem::path launcher_directory = GetCurrentExecutablePath().parent_path();
    const std::filesystem::path hook_dll_path = launcher_directory / L"tool_hook.dll";

    if (!std::filesystem::exists(hook_dll_path))
    {
        std::wcerr << L"Missing hook DLL: " << hook_dll_path << L"\n";
        logging::Log(L"Missing hook DLL: " + hook_dll_path.wstring());
        return 1;
    }

    if (argc < 2)
    {
        std::wcerr << L"Usage: tool_launcher <path to tool.exe> [arguments passed to tool...]\n";
        logging::Log("Missing tool.exe path argument");
        logging::Shutdown();
        return 1;
    }

    const std::wstring tool_path = argv[1];
    const std::wstring tool_arguments = JoinArgumentsFrom(argc, argv, 2);
    const std::filesystem::path tool_working_directory = std::filesystem::path(tool_path).parent_path();

    const DWORD exit_code =
        RunToolWithForwardedArguments(tool_path, tool_arguments, tool_working_directory, hook_dll_path);

    logging::Shutdown();

    std::wcout << L"tool.exe exited with code " << exit_code << L"\n";
    return static_cast<int>(exit_code);
}
