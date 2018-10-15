rd /q/s release

md release\plugins\x264
md release\lang\encoder-x264

"c:\program files\7-zip\7z.exe" a -r release\plugins\x264\playclaw6-encoder-x264-plugin-src.zip "source code\plugins\encoder-x264"
copy /b bin\plugins\encoder-x264.dll release\plugins\
copy /b bin\plugins\x264 release\plugins\x264
copy /b bin\lang\encoder-x264\*.* release\lang\encoder-x264






del playclaw6-encoder-x264-plugin.zip
cd release
"c:\program files\7-zip\7z.exe" a -r ..\playclaw6-encoder-x264-plugin.zip plugins\ lang\
cd ..

rd /q/s release
