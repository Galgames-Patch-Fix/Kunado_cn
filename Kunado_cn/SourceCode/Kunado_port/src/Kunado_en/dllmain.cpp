#include <Windows.h>
#include "../../lib/RxHook/RxHook.h"
#include "../../lib/CMVSTools/FileHook.h"


static DWORD sg_dwExeBase = (DWORD)GetModuleHandleW(NULL);


typedef void(__thiscall* Fn_CMVS_VM_DrawTextBox_Fixed_Sub)(uint32_t* This, char* cpStr, uint32_t* pUn0);
static Fn_CMVS_VM_DrawTextBox_Fixed_Sub sg_fnCMVS_VM_DrawTextBox_Fixed_Sub = nullptr;

typedef uint32_t(__thiscall* Fn_CMVS_VM_DrawTextBox_Sub)(uint32_t* This, char* cpStr, uint32_t* uiPosX_Ret, uint32_t* uiLines_Ret, uint32_t* uiPosY_Ret);
static Fn_CMVS_VM_DrawTextBox_Sub sg_fnCMVS_VM_DrawTextBox_Sub = nullptr;

typedef uint32_t(__thiscall* Fn_CMVS_VM_DrawBackLog_Sub)(uint32_t* This, char* cpStr, uint32_t* uiPosX_Ret, uint32_t* uiLines_Ret, uint32_t* uiPosY_Ret);
static Fn_CMVS_VM_DrawBackLog_Sub sg_fnCMVS_VM_DrawBackLog_Sub = nullptr;


void __fastcall CMVS_VM_DrawTextBox_Fixed_Sub_Hook(uint32_t* This, uint32_t* uiEDX, char* cpStr, uint32_t* pUn0)
{
	// this + 0x7 = font_size
	*(This + 0x7) = 23;
	sg_fnCMVS_VM_DrawTextBox_Fixed_Sub(This, cpStr, pUn0);
}

uint32_t __fastcall CMVS_VM_DrawTextBox_Sub_Hook(uint32_t* This, uint32_t* uiEDX, char* cpStr, uint32_t* uiPosX_Ret, uint32_t* uiLines_Ret, uint32_t* uiPosY_Ret)
{
	// this + 0x7 = font_size
	// this + 0x1 = indentation
	*(This + 0x7) = 23;
	return sg_fnCMVS_VM_DrawTextBox_Sub(This, cpStr, uiPosX_Ret, uiLines_Ret, uiPosY_Ret);
}

uint32_t __fastcall CMVS_VM_DrawBackLog_Sub_Hook(uint32_t* This, uint32_t* uiEDX, char* cpStr, uint32_t* uiPosX_Ret, uint32_t* uiLines_Ret, uint32_t* uiPosY_Ret)
{
	// this + 0xB = font_size
	*(This + 0xB) = 22;
	return sg_fnCMVS_VM_DrawBackLog_Sub(This, cpStr, uiPosX_Ret, uiLines_Ret, uiPosY_Ret);
}

void FontHook()
{
	Rut::RxHook::HookCreateFontA(0x80, "MS Gothic");

	sg_fnCMVS_VM_DrawBackLog_Sub = (Fn_CMVS_VM_DrawBackLog_Sub)(sg_dwExeBase + 0x39080);
	sg_fnCMVS_VM_DrawTextBox_Sub = (Fn_CMVS_VM_DrawTextBox_Sub)(sg_dwExeBase + 0x63890);
	sg_fnCMVS_VM_DrawTextBox_Fixed_Sub = (Fn_CMVS_VM_DrawTextBox_Fixed_Sub)(sg_dwExeBase + 0x64A60);

	Rut::RxHook::DetourAttachFunc(&sg_fnCMVS_VM_DrawBackLog_Sub, CMVS_VM_DrawBackLog_Sub_Hook);
	Rut::RxHook::DetourAttachFunc(&sg_fnCMVS_VM_DrawTextBox_Sub, CMVS_VM_DrawTextBox_Sub_Hook);
	Rut::RxHook::DetourAttachFunc(&sg_fnCMVS_VM_DrawTextBox_Fixed_Sub, CMVS_VM_DrawTextBox_Fixed_Sub_Hook);
}

VOID StartHook()
{
	FontHook();

	CMVS::FileHook::SetPS3Hook_380_(sg_dwExeBase + 0x6CE10, sg_dwExeBase + 0x6CEF1, sg_dwExeBase + 0x6CF1D);
	CMVS::FileHook::SetPB3Hook_380_(sg_dwExeBase + 0x3FE8D);

	uint8_t aTitleFix[] = { 0x20,0x20 };
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0xD1DD4), (void*)aTitleFix, sizeof(aTitleFix));

	uint8_t aBoot[] = "Settings";
	Rut::RxHook::SysMemWrite((void*)(sg_dwExeBase + 0xD2168), aBoot, sizeof(aBoot));

	uint8_t aStan[] = "[DF]";
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