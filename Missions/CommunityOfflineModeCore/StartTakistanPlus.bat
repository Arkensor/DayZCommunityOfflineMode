@echo off

taskkill /F /IM DayZ_x64.exe /T

RD /s /q "storage_-1" > nul 2>&1

cd ../../

start DayZ_x64.exe -nosplash -noBenchmark -filePatching -scriptDebug=true -mission=.\Missions\dayzOffline.TakistanPlus