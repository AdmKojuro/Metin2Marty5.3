@echo off
CALL "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
cd %~dp0
msbuild DumpProto.sln /property:Configuration=Release /maxcpucount -target:Clean
REM msbuild metin2client.sln /property:Configuration=Release /maxcpucount
pause
