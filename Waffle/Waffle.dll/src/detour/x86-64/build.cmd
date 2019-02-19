@echo	off
set	path=D:\JWasm\bin;%path%
jwasm	-nologo -bin x86-64.asm
del	*.err 2>NUL
pause