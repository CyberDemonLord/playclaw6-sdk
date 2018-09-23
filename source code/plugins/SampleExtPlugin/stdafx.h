#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <string>
#include <memory>

using namespace std;

// TODO: reference additional headers your program requires here
#include "../../common/jsoncpp/value.h"
#include "../../libs/imgui/imgui.h"
#include "../plugin-api-helper.h"
