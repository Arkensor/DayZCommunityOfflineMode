@echo off
setlocal enableextensions enabledelayedexpansion 

cd /D "%~dp0"

set /a failed=0

for /F "Tokens=2* skip=2" %%A In ('REG QUERY "HKLM\SOFTWARE\Wow6432Node\bohemia interactive\Dayz" /v "main" 2^>nul') do (
	set "_DAYZPATH=%%B"
)

if "%_DAYZPATH%" == "" (
	set /a failed=1
	echo.DayZ was not set in the registry path.
) else (
	echo.Found DayZ at "%_DAYZPATH%"
)

if %failed%==1 (
	endlocal
	echo Failed to obtain paths.

	goto:eof
)

set "ClientDirectory=%_DAYZPATH%"

RD /s /q "%ClientDirectory%\Missions\DayZCommunityOfflineMode.ChernarusPlus" > NUL
xcopy /s/e /y /i "%cd%\Missions\DayZCommunityOfflineMode.ChernarusPlus" "%ClientDirectory%\Missions\DayZCommunityOfflineMode.ChernarusPlus" > NUL

chdir /d "%ClientDirectory%\Missions\DayZCommunityOfflineMode.ChernarusPlus"

setlocal DisableDelayedExpansion

CALL "DayZCommunityOfflineMode.bat"

endlocal