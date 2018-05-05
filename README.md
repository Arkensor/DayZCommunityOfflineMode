# DayZCommunityOfflineMode
DayZ Community Offline Mode is a small sideproject to offer all of you,  
who did not have the chance to play on a stress test server for long alreay, the chance to enjoy the 0.63 stress test update.  
There is not much to do ingame, but you can explore the map, fight some infected, test your hardware limits by spawning alot of them
and most important of all CLIMB LADDERS!!! (Jokes aside, ladders have become really nice, definitely try them out)  

This is UNOFFICIAL, so don't blame the DayZ development team if this is not working at all for you!

Note to other developers:  
Yes the code might be messy, yes there are things that do not look like they should work or do anything!  
DayZ is not opened for modding or scripting, so this is pretty much a hacky solution to everything.  
If you want to improve something, go ahead and create a pullrequest ;)

# Installation:
Download [this](https://github.com/Arkensor/DayZCommunityOfflineMode/releases/download/v1.7/DayZ.Community.OfflineMode.zip) zip archive.  
Make sure you are on the "stress test" beta branch on steam. See [this link](https://dayz.com/blog/0-63-stress-tests) for details on how to switch on it!  

Place the ```Missions``` folder inside your DayZ game directory (```For example: C:\Program Files (x86)\Steam\steamapps\common\DayZ```).  
Even though this is not directly bannable by Battleye, just to make sure: Rename your Battleye folder to Battleye.disabled, and rename the DayZ_BE.exe to DayZ_BE.exe.disabled.

Now you now start the script ```DayZ/Missions/DayZCommunityOfflineMode.ChernarusPlus/Reset Hive and Play.bat``` to start the game. You will be loaded directly into the offline mode.  
Or add the following start parameter to your game: ```-mission=.\Missions\DayZCommunityOfflineMode.ChernarusPlus```

To unstinstall this mod, simple delete all downloaded files, and rename your .disabled folders back to their original name.   
You might delete your whole ```Missions``` folder and validate your game files via steam to get the original DayZ files back.

# Controls:
* X - Toggle Autowalk
* X + SHIFT - Enable Autorun ( Just X to disable it again )
* T - Teleport at the position you are looking at
* O - Spawn a random infected (Zombies)
* O + CTRL - Spawn a doge (Actually a wolf, but you can pet him if you have godmode enabled!)
* O + SHIFT - Spawn a random animal
* R - Reload the weapon and refill the ammo (Infinite ammo)
* P - Display your current position in the chat and print it to your logfiles (See logfiles section for their location)
* B - Toggle debug monitor
* K - Restart the mission (useful when you spawned too much of everything @ 5 fps)
* N - Spawn the Sedan (not driveable in the current build of DayZ, sorry)
* DELETE + CTRL - Open the teleport menu
* DELETE + SHIFT - Open the weather menu
* DELETE - Open the item / object spawner
* INSERT - Toggle free camera. This teleports your player to the position you looked at when turning it off
* END - Toggle godmode
* POS1 - Particle effect demonstration (aka poof)

Free Camera Controls
* SCROLL WHEEL - Speed of Camera
* Q / Z - Raise Lower Camera
* SHIFT/CTRL - Fast Forward (Also altered with Mouse Wheel)
* ALT - Slow Speed

Object Editor Controls
Click objects to select them.
Click drag objects to move them.
Click on nothing to deselect object

* SCROLL WHEEL - Move object up/down
* CTRL + SCROLL WHEEL - Yaw
* SHIFT + SCROLL WHEEL - Pitch
* ALT + SCROLL WHEEL - Roll

Control Tips
* ALT + MOUSE - Turning Camera without turning player (For Freecam Pictures with Player)

# Logfiles:
In case you want to report errors to us or the offical dayz dev team, you might need logfile info.
We also save the positions you printed ingame in it so that you might revisit them later on by saving them in some textfile.
Locations are stored inside the script.log for now.

You find your logfiles here: ```Press WINDOWS + R  -> Type in %localappdata%/DayZ -> Hit enter```. 

# Future:
* Adding a dedicated logfile
* Adding persitent settings and stats saving
* Adding a cinematic manager for content creators

# Contributors:
* [DuhOneZ](https://twitter.com/DuhOneZ) - Code snippets
* [Watchman](https://twitter.com/watchman113) - Documentation
* [gallexme](https://github.com/gallexme) - Suggestion for the mission based version
* [DannyDog](https://github.com/DannyDog) - Code snippets and Object Editor
