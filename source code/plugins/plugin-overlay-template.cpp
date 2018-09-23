#include "stdafx.h"
#include "plugin-overlay-template.h"

CPluginOverlayTemplate::CPluginOverlayTemplate(void* playclawdata) : CPluginTemplate(playclawdata)
{
}


void CPluginOverlayTemplate::Update(void* instance)
{
	CPluginOverlayTemplate* baseclass = reinterpret_cast<CPluginOverlayTemplate*> (instance);
	if (baseclass != NULL)
	{
		baseclass->LockVars();
		baseclass->UpdateOverlay();
		baseclass->UnlockVars();
	}
}


unsigned CPluginOverlayTemplate::GetInteractiveModeFPS(void* instance)
{
	if (instance != NULL)
	{
		CPluginOverlayTemplate* baseclass = reinterpret_cast<CPluginOverlayTemplate*> (instance);
		return baseclass->GetInteractiveModeFPS();
	}
	return 0;
}

BOOL CPluginOverlayTemplate::SetInteractiveMode(void* instance, BOOL bEnable)
{
	if (instance != NULL)
	{
		CPluginOverlayTemplate* baseclass = reinterpret_cast<CPluginOverlayTemplate*> (instance);
		return baseclass->SetInteractiveMode(bEnable);
	}
	return FALSE;
}

LRESULT CPluginOverlayTemplate::OnMouseMove(void* instance, int	x, int y, unsigned modifiers)
{
	CPluginOverlayTemplate* baseclass = reinterpret_cast<CPluginOverlayTemplate*> (instance);
	return baseclass != NULL ? baseclass->OnMouseMove(x, y, modifiers) : 1;
}

LRESULT CPluginOverlayTemplate::OnMouseWheel(void* instance, int x, int y, int distance, unsigned modifiers)
{
	CPluginOverlayTemplate* baseclass = reinterpret_cast<CPluginOverlayTemplate*> (instance);
	return baseclass != NULL ? baseclass->OnMouseWheel(x, y, distance, modifiers) : 1;
}

LRESULT CPluginOverlayTemplate::OnKeyDown(void* instance, unsigned vkey, unsigned scancode)
{
	CPluginOverlayTemplate* baseclass = reinterpret_cast<CPluginOverlayTemplate*> (instance);
	return baseclass != NULL ? baseclass->OnKeyDown(vkey, scancode) : 1;
}

LRESULT CPluginOverlayTemplate::OnKeyUp(void* instance, unsigned vkey, unsigned scancode)
{
	CPluginOverlayTemplate* baseclass = reinterpret_cast<CPluginOverlayTemplate*> (instance);
	return baseclass != NULL ? baseclass->OnKeyUp(vkey, scancode) : 1;
}

LRESULT CPluginOverlayTemplate::OnMouseButtonDown(void* instance, int x, int y, int button, unsigned modifiers)
{
	CPluginOverlayTemplate* baseclass = reinterpret_cast<CPluginOverlayTemplate*> (instance);
	return baseclass != NULL ? baseclass->OnMouseButtonDown(x, y, button, modifiers) : 1;
}

LRESULT CPluginOverlayTemplate::OnMouseButtonUp(void* instance, int x, int y, int button, unsigned modifiers)
{
	CPluginOverlayTemplate* baseclass = reinterpret_cast<CPluginOverlayTemplate*> (instance);
	return baseclass != NULL ? baseclass->OnMouseButtonUp(x, y, button, modifiers) : 1;
}

LPCTSTR CPluginOverlayTemplate::OnSetCursor(void* instance)
{
	CPluginOverlayTemplate* baseclass = reinterpret_cast<CPluginOverlayTemplate*> (instance);
	return baseclass != NULL ? baseclass->OnSetCursor() : NULL;
}