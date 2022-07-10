# DayZCommunityOfflineMode.ExclusionZone based on DayZCommunityOfflineMode 3.8 version.

The DayZ Community Offline Mode offers the ability, to explore the world of DayZ in a singleplayer environment. While this mode does not represent the actual DayZ gameplay it offers other possibilites for developers, content creators and player who do want to experiment around but not loose all their progress from online.

This is an UNOFFICIAL modification of the modification from Arkensor!

# Info for server owners & mappers
There is now a wiki page available on how to use the community offline mode to spawn map objects on your server or mission you are working on: [Add custom objects to your server or mission](https://github.com/Arkensor/DayZCommunityOfflineMode/wiki/Add-custom-objects-to-your-server-or-mission)

# Installation:
Download [this](https://github.com/JohnDamis/DayZCommunityOfflineMode.ExclusionZone/releases/tag/DayZCommunityOfflineMode.ExclusionZone.zip) zip archive.  
Make sure you are on the latest `STABLE` or `EXPERIMENTAL` version of the game.

Place the ```Missions``` folder inside your DayZ game directory (```For example: C:\Program Files (x86)\Steam\steamapps\common\DayZ```).  

Now you now start the script ```DayZ/Missions/DayZCommunityOfflineMode.ExclusionZone/DayZCommunityOfflineMode.bat``` to start the game. You will be loaded directly into the offline mode.  
Or add the following start parameter to your game: ```-mission=.\Missions\DayZCommunityOfflineMode.ExclusionZone``` 
(I HIGLY RECOMMEND USING THIS METHOD FOR THE BEST PERFOMANCE)

To unstinstall this mod, simple delete all downloaded files, and rename your .disabled folders back to their original name.   
You might delete your whole ```Missions``` folder and validate your game files via steam to get the original DayZ files back.

# Controls:
* Y (Z for QWERTZ-Keyboard) - Open the COM toolbar menu.
* X - Toggle Autojog/walk/run
* X + SHIFT - Enable Autorun (Just X to disable it again)
* X + CTRL - Enable Autowalk (Just X to disable it again)
* END - Teleport at the position you are looking at
* O - Spawn a random infected (Zombies)
* O + CTRL - Spawn a wolf (Agressive and will fight both players and infected)
* O + SHIFT - Spawn a random animal
* R - Reload the weapon and refill the ammo (Infinite ammo)
* P - Display your current position in the chat and print it to your logfiles (See logfiles section for their location)
* B - Toggle debug monitor
* INSERT - Toggle free camera. This teleports your player to the position you looked at when turning it off

# Object Editor Controls: 
* The view distance can be increased in the camera controls or in the file (```\core\modules\Camera Tool\Camera Tool.c```) in line 60.
* Click objects to select them.  
* Click and drag objects to move them.
* Click on nothing to deselect the current object.
* Middle Click to snap to ground (Might not be accurate)
* Spawn in new items using the object spawner menu that can be found in the toolbar.
* You can either enter values on the onject editor gui or hover above the value with your mouse and use the scroll wheel to in-/decrease them.

# Loot and infected spawn
By default the "Hive" that is repsonsible for spawning loot and infected is **enabled**. If you want to prevent loot and infected from spawning you need to follow this guide: [Toggle loot and infected spawn](https://github.com/Arkensor/DayZCommunityOfflineMode/wiki/Toggle-loot-and-infected-spawn)  
> Disabling the hive increases the game performace. Disabled by default.

# Logfiles:
In case you want to report errors to us or the offical dayz dev team, you might need logfile info.
We also save the positions you printed ingame in it so that you might revisit them later on by saving them in some textfile.
Locations are stored inside the script.log for now.

You find your logfiles here: ```Press WINDOWS + R  -> Type in %localappdata%/DayZ -> Hit enter```. 

# Developers
* [Arkensor](https://github.com/Arkensor)
* [DannyDog](https://github.com/DannyDog)
* [Jacob_Mango](https://github.com/Jacob-Mango)

# Contributors:
* [gallexme](https://github.com/gallexme) - Suggestion for the mission based version
* [DuhOneZ](https://twitter.com/DuhOneZ) - Code snippets
* [Watchman](https://twitter.com/watchman113) - Documentation
* [n8m4re](https://github.com/n8m4re) - Code: SaveManager
* [wriley](https://github.com/wriley) - Code snippets beards
* [PR9INICHEK](https://github.com/PR9INICHEK) - Object spawner additions
* [LiPeK](https://github.com/LiPeK) - Object editor additions
* [Cypher](https://github.com/CypherMediaGIT) - Livonia positions
* [BigBenREDUX](https://twitter.com/BigBenREDUX) - Namalsk positions
* [JXHN](https://github.com/JohnDamis) - Area Of Decay positions and fixes
