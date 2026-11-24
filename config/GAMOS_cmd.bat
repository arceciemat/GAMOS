@echo off
rem GAMOS 7.0 console (cmd) with the GAMOS environment already set.
rem Double-click it, or use the desktop shortcut "GAMOS 7.0 (cmd)" made by the Windows installer.
title GAMOS 7.0 (cmd)
cd /d "%USERPROFILE%"
cmd /k call "%~dp0confgamos.win.bat"
