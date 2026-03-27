#include "common/logging.h"

#include <Windows.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace
{
std::wstring Quote(const std::wstring& value)
{
    return L"\"" + value + L"\"";
}

std::filesystem::path GetCurrentExecutablePath()
{
    std::wstring path(MAX_PATH, L'\0');
    const DWORD length = GetModuleFileNameW(nullptr, path.data(), static_cast<DWORD>(path.size()));
    path.resize(length);
    return path;
}

std::filesystem::path FindProjectRoot(const std::filesystem::path& start_directory)
{
    for (auto directory = start_directory;
         !directory.empty() && directory != directory.root_path();
         directory = directory.parent_path())
    {
        const auto tests_expected_dir = directory / L"tests" / L"expected";
        const auto legacy_expected_file = directory / L"tests" / L"expected.xml";
        if (std::filesystem::is_directory(tests_expected_dir) || std::filesystem::exists(legacy_expected_file))
        {
            return directory;
        }
    }

    return {};
}

std::wstring JoinLauncherCommandLine(const std::filesystem::path& launcher_exe, const std::vector<std::wstring>& args)
{
    std::wstring line = Quote(launcher_exe.wstring());
    for (const std::wstring& argument : args)
    {
        line.append(L" ");
        if (argument.find_first_of(L" \t") != std::wstring::npos)
        {
            line.append(Quote(argument));
        }
        else
        {
            line.append(argument);
        }
    }

    return line;
}

DWORD RunProcess(
    const std::wstring& command_line,
    const std::filesystem::path& working_directory)
{
    std::vector<wchar_t> mutable_command_line(command_line.begin(), command_line.end());
    mutable_command_line.push_back(L'\0');

    STARTUPINFOW startup_info{};
    startup_info.cb = sizeof(startup_info);

    PROCESS_INFORMATION process_info{};

    const BOOL create_ok = CreateProcessW(
        nullptr,
        mutable_command_line.data(),
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        working_directory.wstring().c_str(),
        &startup_info,
        &process_info);

    if (!create_ok)
    {
        const DWORD last_error = GetLastError();
        std::wcerr << L"CreateProcessW failed: " << last_error << L"\n";
        logging::Log("CreateProcessW failed");
        return static_cast<DWORD>(-1);
    }

    WaitForSingleObject(process_info.hProcess, INFINITE);

    DWORD exit_code = 0;
    GetExitCodeProcess(process_info.hProcess, &exit_code);

    CloseHandle(process_info.hThread);
    CloseHandle(process_info.hProcess);
    return exit_code;
}

std::filesystem::path BuildZoneRelativePath(const std::wstring& map_name)
{
    return std::filesystem::path(L"cache_builder") / (map_name + L".cache_file_resource_gestalt");
}

bool move_zone_to_tags_directory(const std::filesystem::path& tool_root, const std::wstring& map_name)
{
    const std::filesystem::path built_zone_relative_path = 
        std::filesystem::path(L"cache_builder") / (map_name + L".cache_file_resource_gestalt");
    const std::filesystem::path source_tag = tool_root / built_zone_relative_path;
    const std::filesystem::path target_tag = tool_root / L"tags" / built_zone_relative_path;

    if (!std::filesystem::exists(source_tag))
    {
        std::wcerr << L"Source tag not found: " << source_tag.wstring() << L"\n";
        return false;
    }

    const std::filesystem::path target_dir = target_tag.parent_path();
    std::error_code create_error;
    std::filesystem::create_directories(target_dir, create_error);
    if (create_error)
    {
        std::wcerr << L"Failed to create directory: " << target_dir.wstring() << L"\n";
        return false;
    }

    std::error_code copy_error;
    std::filesystem::copy_file(source_tag, target_tag, std::filesystem::copy_options::overwrite_existing, copy_error);
    if (copy_error)
    {
        std::cerr << "Copy failed: " << copy_error.message() << "\n";
        return false;
    }

    return true;
}

bool CompareLargeTextFiles(const std::filesystem::path& expected_path, const std::filesystem::path& actual_path)
{
    std::ifstream expected_file(expected_path);
    std::ifstream actual_file(actual_path);

    if (!expected_file || !actual_file)
    {
        std::wcerr << L"Failed to open files for comparison.\n";
        return false;
    }

    std::string expected_line;
    std::string actual_line;
    int line_number = 0;
    int difference_count = 0;
    int samples_printed = 0;
    constexpr int kMaxSamples = 15;

    auto truncate = [](const std::string& line) -> std::string
    {
        constexpr size_t kMax = 160;
        if (line.size() <= kMax)
        {
            return line;
        }

        return line.substr(0, kMax) + "...";
    };

    while (true)
    {
        const bool expected_ok = static_cast<bool>(std::getline(expected_file, expected_line));
        const bool actual_ok = static_cast<bool>(std::getline(actual_file, actual_line));

        if (!expected_ok && !actual_ok)
        {
            break;
        }

        ++line_number;

        const std::string expected_text = expected_ok ? expected_line : std::string();
        const std::string actual_text = actual_ok ? actual_line : std::string();

        if (expected_text != actual_text)
        {
            ++difference_count;
            if (samples_printed < kMaxSamples)
            {
                std::cout << "Line " << line_number << "\n";
                std::cout << "  expected: " << truncate(expected_ok ? expected_line : std::string("<EOF>")) << "\n";
                std::cout << "  actual:   " << truncate(actual_ok ? actual_line : std::string("<EOF>")) << "\n";
                ++samples_printed;
            }
        }
    }

    if (difference_count == 0)
    {
        std::cout << "XML files are identical.\n";
        return true;
    }

    std::cout << "Differences found. Differing lines: " << difference_count << "\n";
    return false;
}

struct BuildCacheFileTestCase
{
    const wchar_t* map_path;
    const wchar_t* map_name;
};

const BuildCacheFileTestCase kBuildCacheFileTests[] = {
    {L"levels\\ui\\mainmenu\\mainmenu", L"mainmenu"},
};

} // namespace

