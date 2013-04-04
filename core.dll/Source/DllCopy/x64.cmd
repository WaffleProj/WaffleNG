@echo off
set cpp=D:\Program Files (x86)\Microsoft Visual Studio 11.0\VC
set sdk=D:\SDK\v7.1

set path=%cpp%\bin\amd64;%sdk%\bin\x64;%cpp%\..\Common7\IDE;%windir%\System32
set include=%cpp%\include;%sdk%\include
set lib=%cpp%\lib\amd64;%sdk%\lib\x64

cl /nologo /c /GL /MT /Ox /Ot /W3 /EHsc /D_USING_V110_SDK71_ core.c
link /nologo /subsystem:windows,5.02 /LTCG /out:Bin\core.exe core.obj
del core.obj 2>NUL

pause