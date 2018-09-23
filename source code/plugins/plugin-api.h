#pragma once


#ifdef GAMEHOOK_APP
#define GAMEHOOK_API __declspec(dllexport)
#else
#define GAMEHOOK_API __declspec(dllimport)
#endif

#define PLUGIN_EXPORT extern "C" __declspec(dllexport)


//
//	Overlay description
//
struct GH_PLUGIN_DESC
{
	unsigned int	pluginType;

	// unique ID
	const char*		szClassID;

	// title or title lang ID
	const char*		szTitleStr;

	// flags
	unsigned int	flags;

	// - called to create a plugin instance (data)
	// - plugin should create its own structure or class and return unique pointer/number
	//   which will be used then by PlayClaw as unique realtime plugin id
	// - returning 0 will indicate error creating instance
	// - p_plugin is internal pointer which is used to call playclaw imports
	void*	(*CreatePlugin)(void *p_plugin);

	// - called to destroy an instance data
	// - instance is a unique pointer/number returned by CreatePlugin method
	void	(*DestroyPlugin)(void* instance);

	// - called to set default variables
	void	(*SetDefaults)(void* instance);

	// - load variables from json config
	void	(*LoadVars)(void* instance, const Json::Value& json);

	// - save variables to json config
	void	(*SaveVars)(void* instance, Json::Value& json);

	// - show configuration window (dialog)
	int		(*Configure)(void* instance, HWND hParentWnd);

	// additional plugin data (for every plugin type)
	void*	pExtraData;
};

// plugins types
enum
{
	GH_PLUGIN_TYPE_OVERLAY			= 1,
	GH_PLUGIN_TYPE_EXTENSION,
	GH_PLUGIN_TYPE_VIDEO_ENCODER,
};

//////////////////////////////////////////////////////////////////////////
//	plugins flags

// this overlay supports interactive mode
#define GH_FLAG_OVERLAY_INTERACTIVE_MODE	(1<<1)

// video encoder: this encoder produces h264 frames
#define GH_FLAG_VIDEO_ENCODER_H264			(1<<8)




// register overlay description
GAMEHOOK_API void GH_RegisterPlugin(const GH_PLUGIN_DESC* desc);


// overlays specific methods
GAMEHOOK_API unsigned GH_GetOverlayUpdateRate(void* playclawdata);



struct GH_OVERLAY_EXTRA_DATA
{
	// max overlay fps
	unsigned int	maxfps;

	// - called to update overlay image
	// - instance is a unique pointer/number returned by CreatePlugin method
	void	(*UpdateOverlay)(void* instance);

	// ----- interactive mode methods --------
	unsigned(*GetInteractiveModeFPS)(void* instance);
	BOOL	(*SetInteractiveMode)(void* instance, BOOL bEnable);
	LRESULT	(*OnMouseMove)(void* instance, int	x, int y, unsigned modifiers);
	LRESULT	(*OnMouseWheel)(void* instance, int x, int y, int distance, unsigned modifiers);
	LRESULT	(*OnMouseButtonDown)(void* instance, int x, int y, int button, unsigned modifiers);
	LRESULT	(*OnMouseButtonUp)(void* instance, int x, int y, int button, unsigned modifiers);
	LRESULT	(*OnKeyDown)(void* instance, unsigned vkey, unsigned character);
	LRESULT	(*OnKeyUp)(void* instance, unsigned vkey, unsigned character);
	LPCTSTR	(*OnSetCursor)(void* instance);
};

struct GH_EXTENSION_EXTRA_DATA
{
	// file name of png image for toolbar button, if NULL, then no button is created
	const char*		szToolbarIconFilename;
	// imgui callback if this extension is used in simple mode
	void	(*SimpleModeDraw)(void* instance, float indent);
};

struct GH_VIDEO_ENCODER_EXTRA_DATA
{
	struct INIT_DATA
	{
		enum
		{
			COLOR_NV12 = 0,
			COLOR_I420,
			COLOR_RGB24,
		};

		int		colorSpace;			// out
		bool	allFrames;			// out
		bool	acceptGPU;			// out
		int		dropLimit;			// out
		void*	lpAllocator;	// in
	};