int wmain(int argc, wchar_t** argv)
{
    logging::Initialize(L"tool_hook_tests");
    logging::Log(L"tests log path: " + logging::GetLogPath().wstring());

    if (argc < 2 || argv[1] == nullptr || argv[1][0] == L'\0')
    {
        std::wcerr << L"Usage: tool_hook_tests <path to H3EK directory (folder containing tool.exe)>\n";
        logging::Shutdown();
        return 1;
    }

    const std::filesystem::path tool_root = std::filesystem::path(argv[1]).lexically_normal();
    const std::filesystem::path tool_exe = tool_root / L"tool.exe";

    if (!std::filesystem::is_directory(tool_root))
    {
        std::wcerr << L"Not a directory: " << tool_root.wstring() << L"\n";
        logging::Shutdown();
        return 1;
    }

    if (!std::filesystem::exists(tool_exe))
    {
        std::wcerr << L"tool.exe not found: " << tool_exe.wstring() << L"\n";
        logging::Shutdown();
        return 1;
    }

    logging::Log(L"H3EK root: " + tool_root.wstring());
    logging::Log(L"tool.exe: " + tool_exe.wstring());

    const std::filesystem::path executable_directory = GetCurrentExecutablePath().parent_path();
    const std::filesystem::path launcher_exe = executable_directory / L"tool_launcher.exe";

    if (!std::filesystem::exists(launcher_exe))
    {
        std::wcerr << L"tool_launcher.exe not found next to this executable: " << launcher_exe << L"\n";
        logging::Shutdown();
        return 1;
    }

    const std::filesystem::path project_root = FindProjectRoot(executable_directory);
    if (project_root.empty())
    {
        std::wcerr << L"Could not find project root (tests\\expected\\ or tests\\expected.xml). Run from build output or set up repo layout.\n";
        logging::Shutdown();
        return 1;
    }

    logging::Log(L"Project root: " + project_root.wstring());

    const BuildCacheFileTestCase* last_test = nullptr;
    for (const BuildCacheFileTestCase& test : kBuildCacheFileTests)
    {
        std::wcout << L"\n== build-cache-file: " << test.map_path << L" ==\n";

        const std::vector<std::wstring> launcher_args = {
            tool_exe.wstring(),
            L"build-cache-file",
            test.map_path,
        };

        const std::wstring launcher_cmd = JoinLauncherCommandLine(launcher_exe, launcher_args);
        logging::Log(std::wstring(L"Launcher command: ") + launcher_cmd);

        const DWORD build_exit = RunProcess(launcher_cmd, executable_directory);
        if (build_exit != 0)
        {
            std::wcerr << L"tool_launcher (build-cache-file) failed with exit code " << build_exit << L"\n";
            logging::Shutdown();
            return static_cast<int>(build_exit);
        }

        std::wcout << L"Build step exited with code " << build_exit << L"\n";
        last_test = &test;
    }

    if (last_test == nullptr)
    {
        std::wcerr << L"No tests configured.\n";
        logging::Shutdown();
        return 1;
    }

    const std::wstring map_name = last_test->map_name;
    const std::filesystem::path expected_xml = project_root / L"tests" / L"expected" / (map_name + L".xml");
    const std::filesystem::path actual_xml = project_root / L"tests" / L"actual" / (map_name + L".xml");

    if (!std::filesystem::exists(expected_xml))
    {
        std::wcerr << L"Expected baseline XML not found: " << expected_xml.wstring() << L"\n";
        logging::Shutdown();
        return 1;
    }

    {
        std::error_code create_dirs_error;
        std::filesystem::create_directories(actual_xml.parent_path(), create_dirs_error);
        if (create_dirs_error)
        {
            std::wcerr << L"Failed to create directory: " << actual_xml.parent_path().wstring() << L"\n";
            logging::Shutdown();
            return 1;
        }
    }

    if (std::filesystem::exists(actual_xml))
    {
        std::filesystem::remove(actual_xml);
    }
    const std::filesystem::path zone_relative_path = BuildZoneRelativePath(map_name);
    const std::filesystem::path zone_tag_path = std::filesystem::path(L"\\tags") / zone_relative_path;

    std::wcout << L"\n== export-tag-to-xml ==\n";

    if (!move_zone_to_tags_directory(tool_root, map_name))
    {
        logging::Shutdown();
        return 1;
    }

    {
        const std::wstring export_cmd =
            Quote(tool_exe.wstring()) + L" export-tag-to-xml " + Quote(zone_tag_path.wstring()) + L" " +
            Quote(actual_xml.wstring());

        logging::Log(std::wstring(L"Export command: ") + export_cmd);

        const DWORD export_exit = RunProcess(export_cmd, tool_root);
        if (export_exit != 0)
        {
            std::wcerr << L"tool.exe export-tag-to-xml failed with exit code " << export_exit << L"\n";
            logging::Shutdown();
            return static_cast<int>(export_exit);
        }
    }

    if (!std::filesystem::exists(actual_xml))
    {
        std::wcerr << L"Actual XML was not created: " << actual_xml.wstring() << L"\n";
        logging::Shutdown();
        return 1;
    }

    std::wcout << L"\n== compare actual vs expected (" << map_name << L".xml) ==\n";

    const bool match = CompareLargeTextFiles(expected_xml, actual_xml);
    logging::Shutdown();
    return match ? 0 : 2;
}
