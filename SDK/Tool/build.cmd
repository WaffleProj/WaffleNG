@echo	off

set	dotnet=%SystemRoot%\Microsoft.NET\Framework

for /f	"delims=" %%v in ('dir /ad /b "%dotnet%\v?.*"') do (
	if exist "%dotnet%\%%v\csc.exe" set csc="%dotnet%\%%v\csc.exe"
	if exist "%dotnet%\%%v\jsc.exe" set jsc="%dotnet%\%%v\jsc.exe"
	if exist "%dotnet%\%%v\vbc.exe" set vbc="%dotnet%\%%v\vbc.exe"
	)

%csc%	dll2lib.cs
%vbc%	signtool.vb
pause