#include "app/app.hpp"

DWORD WINAPI mainThread(LPVOID param)
{
	if (const int errc{ app::load() }) {
		MessageBoxA(0, std::format("failed to load ({})", errc).c_str(), "jb64", 0);
		FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(param), EXIT_FAILURE);
	}

	while (!GetAsyncKeyState(VK_F11)) {
		Sleep(500);
	}

	if (const int errc{ app::unload() }) {
		MessageBoxA(0, std::format("failed to unload ({})", errc).c_str(), "jb64", 0);
		FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(param), EXIT_FAILURE);
	}

	Sleep(500);

	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(param), EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID reserved)
{
	if (reason != DLL_PROCESS_ATTACH) {
		return FALSE;
	}

	const HANDLE main_thread{ CreateThread(0, 0, mainThread, inst, 0, 0) };

	if (!main_thread) {
		return FALSE;
	}

	CloseHandle(main_thread);

	return TRUE;
}