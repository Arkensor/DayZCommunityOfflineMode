class ObjectInfoMenu extends PopupMenu
{
	private ref WidgetStore widgetStore;
	static bool isReady = false;
	static ObjectEditor objEditor;

	static EditBoxWidget infoPosX;
	static EditBoxWidget infoPosY;
	static EditBoxWidget infoPosZ;
	static EditBoxWidget infoPosYaw;
	static EditBoxWidget infoPosPitch;
	static EditBoxWidget infoPosRoll;
	static EditBoxWidget infoPosScale;
	static TextListboxWidget listBox;
	static TextListboxWidget groupListBox;
	static TextListboxWidget groupEditorListBox;
	static TextWidget groupEditorName;
	static EditBoxWidget groupEditorNameInput;
	static TextWidget objectsCount;
	static ButtonWidget activeButton;
	static TextWidget dateOutput;
	static TextWidget selectedObjectWidget;
	static TextWidget selectedObjectDisplayNameWidget;
	static ButtonWidget lockYAxisButton;
	static ButtonWidget deleteAllButton;
	static ButtonWidget reloadAllButton;
	static Widget lockYAxisButtonBkg;
	static Widget deleteAllButtonBkg;
	static Widget reloadAllButtonBkg;

	static bool editingPos = false;
	static bool newPosChange = false;
	static bool newYPRChange = false;
	static bool newScaleChange = false;

	void ObjectInfoMenu() { }
	void ~ObjectInfoMenu() { GetGame().GetCallQueue( CALL_CATEGORY_GUI ).Remove( Update ); }
	override void Init( )
	{
		isReady = true;
		objEditor = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor));
		widgetStore = new WidgetStore( layoutRoot );
		selectedObjectWidget = TextWidget.Cast(layoutRoot.FindAnyWidget( "object_editor_info_select_input" ));
        selectedObjectDisplayNameWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("object_editor_info_select_display_name"));
		listBox = widgetStore.GetListboxWidget("object_editor_info_list");
		objectsCount = TextWidget.Cast(layoutRoot.FindAnyWidget( "object_editor_object_name" ));
		groupListBox = widgetStore.GetListboxWidget("object_editor_group_list");
		infoPosX = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_pos_x" ));
		infoPosY = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_pos_y" ));
		infoPosZ = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_pos_z" ));
		infoPosYaw = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_yaw_input" ));
		infoPosPitch = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_pitch_input" ));
		infoPosRoll = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_roll_input" ));
		infoPosScale = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_scale_input" ));

		groupEditorListBox = widgetStore.GetListboxWidget("groupEditor_list");
		groupEditorNameInput = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "groupEditor_name_input" ));
		groupEditorName = TextWidget.Cast(layoutRoot.FindAnyWidget( "groupEditor_name_current" ));
		activeButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget( "groupEditor_active_button" ));
		dateOutput = TextWidget.Cast(layoutRoot.FindAnyWidget( "groupEditor_date_current" ));
		lockYAxisButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget( "object_info_pos_y_lock" ));
		lockYAxisButtonBkg = layoutRoot.FindAnyWidget("object_info_pos_y_lock_bkg");
		reloadAllButtonBkg = layoutRoot.FindAnyWidget("object_editor_info_load_bkg");
		reloadAllButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget( "object_editor_info_load" ));
		deleteAllButtonBkg = layoutRoot.FindAnyWidget("object_editor_info_clear_bkg");
		deleteAllButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget( "object_editor_info_clear" ));
        objHandler.updateObjectGroups(); UpdateObjectList();
		groupEditorNameInput.SetText(lastEditorGroupInput);
		selectObjectGroup(lastEditorGroup);
		GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( Update, 100, true );
	}
    override void OnShow() { isEditorMenuOpen = true; groupEditorNameInput.SetText(lastEditorGroupInput); SetButtonFocus(lockYAxisButton); }
    override void OnHide() { isEditorMenuOpen = false; lastEditorGroupInput = groupEditorNameInput.GetText(); deleteAllClicks = 0; }
    static void updateInfoPos() {
		if (!isEditorMenuOpen || !isReady) { return; }
		if(objEditor.m_SelectedObject) {
		    auto oData = objHandler.getObjectData(objEditor.m_SelectedObject);
		    if(!oData) { return; }
            if(!editingPos) {
				infoPosX.SetText(oData.getPosition()[0].ToString());
				infoPosY.SetText(oData.getPosition()[1].ToString());
				infoPosZ.SetText(oData.getPosition()[2].ToString());
				infoPosYaw.SetText(oData.getOrientation()[0].ToString());
				infoPosPitch.SetText(oData.getOrientation()[1].ToString());
				infoPosRoll.SetText(oData.getOrientation()[2].ToString());
				infoPosScale.SetText(oData.getScale().ToString());
				groupEditorName.SetText(oData.getGroup());
			}
            if(objEditor.groupEditor) {
                selectedObjectWidget.SetText("Editing All Objects in Group:"); selectedObjectDisplayNameWidget.SetText(currentGroup);
            } else { selectedObjectWidget.SetText(oData.getName()); selectedObjectDisplayNameWidget.SetText(objEditor.m_SelectedObject.GetDisplayName()); }
    	} else if(!editingPos) {
            infoPosX.SetText("0"); infoPosY.SetText("0"); infoPosZ.SetText("0"); infoPosYaw.SetText("0"); infoPosPitch.SetText("0"); infoPosRoll.SetText("0"); infoPosScale.SetText("0");
            activeButton.SetText("Inactive"); dateOutput.SetText("None"); groupEditorName.SetText("None");
            selectedObjectWidget.SetText("No object selected"); selectedObjectDisplayNameWidget.SetText("");
		}
    }
	static void SetButtonFocus( Widget w ) {
		if (!isReady) { return; }
		lockYAxisButtonBkg.SetColor(ARGB(0, 255, 255, 255)); lockYAxisButton.SetText("U");
		deleteAllButtonBkg.SetColor(ARGB(0, 255, 255, 255)); deleteAllButton.SetText("Delete All");
		reloadAllButtonBkg.SetColor(ARGB(0, 255, 255, 255)); reloadAllButton.SetText("Reload");
		if (lockYAxisButton == w && objEditor.lockYAxis) { lockYAxisButtonBkg.SetColor(ARGB(255, 255, 0, 0)); lockYAxisButton.SetText("L"); }
		if (deleteAllButton == w && deleteAllClicks > 0) { 
			deleteAllButton.SetText("Delete All (" + (3-deleteAllClicks).ToString() + ")");
			if(deleteAllClicks == 1) { deleteAllButtonBkg.SetColor(ARGB(150, 150, 0, 0)); } else { deleteAllButtonBkg.SetColor(ARGB(255, 255, 0, 0)); }
		}
		if (reloadAllButton == w && reloadAllClicks > 0) {
			reloadAllButton.SetText("Reload (" + (3-reloadAllClicks).ToString() + ")");
			if(reloadAllClicks == 1) { reloadAllButtonBkg.SetColor(ARGB(150, 150, 0, 0)); } else { reloadAllButtonBkg.SetColor(ARGB(255, 255, 0, 0)); }
		}
	}
	void Update() { updateInfoPos(); }
	static void addListBoxObject(Object obj) { if(listBox) { listBox.AddItem(obj.GetType(), obj, 0); objEditor.currentObjects.Set(obj.GetID(), obj); updateObjectCount(); } }
	static void updateObjectCount() { 
		if (!isReady) { return; }
		int i = listBox.GetNumItems(); string o = " Objects ("; if(i == 1) { o = " Object ("; } objectsCount.SetText(i.ToString() + o + currentGroup + ")");
	}
	static void UpdateObjectList() {
		if(!isReady || objEditor.m_Objects == NULL) { return; }
		listBox.ClearItems(); objEditor.currentObjects.Clear();
		if(currentGroup == "" || !currentGroup) { currentGroup = "All"; }

        if(currentGroup == "All" || currentGroup == "Inactive" || (currentGroup.Contains("Within ") && currentGroup.Contains(" Meters"))) {
            foreach(int i, auto obj : objEditor.m_Objects) {
                if(!obj || obj == NULL) { objEditor.m_Objects.RemoveElement(i); continue; }
                if(currentGroup == "All") { addListBoxObject(obj); }
                else if (currentGroup == "Inactive") { if(!objHandler.isObjectActive(obj)) { addListBoxObject(obj); } }
                else if (currentGroup.Contains("Within ") && currentGroup.Contains(" Meters")) { searchNearbyGroup(obj); }
            }
		}
		else {
            auto currentObjs = objHandler.getGroupObjects(currentGroup);
            foreach(Object oObj : currentObjs) { addListBoxObject(oObj); }
		}
	}
	static void searchNearbyGroup(Object obj) {
        string metersString = currentGroup; metersString.Replace("Within ",""); metersString.Replace(" Meters",""); float meters = metersString.ToFloat(); searchNearbyObject(obj, meters);
	}
	static void searchNearbyObject(Object obj, float meters) {
        float distance = vector.Distance(COM_GetPB().GetPosition(), obj.GetPosition());
        if(distance <= meters) { addListBoxObject(obj); }
	}

	Object getSelectedRowObject() {
		if (listBox.GetSelectedRow() == -1) { return NULL; }
		Object result; listBox.GetItemData(listBox.GetSelectedRow(), 0, result);
		return result;
	}
	string getSelectedRowGroup() {
		if (groupListBox.GetSelectedRow() == -1) { return "All"; }
		string result; groupListBox.GetItemText(groupListBox.GetSelectedRow(), 0, result); return result;
	}
	string getSelectedRowGroupEditor() {
		if (groupEditorListBox.GetSelectedRow() == -1) { return "None"; }
		string result; groupEditorListBox.GetItemText(groupEditorListBox.GetSelectedRow(), 0, result); return result;
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
		if (w.GetName() == "object_editor_info_list") {
			if(COM_SHIFT()) { auto oData = objHandler.getObjectData(getSelectedRowObject()); selectObjectGroup(oData.getGroup()); }
			else { objEditor.SelectObject(getSelectedRowObject()); }
		}
		else if (w == groupListBox) { selectObjectGroup(getSelectedRowGroup()); }
		else if (w == groupEditorListBox) { groupEditorNameInput.SetText(getSelectedRowGroupEditor()); }
		return false;
	}
	static void selectObjectGroup(string group) {
		if(!isReady) { return; }
		currentGroup = group; UpdateObjectList(); updateObjectCount();
        if(objEditor.currentObjects.Count() > 0 && group != "All") { objEditor.SelectObject(objEditor.currentObjects.GetElement(0)); } else { objEditor.DeselectObject(); }
        if(group == "All") { objEditor.groupEditor = false; } else { objEditor.groupEditor = true; }
        //Print("Current Group: " + currentGroup);
		//if(currentGroup != "All" && !(currentGroup.Contains("Within ") && currentGroup.Contains(" Meters"))) {
       // } else { groupEditor = false; }
    }

	override bool OnClick( Widget w, int x, int y, int button )
	{
	    objectData oData; if(objEditor.m_SelectedObject) { oData = objHandler.getObjectData(objEditor.m_SelectedObject); }
	    if (w.GetName() == "object_editor_info_save") { objEditor.SaveScene(); }
		else if (w == reloadAllButton) {
		    reloadAllClicks++;
		    if(reloadAllClicks >= 3) {
		        objEditor.LoadScene(); reloadAllClicks = 0;
				COM_Message("All objects have been reloaded from save file.");
			} else {
				COM_Message("Click 'Reload' " + (3-reloadAllClicks).ToString() + " more times to reload all objects. This will clear any progress you have made and cannot be undone.");
			} SetButtonFocus(reloadAllButton);
		}
		else if (w == deleteAllButton) {
		    deleteAllClicks++;
		    if(deleteAllClicks >= 3) {
		        objEditor.DeleteBulkObjects(objEditor.currentObjects); deleteAllClicks = 0; selectObjectGroup("All");
			    COM_Message("All objects in current list have been deleted. Click 'RELOAD' now if this was a mistake.");
			} else {
				COM_Message("Click 'Delete All' " + (3-deleteAllClicks).ToString() + " more times to delete all objects in current list.");
			} SetButtonFocus(deleteAllButton);
		}
		else if (w.GetName() == "object_editor_info_export") { objEditor.ExportScene(); }
		else if (w.GetName() == "object_editor_info_export_proto") { objEditor.ExportMapGroupProtos(); }
		else if (w.GetName() == "object_editor_info_export_pos") { objEditor.ExportMapGroupPos(); }
		else if (w.GetName() == "groupEditor_update_button") {
		    if(objEditor.m_SelectedObject) {
		        string newGroup = groupEditorNameInput.GetText(); string oldGroup = oData.getGroup();
                if(newGroup == "" || oldGroup == newGroup) { return false; }
				actHandler.addAction("Change Group");
		        if(objEditor.groupEditor) {
					foreach(Object groupObj : objEditor.currentObjects) { auto groupData = objHandler.getObjectData(groupObj); groupData.setGroup(newGroup); }
					COM_Message("All objects in group '" + currentGroup + "' have been moved to group '" + newGroup +"'.");
					objHandler.updateObjectGroups(); selectObjectGroup(newGroup);
				}
		        else {
		            oData.setGroup(newGroup); objHandler.updateObjectGroups();
					if(currentGroup == oldGroup) { UpdateObjectList(); }
					else if(currentGroup == newGroup) { addListBoxObject(objEditor.m_SelectedObject); }
				}
		    }
		} else if (w.GetName() == "groupEditor_active_button") {
		    if(objEditor.m_SelectedObject) { 
				int active = 1; if(oData.isActive()) { active = 0; }
				actHandler.addAction("Change Active");
		        if(objEditor.groupEditor) {
					foreach(Object gObj : objEditor.currentObjects) { auto gData = objHandler.getObjectData(gObj); gData.setActive(active); }
					if(active == 0 ) { COM_Message("All objects in group '" + currentGroup + "' are now INACTIVE."); }
					else { COM_Message("All objects in group '" + currentGroup + "' are now active."); }
		            if(currentGroup == "Inactive") { UpdateObjectList(); }
				} else { oData.setActive(active); }
				objHandler.setObjectActiveText();
			}
		} else if (w.GetName() == "objectExportButton_json") {
		    if(objEditor.m_SelectedObject) {
		        GetGame().CopyToClipboard(objEditor.getObjectJSON(objEditor.m_SelectedObject));
		        COM_Message("Copied selected object to clipboard in JSON form.");
		    }
	    } else if ( w.GetName() == "objectExportButton_proto") {
            if(objEditor.m_SelectedObject) {
         	    GetGame().CopyToClipboard(objEditor.getMapGroupProto(objEditor.m_SelectedObject));
         	    COM_Message("Copied selected object to clipboard in mapgroupProto form.");
         	}
        } else if ( w.GetName() == "objectExportButton_pos") {
            if(objEditor.m_SelectedObject) {
                GetGame().CopyToClipboard(objEditor.getMapGroupPos(objEditor.m_SelectedObject));
                COM_Message("Copied selected object to clipboard in mapgroupPos form.");
            }
        } else if ( w.GetName() == "objectFunctionButton_teleport") {
            if(objEditor.m_SelectedObject) {
                if(COM_GetPB().IsInVehicle()) { COM_Message("Exit the vehicle before teleporting."); return false; }
                vector obj_pos = oData.getPosition();
                COM_GetPB().SetPosition(COM_SnapToGround(obj_pos)); COM_Message("Teleported to " + oData.getName() + " at [" + COM_VectorToString(obj_pos) + "]");
				Camera cam = CameraTool.Cast(COM_GetModuleManager().GetModule(CameraTool)).m_oCamera;
				if(cam) { obj_pos = COM_GetPB().GetPosition(); obj_pos[1] = obj_pos[1] + 2; cam.SetPosition(obj_pos); }
            }
        } else if ( w.GetName() == "objectFunctionButton_duplicate") {
            if(objEditor.m_SelectedObject) {
                objEditor.DupeObjectInPlace(objEditor.m_SelectedObject);
                COM_Message("Duplicated " + oData.getName() + " at [" + COM_VectorToString(objEditor.m_SelectedObject.GetPosition()) + "]");
            }
        } else if ( w.GetName() == "objectFunctionButton_delete") {
            if(objEditor.m_SelectedObject) {
                objEditor.DeleteObject();
            }
        }
        else if (w.GetName() == "object_info_pos_x_minus") { objEditor.moveWest(); }
        else if (w.GetName() == "object_info_pos_x_plus") { objEditor.moveEast(); }
        else if (w == lockYAxisButton) {
            objEditor.lockYAxis = !objEditor.lockYAxis; SetButtonFocus(w);
            if(objEditor.lockYAxis) { COM_Message("The Y Axis is now LOCKED."); }
			else { COM_Message("The Y Axis is now unlocked."); }
        }
        else if (w.GetName() == "object_info_pos_y_minus") { objEditor.moveDown(); }
        else if (w.GetName() == "object_info_pos_y_plus") { objEditor.moveUp(); }
        else if (w.GetName() == "object_info_pos_z_minus") { objEditor.moveSouth(); }
        else if (w.GetName() == "object_info_pos_z_plus") { objEditor.moveNorth(); }
        else if (w.GetName() == "object_info_yaw_minus") { objEditor.rotateLeft(); }
        else if (w.GetName() == "object_info_yaw_plus") { objEditor.rotateRight(); }
        else if (w.GetName() == "object_info_pitch_minus") { objEditor.flipUp(); }
        else if (w.GetName() == "object_info_pitch_plus") { objEditor.flipDown(); }
        else if (w.GetName() == "object_info_roll_minus") { objEditor.flipLeft(); }
        else if (w.GetName() == "object_info_roll_plus") { objEditor.flipRight(); }
        else if (w.GetName() == "object_info_scale_minus") { objEditor.scaleShrink(); }
        else if (w.GetName() == "object_info_scale_plus") { objEditor.scaleEnlarge(); }
		return false;
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel) {
		if (!w.IsInherited(EditBoxWidget) || !objEditor.m_SelectedObject) { return false; }
		auto oData = objHandler.getObjectData(objEditor.m_SelectedObject);
		bool up = wheel < 0; int value = 1; if (up) { value = -1; } int posChange = -1, yprChange = -1, scaleChange = -1; bool changed = false;
		EditBoxWidget editWidget = EditBoxWidget.Cast(w);
		if(!objEditor.groupEditor) { 
			if (editWidget == infoPosYaw) { yprChange = 0; } else if (editWidget == infoPosPitch) { yprChange = 1; } else if (editWidget == infoPosRoll) { yprChange = 2; }
			else if (editWidget == infoPosScale) { scaleChange = 0; }
		}
		if (editWidget == infoPosX) { posChange = 0; } else if (editWidget == infoPosY) { posChange = 1; } else if (editWidget == infoPosZ) { posChange = 2; }
		if(posChange != -1) {
		    if(!newPosChange) { actHandler.addAction("Move"); }
			newPosChange = true; changed = true;
			if(objEditor.groupEditor) {
				foreach(Object obj : objEditor.currentObjects) {
				    auto objData = objHandler.getObjectData(obj);
				    vector currentPos = objData.getPosition(); currentPos[posChange] = currentPos[posChange] + value; objEditor.moveObject(obj, currentPos);
				}
			}
			else { vector pos = oData.getPosition(); pos[posChange] = pos[posChange] + value; objEditor.moveObject(objEditor.m_SelectedObject, pos); }
		}
		if(yprChange != -1) {
		    if(!newYPRChange) { actHandler.addAction("Rotate"); }
			newYPRChange = true; changed = true; vector ypr = oData.getOrientation(); ypr[yprChange] = ypr[yprChange] + value;
			if(Math.AbsFloat(ypr[yprChange]) < 0.001) { ypr[yprChange] = 0; } objEditor.rotateObject(objEditor.m_SelectedObject, ypr);
		}
		if(scaleChange != -1) {
		    if(!newScaleChange) { actHandler.addAction("Scale"); }
			newScaleChange = true; changed = true; float scale = oData.getScale(); scale = scale + (value / 10);
			if(Math.AbsFloat(scale) < 0.01) { scale = 0.01; } objEditor.scaleObject(objEditor.m_SelectedObject, scale);
		}
		if(changed) { editingPos = false; updateInfoPos(); editingPos = true; }
		return false;
	}
	override bool OnChange( Widget w, int x, int y, bool finished ) {
		if (!w.IsInherited(EditBoxWidget) || !objEditor.m_SelectedObject) { return false; }
		auto oData = objHandler.getObjectData(objEditor.m_SelectedObject);
		EditBoxWidget editWidget = EditBoxWidget.Cast(w); float value = editWidget.GetText().ToFloat(); 
		int posChange = -1, yprChange = -1, scaleChange = -1; bool changed = false;
		if(!objEditor.groupEditor) { 
			if (editWidget == infoPosYaw) { yprChange = 0; } else if (editWidget == infoPosPitch) { yprChange = 1; } else if (editWidget == infoPosRoll) { yprChange = 2; }
			else if (editWidget == infoPosScale) { scaleChange = 0; }
		}
		if (editWidget == infoPosX) { posChange = 0; } else if (editWidget == infoPosY) { posChange = 1; } else if (editWidget == infoPosZ) { posChange = 2; }
		if(posChange != -1) {
		    if(!newPosChange) { actHandler.addAction("Move"); }
			newPosChange = true; changed = true; vector pos = oData.getPosition();
			if(objEditor.groupEditor) {
				float offset = value - pos[posChange];
				foreach(Object obj : objEditor.currentObjects) {
				    auto objData = objHandler.getObjectData(obj);
				    vector currentPos = objData.getPosition(); currentPos[posChange] = currentPos[posChange] + offset; objEditor.moveObject(obj, currentPos);
				}
			}
			else { pos[posChange] = value; objEditor.moveObject(objEditor.m_SelectedObject, pos); }
		}
		if(yprChange != -1) {
		    if(!newYPRChange) { actHandler.addAction("Rotate"); }
		    newYPRChange = true; changed = true; vector ypr = oData.getOrientation(); ypr[yprChange] = value; objEditor.rotateObject(objEditor.m_SelectedObject, ypr);
		}
		if(scaleChange != -1) {
		    if(!newScaleChange) { actHandler.addAction("Scale"); }
			newScaleChange = true; changed = true; objEditor.scaleObject(objEditor.m_SelectedObject, value);
		}
		return false;
    }
    override bool OnMouseEnter(Widget w, int x, int y) {
		if ( !w.IsInherited( EditBoxWidget )) { return false; }
		isEditingText = true;
		EditBoxWidget editWidget = EditBoxWidget.Cast(w); string text = editWidget.GetText();
		if (editWidget == infoPosYaw || editWidget == infoPosPitch || editWidget == infoPosRoll || editWidget == infoPosX || editWidget == infoPosY || editWidget == infoPosZ || editWidget == infoPosScale) {
			editingPos = true; newPosChange = false; newYPRChange = false; newScaleChange = false;
		}
		return false;
	}
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y) {
		if ( !w.IsInherited( EditBoxWidget )) { return false; }
		isEditingText = false;
		EditBoxWidget editWidget = EditBoxWidget.Cast(w); string text = editWidget.GetText();
		if (editWidget == infoPosYaw || editWidget == infoPosPitch || editWidget == infoPosRoll || editWidget == infoPosX || editWidget == infoPosY || editWidget == infoPosZ || editWidget == infoPosScale) {
			editingPos = false; newPosChange = false; newYPRChange = false; newScaleChange = false;
		}
		SetFocus( layoutRoot ); return false;
	}
}
