class actionHandler {
	ref array<ref array<ref actionData>> undoActions, redoActions;
    ref ObjectEditor objEditor = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor));
    void actionHandler() {
        if(undoActions == NULL) { undoActions = new array<ref array<ref actionData>>; }
        if(redoActions == NULL) { redoActions = new array<ref array<ref actionData>>; }
    }

    void clear() { undoActions.Clear(); redoActions.Clear(); }

	void updateObjectID(int oldObjectID, int newObjectID) {
		if(newObjectID == -1) { return; }
		if(undoActions) { foreach(auto acts : undoActions) { foreach(int i, auto act : acts) { if(act.getID() == oldObjectID) { act.setID(newObjectID); acts.Set(i, act); } } } }
		if(redoActions) {
		    foreach(auto actions : redoActions) { foreach(int ind, auto action : actions) { if(action.getID() == oldObjectID) { action.setID(newObjectID); actions.Set(i, action); } } } }
	}

	void addAction(string action, ref array<Object> objects = NULL, bool redo = false) { // Move, Rotate, Delete, Spawn
	    if(objEditor.m_IsDragging) { return; }
	    ref array<ref actionData> currentAction = new array<ref actionData>;
		if(!objects) { objects = new array<Object>;
		if(objEditor.groupEditor) { foreach (Object oObj : objEditor.currentObjects) { objects.Insert(oObj); } } else { objects.Insert(objEditor.m_SelectedObject); } }
		foreach (Object obj : objects) {
			auto oData = objHandler.getObjectData(obj);
			actionData aData = new actionData(action, obj.GetID(), obj.GetType(), oData.getPosition(), oData.getOrientation(), oData.getScale(), oData.getGroup(), oData.getDate(), oData.getActive(), oData);//new objectData(oData.getName(), oData.getGroup(), oData.getDate(), oData.getActive(), oData.getPosition(), oData.getOrientation(), oData.getScale()));
			currentAction.Insert(aData);
		}
		if(currentAction.Count() > 0) { if(!redo) { undoActions.Insert(currentAction); } else { redoActions.Insert(currentAction); } }
	}
	void undoAction(bool redo = false)
	{
		if (!objEditor.m_ObjectEditorActive) { return; }
		string undoRedoText = "Undo"; if(redo) { undoRedoText = "Redo"; }
		int index = 0; ref array<ref actionData> actions;
		if(!redo) { index = undoActions.Count() - 1; actions = undoActions.Get(index); if(!actions) { COM_Message("There are no actions to undo."); return; } }
		else { index = redoActions.Count() - 1; actions = redoActions.Get(index); if(!actions) { COM_Message("There are no actions to redo."); return; } }
		// Add reverse action to opposite list first.
		ref array<ref actionData> reverseActions = new array<ref actionData>; string thisAction, newAction;
		string oldGroup;
		foreach(auto act : actions) {
			thisAction = act.getAction(); ref actionData aData;
			if(thisAction == "Spawn") { newAction = "Delete"; } else if(thisAction == "Delete") { newAction = "Spawn"; } else { newAction = thisAction; }
	        auto obj = objEditor.getObject(act.getID());
	        if(obj) {
	            act.data = objHandler.getObjectData(obj);
				oldGroup = act.data.getGroup();
			    aData = new actionData(newAction, obj.GetID(), act.data.getName(), act.data.getPosition(), act.data.getOrientation(), act.data.getScale(), act.data.getGroup(), act.data.getDate(), act.data.getActive(), act.data); }
			else {
				oldGroup = act.getGroup();
				aData = new actionData(newAction, act.getID(), act.getName(), act.getPosition(), act.getOrientation(), act.getScale(), act.getGroup(), act.getDate(), act.getActive(), NULL); }
			reverseActions.Insert(aData);
		}
		if(redo) { undoActions.Insert(reverseActions); }
		else { redoActions.Insert(reverseActions); }
		// Undo the action.
		foreach(int i, auto action : actions) {
			if(thisAction == "Move") { Object moveObj = objEditor.getObject(action.getID()); objEditor.moveObject(moveObj, action.getPosition()); }
			else if(thisAction == "Rotate") { Object rotateObj = objEditor.getObject(action.getID()); objEditor.rotateObject(rotateObj, action.getOrientation()); }
			else if(thisAction == "Scale") { Object scaleObj = objEditor.getObject(action.getID()); objEditor.scaleObject(scaleObj, action.getScale()); }
			else if(thisAction == "Change Group") {
				foreach(auto groupObj : actions) {
					auto groupData = objHandler.getObjectData(NULL, groupObj.getID());
					groupData.setGroup(groupObj.getGroup());
				}
				objHandler.updateObjectGroups();
				if(currentGroup == oldGroup) {
					if(actions.Count() > 1) { ObjectInfoMenu.selectObjectGroup(action.getGroup()); }
				} else if (currentGroup == action.getGroup()) { ObjectInfoMenu.UpdateObjectList(); }
				break;
			}
			else if(thisAction == "Change Active") {
				foreach(auto gObj : actions) { auto gData = objHandler.getObjectData(NULL, gObj.getID()); gData.setActive(gObj.getActive()); }
				objHandler.setObjectActiveText();
				if(currentGroup == "Inactive" && ObjectInfoMenu.isReady) { ObjectInfoMenu.UpdateObjectList(); }
				break;
			}
			else if(thisAction == "Spawn") {
				ref map<int, Object> delObjects = new map<int, Object>;
				foreach(auto oObj : actions) { Object delObj = objEditor.getObject(oObj.getID()); delObjects.Set(delObj.GetID(), delObj); }
				objEditor.DeleteBulkObjects(delObjects, true); break;
			} else if(thisAction == "Delete") {
				array<ref Param8<string, vector, vector, float, string, string, int, int>> objects = new array<ref Param8<string, vector, vector, float, string, string, int, int>>;
				string groupName = "All";
				foreach(auto data : actions) {
					objects.Insert(new Param8<string, vector, vector, float, string, string, int, int>(data.getName(), data.getPosition(), data.getOrientation(), data.getScale(), data.getGroup(), data.getDate(), data.getActive(), data.getID()));
					groupName = data.getGroup();
				}
				objEditor.SpawnBulkObjects(objects, true, redo); ObjectInfoMenu.selectObjectGroup(groupName); break;
			}
		}
		string grouptxt = "", objname = ""; auto actData = actions.Get(0);
		if(actData) { if(actions.Count() > 1) { grouptxt = " Group "; objname = actData.getGroup(); } else { objname = actData.getName(); } }
		COM_Message(undoRedoText + "ing Action: " + thisAction + grouptxt + " '" + objname + "'");
		if(!redo) { undoActions.Remove(index); } else { redoActions.Remove(index); }
	}
}