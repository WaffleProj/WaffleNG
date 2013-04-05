@echo off
set cpp=D:\Program Files (x86)\Microsoft Visual Studio 11.0\VC
set sdk=D:\SDK\v7.1

echo	Building for x86...
set path=%cpp%\bin;%sdk%\bin;%cpp%\..\Common7\IDE;%windir%\System32
set include=%cpp%\include;%sdk%\include
set lib=%cpp%\lib;%sdk%\lib
cl /nologo /c /MT /D_USING_V110_SDK71_ main.c
cl /nologo /c /MT /D_USING_V110_SDK71_ membp.c
link /nologo /subsystem:windows,5.02 /out:Bin\membp_x86.exe main.obj membp.obj

echo	Building for x64...
set path=%cpp%\bin\amd64;%sdk%\bin\x64;%cpp%\..\Common7\IDE;%windir%\System32
set include=%cpp%\include;%sdk%\include
set lib=%cpp%\lib\amd64;%sdk%\lib\x64
cl /nologo /c /MT /D_USING_V110_SDK71_ main.c
cl /nologo /c /MT /D_USING_V110_SDK71_ membp.c
link /nologo /subsystem:windows,5.02 /out:Bin\membp_x64.exe main.obj membp.obj

del *.obj 2>NUL
pause