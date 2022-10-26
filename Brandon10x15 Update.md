# Overview:
- Added object scaling functionality.
- COM now cfgGamePlay.json, cfgEffectArea.json, and cfgUndergroundTriggers.json.
- Added configurable COMPositions.json to C:\Users\USERNAME\Documents\DayZ\CommunityOfflineMode.
- COM Objects now save to your mission/custom folder.
- COM object files are automatically saved to your cfgGamePlay.json objectspawnerArr list when saved.
- - COM will not remove any other items you have in your cfgGamePlay.json objectspawnerArr, dont worry)
- Added MS3 to spawn menu.
- Updated object preview exclusion list to prevent crashes.


## File Changes:

### CommunityOfflineClient.c
* Now reads cfgGamePlay.json, cfgEffectArea.json, and cfgUndergroundTriggers.json variables by default.
* Removed SetupWeather(), use $mission:cfgWeather.xml to configure the weather.
* Added worldMap and gameVersion variables to change editor object save file names automatically.

### StaticFunctions.c
* Added worldMap, gameVersion, objectsFilename, and BASE_COM_DIR to StaticFunctions.c
* Moved BASE_COM_DIR to StaticFunctions.c

### PositionMenu.c
* COMPositions.json can be renamed or deleted to restore the default COMPositions.json file.
* Positions are now inserted into the Positions menu in the same order as the COMPositions.json file.
* Added positions for Namalsk.
* Disabled teleporting while in vehicle to prevent player glitching.

### CameraTool.c
* Disabled camera tool while in vehicle to prevent player glitching.

### ObjectEditor.c
* Changed m_SceneObjects to Objects to match vanilla objectSpawnerArr json format.
* Saving and loading object scale.
* Added ScaleObject function. (Objects cannot be moved or orientated after scaling, objects must be recreated as they are moved, if scaled)
* Checking ObjectInfoMenu.editingPos before updating ObjectInfoMenu positions on SelectObject.

### GameMenu.c
* Changed name of HMMVV to M1025 to match the in-game display name.
* Added MS3 Truck.
* Added additional wheels to all vehicles to accommodate the new spare slot, some wheels were attaching to the spare instead of the tire slot.

### ObjectInfoMenu.c
* Added editingPos bool, set OnMouseEnter and OnMouseLeave to prevent text updating while typing.
* Adjusted OnChange and OnMouseWheel to allow scaling.

### ObjectMenu.c
* Tested all items again and updated m_excludeBroken list.

### SceneSaveST.c
* Moved to ObjectSave.c
* Changed m_SceneObjects to Objects to match vanilla objectSpawnerArr json format.
* Changed param array to ObjectData array to allow proper objectSpawnerArr json format saving.

### ObjectData.c
* Added Scale.

### COMKeyBinds.c
* Disabled teleporting while in vehicle to prevent player glitching.
* - Brandon10x15.


## Dev notes:
* Added worldMap variable to allow automatic detection for file saving. 
* -- (although now that objects save to the custom folder there is no need for different file names)
* Changed all #includes to make it easy to update the core folder for each map.
Example:
Changed This: #include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\ModuleManager.c"
To This: #include "$mission:core\\ModuleManager.c"

* The $mission:core folder and init.c can now be updated across all missions without changing any files.