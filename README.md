**ATTENTION: THIS IS THE DEVELOPMENT BRANCH. PLEASE REFRAIN FROM DOWNLOADING UNLESS YOU ARE WILLING TO PARTICIPATE IN EARLY DEVELOPMENT OF THIS BRANCH:
https://github.com/Arkensor/DayZCommunityOfflineMode/releases**

# DayZCommunityOfflineMode 2.0 WIP

UPDATE: 10/16/2018 - DannyDog

Still many things completed for our **next big patch (2.0)**. The major changes are as follows:

**Editor Panel**

This will be the new way you access all the tools we provide in our mod/mission. **By pressing the key Y you will have access to all the previous menus that you remember PLUS some more features**. Including object previews in the object spawner menu, new object editing menu for the object editor as well as an additional camera settings menu. This will make remembering keybinds a lot easier :)

**Object Editor**

The improved object editor controls should now allow you to seamlessly manipulate and spawn new objects using both the object spawner and the new camera tool. Simply open the object spawner and object editor menu, select the item from the list of objects and double click anywhere on the terrain with the cursor. Then freely click and drag the object around, **hover and scroll wheel the x,y,z/yaw,pitch,roll values** in the object editor menu for adjustments OR set your own individual values. By holding right click you are able to seamlessly hide/show your cursor or camera look movement. **Additionally I've implemented a simple script to allow you to save your placed objects and export them as code format to use on your servers. Simply place all your objects/move them accordingly and press CTRL+S. This will then export the script directly to your clipboard allowing you to paste it easily in your server mission file.**


Example:

https://i.imgur.com/qVwdQFb.png

[Output Code](https://pastebin.com/2gpxfDTq)

**Camera Tool**

I have implemented a new camera movement logic that now **finally has proper smoothing values and fov/roll changes. You can easily adjust these values in the camera menu through the editor panel**. As well as new post processing effects featuring saturation, vignette, chromatic abberation, dof, colour overlays and blurs. Later i will implement a proper keyframe camera system that will allow you to setup camera positions and freely transition between them.

Additionally. We plan to eventually push some of these tools as an all-in-one client mod (or individual mods) for proper server use.
________________________________________________________________________________________________________________________________________

# DayZCommunityOfflineMode 1.9

**Thank you for over 25.000 downloads!**  
This project got a lot of support from various community members and continues to grow.
As a thank you we are currently preparing a big feature (to be released some time soon), that will give you a whole different perspective about the game and what is possible for community modders and scripters in the future.
Besides that we are currently preparing an content update with this planned features:
* Improved visual GUI with new a new and extended object editor and a simple main menu
* Persitency with mutliple characters
* Advanced camera manager for content creators

-- 

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
Download [this](https://github.com/Arkensor/DayZCommunityOfflineMode/releases/download/v1.9/DayZ.Community.OfflineMode.zip) zip archive.  
Make sure you are on the "stress test" beta branch on steam. See [this link](https://dayz.com/blog/0-63-stress-tests) for details on how to switch on it!  

Place the ```Missions``` folder inside your DayZ game directory (```For example: C:\Program Files (x86)\Steam\steamapps\common\DayZ```).  
Even though this is not directly bannable by Battleye, just to make sure: Rename your Battleye folder to Battleye.disabled, and rename the DayZ_BE.exe to DayZ_BE.exe.disabled.

Now you now start the script ```DayZ/Missions/DayZCommunityOfflineMode.ChernarusPlus/DayZCommunityOfflineMode.bat``` to start the game. You will be loaded directly into the offline mode.  
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
* L - Toggle between the old and new Aiming System.
* DELETE + CTRL - Open the teleport menu
* DELETE + SHIFT - Open the weather menu
* DELETE - Open the item / object spawner
* INSERT - Toggle free camera. This teleports your player to the position you looked at when turning it off
* END - Toggle godmode
* END + SHIFT - Toggle object editor mode
* POS1 - Particle effect demonstration (aka poof)

Object Editor Controls:  
Click objects to select them.  
Click and drag objects to move them.
Click on nothing to deselect the current object.
Middle Click to snap to ground (not very accurate xd)
Spawn in new items using the object spawner menu: DELETE

* SCROLL WHEEL - Move object up/down
* CTRL + SCROLL WHEEL - Yaw
* SHIFT + SCROLL WHEEL - Pitch
* ALT + SCROLL WHEEL - Roll

# Logfiles:
In case you want to report errors to us or the offical dayz dev team, you might need logfile info.
We also save the positions you printed ingame in it so that you might revisit them later on by saving them in some textfile.
Locations are stored inside the script.log for now.

You find your logfiles here: ```Press WINDOWS + R  -> Type in %localappdata%/DayZ -> Hit enter```. 

# Future:
* Adding a dedicated logfile
* Adding persitent settings and stats saving
* Adding a cinematic manager for content creators
* GUI for the object manager
* More functions for the object manager
* ~~More adjustments to the freecam to feature fov options etc~~ ✓

# Contributors:
* [DannyDog](https://github.com/DannyDog) - Code snippets, Object Editor, Editor panel, Camera tools
* [Jacob_Mango](https://github.com/Jacob-Mango) - Added key to toggle "Aimgate", Persistency module
* [gallexme](https://github.com/gallexme) - Suggestion for the mission based version
* [DuhOneZ](https://twitter.com/DuhOneZ) - Code snippets
* [Watchman](https://twitter.com/watchman113) - Documentation
* [n8m4re](https://github.com/n8m4re) - Code: SaveManager
* [wriley](https://github.com/wriley) - Code snippets beards
