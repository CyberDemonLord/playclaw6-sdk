#include "stdafx.h"
#include "plugin-template.h"
#include "plugin-api.h"


CPluginTemplate::CPluginTemplate(void* playclawdata) : m_playclawdata(playclawdata)
{
	InitializeCriticalSection(&m_csVars);
}

CPluginTemplate::~CPluginTemplate()
{
	DeleteCriticalSection(&m_csVars);
}

void CPluginTemplate::LockVars()
{
	EnterCriticalSection(&m_csVars);
}

void CPluginTemplate::UnlockVars()
{
	LeaveCriticalSection(&m_csVars);
}


void* CPluginTemplate::Create(void* instance)
{
	if (instance != NULL)
	{
		CPluginTemplate* baseclass = reinterpret_cast<CPluginTemplate*> (instance);
	}
	return instance;
}

void CPluginTemplate::Destroy(void *instance)
{
	if (instance != NULL)
	{
		CPluginTemplate* baseclass = reinterpret_cast<CPluginTemplate*> (instance);
		delete baseclass;
	}
}

int CPluginTemplate::Configure(void* instance, HWND hParentWnd)
{
	CPluginTemplate* baseclass = reinterpret_cast<CPluginTemplate*> (instance);
//	unsigned resid = baseclass->GetConfigDialogResourceID();
//	if (resid != 0)
//		return DialogBoxParam(0, MAKEINTRESOURCE(resid), hParentWnd, DlgProc, (LPARAM)baseclass);

	baseclass->OnImGuiConfigure((void*) hParentWnd);
	return 0;
}

void CPluginTemplate::SetDefaults(void* instance)
{
	if (instance != NULL)
	{
		CPluginTemplate* baseclass = reinterpret_cast<CPluginTemplate*> (instance);

		EnterCriticalSection(&baseclass->m_csVars);
		baseclass->SetDefaults();
		LeaveCriticalSection(&baseclass->m_csVars);
	}
}


void CPluginTemplate::LoadVars(void* instance, const Json::Value& json)
{
	if (instance != NULL)
	{
		CPluginTemplate* baseclass = reinterpret_cast<CPluginTemplate*> (instance);
		EnterCriticalSection(&baseclass->m_csVars);
		baseclass->LoadFromFile(json);
		LeaveCriticalSection(&baseclass->m_csVars);
	}
}

void CPluginTemplate::SaveVars(void* instance, Json::Value& json)
{
	if (instance != NULL)
	{
		CPluginTemplate* baseclass = reinterpret_cast<CPluginTemplate*> (instance);
		EnterCriticalSection(&baseclass->m_csVars);
		baseclass->SaveToFile(json);
		LeaveCriticalSection(&baseclass->m_csVars);
	}
}

const char* CPluginTemplate::LocalizeStr8(const char* str)
{
	return GH_LocalizeStr8(m_playclawdata, str);
}

// ONLY FOR SINGLE THREADING
const char* CPluginTemplate::GetImguiLabel(const char* locid, const char* uniqueid)
{
	static string label;

	label = "";
	if (locid && *locid)
		label += LocalizeStr8(locid);
	if (uniqueid && *uniqueid)
		label += uniqueid;
	return label.c_str();
}