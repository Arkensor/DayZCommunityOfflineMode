RD /s /q "C:\Program Files (x86)\Steam\steamapps\common\DayZ\Missions\DayZCommunityOfflineMode.Enoch" > NUL
xcopy /s/e /y /i "D:\Projekte\Enscript\DayZCommunityOfflineMode\Missions\DayZCommunityOfflineMode.Enoch" "C:\Program Files (x86)\Steam\steamapps\common\DayZ\Missions\DayZCommunityOfflineMode.Enoch" > NUL

chdir /d "C:\Program Files (x86)\Steam\steamapps\common\DayZ\Missions\DayZCommunityOfflineMode.Enoch"

CALL "DayZCommunityOfflineMode.bat"