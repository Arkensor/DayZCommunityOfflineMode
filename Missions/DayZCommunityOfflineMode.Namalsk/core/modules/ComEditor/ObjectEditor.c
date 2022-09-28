class ObjectEditor extends Module
{
	static bool m_ObjectEditorActive = false;
	protected bool m_ObjectDragCancel = false;
	protected bool m_IsDragging;
	static Object m_SelectedObject;
	Object building;
	bool Undo[100], Redo[100];
	vector UndoPos[100], UndoOr[100], RedoPos[100], RedoOr[100];
	ref array<vector> groupPositions, groupPositionsRedo;
    ref array<ref array<ref Param8<string, int, string, vector, vector, string, string, int>>> undoList, redoList;
    //ref array<ref array<ref Param6<string, Object, string, vector, vector>>> redoList = new array<ref array<ref Param6<string, Object, string, vector, vector>>>;
	// TODO ^ Map every object movement and deletion for undo and redo.
	// Undo and Redo currently only save until you select a new object.
	
	ref objectHandler objHandler;
    bool lockYAxis = false;
    bool groupEditor = false;

	autoptr array<Object> m_Objects = new array<Object>;
	bool hasObject(Object obj) { if(m_Objects.Find(obj) != -1) { return true; } else { return false; } }
	Object getObject(Object obj) { foreach(Object oObj : m_Objects) { if(oObj == obj) { return oObj; } } return obj; }
	Object getObjectByID(int objID) { foreach(Object oObj : m_Objects) { if(oObj.GetID() == objID) { return oObj; } } return NULL; }

	override void Init()
	{
		super.Init();
		objHandler = new objectHandler();
		LoadScene();
		ExportObjectLoad();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(clearWeather, 5000, true);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(setDaytime, 5000, true);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(saveSettings, 60000, true);
	}

	ref SceneManager sceneManager;
	SceneData currentSceneData;

	bool isEditing() { return m_ObjectEditorActive; }
	bool isSelected() { if (m_SelectedObject) { return true; } else { return false; } }
	void ObjectEditor() { sceneManager = new SceneManager(); }
	void ExportObjectLoad() { /* Copy paste your exported objects in here to make it look like the example below:*/ }
	void EditorState(bool state) {
		if ( m_ObjectEditorActive == state ) { return; } m_ObjectEditorActive = state;
		if ( m_ObjectEditorActive ) { COM_GetPB().MessageStatus("Object Editor Enabled"); } else { COM_GetPB().MessageStatus("Object Editor Disabled"); }
	}
	void ToggleEditor() {
		m_ObjectEditorActive = !m_ObjectEditorActive;
		if ( m_ObjectEditorActive ) { COM_GetPB().MessageStatus("Object Editor Enabled"); } else { COM_GetPB().MessageStatus("Object Editor Disabled"); }
	}

	void toggleCamera() { if (m_ObjectEditorActive) { CameraTool.Cast(COM_GetModuleManager().GetModule(CameraTool)).ToggleCamera(); } }

	string getObjectJSON(Object m_object) {
		vector obj_pos = m_object.GetPosition();
		vector obj_rtn = m_object.GetOrientation();
		string obj_r = COM_FormatFloat(obj_rtn[2], 6);
		string obj_p = COM_FormatFloat(obj_rtn[1], 6);
		string obj_y = COM_FormatFloat(obj_rtn[0], 6);
		if (obj_r == "0" || obj_r == "-0") { obj_r = "0.000000"; }
		if (obj_p == "0" || obj_p == "-0") { obj_p = "0.000000"; }
		if (obj_y == "0" || obj_y == "-0") { obj_y = "0.000000"; }
		return "\n{\"name\": \"" + m_object.GetType() + "\", \"pos\": [ " + vecToString(obj_pos, ", ") + " ], " + "\"ypr\": [ " + obj_y + ", " + obj_p + ", " + obj_r + " ] },";
	}
	void ExportScene() {
		array<Object> objects = new array<Object>;
		if(isEditing()) { objects = ObjectInfoMenu.currentObjects; } else { objects = m_Objects; }
		if(COM_CTRL()) { ExportMapGroupPos(objects); } else if(COM_SHIFT()) { ExportMapGroupProtos(objects); }
		else {
			string toCopy; foreach(Object obj : objects) { toCopy += getObjectJSON(obj); }
			GetGame().CopyToClipboard(toCopy); COM_Message("Copied current objects list to clipboard in JSON form.");
		}
	}
	string getMapGroupProto(Object m_object) {
		vector obj_pos = m_object.GetPosition();
		vector obj_rtn = m_object.GetOrientation();
		string obj_x = COM_FormatFloat(obj_pos[0] * -1, 6);
		string obj_h = COM_FormatFloat(obj_pos[1], 6);
		string obj_z = COM_FormatFloat(obj_pos[2], 6);
		string obj_r = COM_FormatFloat(obj_rtn[2], 6);
		string obj_p = COM_FormatFloat(obj_rtn[1] * -1, 6);
		string obj_y = COM_FormatFloat(obj_rtn[0], 6);
		if (obj_r == "0" || obj_r == "-0") { obj_r = "0.000000"; }
		if (obj_p == "0" || obj_p == "-0") { obj_p = "0.000000"; }
		if (obj_y == "0" || obj_y == "-0") { obj_y = "0.000000"; }
		return "\n    <proxy type=\"" + m_object.GetType() + "\" pos=\"" + obj_z + " " + obj_h + " " + obj_x + "\" rpy=\"" + obj_p + " " + obj_y + " " + obj_r + "\" />";
	}
	void ExportMapGroupProtos(array<Object> objects = NULL) {
		if(!objects) { objects = ObjectInfoMenu.currentObjects; }
		string toCopy; foreach(Object m_object : objects) { toCopy += getMapGroupProto(m_object); }
		GetGame().CopyToClipboard(toCopy); COM_Message("Copied current objects list to clipboard in MapGroupProto form.");
	}
	string getMapGroupPos(Object m_object) {
        vector obj_pos = m_object.GetPosition();
		vector obj_rtn = m_object.GetOrientation();
		string obj_r = COM_FormatFloat(obj_rtn[2], 6);
		string obj_p = COM_FormatFloat(obj_rtn[1], 6);
		string obj_y = COM_FormatFloat(obj_rtn[0], 6);
		if (obj_r == "0" || obj_r == "-0") { obj_r = "0.000000"; }
		if (obj_p == "0" || obj_p == "-0") { obj_p = "0.000000"; }
		if (obj_y == "0" || obj_y == "-0") { obj_y = "0.000000"; }
		return "\n    <group name=\"Item_" + m_object.GetType() + "\" pos=\"" + vecToString(obj_pos, " ") + "\" rpy=\"" + obj_r + " " + obj_p + " " + obj_y + "\" />";
	}
	void ExportMapGroupPos(array<Object> objects = NULL)
	{
		if(!objects) { objects = ObjectInfoMenu.currentObjects; }
		string toCopy; foreach(Object m_object : objects) { toCopy += getMapGroupPos(m_object); }
		GetGame().CopyToClipboard(toCopy); COM_Message("Copied current objects list to clipboard in MapGroupPos form.");
	}

    void BackupSceneFile() {
        for (int i = 0; i < 100; i++) {
            string fileName = BASE_BACKUP_DIR + objectsFile + i.ToString();
            for (int ex = 25; ex >= 0; ex--) {
                string newFileName = BASE_BACKUP_DIR + ex.ToString() + "_" + objectsFile + i.ToString() +".json";
                if(FileExist(newFileName)) {
                    if(ex == 25) { DeleteFile(newFileName); continue; }
                    CopyFile(newFileName, BASE_BACKUP_DIR + (ex + 1).ToString() + "_" + objectsFile + i.ToString() +".json");
                }
            }
        }
        for (int ii = 0; ii < 100; ii++) {
            if(!FileExist(BASE_COM_DIR + "\\" + objectsFile + ii.ToString() +".json")) { break; }
            CopyFile(BASE_COM_DIR + "\\" + objectsFile + ii.ToString() +".json",  BASE_BACKUP_DIR + "0_" + objectsFile + ii.ToString() +".json");
        }
    }

	void SaveScene()
	{
		MakeDirectory( BASE_COM_DIR ); MakeDirectory( BASE_BACKUP_DIR );
		ref ObjectSave scene = new ObjectSave();
		ref array<ref ObjectSave> scenes = new ref array<ref ObjectSave>;
		int objs = 0;
		foreach(auto data : objHandler.objectData) {
			ref ObjectData oData = new ObjectData(data.param2, data.param3, data.param4, data.param5, data.param1.GetPosition(), data.param1.GetOrientation());
			//auto oData = getObjectData(obj); oData.pos = obj.GetPosition(); oData.ypr = obj.GetOrientation(); //oData.date = getDateTime();
			scene.Objects.Insert(oData); objs += 1;
			if (objs == 175) { objs = 0; scenes.Insert(scene); scene = new ObjectSave(); }
		}
		scenes.Insert(scene);
		int fileNum = 0; BackupSceneFile();
		foreach(auto sc : scenes)  {
		    JsonFileLoader<ObjectSave>.JsonSaveFile(BASE_COM_DIR + "\\" + objectsFile + fileNum.ToString() + ".json", sc);
			fileNum += 1;
		}
        for (int i = fileNum; i < 100; i++) {
            if(FileExist(BASE_COM_DIR + "\\" + objectsFile + i.ToString() + ".json")) { DeleteFile(BASE_COM_DIR + "\\" + objectsFile + i.ToString() + ".json"); }
        }
	    COM_Message("Saved objects to " + BASE_COM_DIR);
	}
	void LoadScene()
	{
	    objHandler.objectData.Clear();
		undoList = new array<ref array<ref Param8<string, int, string, vector, vector, string, string, int>>>;
		redoList = new array<ref array<ref Param8<string, int, string, vector, vector, string, string, int>>>;
		for (int i = 0; i < 100; i++) {
			if (!FileExist(BASE_COM_DIR + "\\" + objectsFile + i.ToString() + ".json")) { break; }
			ref ObjectSave scene = new ObjectSave();
			JsonFileLoader<ObjectSave>.JsonLoadFile(BASE_COM_DIR + "\\" + objectsFile + i.ToString() + ".json", scene);
			array<ref Param7<string, vector, vector, string, string, int, int>> objects = new array<ref Param7<string, vector, vector, string, string, int, int>>;
			foreach(auto data : scene.Objects)  {
				objects.Insert(new Param7<string, vector, vector, string, string, int, int>(data.name, data.pos, data.ypr, data.group, data.date, data.active, -1));
				//objHandler.newObjectData(SpawnObject(data.name, data.pos, data.ypr, "LOAD", false), data.group, data.active, data.date, true);
			}
			SpawnBulkObjects(objects, true);
	        Print("All objects loaded from save file: " + BASE_COM_DIR + "\\" + objectsFile + i.ToString() + ".json");
		}
	}

	void saveSettings() {
	    ref SettingsSave save = new SettingsSave(); settings.lastPosition = COM_GetPB().GetPosition(); settings.lastOrientation = COM_GetPB().GetOrientation();
	    save.Settings.Insert(settings); JsonFileLoader<SettingsSave>.JsonSaveFile(filenameSettings, save);
		Print("Settings saved to '" + filenameSettings + "'");
	}

	void loadSettings()
	{
		MakeDirectory(BASE_COM_DIR); MakeDirectory(BASE_BACKUP_DIR);
		auto settingsFile = OpenFile(filenameSettings, FileMode.READ);
		if (!settingsFile) { settings = new SettingsData(); saveSettings(); Print("Settings file not found. Generating new file '" + filenameSettings + "'"); return; }
		CloseFile(settingsFile);
		ref SettingsSave settingsTemp = new SettingsSave();
		JsonFileLoader<SettingsSave>.JsonLoadFile(filenameSettings, settingsTemp);
		if(settingsTemp.Settings.Count() > 0) { settings = new SettingsData(settingsTemp.Settings[0]); Print("Settings loaded from '" + filenameSettings + "'"); }
		else { settings = new SettingsData(); saveSettings(); Print("Settings file corrupt. Generating new file '" + filenameSettings + "'"); }
	}

	void saveGroupInputs() { 
		if(!ObjectInfoMenu.isReady) { return; }
		lastEditorGroup = currentGroup; lastEditorGroupInput = ObjectInfoMenu.groupEditorNameInput.GetText();
		lastObjectGroupInput = ObjectMenu.groupSelectorNameInput.GetText(); lastObjectSearch = ObjectMenu.m_SearchBox.GetText();
	}

	void SelectObject(Object object) {
		if (object.IsInherited(PlayerBase)) { return; }
		if(m_SelectedObject == object) { groupEditor = false; return; }
		if (hasObject(object)) {
			m_SelectedObject = object; ObjectInfoMenu.updateInfoPos();
			for (int i = 0; i < 100; i++) { Undo[i] = false; Redo[i] = false; }
			objHandler.setObjectActiveText(); objHandler.setObjectDateText();
            ObjectInfoMenu.infoPosX.ClearFlags(WidgetFlags.IGNOREPOINTER); ObjectInfoMenu.infoPosY.ClearFlags(WidgetFlags.IGNOREPOINTER); ObjectInfoMenu.infoPosZ.ClearFlags(WidgetFlags.IGNOREPOINTER);
            ObjectInfoMenu.infoPosYaw.ClearFlags(WidgetFlags.IGNOREPOINTER); ObjectInfoMenu.infoPosPitch.ClearFlags(WidgetFlags.IGNOREPOINTER); ObjectInfoMenu.infoPosRoll.ClearFlags(WidgetFlags.IGNOREPOINTER);
		}
		groupEditor = false;
	}

	void DeselectObject() {
		m_SelectedObject = NULL;
		ObjectInfoMenu.updateInfoPos();
		groupEditor = false;
		if(ObjectInfoMenu.isReady) {
			ObjectInfoMenu.selectedObjectWidget.SetText("No object selected");
			ObjectInfoMenu.selectedObjectDisplayNameWidget.SetText("");
			ObjectInfoMenu.groupEditorName.SetText("None");
			ObjectInfoMenu.activeButton.SetText("Inactive");
            ObjectInfoMenu.infoPosX.SetFlags(WidgetFlags.IGNOREPOINTER); ObjectInfoMenu.infoPosY.SetFlags(WidgetFlags.IGNOREPOINTER); ObjectInfoMenu.infoPosZ.SetFlags(WidgetFlags.IGNOREPOINTER);
            ObjectInfoMenu.infoPosYaw.SetFlags(WidgetFlags.IGNOREPOINTER); ObjectInfoMenu.infoPosPitch.SetFlags(WidgetFlags.IGNOREPOINTER); ObjectInfoMenu.infoPosRoll.SetFlags(WidgetFlags.IGNOREPOINTER);
		}
	}
	
	Object SpawnObject(string type, vector position, vector orientation, string group, string date = "", int active = -1) {
	    Object obj = createObject(type, position, orientation);
		m_Objects.Insert(obj); if(date != "") { objHandler.setObjectData(obj, group, active, date); } else { objHandler.setObjectData(obj, group); }
		SelectObject(obj); 
		array<Object> oObjects = new array<Object>; oObjects.Insert(obj); addAction("Spawn", oObjects);
	    return obj;
	}
	void DeleteObject() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if (groupEditor) { 
		    deleteAllClicks++;
		    if(deleteAllClicks >= 3) {
		        DeleteBulkObjects(ObjectInfoMenu.currentObjects);
				ObjectInfoMenu.selectObjectGroup("All");
                deleteAllClicks = 0;
			    COM_Message("All objects in current list have been deleted. Click 'RELOAD' now if this was a mistake.");
			} else { 
				int timesLeft = 3-deleteAllClicks;
				COM_Message("Group editor is active. Press 'Delete' " + timesLeft.ToString() + " more times to delete all objects in current list.");
			}
			ObjectInfoMenu.SetButtonFocus(ObjectInfoMenu.deleteAllButton);
		} else {
			array<Object> oObjects = new array<Object>; oObjects.Insert(m_SelectedObject); addAction("Delete", oObjects);
			m_Objects.RemoveItem(m_SelectedObject); ObjectInfoMenu.currentObjects.RemoveItem(m_SelectedObject); objHandler.delObjectData(m_SelectedObject);
			COM_Message("Deleted " + m_SelectedObject.GetType() + " at [" + COM_VectorToString(m_SelectedObject.GetPosition()) + "]");
			objectDelete(m_SelectedObject); DeselectObject();
		}
	}
	void SpawnBulkObjects(array<ref Param7<string, vector, vector, string, string, int, int>> objects, bool reload = false, bool redo = false) {
		string cgroup = ""; array<Object> spawnedObjects = new array<Object>;
		foreach(auto oData : objects) {
			Object obj = createObject(oData.param1, oData.param2, oData.param3);
			m_Objects.Insert(obj); if(oData.param5 != "") { objHandler.setObjectData(obj, oData.param4, oData.param6, oData.param5, reload); } else { objHandler.setObjectData(obj, oData.param4); }
			cgroup = oData.param4;
			if(objects.Count() == 1) { SelectObject(obj); }
			spawnedObjects.Insert(obj); updateUndoRedoObject(oData.param7, obj.GetID());
        }
		if(!reload) { if(currentGroup != cgroup && objects.Count() > 1) { ObjectInfoMenu.selectObjectGroup(cgroup); } addAction("Spawn", spawnedObjects); }
		
		objHandler.updateObjectGroups(); ObjectInfoMenu.UpdateObjectList();
	}
	void DeleteBulkObjects(array<Object> objects, bool reload = false) {
		if (!m_ObjectEditorActive) { return; }
		if(!reload) { addAction("Delete", objects); }
		string oldGroup;
		array<Object> newObjects = new array<Object>; foreach (int i, Object oObj : objects) { if(i == 0) { oldGroup = objHandler.getObjectGroup(oObj); } newObjects.Insert(getObject(oObj)); }
		foreach (Object obj : newObjects) {
			if(!obj) { continue; }
		    //if(!reload) { Print("Deleted " + obj.GetType() + " at [" + COM_VectorToString(obj.GetPosition()) + "]"); }
		    m_Objects.RemoveItem(obj); ObjectInfoMenu.currentObjects.RemoveItem(obj); objHandler.delObjectData(obj); objectDelete(obj);
		}
        DeselectObject();
		//objHandler.updateObjectGroups(); ObjectInfoMenu.UpdateObjectList(); ObjectInfoMenu.selectObjectGroup("All");
		COM_Message("Deleted all objects in group '" + oldGroup + "'");
	}

	void updateUndoRedoObject(int oldObjectID, int newObjectID) {
		if(newObjectID == -1) { return; }
		if(undoList) { foreach(auto acts : undoList) { foreach(int i, auto act : acts) { if(act.param2 == oldObjectID) { act.param2 = newObjectID; acts.Set(i, act); } } } }
		if(redoList) { foreach(auto actions : redoList) { foreach(int ind, auto action : actions) { if(action.param2 == oldObjectID) { action.param2 = newObjectID; actions.Set(i, action); } } } }
	}

	void addAction(string type, ref array<Object> objects = NULL, bool redo = false) { // Move, Rotate, Delete, Spawn
		ref array<ref Param8<string, int, string, vector, vector, string, string, int>> actions = new array<ref Param8<string, int, string, vector, vector, string, string, int>>;
		if(!objects) { objects = new array<Object>; if(groupEditor) { foreach (Object oObj : ObjectInfoMenu.currentObjects) { objects.Insert(oObj); } } else { objects.Insert(m_SelectedObject); } }
		foreach (Object obj : objects) {
			auto oData = objHandler.getObjectData(obj); vector pos = oData.param1.GetPosition(), ypr = oData.param1.GetOrientation();
			//Print("Action: " + type + ", Object: " + oData.param2 + ", Group: " + oData.param3 + ", Pos: " + COM_VectorToString(pos) + ", ypr: " + COM_VectorToString(ypr)); 
			actions.Insert(new Param8<string, int, string, vector, vector, string, string, int>(type, obj.GetID(), oData.param2, pos, ypr, oData.param3, oData.param4, oData.param5));
		}
		if(actions.Count() > 0) { if(!redo) { undoList.Insert(actions); } else { redoList.Insert(actions); } }
		//Print("Action added: " + type);
	}
	void undoAction(bool redo = false)
	{
		if (!m_ObjectEditorActive) { return; }
		string undoRedoText = "Undo"; if(redo) { undoRedoText = "Redo"; } // Print("Redoing last action."); } else { Print("Undoing last action."); }
		int index = undoList.Count() - 1; auto actions = undoList.Get(index); if(redo) { index = redoList.Count() - 1; actions = redoList.Get(index); }
		if(!actions) { if(!redo) { COM_Message("There are no actions to undo."); } else { COM_Message("There are no objects to redo."); } return; }
		ref array<ref Param8<string, int, string, vector, vector, string, string, int>> oActions = new array<ref Param8<string, int, string, vector, vector, string, string, int>>;
		string type = "", newType = "";
		foreach(auto act : actions) {
			type = act.param1; newType = act.param1;
			if(newType == "Spawn") { newType = "Delete"; } else if(newType == "Delete") { newType = "Spawn"; }
			vector pos = act.param4, ypr = act.param5; Object aObj = getObjectByID(act.param2); string group = act.param6; int active = act.param8;
			if(aObj) { 
				//Print("Updating position for: " + act.param3);
				pos = aObj.GetPosition(); ypr = aObj.GetOrientation(); auto oData = objHandler.getObjectData(aObj); group = oData.param3; active = oData.param5;
			}
			//Print("ReverseAction: " + newType + ", Object: " + act.param3 + ", Group: " + group + ", Pos: " + COM_VectorToString(pos) + ", ypr: " + COM_VectorToString(ypr)); 
			oActions.Insert(new ref Param8<string, int, string, vector, vector, string, string, int>(newType, act.param2, act.param3, pos, ypr, group, act.param7, active));
		}
		if(redo) { undoList.Insert(oActions); Print("Reverse action added to UndoList: " + newType); } else { redoList.Insert(oActions); Print("Reverse action added to RedoList: " + newType); }
		foreach(int i, auto action : actions) {
			//Print("Action = " + type);
			if(type == "Move") { Object moveObj = getObjectByID(action.param2); moveObj.SetPosition(action.param4); }
			else if(type == "Rotate") { Object rotateObj = getObjectByID(action.param2); rotateObj.SetOrientation(action.param5); }
			else if(type == "Change Group") { Object groupObj = getObjectByID(action.param2); objHandler.setObjectData(groupObj, action.param6); }
			else if(type == "Change Active") { Object activeObj = getObjectByID(action.param2); objHandler.setObjectData(activeObj, "N/A", action.param8); objHandler.setObjectActiveText(); }
			else if(type == "Spawn") {
				array<Object> delObjects = new array<Object>;
				foreach(auto oObj : actions) { Object delObj = getObjectByID(oObj.param2); delObjects.Insert(delObj); } //Print("Added " + oObj.param3 + " to delete list."); 
				DeleteBulkObjects(delObjects, true); break;
			} else if(type == "Delete") {
				array<ref Param7<string, vector, vector, string, string, int, int>> objects = new array<ref Param7<string, vector, vector, string, string, int, int>>;
				string groupName = "All";
				foreach(auto obj : actions) {
					objects.Insert(new Param7<string, vector, vector, string, string, int, int>(obj.param3, obj.param4, obj.param5, obj.param6, obj.param7, obj.param8, obj.param2));
					groupName = obj.param6;
				}
				SpawnBulkObjects(objects, true, redo); ObjectInfoMenu.selectObjectGroup(groupName); break;
			} 
		}
		string grouptxt = "", objname = ""; auto aData = actions.Get(0);
		if(aData) { if(actions.Count() > 1) { grouptxt = " Group "; objname = aData.param6; } else { objname = aData.param3; } }
		COM_Message(undoRedoText + "ing Action: " + type + grouptxt + " '" + objname + "'");
		if(!redo) { undoList.Remove(index); } else { redoList.Remove(index); }
	}

	void DupeObject() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if(groupEditor) { DupeGroup(); return; } else if(COM_CTRL()) { DupeObjectInPlace(m_SelectedObject); return; }
		COM_Message("Duplicating \"" + m_SelectedObject.GetType() + "\" at pointer.");
		vector pos = COM_GetPointerPos(); if(lockYAxis) { vector oldPos = m_SelectedObject.GetPosition(); pos[1] = oldPos[1]; }
		Object obj = SpawnObject(m_SelectedObject.GetType(), pos, m_SelectedObject.GetOrientation(), objHandler.getObjectGroup(m_SelectedObject));
		if(!lockYAxis) { COM_PlaceObjectOnGround(obj); }
	}

	void DupeGroup() {
		ref array<vector> offsets = getGroupPositionOffsets();
		vector pos = COM_GetPointerPos(); if(lockYAxis) { vector oldPos = m_SelectedObject.GetPosition(); pos[1] = oldPos[1]; }
		string groupName = objHandler.getObjectGroup(m_SelectedObject); 
		if(groupName.Contains("_Copy")) { TStringArray strSplit = new TStringArray; groupName.Split("_Copy", strSplit); groupName = strSplit.Get(0); }
		groupName += "_Copy";
		auto groups = objHandler.getObjectGroups();
		for(int i = 0; i < 100; i++) { if(groups.Find(groupName + i.ToString()) == -1) { groupName = groupName + i.ToString(); break; } }
		Object oObj = createObject(m_SelectedObject.GetType(), pos, m_SelectedObject.GetOrientation()); 
		if(!lockYAxis) { COM_PlaceObjectOnGround(oObj); } pos = oObj.GetPosition(); objectDelete(oObj);

		array<ref Param7<string, vector, vector, string, string, int, int>> objects = new array<ref Param7<string, vector, vector, string, string, int, int>>;
		foreach(int index, vector offset : offsets)  {
            Object obj = ObjectInfoMenu.currentObjects.Get(index);
			vector newPos = pos; newPos[0] = newPos[0] + offset[0]; newPos[1] = newPos[1] + offset[1]; newPos[2] = newPos[2] + offset[2];
			objects.Insert(new Param7<string, vector, vector, string, string, int, int>(obj.GetType(), newPos, obj.GetOrientation(), groupName, "", -1, -1));
				//objHandler.newObjectData(SpawnObject(data.name, data.pos, data.ypr, "LOAD", false), data.group, data.active, data.date, true);
		}
		SpawnBulkObjects(objects); ObjectInfoMenu.selectObjectGroup(groupName);
		COM_Message("Duplicating group \"" + currentGroup + "\" at pointer.");
	}
	void GroundObject() { 
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		addAction("Move");
		ref array<vector> offsets = getGroupPositionOffsets();
		COM_SnapObjectToGround(m_SelectedObject);
		if(groupEditor) { GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(GroundObjectGroup, 100, false, offsets); }
	}
	void GroundObjectGroup(array<vector> offsets) { 
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		vector pos = m_SelectedObject.GetPosition();
		foreach(int index, vector offset : offsets)  {
            Object obj = ObjectInfoMenu.currentObjects.Get(index); if(obj == m_SelectedObject) { continue; }
			vector newPos = pos; newPos[0] = newPos[0] + offset[0]; newPos[1] = newPos[1] + offset[1]; newPos[2] = newPos[2] + offset[2];
			obj.SetPosition(newPos); COM_ForceTargetCollisionUpdate(obj);
		}
		COM_Message("Placing group \"" + currentGroup + "\" on ground.");
	}

	void DupeObjectInPlace(Object object)
	{
		vector position = object.GetPosition();
		COM_Message("Duplicating \"" + object.GetType() + "\" in place.");
		Object obj = SpawnObject(object.GetType(), position, object.GetOrientation(), objHandler.getObjectGroup(m_SelectedObject));
	}


	void AddUndoPos(vector position, vector orientation) {
		for (int i = 98; i >= 0; i--) { if (Undo[i]) { UndoPos[i + 1] = UndoPos[i]; UndoOr[i + 1] = UndoOr[i]; Undo[i + 1] = true; } }
		UndoPos[0] = position; UndoOr[0] = orientation; Undo[0] = true;
		//ref array<ref Param4<Object, string, vector, vector>> new array<Param4<Object, string, vector, vector>>
		//UndoList.Insert(new array<Param4<Object, string, vector, vector>>);
	}

	void AddRedoPos(vector position, vector orientation) {
		for (int i = 98; i >= 0; i--) { if (Redo[i]) { RedoPos[i + 1] = RedoPos[i]; RedoOr[i + 1] = RedoOr[i]; Redo[i + 1] = true; } }
		RedoPos[0] = position; RedoOr[0] = orientation; Redo[0] = true;
	}
	void UndoObject() { if(COM_CTRL()) { undoAction(); } }
	void RedoObject() { if(COM_CTRL()) { undoAction(true); } }

	void StartDragObject() { if (!m_ObjectEditorActive || !m_SelectedObject) { return; } m_IsDragging = true; addAction("Move"); }

	void CancelDragObject() {
		if (!m_IsDragging) { return; }
		if (m_ObjectDragCancel) { COM_Message("Cancelled request to cancel drag."); } else { COM_Message("Cancelling current drag when dropped."); }
		m_ObjectDragCancel = !m_ObjectDragCancel;
	}

	void FinishDragObject() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		m_IsDragging = false;
		if (m_ObjectDragCancel) { COM_Message("Cancelling last drag."); undoAction(); m_ObjectDragCancel = false; } 
	}
	
	array<vector> getGroupPositionOffsets() {
		if (!m_ObjectEditorActive || !m_SelectedObject || !groupEditor) { return NULL; }
		vector pos = m_SelectedObject.GetPosition(); ref array<vector> offsets = new array<vector>;
		foreach (Object obj : ObjectInfoMenu.currentObjects) {
		    vector objPos = obj.GetPosition(); objPos[0] = objPos[0] - pos[0]; objPos[1] = objPos[1] - pos[1]; objPos[2] = objPos[2] - pos[2]; offsets.Insert(objPos);
		}
		 return offsets;
	}
	
	void getGroupPositions(bool redo = false) {
		if (!m_ObjectEditorActive || !m_SelectedObject || !groupEditor) { return; }
		groupPositions = new array<vector>; groupPositionsRedo = new array<vector>;
		foreach (Object obj : ObjectInfoMenu.currentObjects) { if(!redo) { groupPositions.Insert(obj.GetPosition()); } else { groupPositionsRedo.Insert(obj.GetPosition()); } }
	}

	void resetGroupPositions(bool redo = false) {
		if (!m_ObjectEditorActive || !m_SelectedObject || !groupEditor) { return; }
		if(!redo) {
			getGroupPositions(true); foreach (int gI, Object gObj : ObjectInfoMenu.currentObjects) { gObj.SetPosition(groupPositions.Get(gI)); } groupPositions.Clear();
		} else { foreach (int i, Object obj : ObjectInfoMenu.currentObjects) { obj.SetPosition(groupPositionsRedo.Get(i)); } groupPositionsRedo.Clear(); }
		
	}

	void DragObject() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		m_IsDragging = true;
		vector dir = GetGame().GetPointerDirection(), from = GetGame().GetCurrentCameraPosition(), to = from + (dir * 10000), contact_pos, contact_dir, bbox; int contact_component;
		m_SelectedObject.GetCollisionBox(bbox);
		if (DayZPhysics.RaycastRV(from, to, contact_pos, contact_dir, contact_component, NULL, m_SelectedObject, COM_GetPB(), false, false)) {
		    if(!groupEditor) {
                if(lockYAxis) { contact_pos[1] = m_SelectedObject.GetPosition()[1]; }
                m_SelectedObject.SetPosition(contact_pos);
                if(!lockYAxis) { COM_PlaceObjectOnGround(m_SelectedObject); } ObjectInfoMenu.updateInfoPos();
			} else {
			    ref array<vector> offsets = getGroupPositionOffsets();
                if(lockYAxis) { contact_pos[1] = m_SelectedObject.GetPosition()[1]; }
				m_SelectedObject.SetPosition(contact_pos);
                if(!lockYAxis) { COM_PlaceObjectOnGround(m_SelectedObject); } ObjectInfoMenu.updateInfoPos();
				contact_pos = m_SelectedObject.GetPosition();
			    foreach (int index, vector offset : offsets) {
	                Object obj = ObjectInfoMenu.currentObjects.Get(index);
                    if(obj == m_SelectedObject) { continue; }
                    vector newPos = contact_pos;
                    newPos[0] = newPos[0] + offset[0]; newPos[1] = newPos[1] + offset[1]; newPos[2] = newPos[2] + offset[2];
                    //COM_Message(COM_VectorToString(oldPos) + " - " + COM_VectorToString(newPos));
                    obj.SetPosition(newPos);
			    }
			}
		}
	}

	void ClickObject() {
		if (!m_ObjectEditorActive) { return; }
		Widget widgetCursor = GetGame().GetUIManager().GetWidgetUnderCursor();
		if (widgetCursor && widgetCursor.GetName() != "EditorMenu") { return; }
		DeselectObject();
		vector dir = GetGame().GetPointerDirection(), from = GetGame().GetCurrentCameraPosition(), to = from + ( dir * 1000 );
		set<Object> objects = COM_GetObjectsAt(from, to, COM_GetPB(), 0.3); if (!objects) { return; }
		for (int newObject = 0; newObject < objects.Count(); ++newObject) { Object obj = objects.Get(newObject); if(obj) { SelectObject(obj); break; } }
	}
	
	void moveNorth() { if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; } addAction("Move"); moveNorthHOLD(); }
	void moveNorthHOLD() {
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; }
		auto amount = settings.moveSpeedSlow; vector currentPos; 
		if(COM_CTRL() && COM_SHIFT()) { amount = settings.moveSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.moveSpeedMedium; }
		if(groupEditor) { foreach(Object obj : ObjectInfoMenu.currentObjects) { currentPos = obj.GetPosition(); currentPos[2] = currentPos[2] + amount; obj.SetPosition(currentPos); } }
		else { currentPos = m_SelectedObject.GetPosition(); currentPos[2] = currentPos[2] + amount; m_SelectedObject.SetPosition(currentPos); }
		ObjectInfoMenu.updateInfoPos();
	}
	void moveSouth() { if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; } addAction("Move"); moveSouthHOLD(); }
	void moveSouthHOLD() {
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; }
		auto amount = settings.moveSpeedSlow; vector currentPos; 
		if(COM_CTRL() && COM_SHIFT()) { amount = settings.moveSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.moveSpeedMedium; }
		if(groupEditor) { foreach(Object obj : ObjectInfoMenu.currentObjects) { currentPos = obj.GetPosition(); currentPos[2] = currentPos[2] - amount; obj.SetPosition(currentPos); } }
		else { currentPos = m_SelectedObject.GetPosition(); currentPos[2] = currentPos[2] - amount; m_SelectedObject.SetPosition(currentPos); }
		ObjectInfoMenu.updateInfoPos();
	}
	void moveEast() { if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; } addAction("Move"); moveEastHOLD(); }
	void moveEastHOLD() {
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; }
		auto amount = settings.moveSpeedSlow; vector currentPos; 
		if(COM_CTRL() && COM_SHIFT()) { amount = settings.moveSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.moveSpeedMedium; }
		if(groupEditor) { foreach(Object obj : ObjectInfoMenu.currentObjects) { currentPos = obj.GetPosition(); currentPos[0] = currentPos[0] + amount; obj.SetPosition(currentPos); } }
		else { currentPos = m_SelectedObject.GetPosition(); currentPos[0] = currentPos[0] + amount; m_SelectedObject.SetPosition(currentPos); }
		ObjectInfoMenu.updateInfoPos();
	}
	void moveWest() { if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; } addAction("Move"); moveWestHOLD(); }
	void moveWestHOLD() {
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; }
		auto amount = settings.moveSpeedSlow; vector currentPos; 
		if(COM_CTRL() && COM_SHIFT()) { amount = settings.moveSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.moveSpeedMedium; }
		if(groupEditor) { foreach(Object obj : ObjectInfoMenu.currentObjects) { currentPos = obj.GetPosition(); currentPos[0] = currentPos[0] - amount; obj.SetPosition(currentPos); } }
		else { currentPos = m_SelectedObject.GetPosition(); currentPos[0] = currentPos[0] - amount; m_SelectedObject.SetPosition(currentPos); }
		ObjectInfoMenu.updateInfoPos();
	}
	void moveUp() { if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; } addAction("Move"); moveUpHOLD(); }
	void moveUpHOLD() {
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; }
		auto amount = settings.moveSpeedSlow; vector currentPos; 
		if(COM_CTRL() && COM_SHIFT()) { amount = settings.moveSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.moveSpeedMedium; }
		if(groupEditor) { foreach(Object obj : ObjectInfoMenu.currentObjects) { currentPos = obj.GetPosition(); currentPos[1] = currentPos[1] + amount; obj.SetPosition(currentPos); } }
		else { currentPos = m_SelectedObject.GetPosition(); currentPos[1] = currentPos[1] + amount; m_SelectedObject.SetPosition(currentPos); }
		ObjectInfoMenu.updateInfoPos();
	}
	void moveDown() { if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; } addAction("Move"); moveDownHOLD(); }
	void moveDownHOLD() {
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; }
		auto amount = settings.moveSpeedSlow; vector currentPos; 
		if(COM_CTRL() && COM_SHIFT()) { amount = settings.moveSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.moveSpeedMedium; }
		if(groupEditor) { foreach(Object obj : ObjectInfoMenu.currentObjects) { currentPos = obj.GetPosition(); currentPos[1] = currentPos[1] - amount; obj.SetPosition(currentPos); } }
		else { currentPos = m_SelectedObject.GetPosition(); currentPos[1] = currentPos[1] - amount; m_SelectedObject.SetPosition(currentPos); }
		ObjectInfoMenu.updateInfoPos();
	}
	void rotateLeft() { 
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; } if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		addAction("Rotate"); rotateLeftHOLD();
	}
	void rotateLeftHOLD() {
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; }
		if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
        auto amount = settings.rotateSpeedSlow; if(COM_CTRL() && COM_SHIFT()) { amount = settings.rotateSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.rotateSpeedMedium; }
		/*if(groupEditor) {
            vector pos = m_SelectedObject.GetPosition(); ref array<vector> offsets = new array<vector>;
            foreach (Object obj : ObjectInfoMenu.currentObjects) {
		        vector newPos = RotateAroundPoint(m_SelectedObject.GetPosition(), obj.GetPosition(), vector.Zero, amount, amount);

                vector objPos = obj.GetPosition(); objPos[0] = objPos[0] - pos[0]; objPos[1] = objPos[1] - pos[1]; objPos[2] = objPos[2] - pos[2]; offsets.Insert(objPos);
            }
		} else {*/
            vector currentOr = m_SelectedObject.GetOrientation();
            currentOr[0] = currentOr[0] - amount;
            m_SelectedObject.SetOrientation(currentOr); ObjectInfoMenu.updateInfoPos();
		//}
	}
	void rotateRight() { 
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; } if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		addAction("Rotate"); rotateRightHOLD();
	}
	void rotateRightHOLD() {
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; }
		if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		vector currentOr = m_SelectedObject.GetOrientation();
		auto amount = settings.rotateSpeedSlow; if(COM_CTRL() && COM_SHIFT()) { amount = settings.rotateSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.rotateSpeedMedium; }
		currentOr[0] = currentOr[0] + amount;
		m_SelectedObject.SetOrientation(currentOr); ObjectInfoMenu.updateInfoPos();
	}
	void flipUp() { 
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; } if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		addAction("Rotate"); flipUpHOLD();
	}
	void flipUpHOLD(){
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; }
		if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		vector currentOr = m_SelectedObject.GetOrientation();
		auto amount = settings.flipSpeedSlow; if(COM_CTRL() && COM_SHIFT()) { amount = settings.flipSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.flipSpeedMedium; }
		currentOr[1] = currentOr[1] - amount;
		m_SelectedObject.SetOrientation(currentOr); ObjectInfoMenu.updateInfoPos();
	}
	void flipDown() { 
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; } if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		addAction("Rotate"); flipDownHOLD();
	}
	void flipDownHOLD(){
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; }
		if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		vector currentOr = m_SelectedObject.GetOrientation();
		auto amount = settings.flipSpeedSlow; if(COM_CTRL() && COM_SHIFT()) { amount = settings.flipSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.flipSpeedMedium; }
		currentOr[1] = currentOr[1] + amount;
		m_SelectedObject.SetOrientation(currentOr); ObjectInfoMenu.updateInfoPos();
	}
	void flipLeft() { 
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; } if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		addAction("Rotate"); flipLeftHOLD();
	}
	void flipLeftHOLD() {
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; }
		if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		vector currentOr = m_SelectedObject.GetOrientation();
		auto amount = settings.flipSpeedSlow; if(COM_CTRL() && COM_SHIFT()) { amount = settings.flipSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.flipSpeedMedium; }
		currentOr[2] = currentOr[2] - amount;
		m_SelectedObject.SetOrientation(currentOr); ObjectInfoMenu.updateInfoPos();
	}
	void flipRight() { 
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; } if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		addAction("Rotate"); flipRightHOLD();
	}
	void flipRightHOLD() {
		if (!m_ObjectEditorActive || (!m_SelectedObject && !groupEditor)) { return; }
		if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		vector currentOr = m_SelectedObject.GetOrientation();
		auto amount = settings.flipSpeedSlow; if(COM_CTRL() && COM_SHIFT()) { amount = settings.flipSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.flipSpeedMedium; }
		currentOr[2] = currentOr[2] + amount;
		m_SelectedObject.SetOrientation(currentOr); ObjectInfoMenu.updateInfoPos();
	}

	override void RegisterKeyMouseBindings()
	{
		//loadSettings();
		Print("Loading keyboard and mouse bindings for ObjectEditor.c");
		//		KeyMouseBinding toggleEditor  = new KeyMouseBinding( GetModuleType(), "ToggleEditor" , "[Shift]+[End]" , "Toggle object editor."            );
		KeyMouseBinding toggleCam = new KeyMouseBinding(GetModuleType(), "toggleCamera", "Toggle the camera tool.", true);
		toggleCam.AddBinding(settings.keyCameraToggleCamera); RegisterKeyMouseBinding(toggleCam);
		KeyMouseBinding sceneSave = new KeyMouseBinding(GetModuleType(), "SaveScene", "Saves current scene of objects", true);
		sceneSave.AddBinding(settings.keyEditorSaveAllObjects); RegisterKeyMouseBinding(sceneSave);
		KeyMouseBinding sceneExport = new KeyMouseBinding(GetModuleType(), "ExportScene", "Exports current scene of objects to clipboard", true);
		sceneExport.AddBinding(settings.keyEditorExportCurrentObjects); RegisterKeyMouseBinding(sceneExport);
		KeyMouseBinding objectSelect = new KeyMouseBinding(GetModuleType(), "ClickObject", "Selects object on cursor.", true);
		objectSelect.AddBinding(settings.keyEditorSelectObject); RegisterKeyMouseBinding(objectSelect);
		KeyMouseBinding objectDrag = new KeyMouseBinding(GetModuleType(), "DragObject", "Drag objects on cursor.", true);
		objectDrag.AddBinding(settings.keyEditorDragObjectHOLD, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectDrag);
		KeyMouseBinding startObjectDrag = new KeyMouseBinding(GetModuleType(), "StartDragObject", "Save positions before dragging.", true);
		startObjectDrag.AddBinding(settings.keyEditorDragObjectHOLD); RegisterKeyMouseBinding(startObjectDrag);
		KeyMouseBinding objectFinishCancelDrag = new KeyMouseBinding(GetModuleType(), "FinishDragObject", "Finish dragging an object.", true);
		objectFinishCancelDrag.AddBinding(settings.keyEditorDragObjectHOLD, KeyMouseActionType.RELEASE); RegisterKeyMouseBinding(objectFinishCancelDrag);
		KeyMouseBinding objectCancelDrag = new KeyMouseBinding(GetModuleType(), "CancelDragObject", "Cancel dragging an object.", true);
		objectCancelDrag.AddBinding(settings.keyEditorCancelDragObject); RegisterKeyMouseBinding(objectCancelDrag);
		KeyMouseBinding objectUndo = new KeyMouseBinding(GetModuleType(), "UndoObject", "Move an object back.", true);
		objectUndo.AddBinding(settings.keyEditorUndoObjectMoveCTRL); RegisterKeyMouseBinding(objectUndo);
		KeyMouseBinding objectUndoHOLD = new KeyMouseBinding(GetModuleType(), "UndoObject", "Move an object back.", true);
		objectUndo.AddBinding(settings.keyEditorUndoObjectMoveCTRL, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectUndoHOLD);
		KeyMouseBinding objectRedo = new KeyMouseBinding(GetModuleType(), "RedoObject", "Move an object back again.", true);
		objectRedo.AddBinding(settings.keyEditorRedoObjectMoveCTRL); RegisterKeyMouseBinding(objectRedo);
		KeyMouseBinding objectRedoHOLD = new KeyMouseBinding(GetModuleType(), "RedoObject", "Move an object back again.", true);
		objectRedo.AddBinding(settings.keyEditorRedoObjectMoveCTRL, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectRedoHOLD);
		KeyMouseBinding objectDelete = new KeyMouseBinding(GetModuleType(), "DeleteObject", "Deletes selected object.", true);
		objectDelete.AddBinding(settings.keyEditorDeleteObject); RegisterKeyMouseBinding(objectDelete);
		KeyMouseBinding objectDupe = new KeyMouseBinding(GetModuleType(), "DupeObject", "Duplicates the selected object.", true);
		objectDupe.AddBinding(settings.keyEditorDuplicateObject); RegisterKeyMouseBinding(objectDupe);
		KeyMouseBinding objectGround = new KeyMouseBinding(GetModuleType(), "GroundObject", "Snaps objects to ground.", true);
		objectGround.AddBinding(settings.keyEditorGroundObject); RegisterKeyMouseBinding(objectGround);
		KeyMouseBinding tabFix = new KeyMouseBinding(GetModuleType(), "DeselectObject", "Fixes issue with tabbing out of the game", true);
		tabFix.AddBinding(settings.keyEditorDeselectObject); RegisterKeyMouseBinding(tabFix);
		KeyMouseBinding objectMoveNorth = new KeyMouseBinding(GetModuleType(), "moveNorth", "Moves object up.", true);
		objectMoveNorth.AddBinding(settings.keyEditorMoveObjectNorth); RegisterKeyMouseBinding(objectMoveNorth);
		KeyMouseBinding objectMoveNorthHOLD = new KeyMouseBinding(GetModuleType(), "moveNorthHOLD", "Moves object up.", true);
		objectMoveNorthHOLD.AddBinding(settings.keyEditorMoveObjectNorth, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectMoveNorthHOLD);
		KeyMouseBinding objectMoveSouth = new KeyMouseBinding(GetModuleType(), "moveSouth", "Moves object down.", true);
		objectMoveSouth.AddBinding(settings.keyEditorMoveObjectSouth); RegisterKeyMouseBinding(objectMoveSouth);
		KeyMouseBinding objectMoveSouthHOLD = new KeyMouseBinding(GetModuleType(), "moveSouthHOLD", "Moves object down.", true);
		objectMoveSouthHOLD.AddBinding(settings.keyEditorMoveObjectSouth, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectMoveSouthHOLD);
		KeyMouseBinding objectMoveEast = new KeyMouseBinding(GetModuleType(), "moveEast", "Moves object right.", true);
		objectMoveEast.AddBinding(settings.keyEditorMoveObjectEast); RegisterKeyMouseBinding(objectMoveEast);
		KeyMouseBinding objectMoveEastHOLD = new KeyMouseBinding(GetModuleType(), "moveEastHOLD", "Moves object right.", true);
		objectMoveEastHOLD.AddBinding(settings.keyEditorMoveObjectEast, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectMoveEastHOLD);
		KeyMouseBinding objectMoveWest = new KeyMouseBinding(GetModuleType(), "moveWest", "Moves object left.", true);
		objectMoveWest.AddBinding(settings.keyEditorMoveObjectWest); RegisterKeyMouseBinding(objectMoveWest);
		KeyMouseBinding objectMoveWestHOLD = new KeyMouseBinding(GetModuleType(), "moveWestHOLD", "Moves object left.", true);
		objectMoveWestHOLD.AddBinding(settings.keyEditorMoveObjectWest, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectMoveWestHOLD);
		KeyMouseBinding objectMoveUp = new KeyMouseBinding(GetModuleType(), "moveUp", "Moves object up.", true);
		objectMoveUp.AddBinding(settings.keyEditorMoveObjectUp); RegisterKeyMouseBinding(objectMoveUp);
		KeyMouseBinding objectMoveUpHOLD = new KeyMouseBinding(GetModuleType(), "moveUpHOLD", "Moves object up.", true);
		objectMoveUpHOLD.AddBinding(settings.keyEditorMoveObjectUp, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectMoveUpHOLD);
		KeyMouseBinding objectMoveDown = new KeyMouseBinding(GetModuleType(), "moveDown", "Moves object down.", true);
		objectMoveDown.AddBinding(settings.keyEditorMoveObjectDown); RegisterKeyMouseBinding(objectMoveDown);
		KeyMouseBinding objectMoveDownHOLD = new KeyMouseBinding(GetModuleType(), "moveDownHOLD", "Moves object down.", true);
		objectMoveDownHOLD.AddBinding(settings.keyEditorMoveObjectDown, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectMoveDownHOLD);
		KeyMouseBinding objectRotateLeft = new KeyMouseBinding(GetModuleType(), "rotateLeft", "Rotate object left.", true);
		objectRotateLeft.AddBinding(settings.keyEditorRotateObjectLeft); RegisterKeyMouseBinding(objectRotateLeft);
		KeyMouseBinding objectRotateLeftHOLD = new KeyMouseBinding(GetModuleType(), "rotateLeftHOLD", "Rotate object left.", true);
		objectRotateLeftHOLD.AddBinding(settings.keyEditorRotateObjectLeft, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectRotateLeftHOLD);
		KeyMouseBinding objectRotateRight = new KeyMouseBinding(GetModuleType(), "rotateRight", "Rotate object right.", true);
		objectRotateRight.AddBinding(settings.keyEditorRotateObjectRight); RegisterKeyMouseBinding(objectRotateRight);
		KeyMouseBinding objectRotateRightHOLD = new KeyMouseBinding(GetModuleType(), "rotateRightHOLD", "Rotate object right.", true);
		objectRotateRightHOLD.AddBinding(settings.keyEditorRotateObjectRight, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectRotateRightHOLD);
		KeyMouseBinding objectFlipUp = new KeyMouseBinding(GetModuleType(), "flipUp", "Flip object up.", true);
		objectFlipUp.AddBinding(settings.keyEditorFlipObjectUp); RegisterKeyMouseBinding(objectFlipUp);
		KeyMouseBinding objectFlipUpHOLD = new KeyMouseBinding(GetModuleType(), "flipUpHOLD", "Flip object up.", true);
		objectFlipUpHOLD.AddBinding(settings.keyEditorFlipObjectUp, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectFlipUpHOLD);
		KeyMouseBinding objectFlipDown = new KeyMouseBinding(GetModuleType(), "flipDown", "Flip object down.", true);
		objectFlipDown.AddBinding(settings.keyEditorFlipObjectDown); RegisterKeyMouseBinding(objectFlipDown);
		KeyMouseBinding objectFlipDownHOLD = new KeyMouseBinding(GetModuleType(), "flipDownHOLD", "Flip object down.", true);
		objectFlipDownHOLD.AddBinding(settings.keyEditorFlipObjectDown, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectFlipDownHOLD);
		KeyMouseBinding objectFlipLeft = new KeyMouseBinding(GetModuleType(), "flipLeft", "Flip object left.", true);
		objectFlipLeft.AddBinding(settings.keyEditorFlipObjectLeft); RegisterKeyMouseBinding(objectFlipLeft);
		KeyMouseBinding objectFlipLeftHOLD = new KeyMouseBinding(GetModuleType(), "flipLeftHOLD", "Flip object left.", true);
		objectFlipLeftHOLD.AddBinding(settings.keyEditorFlipObjectLeft, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectFlipLeftHOLD);
		KeyMouseBinding objectFlipRight = new KeyMouseBinding(GetModuleType(), "flipRight", "Flip object right.", true);
		objectFlipRight.AddBinding(settings.keyEditorFlipObjectRight); RegisterKeyMouseBinding(objectFlipRight);
		KeyMouseBinding objectFlipRightHOLD = new KeyMouseBinding(GetModuleType(), "flipRightHOLD", "Flip object right.", true);
		objectFlipRightHOLD.AddBinding(settings.keyEditorFlipObjectRight, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectFlipRightHOLD);

		//		KeyMouseBinding objectScroll  = new KeyMouseBinding( GetModuleType(), "ScrollObject" , "[Shift][Ctrl][Alt]+(Wheel)" , "Raise or lower objects with mouse wheel as well as rotate.", true );
		//		toggleEditor.AddBinding( KeyCode.KC_LSHIFT, KeyMouseBinding.KB_EVENT_HOLD    );
		//		toggleEditor.AddBinding( KeyCode.KC_END   , KeyMouseBinding.KB_EVENT_RELEASE ); // Press END. Using Release prevents key HOLD spam from onKeyPress (could use ClearKey in onKeyPress however)

		/*RegisterKeyMouseBinding(objectMoveWestHOLD);
		RegisterKeyMouseBinding(objectMoveEastHOLD);
		RegisterKeyMouseBinding(objectMoveNorthHOLD);
		RegisterKeyMouseBinding(objectMoveSouthHOLD);
		RegisterKeyMouseBinding(objectMoveUpHOLD);
		RegisterKeyMouseBinding(objectMoveDownHOLD);
		RegisterKeyMouseBinding(objectRotateLeftHOLD);
		RegisterKeyMouseBinding(objectRotateRightHOLD);
		RegisterKeyMouseBinding(objectFlipUpHOLD);
		RegisterKeyMouseBinding(objectFlipDownHOLD);
		RegisterKeyMouseBinding(objectFlipLeftHOLD);
		RegisterKeyMouseBinding(objectFlipRightHOLD);*/
		
	}
}
