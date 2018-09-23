#pragma once



class CPluginTemplate
{
	CRITICAL_SECTION	m_csVars;

	virtual unsigned	GetConfigDialogResourceID() { return 0; };

	virtual BOOL		OnOK(HWND wnd) { return TRUE; };
	virtual void		OnCancel(HWND wnd) {};
	virtual void		OnCommand(HWND hwnd, unsigned id, unsigned notification) {};
	virtual void		OnInitDialog(HWND hwnd) {};
	virtual void		OnDrawItem(LPDRAWITEMSTRUCT lpDIS) {};
	virtual INT_PTR		OnNotify(HWND hwnd, NMHDR* lpNMHeader) { return 0; };

	virtual void		OnImGuiConfigure(void* param) {};

protected:
	void*				m_playclawdata;

	static void*		Create(void* instance);

	virtual void		SetDefaults() abstract;
	virtual void		SaveToFile(Json::Value& config) abstract;
	virtual void		LoadFromFile(const Json::Value& config) abstract;

	const char*			LocalizeStr8(const char* str);
	const char*			GetImguiLabel(const char* locid, const char* uniqueid);

public:
	CPluginTemplate(void* playclawdata);
	virtual ~CPluginTemplate();

	void				LockVars();
	void				UnlockVars();

	static int			Configure(void* instance, HWND hParentWnd);
	static void			Destroy(void *instance);
	static void			SetDefaults(void* instance);
	static void			LoadVars(void* instance, const Json::Value& json);
	static void			SaveVars(void* instance, Json::Value& json);
};


#define GH_PLUGIN_CREATE_TEMPLATE_EX(cppclassname)			\
		static void*		Create(void *playclawdata)	{ return CPluginTemplate::Create(new cppclassname(playclawdata)); };		\
		static GH_PLUGIN_DESC	plugin_desc;	\


#define GH_PLUGIN_DESC_TEMPLATE_EX(pluginType, cppclassname, classID, titleStr, flags, extra)	\
		GH_PLUGIN_DESC cppclassname::plugin_desc = {pluginType, classID, titleStr, flags, \
		cppclassname::Create,																				\
		CPluginTemplate::Destroy, CPluginTemplate::SetDefaults,			\
		CPluginTemplate::LoadVars,CPluginTemplate::SaveVars,			\
		CPluginTemplate::Configure,	\
		extra	\
};
