@echo off
echo NOTICE: Script MUST be run as Administrator.
:: Errors from "reg" tool are muted to avoid flooding the build log with errors from already deleted registry entries.

:: Fix issue with "Run as Administrator" current dir
setlocal enableextensions
cd /d "%~dp0"


:: TypeLib
reg delete "HKCR\TypeLib\{0C31A6C5-DD54-4A5C-8067-3A3F692B877F}" /f 2> NUL

for %%P in (32 64) do (
  :: MfcApp class
  reg delete "HKCR\CLSID\{45C33494-127D-4AEA-B7EB-63A203D69E8A}"     /f /reg:%%P 2> NUL
  reg delete "HKCR\Interface\{2636F667-54BA-4DFC-8345-919016056346}" /f /reg:%%P 2> NUL
  reg delete "HKCR\Interface\{5A956D8D-5BE4-4C41-9FC5-530BEDAD8B6E}" /f /reg:%%P 2> NUL
)

::pause
