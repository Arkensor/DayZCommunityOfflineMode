@echo off

taskkill /F /IM DayZ_x64.exe /T

RD /s /q "storage_-1" > nul 2>&1

cd ../../

start DayZ_x64.exe -mission=.\Missions\DayZCommunityOfflineMode.ChernarusPlus -scrAllowFileWrite -nosplash -noPause -noBenchmark -scriptDebug=true -filePatching -doLogs