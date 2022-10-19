class objectHandler {
	ref map<int, ref objectData> objData;
    ref ObjectEditor objEditor = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor));
    void objectHandler() { if(objData == NULL) { objData = new map<int, ref objectData>; } }
    void clear() { objData.Clear(); }
    // Data
    ref objectData getObjectData(Object obj = NULL, int objID = -1) {
        if(obj != NULL) { return objData.Get(obj.GetID()); }
        else if(objID != -1) { return objData.Get(objID); }
        else { return NULL; }
    }
    void newObjectData(Object obj, vector pos, vector ypr, float scale, string group = "", string date = "", int active = -1, bool firstLoad = false) {
        int groupSize = getObjectGroupsSize();
        if(group == "" || group == "All" || (group.Contains("Within ") && group.Contains(" Meters"))) { group = "None"; }
        if(active == -1) { active = 1; }
        if(date == "") { date = getDateTime(); }
        auto oData = new objectData(obj.GetType(), group, date, active, pos, ypr, scale);
        objData.Insert(obj.GetID(), oData);
        if(!firstLoad) {
            //scriptLog(string.Format("Added %1 to group %2.", oData.getName(), oData.getGroup()));
            checkObjectUpdate(obj, groupSize, group, "", true);
        }
    }
	void swapData(int objID, Object newObject) {
	    auto oldData = objData.Get(objID); if(!oldData) { return; }
	    int newID = newObject.GetID();
	    auto oData = new objectData(oldData.getName(), oldData.getGroup(), oldData.getDate(), oldData.getActive(), oldData.getPosition(), oldData.getOrientation(), oldData.getScale());
	    objData.Remove(objID); objData.Insert(newID, oData);
    	objEditor.m_Objects.Remove(objID); objEditor.m_Objects.Set(newID, newObject);
    	if(objEditor.currentObjects.Contains(objID)) { objEditor.currentObjects.Remove(objID); objEditor.currentObjects.Set(newID, newObject); }
	}

    void delObjectData(Object obj) {
        if(!obj) { return; }
        int groupSize = getObjectGroupsSize(); string oldGroup; bool foundObj = false;
        auto oData = getObjectData(obj);
        if(oData) {
            //scriptLog(string.Format("Deleted %1's object data.", oData.getName()));
            oldGroup = oData.getGroup(); foundObj = true;
            checkObjectUpdate(obj, groupSize, "", oldGroup, false);
        }
        objData.Remove(obj.GetID());
    }
    void delObjectDataKey(int key) {
        int groupSize = getObjectGroupsSize(); string oldGroup; bool foundObj = false;
        auto oData = objData.Get(key); auto obj = objEditor.getObject(key);
        if(oData && obj) {
            //scriptLog(string.Format("Deleted %1's object data.", oData.getName()));
            oldGroup = oData.getGroup(); foundObj = true;
            checkObjectUpdate(obj, groupSize, "", oldGroup, false);
        }
        objData.Remove(key);
    }
    void setObjectData(Object obj, string group = "", string date = "", int active = -1, vector pos = "0 0 0", vector ypr = "0 0 0", float scale = 0, bool firstLoad = false) {
        int groupSize = getObjectGroupsSize(); string oldGroup, newGroup; bool foundObj = false;
        if(!objData.Contains(obj.GetID())) { newObjectData(obj, pos, ypr, scale, group, date, active, firstLoad); return; }
        auto oData = getObjectData(obj); bool changedObj = false;
        oldGroup = oData.getGroup();
        if(date == "") { date = oData.getDate(); }
        if(group != "" && oldGroup != group) {
            oData.setGroup(group); changedObj = true;
            //scriptLog(string.Format("Changed %1's group from %2 to %3.", oData.getName(), oldGroup, group));
        }
        if(active != -1 && oData.getActive() != active) {
            //string activeText = "INACTIVE"; if(active == 1) { activeText = "Active";}
            oData.setActive(active); changedObj = true;
            //scriptLog(string.Format("%1 is now %2.", oData.getName(), activeText));
        }
        if(changedObj) { objData.Set(obj.GetID(), oData); }
        checkObjectUpdate(obj, groupSize, group, oldGroup, true, active);
    }


    void checkObjectUpdate(Object obj, int oldGroupSize, string newGroup, string oldGroup, bool added = true, int active = -1) {
        if(!ObjectInfoMenu.isReady) { return; }
        if(getObjectGroupsSize() != oldGroupSize) { updateObjectGroups(); }
        if(active != -1) {
            if(currentGroup == "Inactive") { 
                if(active == 1 || !added) { ObjectInfoMenu.UpdateObjectList(); if(ObjectInfoMenu.listBox.GetNumItems() == 0) { ObjectInfoMenu.selectObjectGroup("All"); } } 
                else { ObjectInfoMenu.addListBoxObject(obj); } return;
            }
        } else {
		    if(currentGroup == "All") { if(added) { ObjectInfoMenu.addListBoxObject(obj); } else { ObjectInfoMenu.UpdateObjectList(); } }
            else if (currentGroup.Contains("Within ") && currentGroup.Contains(" Meters")) { 
                string metersString = currentGroup; metersString.Replace("Within ",""); metersString.Replace(" Meters",""); float meters = metersString.ToFloat();
                float distance = vector.Distance(COM_GetPB().GetPosition(), obj.GetPosition());
                if(distance <= meters) { if(added) { ObjectInfoMenu.addListBoxObject(obj); } else { ObjectInfoMenu.UpdateObjectList(); } }
            }
            if(getGroupSize(currentGroup) == 0) { updateObjectGroups(); if(newGroup != "") { ObjectInfoMenu.selectObjectGroup(newGroup); } else { ObjectInfoMenu.selectObjectGroup("All"); }}
            else if(currentGroup == oldGroup) { ObjectInfoMenu.UpdateObjectList(); }
            else if(currentGroup == newGroup) { ObjectInfoMenu.addListBoxObject(obj); }
        }
    }

    // Groups
    string getObjectGroup(Object obj) { auto oData = getObjectData(obj); if(oData) { return oData.getGroup(); } return "Not Found"; }
    ref array<string> getObjectGroups() {
        ref array<string> spyZGroups = new array<string>;
        ref array<string> playerGroups = new array<string>;
        ref array<string> otherGroups = new array<string>;
        ref array<string> objectGroups = new array<string>;
        foreach(auto data : objData) {
            if(data.getGroup().Contains("SpyZ: ")) { if(spyZGroups.Find(data.getGroup()) == -1) { spyZGroups.Insert(data.getGroup()); } }
            else if(data.getGroup().Contains("P: ")) { if(playerGroups.Find(data.getGroup()) == -1) { playerGroups.Insert(data.getGroup()); } }
            else { if(otherGroups.Find(data.getGroup()) == -1) { otherGroups.Insert(data.getGroup()); } }
        }
        spyZGroups.Sort(); playerGroups.Sort(); otherGroups.Sort();
        foreach(auto spyZ : spyZGroups) { objectGroups.Insert(spyZ); }
        foreach(auto player : playerGroups) { objectGroups.Insert(player); }
        foreach(auto other : otherGroups) { objectGroups.Insert(other); }
        return objectGroups;
    }
    int getGroupSize(string group) { auto objs = getGroupObjects(group); return objs.Count(); }

    ref array<Object> getGroupObjects(string group) { 
        ref array<Object> groupObjects = new array<Object>;
        ref array<Object> childObjects = new array<Object>;
        bool foundGate = false; Object firstObject = NULL;
        foreach(int key, auto data : objData) {
            auto obj = objEditor.getObject(key);
            if(!obj) { delObjectDataKey(key); continue; }
            if(data.getGroup() == group) { if(data.getName() == "Land_Castle_Gate") { foundGate = true; firstObject = obj; groupObjects.Insert(obj); break; } }
        }
        if(firstObject == NULL) {
            float lowestHeight = 5000;
            foreach(int key1, auto oData : objData) {
                auto oObj = objEditor.getObject(key1);
                if(!oObj) { delObjectDataKey(key1); continue; }
                if(oData.getGroup() == group) {
                    if(oData.getName().Contains("Land_")) {
                        if(lowestHeight > oData.getPosition()[1]) {
                            lowestHeight = oData.getPosition()[1]; firstObject = oObj;
                        }
                    }
                }
            }
            if(firstObject != NULL) { groupObjects.Insert(firstObject); }
        }
        foreach(int key2, auto objData : objData) {
            auto oObject = objEditor.getObject(key2);
            if(!oObject) { delObjectDataKey(key2); continue; }
            if(firstObject == oObject) { continue; }
            if(objData.getGroup() == group) { childObjects.Insert(oObject); }
        }
        childObjects.Sort();
        foreach(auto child : childObjects) { groupObjects.Insert(child); }
        return groupObjects;
    }

    int getObjectGroupsSize() { auto groups = getObjectGroups(); return groups.Count(); }
    
    void updateObjectGroups() {
        if(!ObjectInfoMenu.isReady) { return; }
	    ObjectInfoMenu.groupListBox.ClearItems(); ObjectInfoMenu.groupEditorListBox.ClearItems(); ObjectMenu.groupSelectorListBox.ClearItems();
        ObjectInfoMenu.groupListBox.AddItem("All", NULL, 0); ObjectInfoMenu.groupListBox.AddItem("Inactive", NULL, 0); ObjectInfoMenu.groupListBox.AddItem("Within 100 Meters", NULL, 0);
        ObjectInfoMenu.groupListBox.AddItem("Within 250 Meters", NULL, 0); ObjectInfoMenu.groupListBox.AddItem("Within 500 Meters", NULL, 0);
        auto groups = getObjectGroups();
	    foreach(string group : groups) {
			ObjectInfoMenu.groupListBox.AddItem(group, NULL, 0); ObjectInfoMenu.groupEditorListBox.AddItem(group, NULL, 0); ObjectMenu.groupSelectorListBox.AddItem(group, NULL, 0);
	    }
    }

    // Active Status
    bool isObjectActive(Object obj) { auto oData = getObjectData(obj); if(oData) { return oData.isActive(); } return false; }
   
    string getObjectActiveText(Object obj) { if(isObjectActive(obj)) { return "Active"; } else { return "Inactive"; } }
	
    void setObjectActiveText() { if(ObjectInfoMenu.isReady) { ObjectInfoMenu.activeButton.SetText(getObjectActiveText(objEditor.m_SelectedObject)); } }
    
    // Object Creation Date
    string getObjectDate(Object obj) { auto oData = getObjectData(obj); if(oData) { return oData.getDate(); } else { return "Not Found"; } }
	void setObjectDateText() { if(ObjectInfoMenu.isReady) { ObjectInfoMenu.dateOutput.SetText(getObjectDate(objEditor.m_SelectedObject)); } }

}