#include <Windows.h>

#define BUILD_NUMBER_OFFSET	0x14307c298
#define BUILD_NUMBER_STRING	"402862"

#define TITLE TEXT("nativePC Loader")

BOOL LinkFunctions(VOID);

BOOL APIENTRY DllEntryPoint(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);

		if (!LinkFunctions()) {
			MessageBox(NULL, TEXT("Couldn't load original ")TEXT("dinput8.dll")TEXT("!"), TITLE, MB_OK);
			return FALSE;
		}

		// Check Version
		if (lstrcmpA((const char*)BUILD_NUMBER_OFFSET, BUILD_NUMBER_STRING) != 0) {
			MessageBox(NULL, TEXT("Unsupported Version"), TITLE, MB_OK);
			return TRUE;
		}

		// Fix nativePC
		// 74 44 45 31 c0 48 8d 93 f8 62 21 00 48 8b 02 48 85 c0 74 23 48 8b 4d 08 48 39 48 08 74 28
		BYTE* jz = (BYTE*)0x15ed87808;
		if (*jz == 0x74)
			*jz = 0xEB;
	}
	return TRUE;
}
