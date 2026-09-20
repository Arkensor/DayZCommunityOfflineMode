@echo off

taskkill /F /IM DayZ_x64.exe /T

cd ../../

start DayZ_x64.exe -mission=.\Missions\DayZCommunityOfflineMode.Namalsk -nosplash -noPause -noBenchmark -filePatching -doLogs -scriptDebug=true "-mod=!Workshop\@Namalsk Island;"