	struct FRAME_DATA
	{
		unsigned char		*lpData;
		__int64				timestamp;
		unsigned			framenum;
		void*				lpAllocator;
		void*				contextData;
	};

	struct ENCODED_DATA
	{
		void*				lpMediaSample;
		__int64				timestamp;
		unsigned			framenum;
		bool				h264;
		bool				keyFrame;
		int					priority;
		unsigned			fccHandler;
		unsigned char*		lpPrivateData;
		unsigned			privateDataSize;
		bool				droppedFrame;
		void*				contextData;
	};

	// check if encoder is available
	BOOL	(*IsAvailable)();
	// initialize encoder
	BOOL	(*StartEncoder)(void* instance, unsigned *lpuWidth, unsigned *lpuHeight, unsigned fps1000, INIT_DATA* lpInitData);
	// send frame to encoder, get next encoded frame
	BOOL	(*EncodeFrame)(void* instance, const FRAME_DATA* lpFrameData, ENCODED_DATA* lpEncodedData);
	// wait for encoded frames until encoder pool is empty
	BOOL	(*DrainFrames)(void* instance, ENCODED_DATA* lpEncodedData);
};

// creates media sample in system memory for encoder output
GAMEHOOK_API void* GH_VideoEncoderCreateSample(void* lpAllocator, void* lpBuffer, unsigned bufferSize);




//---------------- hotkeys -----------------------

typedef void(__stdcall* PC_HOTKEY_CB)(LPVOID, UINT, BOOL);
typedef DWORD(__stdcall* GH_HOTKEY_EDIT_CB)(void* param, int hotkeyIdent, DWORD hotkey, bool sethotkey);


GAMEHOOK_API void			GH_SetHotkeyText(HWND hwnd, int ctrlID, DWORD hotkey);
GAMEHOOK_API DWORD			GH_ShowHotkeyBindDialog(HINSTANCE inst, UINT dialogID, HWND parent, DLGPROC DialogFunc, UINT hotkeyTitleID);
GAMEHOOK_API void			GH_GetHotkeyTitle(DWORD hotkey, WCHAR *szTitle, DWORD dwTitleSize);
GAMEHOOK_API void			GH_GetHotkeyTitle(DWORD hotkey, char *szTitle, DWORD dwTitleSize);
GAMEHOOK_API LPVOID			GH_CreateHotkey(DWORD hotkey, PC_HOTKEY_CB proc, UINT param, LPVOID hotkeyID);
GAMEHOOK_API void			GH_DeleteHotkey(LPVOID hotkeyID);
GAMEHOOK_API void			GH_CallHotkeyProc(LPVOID hotkeyID, BOOL bDown);
GAMEHOOK_API BOOL			GH_IsHotkeyPressed(DWORD hotkey);
GAMEHOOK_API void			GH_AddHotkeyInEditor(GH_HOTKEY_EDIT_CB callback, void* param, int hotkeyIdent, const wchar_t* szHotkeyGroup, const wchar_t* szHotkeyTitleID);
GAMEHOOK_API void			GH_RemoveHotkeyFromEditor(void* param, int hotkeyIdent);



// ----- locking overlays ------
struct OVERLAY_LOCK_INFO
{
	BYTE*		lpBuffer;
	DWORD		dwWidth;
	DWORD		dwHeight;
	DWORD		dwPitch;
};

GAMEHOOK_API BOOL				GH_LockOverlayEx(void* playclawdata, OVERLAY_LOCK_INFO *lpLockInfo);
GAMEHOOK_API void				GH_UnlockOverlayEx(void* playclawdata, BOOL bMarkUpdated);



// ----- common methods ----------------------------------

