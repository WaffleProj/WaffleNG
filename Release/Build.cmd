@echo off
cd ..
call Build_Waffle.cmd
call Build_Waffle.cmd AMD64
call Build_Mojibake.cmd
call Build_Mojibake.cmd AMD64
cd Release
@pause