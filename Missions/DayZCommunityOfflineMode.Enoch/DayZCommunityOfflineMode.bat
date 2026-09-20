@echo off

taskkill /F /IM DayZ_x64.exe /T

cd ../../

start DayZ_x64.exe -mission=.\Missions\DayZCommunityOfflineMode.Enoch -nosplash -noPause -noBenchmark -filePatching -doLogs -scriptDebug=true

cd Missions/DayZCommunityOfflineMode.Enoch
