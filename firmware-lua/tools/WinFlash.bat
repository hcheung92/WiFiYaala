@echo off
echo.

if "%1" == "" goto error1

set ESPTOOL=..\tools\esptool.py
set FIRMWAREDIR=..\bin\

%ESPTOOL% --port %1 write_flash 0x00000 %FIRMWAREDIR%0x00000.bin 0x10000 %FIRMWAREDIR%0x10000.bin
if ERRORLEVEL 1 (
	echo --- FAILED ---
	goto end
)

echo --- SUCCESS ---
goto end

:error1
echo ERROR: COM port not specified!
echo Example: WinFlash.bat COM6

:end
echo.
