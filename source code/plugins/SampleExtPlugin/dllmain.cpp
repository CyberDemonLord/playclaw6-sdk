#include "stdafx.h"
#include "../plugin-api.h"
#include "sample-overlay.h"


extern "C" __declspec(dllexport) BOOL Plugin_Load()
{
	GH_RegisterPlugin(&CSampleOverlay::plugin_desc);

	return TRUE;
}

extern "C" __declspec(dllexport) void Plugin_Unload()
{

}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

