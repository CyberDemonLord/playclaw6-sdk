// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <stdint.h>
#include <Shlwapi.h>

#include <string>
#include <memory>

using namespace std;

// TODO: reference additional headers your program requires here
#define JSON_DLL
#include "../../common/jsoncpp/value.h"
#include "../../libs/imgui/imgui.h"
#include "../plugin-api.h"
#include "../plugin-api-helper.h"
#include "../plugin-api-gui.h"
#include "../plugin-encoder-template.h"
#include "../plugin-api-json.h"


extern "C"
{
#include "x264.h"
}
