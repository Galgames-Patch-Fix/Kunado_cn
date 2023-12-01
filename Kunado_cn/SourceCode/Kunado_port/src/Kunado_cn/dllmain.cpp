#include <Windows.h>
#include "../../lib/RxHook/RxHook.h"
#include "../../lib/CMVSTools/FileHook.h"


static DWORD sg_dwExeBase = (DWORD)GetModuleHandleW(NULL);


VOID StartHook()
{
	Rut::RxHook::HookCreateFontA(0x86, "黑体");

	//CMVS::FileHook::SetPS3Hook_380_(sg_dwExeBase + 0x6CE10, sg_dwExeBase + 0x6CEF1, sg_dwExeBase + 0x6CF1D);
	//CMVS::FileHook::SetPB3Hook_380_(sg_dwExeBase + 0x3FE8D);

	BYTE aRange[] = {0xFE};
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0x18379), aRange, sizeof(aRange));
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0x19DB9), aRange, sizeof(aRange));
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0x1A0CB), aRange, sizeof(aRange));
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0x1A109), aRange, sizeof(aRange));
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0x1A0CB), aRange, sizeof(aRange));
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0x1A156), aRange, sizeof(aRange));
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0x1A1C6), aRange, sizeof(aRange));
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0x1A239), aRange, sizeof(aRange));
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0x1A34B), aRange, sizeof(aRange));
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0x1A429), aRange, sizeof(aRange));

	uint8_t patchFont[] = "黑体";
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0xD0C08), patchFont, sizeof(patchFont));
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0xE9298), patchFont, sizeof(patchFont));
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0xE9670), patchFont, sizeof(patchFont));
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0xE97B0), patchFont, sizeof(patchFont));

	uint8_t aTitleFix[] = { 0xA1,0xA1 };
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0xD1DD4), (void*)aTitleFix, sizeof(aTitleFix));

	uint8_t aBoot[] = "启动设置";
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0xD2168), aBoot, sizeof(aBoot));

	uint8_t aStan[] = "[标准]";
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0xD218C), aStan, sizeof(aStan));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		StartHook();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" VOID __declspec(dllexport) Dir_A() {}