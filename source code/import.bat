rd /S/Q plugins
rd /S/Q common
rd /S/Q libs

md plugins
copy /Y "..\..\work\source code\plugins\playclaw6.lib" ".\plugins
copy /Y "..\..\work\source code\plugins\plugin-api.h"  .\plugins
copy /Y "..\..\work\source code\plugins\plugin-api-helper.h" .\plugins
copy /Y "..\..\work\source code\plugins\plugin-template.*" .\plugins
copy /Y "..\..\work\source code\plugins\plugin-overlay-template.*" .\plugins
xcopy "..\..\work\source code\plugins\SampleExtPlugin\*.*" .\plugins\SampleExtPlugin\ /e


md common\jsoncpp
copy /Y "..\..\work\source code\common\jsoncpp\value.h" .\common\jsoncpp

md libs\imgui
copy /Y "..\..\work\source code\libs\imgui\imgui.h" .\libs\imgui