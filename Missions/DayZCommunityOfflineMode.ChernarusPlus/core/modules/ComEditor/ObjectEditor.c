class ObjectEditor extends Module
{
	static bool m_ObjectEditorActive = false;
	protected bool m_ObjectDragCancel = false;
	static bool m_IsDragging = false;
	protected bool m_DragTimeout = false;
	static Object m_SelectedObject;
	static vector m_LastDragPos;
	Object building;
	bool Undo[100], Redo[100];
	vector UndoPos[100], UndoOr[100], RedoPos[100], RedoOr[100];
	ref array<vector> groupPositions, groupPositionsRedo;
	bool lockYAxis = false;
	bool groupEditor = false;

	ref map<int, Object> m_Objects, currentObjects;
	ref map<int, vector> groupOffsets;

	bool hasObject(Object obj) { if(m_Objects.Contains(obj.GetID()) != -1) { return true; } else { return false; } }
	//Object getObject(Object obj) { foreach(Object oObj : m_Objects) { if(oObj == obj) { return oObj; } } return NULL; }
	//Object getObject(int objID) { foreach(Object oObj : m_Objects) { if(oObj.GetID() == objID) { return oObj; } } return NULL; }
	Object getObject(int objID) { return m_Objects.Get(objID); }

	override void Init()
	{
		super.Init();
		objHandler = new objectHandler();
		actHandler = new actionHandler();
		m_Objects = new map<int, Object>;
		currentObjects = new map<int, Object>;
		groupOffsets = new map<int, vector>;
		LoadScene();
		ExportObjectLoad();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(keepWeatherClear, 5000, true);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(setAlwaysDaytime, 5000, true);
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
	void toggleCamera() { if (m_ObjectEditorActive || isSpawnMenuOpen) { CameraTool.Cast(COM_GetModuleManager().GetModule(CameraTool)).ToggleCamera(); } }

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
		MakeDirectory(BASE_COM_DIR); MakeDirectory(BASE_BACKUP_DIR);
		ref objectSave scene = new objectSave();
		ref array<ref objectSave> scenes = new ref array<ref objectSave>;
		int objs = 0;
		foreach(int key, auto data : objHandler.objData) {
			scene.Objects.Insert(data); objs += 1;
			if (objs == 150) { objs = 0; scenes.Insert(scene); scene = new objectSave(); }
		}
		scenes.Insert(scene);
		int fileNum = 0; BackupSceneFile();
		foreach(auto sc : scenes)  {
			JsonFileLoader<objectSave>.JsonSaveFile(BASE_COM_DIR + "\\" + objectsFile + fileNum.ToString() + ".json", sc);
			fileNum += 1;
		}
		for (int i = fileNum; i < 100; i++) {
			if(FileExist(BASE_COM_DIR + "\\" + objectsFile + i.ToString() + ".json")) { DeleteFile(BASE_COM_DIR + "\\" + objectsFile + i.ToString() + ".json"); }
		}
		COM_Message("Saved objects to " + BASE_COM_DIR);
	}
	void LoadScene()
	{
		foreach(auto object : m_Objects) { object.SetPosition(vector.Zero); GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(GetGame().ObjectDelete, 100, false, object); }
		m_Objects.Clear(); currentObjects.Clear(); objHandler.clear(); actHandler.clear();
		for (int i = 0; i < 100; i++) {
			if (!FileExist(BASE_COM_DIR + "\\" + objectsFile + i.ToString() + ".json")) { break; }
			ref objectSave scene = new objectSave();
			JsonFileLoader<objectSave>.JsonLoadFile(BASE_COM_DIR + "\\" + objectsFile + i.ToString() + ".json", scene);
			foreach(auto data : scene.Objects)  {
				Object obj = createObject(data.getName(), data.getPosition(), data.getOrientation(), data.getScale());
				m_Objects.Set(obj.GetID(), obj);
				objHandler.newObjectData(obj, data.getPosition(), data.getOrientation(), data.getScale(), data.getGroup(), data.getDate(), data.getActive());
			}
			scriptLog("All objects loaded from save file: " + BASE_COM_DIR + "\\" + objectsFile + i.ToString() + ".json");
		}
	}
	void sortObjects() {
		if(m_Objects.Count() == 0) { return; }
		auto groups = objHandler.getObjectGroups();
		ref map<int, Object> objects = new map<int, Object>;
		foreach(auto group : groups)  {
			auto groupObjects = objHandler.getGroupObjects(group);
			foreach(auto obj : groupObjects)  { objects.Set(obj.GetID(), obj); }
		}
		m_Objects = objects;
	}

	void saveSettings() {
		ref SettingsSave save = new SettingsSave();
		vector pos = COM_GetPB().GetPosition();
		if(pos[1] > 1) { settings.lastPosition = COM_GetPB().GetPosition(); settings.lastOrientation = COM_GetPB().GetOrientation(); }
		save.Settings.Insert(settings); JsonFileLoader<SettingsSave>.JsonSaveFile(filenameSettings, save);
		//scriptLog("Settings saved to '" + filenameSettings + "'");
	}
	void loadSettings()
	{
		MakeDirectory(BASE_COM_DIR); MakeDirectory(BASE_BACKUP_DIR);
		auto settingsFile = OpenFile(filenameSettings, FileMode.READ);
		if (!settingsFile) { settings = new SettingsData(); saveSettings(); scriptLog("Settings file not found. Generating new file '" + filenameSettings + "'"); return; }
		CloseFile(settingsFile);
		ref SettingsSave settingsTemp = new SettingsSave();
		JsonFileLoader<SettingsSave>.JsonLoadFile(filenameSettings, settingsTemp);
		if(settingsTemp.Settings.Count() > 0) { settings = new SettingsData(settingsTemp.Settings[0]); scriptLog("Settings loaded from '" + filenameSettings + "'"); }
		else { settings = new SettingsData(); saveSettings(); scriptLog("Settings file corrupt. Generating new file '" + filenameSettings + "'"); }
	}

	string getObjectJSON(Object obj) {
		auto oData = objHandler.getObjectData(obj);
		return "\n{\"name\": \"" + oData.getName() + "\", \"group\": \"" + oData.getGroup() + "\", \"date\": \"" + oData.getDate() + "\", \"active\": \"" + oData.getActive() + "\", \"pos\": [ " + vecToString(oData.getPosition(), ", ") + " ], \"ypr\": [ " + vecToString(oData.getOrientation(), ", ") + " ], \"scale\": " + oData.getScale().ToString() + " },";
	}
	void ExportScene() {
		map<int, Object> objects = new map<int, Object>;
		if(isEditing()) { objects = currentObjects; } else { objects = m_Objects; }
		if(COM_CTRL()) { ExportMapGroupPos(objects); } else if(COM_SHIFT()) { ExportMapGroupProtos(objects); }
		else {
			string toCopy; foreach(Object obj : objects) { toCopy += getObjectJSON(obj); }
			GetGame().CopyToClipboard(toCopy); COM_Message("Copied current objects list to clipboard in JSON form.");
		}
	}
	string getMapGroupProto(Object obj) {
		auto oData = objHandler.getObjectData(obj);
		vector obj_pos = oData.getPosition();
		vector obj_rtn = oData.getOrientation();
		string obj_x = COM_FormatFloat(obj_pos[0] * -1, 6);
		string obj_h = COM_FormatFloat(obj_pos[1], 6);
		string obj_z = COM_FormatFloat(obj_pos[2], 6);
		string obj_r = COM_FormatFloat(obj_rtn[2], 6);
		string obj_p = COM_FormatFloat(obj_rtn[1] * -1, 6);
		string obj_y = COM_FormatFloat(obj_rtn[0], 6);
		if (obj_r == "0" || obj_r == "-0") { obj_r = "0.000000"; }
		if (obj_p == "0" || obj_p == "-0") { obj_p = "0.000000"; }
		if (obj_y == "0" || obj_y == "-0") { obj_y = "0.000000"; }
		return "\n    <proxy type=\"" + obj.GetType() + "\" pos=\"" + obj_z + " " + obj_h + " " + obj_x + "\" rpy=\"" + obj_p + " " + obj_y + " " + obj_r + "\" />";
	}
	void ExportMapGroupProtos(map<int, Object> objects = NULL) {
		if(objects == NULL) { objects = currentObjects; }
		string toCopy; foreach(Object m_object : objects) { toCopy += getMapGroupProto(m_object); }
		GetGame().CopyToClipboard(toCopy); COM_Message("Copied current objects list to clipboard in MapGroupProto form.");
	}
	string getMapGroupPos(Object obj) {
		auto oData = objHandler.getObjectData(obj);
		vector obj_pos = oData.getPosition();
		vector obj_rtn = oData.getOrientation();
		string obj_r = COM_FormatFloat(obj_rtn[2], 6);
		string obj_p = COM_FormatFloat(obj_rtn[1], 6);
		string obj_y = COM_FormatFloat(obj_rtn[0], 6);
		if (obj_r == "0" || obj_r == "-0") { obj_r = "0.000000"; }
		if (obj_p == "0" || obj_p == "-0") { obj_p = "0.000000"; }
		if (obj_y == "0" || obj_y == "-0") { obj_y = "0.000000"; }
		return "\n    <group name=\"Item_" + obj.GetType() + "\" pos=\"" + vecToString(obj_pos, " ") + "\" rpy=\"" + obj_r + " " + obj_p + " " + obj_y + "\" />";
	}
	void ExportMapGroupPos(map<int, Object> objects = NULL)
	{
		if(objects == NULL) { objects = currentObjects; }
		string toCopy; foreach(Object m_object : objects) { toCopy += getMapGroupPos(m_object); }
		GetGame().CopyToClipboard(toCopy); COM_Message("Copied current objects list to clipboard in MapGroupPos form.");
	}

	void saveGroupInputs() { 
		lastEditorGroup = currentGroup;
		if(!ObjectInfoMenu.groupEditorNameInput) { return; }
		lastEditorGroupInput = ObjectInfoMenu.groupEditorNameInput.GetText();
		if(!ObjectMenu.groupSelectorNameInput || !ObjectMenu.m_SearchBox) { return; }
		lastObjectGroupInput = ObjectMenu.groupSelectorNameInput.GetText(); lastObjectSearch = ObjectMenu.m_SearchBox.GetText();
	}

	void SelectObject(Object object) {
		if (object.IsInherited(PlayerBase)) { return; }
		if(m_SelectedObject == object) { groupEditor = false; return; }
		if (hasObject(object)) {
			m_SelectedObject = object; ObjectInfoMenu.updateInfoPos();
			for (int i = 0; i < 100; i++) { Undo[i] = false; Redo[i] = false; }
			objHandler.setObjectActiveText(); objHandler.setObjectDateText();
			if(ObjectInfoMenu.isReady) {
				ObjectInfoMenu.infoPosX.ClearFlags(WidgetFlags.IGNOREPOINTER); ObjectInfoMenu.infoPosY.ClearFlags(WidgetFlags.IGNOREPOINTER); ObjectInfoMenu.infoPosZ.ClearFlags(WidgetFlags.IGNOREPOINTER);
				ObjectInfoMenu.infoPosYaw.ClearFlags(WidgetFlags.IGNOREPOINTER); ObjectInfoMenu.infoPosPitch.ClearFlags(WidgetFlags.IGNOREPOINTER); ObjectInfoMenu.infoPosRoll.ClearFlags(WidgetFlags.IGNOREPOINTER);
			}
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
	
	Object SpawnObject(string type, vector position, vector orientation, float scale, string group, string date = "", int active = -1) {
		Object obj = createObject(type, position, orientation, scale);
		m_Objects.Set(obj.GetID(), obj);
		objHandler.newObjectData(obj, position, orientation, scale, group, date, active);
		SelectObject(obj); 
		array<Object> oObjects = new array<Object>; oObjects.Insert(obj); actHandler.addAction("Spawn", oObjects);
		sortObjects();
		return obj;
	}
	Object SpawnObjectAI(string type, vector position, vector orientation, float scale, string group, string date = "", int active = -1) {
		Object obj = createObjectAI(type, position, orientation, scale);
		m_Objects.Set(obj.GetID(), obj);
		objHandler.newObjectData(obj, position, orientation, scale, group, date, active);
		SelectObject(obj);
		array<Object> oObjects = new array<Object>; oObjects.Insert(obj); actHandler.addAction("Spawn", oObjects);
		sortObjects();
		return obj;
	}

	void DeleteObject() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if (groupEditor) { 
			deleteAllClicks++;
			if(deleteAllClicks >= 3) {
				DeleteBulkObjects(currentObjects);
				ObjectInfoMenu.selectObjectGroup("All");
				deleteAllClicks = 0;
				COM_Message("All objects in current list have been deleted. Click 'RELOAD' now if this was a mistake.");
			} else { 
				int timesLeft = 3-deleteAllClicks;
				COM_Message("Group editor is active. Press 'Delete' " + timesLeft.ToString() + " more times to delete all objects in current list.");
			}
			ObjectInfoMenu.SetButtonFocus(ObjectInfoMenu.deleteAllButton);
		} else {
			array<Object> oObjects = new array<Object>; oObjects.Insert(m_SelectedObject); actHandler.addAction("Delete", oObjects);
			COM_Message("Deleted " + m_SelectedObject.GetType() + " at [" + COM_VectorToString(m_SelectedObject.GetPosition()) + "]");
			m_SelectedObject.SetPosition(vector.Zero); 
			m_Objects.Remove(m_SelectedObject.GetID()); currentObjects.Remove(m_SelectedObject.GetID()); objHandler.delObjectData(m_SelectedObject);
			//GetGame().ObjectDelete(m_SelectedObject); DeselectObject();
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(GetGame().ObjectDelete, 100, false, m_SelectedObject);
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DeselectObject, 150, false);
		}
	}
	void SpawnBulkObjects(array<ref Param8<string, vector, vector, float, string, string, int, int>> objects, bool reload = false, bool redo = false, bool select = false) {
		string group = ""; array<Object> spawnedObjects = new array<Object>;
		foreach(auto oData : objects) {
			Object obj = createObject(oData.param1, oData.param2, oData.param3, oData.param4);
			m_Objects.Set(obj.GetID(), obj);
			objHandler.newObjectData(obj, oData.param2, oData.param3, oData.param4, oData.param5, oData.param6, oData.param7);
			group = oData.param5;
			if(objects.Count() == 1) { SelectObject(obj); }
			spawnedObjects.Insert(obj); actHandler.updateObjectID(oData.param8, obj.GetID());
			SelectObject(obj);
		}
		if(!reload) { if(currentGroup != group && objects.Count() > 1) { ObjectInfoMenu.selectObjectGroup(group); } actHandler.addAction("Spawn", spawnedObjects); }
		sortObjects();
		objHandler.updateObjectGroups(); ObjectInfoMenu.UpdateObjectList();
	}
	void DeleteBulkObjects(map<int, Object> objects, bool reload = false) {
		if (!m_ObjectEditorActive) { return; }
		if(!reload) { actHandler.addAction("Delete", objects.GetValueArray()); }
		string oldGroup = "";
		//array<Object> newObjects = new array<Object>;
		//foreach (int i, Object oObj : objects) { if(i == 0) { oldGroup = objHandler.getObjectGroup(oObj); } newObjects.Insert(getObject(oObj)); }
		foreach (Object obj : objects) {
			if(!obj) { continue; }
			if(oldGroup == "") { oldGroup = objHandler.getObjectGroup(obj); }
			//if(!reload) { scriptLog("Deleted " + obj.GetType() + " at [" + COM_VectorToString(obj.GetPosition()) + "]"); }
			obj.SetPosition(vector.Zero);
			m_Objects.Remove(obj.GetID()); currentObjects.Remove(obj.GetID()); objHandler.delObjectData(obj);
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(GetGame().ObjectDelete, 100, false, obj);
		}
		DeselectObject();
		//objHandlerOld.updateObjectGroups(); ObjectInfoMenu.UpdateObjectList(); ObjectInfoMenu.selectObjectGroup("All");
		COM_Message("Deleted all objects in current list.");
	}

	void DupeObject() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if(groupEditor) { DupeGroup(); return; } else if(COM_CTRL()) { DupeObjectInPlace(m_SelectedObject); return; }
		COM_Message("Duplicating \"" + m_SelectedObject.GetType() + "\" at pointer.");

		auto oData = objHandler.getObjectData(m_SelectedObject);
		vector pos = COM_GetPointerPos(); if(lockYAxis) { vector oldPos = oData.getPosition(); pos[1] = oldPos[1]; }
		Object obj = SpawnObject(oData.getName(), pos, oData.getOrientation(), oData.getScale(), oData.getGroup());
		if(!lockYAxis) { GroundObject(obj); }
	}

	void DupeGroup() {
		getGroupPositionOffsets();
		auto oData = objHandler.getObjectData(m_SelectedObject);
		vector pos = COM_GetPointerPos(); if(lockYAxis) { vector oldPos = oData.getPosition(); pos[1] = oldPos[1]; }
		string groupName = oData.getGroup();
		if(groupName.Contains("_Copy")) { TStringArray strSplit = new TStringArray; groupName.Split("_Copy", strSplit); groupName = strSplit.Get(0); }
		groupName += "_Copy";
		auto groups = objHandler.getObjectGroups();
		for(int i = 0; i < 100; i++) { if(groups.Find(groupName + i.ToString()) == -1) { groupName = groupName + i.ToString(); break; } }
		Object oObj = createObject(oData.getName(), pos, oData.getOrientation(), oData.getScale());
		if(!lockYAxis) { GroundObject(oObj); pos = oObj.GetPosition(); } oObj.SetPosition(vector.Zero);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(GetGame().ObjectDelete, 100, false, oObj);

		array<ref Param8<string, vector, vector, float, string, string, int, int>> objects = new array<ref Param8<string, vector, vector, float, string, string, int, int>>;
		foreach(auto obj : currentObjects)  {
			vector newPos = pos; vector offset = groupOffsets.Get(obj.GetID());
			newPos[0] = newPos[0] + offset[0]; newPos[1] = newPos[1] + offset[1]; newPos[2] = newPos[2] + offset[2];
			objects.Insert(new Param8<string, vector, vector, float, string, string, int, int>(obj.GetType(), newPos, obj.GetOrientation(), obj.GetScale(), groupName, "", -1, -1));
		}
		SpawnBulkObjects(objects); ObjectInfoMenu.selectObjectGroup(groupName);
		COM_Message("Duplicating group \"" + currentGroup + "\" at pointer.");
	}
	void groundObj() { if(groupEditor) { getGroupPositionOffsets(); } GroundObject(m_SelectedObject, true); }

	void GroundObject(Object obj = m_SelectedObject, bool manual = false) {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if(manual) { actHandler.addAction("Move"); }
		COM_PlaceObjectOnGround(obj); vector pos = obj.GetPosition();
		moveObject(obj, pos);
		//auto oData = objHandler.getObjectData(obj);
		//if(oData) { oData.setPosition(pos); }
		//if(oData.getScale().ToString() != "1") { scaleObject(obj, oData.getScale()); } }
		//else if(COM_RoundFloat(obj.GetScale()) != 1) { scaleObject(obj, obj.GetScale()); }
		if(manual && groupEditor) { moveObjectGroup(pos); }
	}
	void moveObjectGroup(vector pos) {
		if (!m_ObjectEditorActive || !m_SelectedObject || !groupEditor) { return; }
		foreach(auto obj : currentObjects)  {
			vector newPos = pos; vector offset = groupOffsets.Get(obj.GetID());
			newPos[0] = newPos[0] + offset[0]; newPos[1] = newPos[1] + offset[1]; newPos[2] = newPos[2] + offset[2];
			moveObject(obj, newPos);
		}
	}

	void DupeObjectInPlace(Object object)
	{
		COM_Message("Duplicating \"" + object.GetType() + "\" in place.");
		auto oData = objHandler.getObjectData(m_SelectedObject);
		Object obj = SpawnObject(oData.getName(), oData.getPosition(), oData.getOrientation(), oData.getScale(), oData.getGroup());
	}

	void UndoObject() { if(COM_CTRL()) { actHandler.undoAction(); } }
	void RedoObject() { if(COM_CTRL()) { actHandler.undoAction(true); } }
	
	void getGroupPositionOffsets() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		vector pos = m_SelectedObject.GetPosition();
		groupOffsets.Clear();
		foreach (auto obj : currentObjects) {
			vector objPos = obj.GetPosition();
			objPos[0] = objPos[0] - pos[0]; objPos[1] = objPos[1] - pos[1]; objPos[2] = objPos[2] - pos[2];
			groupOffsets.Set(obj.GetID(), objPos);
		}
	}

	void ClickObject() { // CLICK/SELECT OBJECT
		if (!m_ObjectEditorActive || m_IsDragging) { return; }
		Widget widgetCursor = GetGame().GetUIManager().GetWidgetUnderCursor();
		if (widgetCursor && widgetCursor.GetName() != "EditorMenu") { return; }
		DeselectObject();
		vector dir = GetGame().GetPointerDirection(), from = GetGame().GetCurrentCameraPosition(), to = from + ( dir * 1000 );
		set<Object> objects = COM_GetObjectsAt(from, to, COM_GetPB(), 0.3); if (!objects) { return; }
		for (int newObject = 0; newObject < objects.Count(); ++newObject) {
			Object obj = objects.Get(newObject);
			if(obj) { if(COM_SHIFT()) { auto oData = objHandler.getObjectData(obj); ObjectInfoMenu.selectObjectGroup(oData.getGroup()); } else { SelectObject(obj); } break; }
		}
	}
	void StartDragObject() {
		if (!m_ObjectEditorActive || !m_SelectedObject || m_IsDragging) { return; } actHandler.addAction("Move"); m_IsDragging = true;
		if(groupEditor) { getGroupPositionOffsets(); }
	}
	void CancelDragObject() {
		if (!m_IsDragging) { return; }
		if (m_ObjectDragCancel) { COM_Message("Cancelled request to cancel drag."); } else { COM_Message("Cancelling current drag when dropped."); }
		m_ObjectDragCancel = !m_ObjectDragCancel;
	}
	void FinishDragObject() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		m_IsDragging = false;
		if (m_ObjectDragCancel) { COM_Message("Cancelling last drag."); actHandler.undoAction(); m_ObjectDragCancel = false; }
	}
	void DragObject() { // DRAG/MOVE OBJECT
		if (!m_ObjectEditorActive || !m_SelectedObject || m_DragTimeout) { return; }
		auto oData = objHandler.getObjectData(m_SelectedObject);
		m_DragTimeout = true;
		vector dir = GetGame().GetPointerDirection(), from = GetGame().GetCurrentCameraPosition(), to = from + (dir * 10000), contact_pos, contact_dir; int contact_component;
		if (DayZPhysics.RaycastRV(from, to, contact_pos, contact_dir, contact_component, NULL, m_SelectedObject, COM_GetPB(), false, false)) {
			if(vector.Distance(contact_pos, m_LastDragPos) < 0.001) { m_DragTimeout = false; return; }
			m_LastDragPos = contact_pos;
			if(!groupEditor) {
				if(lockYAxis) { contact_pos[1] = oData.getPosition()[1]; moveObject(m_SelectedObject, contact_pos); }
				else { moveObject(m_SelectedObject, contact_pos); GroundObject(m_SelectedObject); }
			} else {
				if(lockYAxis) { contact_pos[1] = oData.getPosition()[1]; }
				else { m_SelectedObject.SetPosition(contact_pos); m_SelectedObject.PlaceOnSurface(); contact_pos = m_SelectedObject.GetPosition(); }
				moveObjectGroup(contact_pos);
			}
			ObjectInfoMenu.updateInfoPos();
		}
		m_DragTimeout = false;
	}

	void moveObject(Object obj, vector pos) {
		auto oData = objHandler.getObjectData(obj); if(!oData) { return; }
		oData.setPosition(pos);
		obj.SetPosition(pos);
		if(oData.getScale().ToString() != "1") { scaleObject(obj, oData.getScale()); }
		else { COM_ForceTargetCollisionUpdate(obj); }
	}
	void rotateObject(Object obj, vector ypr) {
		auto oData = objHandler.getObjectData(obj); if(!oData) { return; }
		oData.setOrientation(ypr); obj.SetOrientation(ypr);
		if(oData.getScale().ToString() != "1") { scaleObject(obj, oData.getScale()); }
		else { COM_ForceTargetCollisionUpdate(obj); }
	}
	void scaleObject(Object object, float scale) { // NEEDS WORK
		if(scale <= 0 || COM_RoundFloat(scale) == COM_RoundFloat(object.GetScale())) { return; }
		bool select = false;
		if(groupEditor) { select = true; } else if(m_SelectedObject == object) { select = true; }
		//COM_Message("Setting scale to: " + scale.ToString());
		auto data = objHandler.getObjectData(object); if(!data) { return; }
		int objID = object.GetID();
		data.setScale(scale);
		Object obj = createObject(data.getName(), data.getPosition(), data.getOrientation(), scale);
		objHandler.swapData(object.GetID(), obj);
		actHandler.updateObjectID(objID, obj.GetID());
		if(select) { if(groupEditor) { ObjectInfoMenu.selectObjectGroup(currentGroup); } else { SelectObject(obj); } }
		//ObjectInfoMenu.updateInfoPos();
		object.SetPosition(vector.Zero);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(GetGame().ObjectDelete, 100, false, object);
	}

	void scaleEnlarge() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if (groupEditor) { COM_Message("You cannot scale groups yet,"); return; }
		 actHandler.addAction("Scale"); scaleEnlargeHOLD();
	}
	void scaleEnlargeHOLD() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if (groupEditor) { COM_Message("You cannot scale groups yet,"); return; }
		auto oData = objHandler.getObjectData(m_SelectedObject); if(!oData) { return; }
		auto amount = settings.scaleSpeedSlow;
		if(COM_CTRL() && COM_SHIFT()) { amount = settings.scaleSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.scaleSpeedMedium; }
		scaleObject(m_SelectedObject, (oData.getScale() + amount));
		ObjectInfoMenu.updateInfoPos();
	}
	void scaleShrink() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if (groupEditor) { COM_Message("You cannot scale groups yet,"); return; }
		 actHandler.addAction("Scale"); scaleShrinkHOLD();
	}
	void scaleShrinkHOLD() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if (groupEditor) { COM_Message("You cannot scale groups yet,"); return; }
		auto oData = objHandler.getObjectData(m_SelectedObject); if(!oData) { return; }
		auto amount = settings.scaleSpeedSlow;
		if(COM_CTRL() && COM_SHIFT()) { amount = settings.scaleSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.scaleSpeedMedium; }
		scaleObject(m_SelectedObject, (oData.getScale() - amount));
		ObjectInfoMenu.updateInfoPos();
	}
	void moveNorth() { if (!m_ObjectEditorActive || !m_SelectedObject) { return; } actHandler.addAction("Move"); moveNorthHOLD(); }
	void moveNorthHOLD() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		auto oData = objHandler.getObjectData(m_SelectedObject);
		auto amount = settings.moveSpeedSlow; vector currentPos;
		if(COM_CTRL() && COM_SHIFT()) { amount = settings.moveSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.moveSpeedMedium; }
		if(groupEditor) {
			foreach(Object obj : currentObjects) {
				auto objData = objHandler.getObjectData(obj);
				currentPos = objData.getPosition(); currentPos[2] = currentPos[2] + amount; moveObject(obj, currentPos);
			}
		}
		else { currentPos = oData.getPosition(); currentPos[2] = currentPos[2] + amount; moveObject(m_SelectedObject, currentPos); }
		ObjectInfoMenu.updateInfoPos();
	}
	void moveSouth() { if (!m_ObjectEditorActive || !m_SelectedObject) { return; } actHandler.addAction("Move"); moveSouthHOLD(); }
	void moveSouthHOLD() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		auto oData = objHandler.getObjectData(m_SelectedObject);
		auto amount = settings.moveSpeedSlow; vector currentPos; 
		if(COM_CTRL() && COM_SHIFT()) { amount = settings.moveSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.moveSpeedMedium; }
		if(groupEditor) {
			foreach(Object obj : currentObjects) {
				auto objData = objHandler.getObjectData(obj);
				currentPos = objData.getPosition(); currentPos[2] = currentPos[2] - amount; moveObject(obj, currentPos);
			}
		}
		else { currentPos = oData.getPosition(); currentPos[2] = currentPos[2] - amount; moveObject(m_SelectedObject, currentPos); }
		ObjectInfoMenu.updateInfoPos();
	}
	void moveEast() { if (!m_ObjectEditorActive || !m_SelectedObject) { return; } actHandler.addAction("Move"); moveEastHOLD(); }
	void moveEastHOLD() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		auto oData = objHandler.getObjectData(m_SelectedObject);
		auto amount = settings.moveSpeedSlow; vector currentPos; 
		if(COM_CTRL() && COM_SHIFT()) { amount = settings.moveSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.moveSpeedMedium; }
		if(groupEditor) {
			foreach(Object obj : currentObjects) {
				auto objData = objHandler.getObjectData(obj);
				currentPos = objData.getPosition(); currentPos[0] = currentPos[0] + amount; moveObject(obj, currentPos);
			}
		}
		else { currentPos = oData.getPosition(); currentPos[0] = currentPos[0] + amount; moveObject(m_SelectedObject, currentPos); }
		ObjectInfoMenu.updateInfoPos();
	}
	void moveWest() { if (!m_ObjectEditorActive || !m_SelectedObject) { return; } actHandler.addAction("Move"); moveWestHOLD(); }
	void moveWestHOLD() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		auto oData = objHandler.getObjectData(m_SelectedObject);
		auto amount = settings.moveSpeedSlow; vector currentPos; 
		if(COM_CTRL() && COM_SHIFT()) { amount = settings.moveSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.moveSpeedMedium; }
		if(groupEditor) {
			foreach(Object obj : currentObjects) {
				auto objData = objHandler.getObjectData(obj);
				currentPos = objData.getPosition(); currentPos[0] = currentPos[0] - amount; moveObject(obj, currentPos);
			}
		}
		else { currentPos = oData.getPosition(); currentPos[0] = currentPos[0] - amount; moveObject(m_SelectedObject, currentPos); }
		ObjectInfoMenu.updateInfoPos();
	}
	void moveUp() { if (!m_ObjectEditorActive || !m_SelectedObject) { return; } actHandler.addAction("Move"); moveUpHOLD(); }
	void moveUpHOLD() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		auto oData = objHandler.getObjectData(m_SelectedObject);
		auto amount = settings.moveSpeedSlow; vector currentPos; 
		if(COM_CTRL() && COM_SHIFT()) { amount = settings.moveSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.moveSpeedMedium; }
		if(groupEditor) {
			foreach(Object obj : currentObjects) {
				auto objData = objHandler.getObjectData(obj);
				currentPos = objData.getPosition(); currentPos[1] = currentPos[1] + amount; moveObject(obj, currentPos);
			}
		}
		else { currentPos = oData.getPosition(); currentPos[1] = currentPos[1] + amount; moveObject(m_SelectedObject, currentPos); }
		ObjectInfoMenu.updateInfoPos();
	}
	void moveDown() { if (!m_ObjectEditorActive || !m_SelectedObject) { return; } actHandler.addAction("Move"); moveDownHOLD(); }
	void moveDownHOLD() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		auto oData = objHandler.getObjectData(m_SelectedObject);
		auto amount = settings.moveSpeedSlow; vector currentPos; 
		if(COM_CTRL() && COM_SHIFT()) { amount = settings.moveSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.moveSpeedMedium; }
		if(groupEditor) {
			foreach(Object obj : currentObjects) {
				auto objData = objHandler.getObjectData(obj);
				currentPos = objData.getPosition(); currentPos[1] = currentPos[1] - amount; moveObject(obj, currentPos);
			}
		}
		else { currentPos = oData.getPosition(); currentPos[1] = currentPos[1] - amount; moveObject(m_SelectedObject, currentPos); }
		ObjectInfoMenu.updateInfoPos();
	}
	void rotateLeft() { 
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; } if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		actHandler.addAction("Rotate"); rotateLeftHOLD();
	}
	void rotateLeftHOLD() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		auto oData = objHandler.getObjectData(m_SelectedObject);
		auto amount = settings.rotateSpeedSlow; if(COM_CTRL() && COM_SHIFT()) { amount = settings.rotateSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.rotateSpeedMedium; }
		/*if(groupEditor) {
			vector pos = oData.getPosition(); ref array<vector> offsets = new array<vector>;
			foreach (Object obj : ObjectInfoMenu.currentObjects) {
				vector newPos = RotateAroundPoint(oData.getPosition(), obj.GetPosition(), vector.Zero, amount, amount);

				vector objPos = obj.GetPosition(); objPos[0] = objPos[0] - pos[0]; objPos[1] = objPos[1] - pos[1]; objPos[2] = objPos[2] - pos[2]; offsets.Insert(objPos);
			}
		} else {*/
			vector currentOr = oData.getOrientation();
			currentOr[0] = currentOr[0] - amount;
			rotateObject(m_SelectedObject, currentOr); ObjectInfoMenu.updateInfoPos();
		//}
	}
	void rotateRight() { 
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		actHandler.addAction("Rotate"); rotateRightHOLD();
	}
	void rotateRightHOLD() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		auto oData = objHandler.getObjectData(m_SelectedObject);
		vector currentOr = oData.getOrientation();
		auto amount = settings.rotateSpeedSlow; if(COM_CTRL() && COM_SHIFT()) { amount = settings.rotateSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.rotateSpeedMedium; }
		currentOr[0] = currentOr[0] + amount;
		rotateObject(m_SelectedObject, currentOr); ObjectInfoMenu.updateInfoPos();
	}
	void flipUp() { 
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		actHandler.addAction("Rotate"); flipUpHOLD();
	}
	void flipUpHOLD(){
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		auto oData = objHandler.getObjectData(m_SelectedObject);
		vector currentOr = oData.getOrientation();
		auto amount = settings.flipSpeedSlow; if(COM_CTRL() && COM_SHIFT()) { amount = settings.flipSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.flipSpeedMedium; }
		currentOr[1] = currentOr[1] - amount;
		rotateObject(m_SelectedObject, currentOr); ObjectInfoMenu.updateInfoPos();
	}
	void flipDown() { 
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; } if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		actHandler.addAction("Rotate"); flipDownHOLD();
	}
	void flipDownHOLD(){
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		auto oData = objHandler.getObjectData(m_SelectedObject);
		vector currentOr = oData.getOrientation();
		auto amount = settings.flipSpeedSlow; if(COM_CTRL() && COM_SHIFT()) { amount = settings.flipSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.flipSpeedMedium; }
		currentOr[1] = currentOr[1] + amount;
		rotateObject(m_SelectedObject, currentOr); ObjectInfoMenu.updateInfoPos();
	}
	void flipLeft() { 
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; } if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		actHandler.addAction("Rotate"); flipLeftHOLD();
	}
	void flipLeftHOLD() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		auto oData = objHandler.getObjectData(m_SelectedObject);
		vector currentOr = oData.getOrientation();
		auto amount = settings.flipSpeedSlow; if(COM_CTRL() && COM_SHIFT()) { amount = settings.flipSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.flipSpeedMedium; }
		currentOr[2] = currentOr[2] - amount;
		rotateObject(m_SelectedObject, currentOr); ObjectInfoMenu.updateInfoPos();
	}
	void flipRight() { 
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; } if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		actHandler.addAction("Rotate"); flipRightHOLD();
	}
	void flipRightHOLD() {
		if (!m_ObjectEditorActive || !m_SelectedObject) { return; }
		if (groupEditor) { COM_Message("You cannot rotate groups yet,"); return; }
		auto oData = objHandler.getObjectData(m_SelectedObject);
		vector currentOr = oData.getOrientation();
		auto amount = settings.flipSpeedSlow; if(COM_CTRL() && COM_SHIFT()) { amount = settings.flipSpeedFast; } else if(COM_CTRL() || COM_SHIFT()) { amount = settings.flipSpeedMedium; }
		currentOr[2] = currentOr[2] + amount;
		rotateObject(m_SelectedObject, currentOr); ObjectInfoMenu.updateInfoPos();
	}

	override void RegisterKeyMouseBindings()
	{
		//loadSettings();
		scriptLog("Loading keyboard and mouse bindings for ObjectEditor.c");
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
		KeyMouseBinding objectGround = new KeyMouseBinding(GetModuleType(), "groundObj", "Snaps objects to ground.", true);
		objectGround.AddBinding(settings.keyEditorGroundObject); RegisterKeyMouseBinding(objectGround);
		KeyMouseBinding tabFix = new KeyMouseBinding(GetModuleType(), "DeselectObject", "Fixes issue with tabbing out of the game", true);
		tabFix.AddBinding(settings.keyEditorDeselectObject); RegisterKeyMouseBinding(tabFix);

		KeyMouseBinding objectScaleEnlarge = new KeyMouseBinding(GetModuleType(), "scaleEnlarge", "Enlarge object.", true);
		objectScaleEnlarge.AddBinding(settings.keyEditorScaleObjectEnlarge); RegisterKeyMouseBinding(objectScaleEnlarge);
		KeyMouseBinding objectScaleEnlargeHOLD = new KeyMouseBinding(GetModuleType(), "scaleEnlargeHOLD", "Enlarge object.", true);
		objectScaleEnlargeHOLD.AddBinding(settings.keyEditorScaleObjectEnlarge, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectScaleEnlargeHOLD);
		KeyMouseBinding objectScaleShrink = new KeyMouseBinding(GetModuleType(), "scaleShrink", "Shrink object.", true);
		objectScaleShrink.AddBinding(settings.keyEditorScaleObjectShrink); RegisterKeyMouseBinding(objectScaleShrink);
		KeyMouseBinding objectScaleShrinkHOLD = new KeyMouseBinding(GetModuleType(), "scaleShrinkHOLD", "Shrink object.", true);
		objectScaleShrinkHOLD.AddBinding(settings.keyEditorScaleObjectShrink, KeyMouseActionType.HOLD); RegisterKeyMouseBinding(objectScaleShrinkHOLD);

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
