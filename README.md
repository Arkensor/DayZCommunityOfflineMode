# Table of Contents

 - [Home](./README.md)
 - [Controls](./Controls.md)
 - [All Keybind Codes](./All%20Keybind%20Codes.md)
 - [Original README](./oldREADME.md)
 
 
# Community Offline Mode (Object Editor Edition) 4.0 worthy?

## Known Issues:
- Cannot rotate object groups yet.
- Some keybinds are non-configureable.


## Changes:
- Main $saves\COM folder is now located in: C:\Users\YOURUSERNAME\Documents\DayZ\CommunityOfflineMode\.
- Created object groups, allowing organization, selection and movement of multiple object of once.
- Added saved object variables: Group, Time/Date Created, and Active.
- Created movement hotkeys to easily fine to your object's position.
- Add a "Lock Y Axis" toggle to the Object Info Menu, which allows you to move objects without changing their altitude.
- Made built-in map objects non-selectable.
- Objects now save in a format that can be copied directly to your mission\custom folder.
- Object files are now backed up 25 times in $saves\COM\Backups\.
- Every object or group action including spawn/movement/rotation/deletion/group change/active status change can be Undone and Redone up to the last Object Reload using keybinds.
- Made almost every keybind configureable in $saves\COM\COMSettings.json or COMSettingsEnoch.json respectively.
- Added settings: spawnPosition, spawnAtLastPosition, alwaysDay, keepWeatherClear, moveSpeedSlow-Medium-Fast, rotateSpeedSlow-Medium-Fast, flipSpeedSlow-Medium-Fast.
- Made the Object Menu and Object Editor Menu layouts larger and easier to use.
- Added more filter buttons to the Object Menu.
- Moved Copy LODs function to work in the Object Menu.
- Added Amount variable to the Object Menu, to allow spawning multiple of the same object at once.
- Enabled Camera Tool enable/disable keybind while in Object Editor Menu.
- Added object position export formats for easy copying and pasting of locations.
- Added an ongoing position list hotkey, which copies multiple positions at once in CfgEventSpawns form.
- Added hotkey to spawn Large_Giftbox_1 for loot offset positions.
- Godmode is now enabled by default.
- Godmode now protects your vehicle.
- Godmode now repairs your weapon-in-hand and suppressor if attached.
- Disabling the camera will now enable godmode and drop you from your cameras position instead of placing you on the ground, potentially inside of an object.
- Changed Camera Tool altitude down button from kZ to kQ to prevent interference with kZ Undo keybind.
- Moved enableHive variable to $saves\COM\COMSettings.json.


## Fixes
- Re-tested and fixed all excluded items to prevent crashing while scrolling through the object menu.
- Added MS3 to vehicle spawn list. (HMMWV is ready for 1.19 release)
- Adjusted many other functions for ease of use.
- Fixed issue where the mouse cursor would not in menus show after switching screens.
- Teleporting to land while swimming now makes you stop swimming.
- Adjusted the Position Menu to show positions in alphabetical order.
- Searching the Object Menu will now only search the current filtered list, instead of all objects.


## Loot and infected spawn
By default the "Hive" that is repsonsible for spawning loot and infected is **enabled**. If you want to prevent loot and infected from spawning, set enableHive to 0 in $saves\COM\COMSettings.json.
> Disabling the hive increases the game performace.


## Logfiles
In case you want to report errors to us or the offical dayz dev team, you might need logfile info.
We also save the positions you printed ingame in it so that you might revisit them later on by saving them in some textfile.
Locations are stored inside the script.log for now.

You find your logfiles here: ```Press WINDOWS + R  -> Type in %localappdata%/DayZ -> Hit enter```. 


# Contributors:
* [DannyDog](https://github.com/DannyDog) - Inital core developer
* [Jacob_Mango](https://github.com/Jacob-Mango) - Inital core developer
* [gallexme](https://github.com/gallexme) - Suggestion for the mission based version
* [DuhOneZ](https://twitter.com/DuhOneZ) - Code snippets
* [Watchman](https://twitter.com/watchman113) - Documentation
* [n8m4re](https://github.com/n8m4re) - Code: SaveManager
* [wriley](https://github.com/wriley) - Code snippets beards
* [PR9INICHEK](https://github.com/PR9INICHEK) - Object spawner additions
* [LiPeK](https://github.com/LiPeK) - Object editor additions
* [Cypher](https://github.com/CypherMediaGIT) - Livonia positions
* [BigBenREDUX](https://twitter.com/BigBenREDUX) - Namalsk positions
* [Arkensor](https://github.com/Arkensor) - Current main developer
* [Brandon10x15](https://github.com/Brandon10x15) - Object editor and QOL additions
