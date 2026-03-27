#include "hookdll/hooks.h"

#include "common/logging.h"

#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE module_handle, DWORD reason, LPVOID reserved)
{
	(void)module_handle;
	(void)reserved;

	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(module_handle);

		// Install before returning so LoadLibrary completes and the launcher resumes the tool
		// only after hooks are committed (avoids racing the main thread).
		if (!logging::Initialize(L"hookdll"))
		{
			return TRUE;
		}

		logging::Log("hookdll loaded");
		logging::Log(L"log file: " + logging::GetLogPath().wstring());

		if (!hooks::InstallAll())
		{
			logging::Log("hook installation failed");
			return TRUE;
		}

		logging::Log("hook installation finished");
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		hooks::RemoveAll();
		logging::Shutdown();
	}

	return TRUE;
}
