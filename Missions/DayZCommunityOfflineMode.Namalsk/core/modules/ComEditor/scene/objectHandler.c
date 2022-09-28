class objectHandler {
	ref array<ref Param7<Object, string, string, string, int, vector, vector>> objectData;
    ref ObjectEditor objEditor = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor));
    void objectHandler() { 
        if(objectData == NULL) { objectData = new array<ref Param7<Object, string, string, string, int, vector, vector>>; } 
    }

    // Data
    void newObjectData(Object obj, string group = "N/A", int active = -1, string date = "N/A", bool firstLoad = false) {
        int groupSize = getObjectGroupsSize();
        if(group == "N/A" || group == "All" || (group.Contains("Within ") && group.Contains(" Meters"))) { group = "None"; }
        if(active == -1) { active = 1; }
        if(date == "N/A") { date = getDateTime(); }
        objectData.Insert(new ref Param7<Object, string, string, string, int, vector, vector>(obj, obj.GetType(), group, date, active, obj.GetPosition(), obj.GetOrientation()));
                                                                                       // param1,  param2,    param3, param4, param5,      param6,            param7
        //auto gData = objectData.Get(objectData.Count() - 1);
        if(!firstLoad) {
            Print(string.Format("Added %1 to group %2.", obj.GetType(), group));
            checkObjectUpdate(obj, groupSize, group, "N/A", true);
        }
    }
    void delObjectData(Object obj, int index = -1) {
        int groupSize = getObjectGroupsSize(); string oldGroup; bool foundObj = false;
        if(index == -1) {
            foreach(int i, auto data : objectData) { 
                if(data.param1 == obj) {
                    Print(string.Format("Deleted %1's object data.", data.param2));
                    oldGroup = data.param3; foundObj = true; objectData.Remove(i); break;
                }
            }
        } else {
            auto oData = objectData.Get(index);
            if(oData) {
                Print(string.Format("Deleted %1's object data at index %2.", oData.param2, index));
                oldGroup = oData.param3; objectData.Remove(index); foundObj = true;
            }
        }
        checkObjectUpdate(obj, groupSize, "N/A", oldGroup, false);
    }

    void checkObjectUpdate(Object obj, int groupSize, string newGroup, string oldGroup, bool added = true, int active = -1) {
        if(!ObjectInfoMenu.isReady) { return; }
        if(getObjectGroupsSize() != groupSize) { updateObjectGroups(); }
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
            if(getGroupSize(currentGroup) == 0) { updateObjectGroups(); ObjectInfoMenu.selectObjectGroup("All"); }
            else if(currentGroup == oldGroup) { ObjectInfoMenu.UpdateObjectList(); }
            else if(currentGroup == newGroup) { ObjectInfoMenu.addListBoxObject(obj); }
        }
    }

    void setObjectData(Object obj, string group = "N/A", int active = -1, string date = "N/A", bool firstLoad = false) {
        int groupSize = getObjectGroupsSize(); string oldGroup, newGroup; bool foundObj = false;
        foreach(int i, auto data : objectData) { 
            if(data.param1 == obj) {
                foundObj = true; oldGroup = data.param3;
                bool changedObj = false;
                if(date == "N/A") { date = data.param4; }
                if(group != "N/A" && data.param3 != group) {
                    Print(string.Format("Changed %1's group from %2 to %3.", data.param2, data.param3, group));
                    data.param3 = group; changedObj = true;
                }
                if(active != -1 && data.param5 != active) {
                    string activeText = "INACTIVE"; if(data.param5 == 1) { activeText = "Active";}
                    Print(string.Format("%1 is now %2.", data.param5, activeText));
                    data.param5 = active; changedObj = true;
                }
                if(changedObj) { objectData.Set(i, data); }
                break;
            }
        }
        if(foundObj) { checkObjectUpdate(obj, groupSize, group, oldGroup, true, active); } else { newObjectData(obj, group, active, date, firstLoad); }
    }

    ref Param7<Object, string, string, string, int, vector, vector> getObjectData(Object obj) { foreach(auto data : objectData) { if(data.param1 == obj) { return data; } } return NULL; }


    // Groups
    string getObjectGroup(Object obj) { auto oData = getObjectData(obj); if(oData) { return oData.param3; } return "Not Found"; }
    ref array<string> getObjectGroups() {
        ref array<string> objectGroups = new array<string>;
        foreach(auto data : objectData) { if(objectGroups.Find(data.param3) == -1) { objectGroups.Insert(data.param3); } }
        return objectGroups;
    }
    int getGroupSize(string group) { auto objs = getGroupObjects(group); return objs.Count(); }
    ref array<Object> getGroupObjects(string group) { 
        ref array<Object> groupObjects = new array<Object>;
        foreach(auto data : objectData) { if(data.param3 == group) { groupObjects.Insert(data.param1); } }
        return groupObjects;
    }
    int getObjectGroupsSize() { auto groups = getObjectGroups(); return groups.Count(); }
	void deleteObjectsInGroup(string group) { objEditor.DeleteBulkObjects(getGroupObjects(group)); }
    
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
    int isObjectActive(Object obj) { auto oData = getObjectData(obj); if(oData) { return oData.param5; } return -1; }
   
    string getObjectActiveText(Object obj) { int active = isObjectActive(obj); if(active == 1) { return "Active"; } else if (active == 0) { return "Inactive"; } else { return "Not Found"; } }
	
    void setObjectActiveText() { if(ObjectInfoMenu.isReady) { ObjectInfoMenu.activeButton.SetText(getObjectActiveText(objEditor.m_SelectedObject)); } }
	
    
    // Object Creation Date
    string getObjectDate(Object obj) { auto oData = getObjectData(obj); if(oData) { return oData.param4; } else { return "Not Found"; } }
	void setObjectDateText() { if(ObjectInfoMenu.isReady) { ObjectInfoMenu.dateOutput.SetText(getObjectDate(objEditor.m_SelectedObject)); } }

    string dumpObjectData() {
        string copyThis = objectData.Count().ToString();
        //for(int i = 0; i < objData.Count(); i++) {
        foreach(int i, auto data : objectData) {
            string addCopy = "\n'" + i.ToString() + "' '" + data.param2 + "' '" + data.param3 + "' '" + data.param4 + "' '" + data.param5.ToString() + "' '" + COM_VectorToString(data.param6) + "' '" + COM_VectorToString(data.param7) + "'";
            Print(addCopy); copyThis += addCopy;
        }
        return copyThis;
    }

}