GAMEHOOK_API const wchar_t*	GH_GetAppPath();
GAMEHOOK_API DWORD			GH_GetFPSCounter();
//GAMEHOOK_API BOOL GH_GetPluginDataPath(LPVOID playclawdata, WCHAR* szPath, DWORD dwMaxPath);
GAMEHOOK_API void			GH_LocalizeDialog(LPVOID playclawdata, HWND wnd);
GAMEHOOK_API const wchar_t*	GH_LocalizeStr(LPVOID playclawdata, const char* strid);
GAMEHOOK_API const char*	GH_LocalizeStr8(LPVOID playclawdata, const char* strid);
GAMEHOOK_API int			GH_InputBox(HWND parent, const wchar_t* szTitle, wchar_t* szResult, unsigned uResultSizeInWords);
GAMEHOOK_API int			GH_MessageBox(const wchar_t* szMessage, int opt);
GAMEHOOK_API void			GH_GetCurrentGameFrameSize(LPDWORD lpdwWidth, LPDWORD lpdwHeight);
GAMEHOOK_API DWORD			GH_GetCurrentGameRenderType();
GAMEHOOK_API HWND			GH_GetAppWindow();

// ----- toolbar --------------------------------------------

typedef BOOL(__stdcall* GH_TOOLBAR_RIGHT_CTRL_CB)(void*, WPARAM, LPARAM);

GAMEHOOK_API HWND GH_CreateToolbarRightControl(const wchar_t* classname, DWORD styles, int w, int h, GH_TOOLBAR_RIGHT_CTRL_CB callback, void* callback_param);


// ----- web render ------------------------------------

struct GH_MOUSE_EVENT
{
	int			x, y;
	unsigned	modifiers;
};

struct GH_KEY_EVENT
{
	unsigned			vkey;
	unsigned			character;
	unsigned			modifiers;
};

GAMEHOOK_API LPVOID			GH_WebRenderCreateInstance(int width, int height, int fps, const WCHAR *lpszURL, const char *lpszCSS);
GAMEHOOK_API void			GH_WebRenderUpdateInstance(LPVOID data, int width, int height, int fps, const WCHAR *lpszURL, const char *lpszCSS);
GAMEHOOK_API void			GH_WebRenderDestroyInstance(LPVOID data);
GAMEHOOK_API void			GH_WebRenderDraw(LPVOID data, BYTE *lpImageBuffer, DWORD dwPitch, DWORD dwHeight);
GAMEHOOK_API void			GH_WebRenderMouseMove(LPVOID data, GH_MOUSE_EVENT* lpEvent, BOOL bMouseLeave);
GAMEHOOK_API void			GH_WebRenderMouseClick(LPVOID data, const GH_MOUSE_EVENT* lpEvent, int mouseButton, BOOL bButtonUp, DWORD click_count);
GAMEHOOK_API void			GH_WebRenderMouseWheel(LPVOID data, GH_MOUSE_EVENT* lpEvent, int distance);
GAMEHOOK_API void			GH_WebRenderKeyClick(LPVOID data, const GH_KEY_EVENT* lpEvent, BOOL bKeyUp);
GAMEHOOK_API LPCTSTR		GH_WebRenderGetBrowserCursor(LPVOID data);



#define RENDERER_TYPE_X64		0x10000000

#ifdef _WIN64
#define RENDERER_TYPE_DX7				(1 | RENDERER_TYPE_X64)
#define RENDERER_TYPE_DX8				(2 | RENDERER_TYPE_X64)
#define RENDERER_TYPE_DX9				(3 | RENDERER_TYPE_X64)
#define RENDERER_TYPE_DX10				(4 | RENDERER_TYPE_X64)
#define RENDERER_TYPE_DX11				(5 | RENDERER_TYPE_X64)
#define RENDERER_TYPE_DX12				(6 | RENDERER_TYPE_X64)
#define RENDERER_TYPE_OGL				(7 | RENDERER_TYPE_X64)
#define RENDERER_TYPE_VULKAN			(8 | RENDERER_TYPE_X64)
#else
#define RENDERER_TYPE_DX7				1
#define RENDERER_TYPE_DX8				2
#define RENDERER_TYPE_DX9				3
#define RENDERER_TYPE_DX10				4
#define RENDERER_TYPE_DX11				5
#define RENDERER_TYPE_DX12				6
#define RENDERER_TYPE_OGL				7
#define RENDERER_TYPE_VULKAN			8
#endif