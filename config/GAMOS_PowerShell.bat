@echo off
rem GAMOS 7.0 console (PowerShell) with the GAMOS environment already set.
rem Double-click it, or use the desktop shortcut "GAMOS 7.0 (PowerShell)" made by the Windows installer.
rem
rem It opens PowerShell in a new window with the colours of Windows PowerShell (white on dark blue).
rem A console started with the title "GAMOS 7.0 (PowerShell)" takes its colours and font from the registry
rem key HKCU\Console\GAMOS 7.0 (PowerShell): it is created the first time (no administrator rights needed);
rem if you change the colours in the window Properties, they are kept there and not overwritten.
set "GAMOS_CONSOLE_KEY=HKCU\Console\GAMOS 7.0 (PowerShell)"
reg query "%GAMOS_CONSOLE_KEY%" >nul 2>&1 && goto :start
reg add "%GAMOS_CONSOLE_KEY%" /v ScreenColors /t REG_DWORD /d 0x56 /f >nul
reg add "%GAMOS_CONSOLE_KEY%" /v PopupColors /t REG_DWORD /d 0xf3 /f >nul
reg add "%GAMOS_CONSOLE_KEY%" /v ColorTable05 /t REG_DWORD /d 0x562401 /f >nul
reg add "%GAMOS_CONSOLE_KEY%" /v ColorTable06 /t REG_DWORD /d 0xf0edee /f >nul
reg add "%GAMOS_CONSOLE_KEY%" /v FaceName /t REG_SZ /d Consolas /f >nul
reg add "%GAMOS_CONSOLE_KEY%" /v FontFamily /t REG_DWORD /d 0x36 /f >nul
reg add "%GAMOS_CONSOLE_KEY%" /v FontSize /t REG_DWORD /d 0x100000 /f >nul
reg add "%GAMOS_CONSOLE_KEY%" /v FontWeight /t REG_DWORD /d 400 /f >nul
reg add "%GAMOS_CONSOLE_KEY%" /v QuickEdit /t REG_DWORD /d 1 /f >nul
:start
set "GAMOS_CONSOLE_KEY="
cd /d "%USERPROFILE%"
start "GAMOS 7.0 (PowerShell)" powershell -NoLogo -NoExit -ExecutionPolicy Bypass -File "%~dp0confgamos.win.ps1"
