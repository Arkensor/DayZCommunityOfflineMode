@echo off
SETLOCAL

echo This bat script will rename the file DayZ_BE.exe and the BattlEye folder.

PAUSE

cd ../../

if exist DayZ_BE.exe (set /a file=1) else (set /a file=0)
if exist BattlEye (set /a folder=1) else (set /a folder=0)

if %file%==1 if %folder%==1 rename DayZ_BE.exe DayZ_BE.exe.disabled & echo Renamed file Dayz_BE.exe to DayZ_BE.exe.disabled & rename BattlEye BattlEye.disabled & echo Renamed folder BattlEye to BattlEye.disabled
if %file%==0 if %folder%==0 rename DayZ_BE.exe.disabled DayZ_BE.exe & echo Renamed file Dayz_BE.exe.disabled to DayZ_BE.exe & rename BattlEye.disabled BattlEye & echo Renamed folder BattlEye.disabled to BattlEye

ENDLOCAL

PAUSE
