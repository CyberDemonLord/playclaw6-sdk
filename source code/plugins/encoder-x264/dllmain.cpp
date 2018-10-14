#include "stdafx.h"
#include "../plugin-api.h"
#include "encoder-x264.h"

static HMODULE hPluginInstance = 0;

HMODULE LoadX264()
{
	wchar_t szMyPath[MAX_PATH];
	GetModuleFileName(hPluginInstance, szMyPath, MAX_PATH);
	PathRemoveFileSpec(szMyPath);
	wcscat_s(szMyPath, L"\\x264\\libx264.dll");
	return LoadLibraryEx(szMyPath, 0, LOAD_WITH_ALTERED_SEARCH_PATH);
}

BOOL IsAvailableX264()
{
	HMODULE lib = LoadX264();
	if (lib)
		FreeLibrary(lib);
	return lib!= 0;
}




extern "C" __declspec(dllexport) BOOL Plugin_Load()
{
	GH_RegisterPlugin(&CVideoEncoderX264::plugin_desc);

	return TRUE;
}

extern "C" __declspec(dllexport) void Plugin_Unload()
{

}


BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hPluginInstance = hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

