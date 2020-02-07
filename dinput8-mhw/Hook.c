#include <Windows.h>

#define HOOK_DLL_FILE_NAME TEXT("dinput8.dll")
#define HOOK_FUNCTION_NAME DirectInput8Create
#define HOOK_FUNCTION_ARGS HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter
#define HOOK_FUNCTION_CALL hinst, dwVersion, riidltf, ppvOut, punkOuter

typedef HRESULT(WINAPI* HOOK)(HOOK_FUNCTION_ARGS);
HOOK hook;

__declspec(dllexport) HRESULT WINAPI HOOK_FUNCTION_NAME(HOOK_FUNCTION_ARGS)
{
	return hook(HOOK_FUNCTION_CALL);
}

BOOL LinkFunctions(VOID)
{
	TCHAR tczSystemPath[MAX_PATH + 1 + 12 + 1];
	GetSystemDirectory(tczSystemPath, MAX_PATH);
	lstrcat(tczSystemPath, TEXT("\\"));
	lstrcat(tczSystemPath, HOOK_DLL_FILE_NAME);
	HMODULE hModule = LoadLibrary(tczSystemPath);
	if (!hModule)
		return FALSE;
	hook = (HOOK)GetProcAddress(hModule, "DirectInput8Create");
	if (!hook)
		return FALSE;
	return TRUE;
}
