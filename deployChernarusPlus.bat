RD /s /q "C:\Program Files (x86)\Steam\steamapps\common\DayZ\Missions\DayZCommunityOfflineMode.ChernarusPlus" > NUL
xcopy /s/e /y /i "D:\Projekte\Enscript\DayZCommunityOfflineMode\Missions\DayZCommunityOfflineMode.ChernarusPlus" "C:\Program Files (x86)\Steam\steamapps\common\DayZ\Missions\DayZCommunityOfflineMode.ChernarusPlus" > NUL

chdir /d "C:\Program Files (x86)\Steam\steamapps\common\DayZ\Missions\DayZCommunityOfflineMode.ChernarusPlus"

CALL "DayZCommunityOfflineMode.bat"