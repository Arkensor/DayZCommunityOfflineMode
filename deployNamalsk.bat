RD /s /q "C:\Program Files (x86)\Steam\steamapps\common\DayZ\Missions\DayZCommunityOfflineMode.Namalsk" > NUL
xcopy /s/e /y /i "D:\Projekte\Enscript\DayZCommunityOfflineMode\Missions\DayZCommunityOfflineMode.Namalsk" "C:\Program Files (x86)\Steam\steamapps\common\DayZ\Missions\DayZCommunityOfflineMode.Namalsk" > NUL

chdir /d "C:\Program Files (x86)\Steam\steamapps\common\DayZ\Missions\DayZCommunityOfflineMode.Namalsk"

CALL "DayZCommunityOfflineMode.bat"