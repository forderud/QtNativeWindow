@echo off
echo NOTICE: Script MUST be run as Administrator.
:: Errors from "reg" tool are muted to avoid flooding the build log with errors from already deleted registry entries.

:: Fix issue with "Run as Administrator" current dir
setlocal enableextensions
cd /d "%~dp0"


:: Remove all traces of AtlMiniHost from registry
reg delete "HKCR\TypeLib\{6DD3518F-3716-43F8-ACAC-F42F7E033989}" /f 2> NUL


::pause
