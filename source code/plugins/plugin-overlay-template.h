#pragma once

#include "plugin-template.h"
#include "plugin-api.h"

class CPluginOverlayTemplate: public CPluginTemplate
{
protected:

	virtual void		UpdateOverlay() abstract;

	virtual unsigned	GetInteractiveModeFPS() { return 1; };
	virtual BOOL		SetInteractiveMode(BOOL bEnable) { return FALSE; };
	virtual LRESULT		OnMouseMove(int	x, int y, unsigned modifiers) { return 1; };
	virtual LRESULT		OnMouseWheel(int x, int y, int distance, unsigned modifiers) { return 1; };
	virtual LRESULT		OnMouseButtonDown(int x, int y, int button, unsigned modifiers) { return 1; };
	virtual LRESULT		OnMouseButtonUp(int x, int y, int button, unsigned modifiers) { return 1; };
	virtual LRESULT		OnKeyDown(unsigned vkey, unsigned scancode) { return 1; };
	virtual LRESULT		OnKeyUp(unsigned vkey, unsigned scancode) { return 1; };
	virtual LPCTSTR		OnSetCursor() { return NULL; };
public:
	CPluginOverlayTemplate(void* playclawdata);

	static void			Update(void* instance);

	static unsigned		GetInteractiveModeFPS(void* instance);
	static BOOL			SetInteractiveMode(void* instance, BOOL bEnable);
	static LRESULT		OnMouseMove(void* instance, int	x, int y, unsigned modifiers);
	static LRESULT		OnMouseWheel(void* instance, int x, int y, int distance, unsigned modifiers);
	static LRESULT		OnMouseButtonDown(void* instance, int x, int y, int button, unsigned modifiers);
	static LRESULT		OnMouseButtonUp(void* instance, int x, int y, int button, unsigned modifiers);
	static LRESULT		OnKeyDown(void* instance, unsigned vkey, unsigned scancode);
	static LRESULT		OnKeyUp(void* instance, unsigned vkey, unsigned scancode);
	static LPCTSTR		OnSetCursor(void* instance);
};

#define GH_PLUGIN_OVERLAY_CREATE_TEMPLATE(cppclassname, dlgresourceid)	\
		GH_PLUGIN_CREATE_TEMPLATE_EX(cppclassname)			\
		unsigned GetConfigDialogResourceID() override {return dlgresourceid;}; \
		static GH_OVERLAY_EXTRA_DATA	overlay_desc;	\

#define GH_PLUGIN_OVERLAY_CREATE_TEMPLATE_IMGUI(cppclassname)	\
		GH_PLUGIN_CREATE_TEMPLATE_EX(cppclassname)				\
		void	OnImGuiConfigure(void* param) override;					\
		static GH_OVERLAY_EXTRA_DATA	overlay_desc;			\


#define GH_PLUGIN_OVERLAY_DESC_TEMPLATE(cppclassname, overlayClassID, titleStr, flags, maxfps)	\
		GH_OVERLAY_EXTRA_DATA cppclassname::overlay_desc = {maxfps, \
		CPluginOverlayTemplate::Update,					\
		CPluginOverlayTemplate::GetInteractiveModeFPS, CPluginOverlayTemplate::SetInteractiveMode, \
		CPluginOverlayTemplate::OnMouseMove, CPluginOverlayTemplate::OnMouseWheel, \
		CPluginOverlayTemplate::OnMouseButtonDown, CPluginOverlayTemplate::OnMouseButtonUp, \
		CPluginOverlayTemplate::OnKeyDown, CPluginOverlayTemplate::OnKeyUp, \
		CPluginOverlayTemplate::OnSetCursor	\
		};		\
		GH_PLUGIN_DESC_TEMPLATE_EX(GH_PLUGIN_TYPE_OVERLAY, cppclassname, overlayClassID, titleStr, flags, &cppclassname::overlay_desc);	\

