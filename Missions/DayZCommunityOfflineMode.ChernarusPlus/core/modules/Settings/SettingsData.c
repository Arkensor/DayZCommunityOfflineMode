class SettingsData
{
    int hiveEnabled = 1;
    vector spawnPosition = Vector(7025, 0, 8236);
    vector lastPosition;
    vector lastOrientation = vector.Zero;
    int spawnAtLastPosition = 1;
    string timeZone = "EST";
    int alwaysDay = 1;
    int keepWeatherClear = 1;
    string keyCOMOpenMenu = "kY";
    string keyCOMCloseMenu = "kEscape";
    string keyCOMToggleDebugMonitor = "kB";
    string keyCOMToggleCursor = "kU";
    string keyCOMTeleportToCursor = "kH";
    string keyCOMInstantReload = "kR";
    string keyCOMSpawnInfected = "kO";
    string keyCOMSpawnGiftBox = "kI";
    string keyCOMHideHUD = "kHome";
    string keyCOMCopyPlayerPosition = "kP";
    string keyCOMCopyCursorPositions = "kL";
    string keyCOMClearCursorPositions = "kSemicolon";
    string keyCOMAutoRun = "kApostrophe";

    string keyCameraToggleCamera = "kRControl";
    string keyCameraFreezeCamera = "kBackslash";
    string keyCameraFreezePlayer = "kRShift";
    string keyCameraTargetCamera = "mBMiddle";
    string keyCameraFollowTarget = "kLBracket";
    string keyCameraToggleOrbital = "kRBracket";
    string keyCameraZoomCameraHOLD = "mBRight";
    string keyCameraIncreaseCameraSpeed = "mWheelUp";
    string keyCameraDecreaseCameraSpeed = "mWheelDown";
    
    string keyEditorSaveAllObjects = "kPause";
    string keyEditorExportCurrentObjects = "kScrollLock";
    string keyEditorSelectObject = "mBLeft";
    string keyEditorDragObjectHOLD = "mBMiddle";
    string keyEditorCancelDragObject = "kSpace";
    string keyEditorUndoObjectMoveCTRL = "kZ";
    string keyEditorRedoObjectMoveCTRL = "kY";
    string keyEditorDeleteObject = "kDelete";
    string keyEditorDuplicateObject = "kInsert";
    string keyEditorGroundObject = "kG";
    string keyEditorDeselectObject = "kGrave";
    
    string keyEditorMoveObjectNorth = "kNumPad8";
    string keyEditorMoveObjectSouth = "kNumPad2";
    string keyEditorMoveObjectEast = "kNumPad6";
    string keyEditorMoveObjectWest = "kNumPad4";
    string keyEditorMoveObjectUp = "kNumPad9";
    string keyEditorMoveObjectDown = "kNumPad7";
 	float moveSpeedSlow = 0.01;
 	float moveSpeedMedium = 0.1;
 	float moveSpeedFast = 0.25;

    string keyEditorRotateObjectLeft = "kNumPad1";
    string keyEditorRotateObjectRight = "kNumPad3";
 	float rotateSpeedSlow = 1.0;
 	float rotateSpeedMedium = 5.0;
 	float rotateSpeedFast = 15.0;

    string keyEditorFlipObjectUp = "kMultiply";
    string keyEditorFlipObjectDown = "kDivide";
    string keyEditorFlipObjectLeft = "kNumPad0";
    string keyEditorFlipObjectRight = "kDecimal";
 	float flipSpeedSlow = 0.1;
 	float flipSpeedMedium = 1.0;
 	float flipSpeedFast = 5;
    
 	void SettingsData(SettingsData config = NULL) {
        if(!config) { if(worldMap == "Enoch") { spawnPosition = Vector(6296, 0, 9755); } else if(worldMap == "Namalsk") { spawnPosition = Vector(7000, 0, 11025); } lastPosition = spawnPosition; return; }
        hiveEnabled = config.hiveEnabled;
        spawnPosition = config.spawnPosition;
 	    lastPosition = config.lastPosition;
 	    lastOrientation = config.lastOrientation;
        spawnAtLastPosition = config.spawnAtLastPosition;
 	    timeZone = config.timeZone;
 	    alwaysDay = config.alwaysDay;
 	    keepWeatherClear = config.keepWeatherClear;

 	    keyCOMOpenMenu = config.keyCOMOpenMenu;
 	    keyCOMCloseMenu = config.keyCOMCloseMenu;
        keyCOMToggleDebugMonitor = config.keyCOMToggleDebugMonitor;
 	    keyCOMToggleCursor = config.keyCOMToggleCursor;
        keyCOMTeleportToCursor = config.keyCOMTeleportToCursor;
        keyCOMInstantReload = config.keyCOMInstantReload;
        keyCOMSpawnInfected = config.keyCOMSpawnInfected;
        keyCOMSpawnGiftBox = config.keyCOMSpawnGiftBox;
        keyCOMHideHUD = config.keyCOMHideHUD;
        keyCOMCopyPlayerPosition = config.keyCOMCopyPlayerPosition;
        keyCOMCopyCursorPositions = config.keyCOMCopyCursorPositions;
        keyCOMClearCursorPositions = config.keyCOMClearCursorPositions;
        keyCOMAutoRun = config.keyCOMAutoRun;

        keyCameraToggleCamera = config.keyCameraToggleCamera;
        keyCameraFreezeCamera = config.keyCameraFreezeCamera;
        keyCameraFreezePlayer = config.keyCameraFreezePlayer;
        keyCameraTargetCamera = config.keyCameraTargetCamera;
        keyCameraFollowTarget = config.keyCameraFollowTarget;
        keyCameraToggleOrbital = config.keyCameraToggleOrbital;
        keyCameraZoomCameraHOLD = config.keyCameraZoomCameraHOLD;
        keyCameraIncreaseCameraSpeed = config.keyCameraIncreaseCameraSpeed;
        keyCameraDecreaseCameraSpeed = config.keyCameraDecreaseCameraSpeed;
        
 	    keyEditorSaveAllObjects = config.keyEditorSaveAllObjects;
 	    keyEditorExportCurrentObjects = config.keyEditorExportCurrentObjects;
        keyEditorSelectObject = config.keyEditorSelectObject;
 	    keyEditorDragObjectHOLD = config.keyEditorDragObjectHOLD;
        keyEditorCancelDragObject = config.keyEditorCancelDragObject;
        keyEditorUndoObjectMoveCTRL = config.keyEditorUndoObjectMoveCTRL;
        keyEditorRedoObjectMoveCTRL = config.keyEditorRedoObjectMoveCTRL;
        keyEditorDeleteObject = config.keyEditorDeleteObject;
        keyEditorDuplicateObject = config.keyEditorDuplicateObject;
        keyEditorGroundObject = config.keyEditorGroundObject;
        keyEditorDeselectObject = config.keyEditorDeselectObject;

 	    keyEditorMoveObjectNorth = config.keyEditorMoveObjectNorth;
 	    keyEditorMoveObjectSouth = config.keyEditorMoveObjectSouth;
        keyEditorMoveObjectEast = config.keyEditorMoveObjectEast;
 	    keyEditorMoveObjectWest = config.keyEditorMoveObjectWest;
        keyEditorMoveObjectUp = config.keyEditorMoveObjectUp;
        keyEditorMoveObjectDown = config.keyEditorMoveObjectDown;
        moveSpeedSlow = config.moveSpeedSlow;
        moveSpeedMedium = config.moveSpeedMedium;
        moveSpeedFast = config.moveSpeedFast;

        keyEditorRotateObjectLeft = config.keyEditorRotateObjectLeft;
        keyEditorRotateObjectRight = config.keyEditorRotateObjectRight;
        rotateSpeedSlow = config.rotateSpeedSlow;
        rotateSpeedMedium = config.rotateSpeedMedium;
        rotateSpeedFast = config.rotateSpeedFast;

        keyEditorFlipObjectUp = config.keyEditorFlipObjectUp;
        keyEditorFlipObjectDown = config.keyEditorFlipObjectDown;
        keyEditorFlipObjectLeft = config.keyEditorFlipObjectLeft;
        keyEditorFlipObjectRight = config.keyEditorFlipObjectRight;
        flipSpeedSlow = config.flipSpeedSlow;
        flipSpeedMedium = config.flipSpeedMedium;
        flipSpeedFast = config.flipSpeedFast;
 	}
}