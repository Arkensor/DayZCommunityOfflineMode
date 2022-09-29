class ObjectMenu extends PopupMenu
{
	static bool isReady = false;
	static TextListboxWidget m_classList;
	static EditBoxWidget m_SearchBox;
	protected ButtonWidget m_btnSpawnGround;
	protected ButtonWidget m_btnSpawnCursor;
	protected ButtonWidget m_btnSpawnInventory;
	protected ButtonWidget m_btnCancel;
	static EditBoxWidget m_QuantityItem;
	static EditBoxWidget m_AmountItem;
	static EditBoxWidget m_StateItem;

	static EditBoxWidget groupSelectorNameInput;
	static TextListboxWidget groupSelectorListBox;

	private ItemPreviewWidget m_item_widget;
	protected EntityAI previewItem;
	private int m_characterRotationX;
	private int m_characterRotationY; // Borrowed from inspectmenu
	private int m_characterScaleDelta;
	private vector m_characterOrientation;
	static ObjectEditor objEditor;

	static string currentClassNames;
	static string itemLODsString;

	static autoptr array<string> m_excludeBroken = { "GP25Base", "M203Base", "ItemOptics", "Head", "LargeTentBackPack", "CarWheel", "CarDoor", };

	void ~ObjectMenu() { if ( previewItem ) { GetGame().ObjectDelete( previewItem ); } }

	void Init() {
		objEditor = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor));
		m_classList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("classlist"));
		m_SearchBox = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("search_input"));
		m_btnSpawnGround = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btn_spawn_ground"));
		m_btnSpawnCursor = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btn_spawn_cursorpos"));
		m_btnSpawnInventory = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btn_spawn_inventory"));
		m_btnCancel = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_cancel"));
		m_QuantityItem = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("quantity_items"));
		m_StateItem = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("state_items"));
		m_AmountItem = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("amount_items"));
		groupSelectorNameInput = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("groupSelector_name_input"));
		groupSelectorListBox = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("groupSelector_list"));
		groupSelectorNameInput.SetText(lastObjectGroupInput);
		m_SearchBox.SetText(lastObjectSearch);
		UpdateList(lastObjectFilter);
		showItemPreview(lastObjectSelection);
		isReady = false;
	}

	override void OnShow() { UpdateList("All"); isSpawnMenuOpen = true; groupSelectorNameInput.SetText(lastObjectGroupInput); }

	override void OnHide()  { if (previewItem) { GetGame().ObjectDelete(previewItem); } isSpawnMenuOpen = false; lastObjectGroupInput = groupSelectorNameInput.GetText(); }

	override bool OnChange(Widget w, int x, int y, bool finished) { 
		if ( !w.IsInherited( EditBoxWidget )) { return false; }
		if (w == m_SearchBox) { UpdateList(lastObjectFilter); return true; } 
		if (w == groupSelectorNameInput) { lastObjectGroupInput = groupSelectorNameInput.GetText(); return true; } 
		return false; 
	}

    bool OnMouseEnter( Widget w , int x, int y ) {
		if ( !w.IsInherited( EditBoxWidget )) { return false; }
		isEditingText = true;
		if (w == m_SearchBox) { COM_GetPB().GetInputController().OverrideMovementSpeed( true, 0 ); } 
		return false;
	}

    bool OnMouseLeave(Widget w, Widget enterW, int x, int y) {
		if ( !w.IsInherited( EditBoxWidget )) { return false; }
		isEditingText = false;
		COM_GetPB().GetInputController().OverrideMovementSpeed(false, 0);
		return false;
	}

	override bool OnClick( Widget w, int x, int y, int button ) {
        string strSelection = GetCurrentSelection();
        if (w.GetName().Contains("btn_copy_list")) { GetGame().CopyToClipboard(currentClassNames); COM_Message("Copied current class names list to clipboard."); return true; }
        else if (w.GetName().Contains("btn_copy_lods")) { GetGame().CopyToClipboard(itemLODsString); COM_Message("Copied current class LODs to clipboard."); return true; }
        else if (w.GetName().Contains("btn_filter")) { string buttonName = w.GetName(); buttonName.Replace("btn_filter_", ""); UpdateList( buttonName ); return true; }
        else if (w == groupSelectorListBox) { groupSelectorNameInput.SetText(getSelectedRowGroupSelector()); return true; }
        else if(strSelection != "" && (w == m_btnSpawnInventory || w == m_btnSpawnCursor || w == m_btnSpawnGround)) {
            if(m_excludeBroken.Find(strSelection) != -1 ) { return false; }
            strSelection.ToLower();
            int amount = m_AmountItem.GetText().ToInt();
            if(amount < 1) { amount = 1; m_AmountItem.SetText(amount.ToString()); } else if (amount > 25) { amount = 25; m_AmountItem.SetText(amount.ToString()); }
            for (int i = 0; i < amount; i++) {
                Object newObject; vector pos;
                if (w == m_btnSpawnInventory) { newObject = g_Game.GetPlayer().GetInventory().CreateInInventory(strSelection); }
                else {
                    if(w == m_btnSpawnCursor) { pos = COM_GetCursorPos(); } else { pos = COM_GetPB().GetPosition(); }
                    newObject = objEditor.SpawnObject(strSelection, pos, vector.Zero, groupSelectorNameInput.GetText());
                }
                setupNewItem(newObject);
            }
        }
        return false;
	}

	static void setupNewItem(Object obj) {
	    float itemHealth = 0.0; float itemQuantity = 0.0;
        if (obj.IsInherited(Magazine)) {
            itemHealth = m_StateItem.GetText().ToFloat(); SetupSpawnedItem(ItemBase.Cast(obj), itemHealth, 1);
            Magazine.Cast(obj).LocalSetAmmoCount(m_QuantityItem.GetText().ToInt());
        } else if (obj.IsInherited(ItemBase)) {
            itemHealth = m_StateItem.GetText().ToFloat(); itemQuantity = ItemBase.Cast(obj).SetQuantity(m_QuantityItem.GetText().ToFloat());
            SetupSpawnedItem(ItemBase.Cast(obj), itemHealth, itemQuantity);
        }
    }

	string getSelectedRowGroupSelector() {
		if (groupSelectorListBox.GetSelectedRow() == -1) { return "None"; }
		string result; groupSelectorListBox.GetItemText(groupSelectorListBox.GetSelectedRow(), 0, result); return result;
	}
    string getObjectLODs(Object object) {
        itemLODsString = "";
        array<LOD> lods = new array<LOD>; object.GetLODS(lods);
    	foreach(LOD lod : lods) {
    		itemLODsString = itemLODsString + object.GetLODName(lod) + "\n";
    		array<Selection> selections = new array<Selection>; lod.GetSelections(selections);
    		foreach ( Selection selection : selections ) { itemLODsString = itemLODsString + selection.GetName() + "\n"; }
    	}
    	return itemLODsString;
    }

    override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn ) {
        if (w == groupSelectorListBox) { groupSelectorNameInput.SetText(getSelectedRowGroupSelector()); return true; }
        else if (w == m_classList) {
            string strSelection = GetCurrentSelection(); lastObjectSelection = strSelection;
			showItemPreview(lastObjectSelection);
        }
        return true;
    }
	void showItemPreview(string strSelection) {
        m_characterOrientation = vector.Zero;
        if (!m_item_widget) {
            Widget preview_panel = layoutRoot.FindAnyWidget("preview_pnl");
            if (preview_panel) {
                float width; float height;
                preview_panel.GetSize(width, height);
                m_item_widget = ItemPreviewWidget.Cast(GetGame().GetWorkspace().CreateWidget(ItemPreviewWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, ARGB(255, 255, 255, 255), 10, preview_panel));
            }
        }
        if (previewItem) { GetGame().ObjectDelete(previewItem); }
        if(m_excludeBroken.Find(strSelection) == -1) {
			previewItem = EntityAI.Cast(GetGame().CreateObject(strSelection, vector.Zero, false)); if(!previewItem) { return; }
			getObjectLODs(previewItem);
			m_item_widget.SetItem( previewItem );
			m_item_widget.SetModelPosition( Vector(0,0,0.5) );
			m_item_widget.SetModelOrientation( m_characterOrientation );
			float itemx, itemy;
			m_item_widget.GetPos(itemx, itemy);
			m_item_widget.SetSize(1, 1); m_item_widget.SetPos(0, 0);
			if (previewItem.IsInherited(Magazine)) { m_QuantityItem.SetText(Magazine.Cast(previewItem).GetAmmoCount().ToString()); }
			else if (previewItem.IsInherited(ItemBook)) { m_QuantityItem.SetText(""); }
			else { m_QuantityItem.SetText((previewItem.ConfigGetInt("varQuantityMax")).ToString()); }
			TextWidget selectObjectWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("editor_object_name"));
			TextWidget selectObjectDisplayNameWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("editor_object_name_display"));
			selectObjectWidget.SetText(strSelection);
			selectObjectDisplayNameWidget.SetText(previewItem.GetDisplayName());
		}
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)  {
		if (w == m_item_widget) { GetGame().GetDragQueue().Call(this, "UpdateRotation"); g_Game.GetMousePos(m_characterRotationX, m_characterRotationY); return true; } return false;
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)  {
	    if (w == m_item_widget) { GetGame().GetDragQueue().Call(this, "UpdateScale"); m_characterScaleDelta = wheel ; return true; } return false;
	}

	void UpdateScale(int mouse_x, int mouse_y, int wheel, bool is_dragging) {
		float w, h, x, y;		
		m_item_widget.GetPos(x, y);
		m_item_widget.GetSize(w,h);
		w = w + ( m_characterScaleDelta / 4);
		h = h + ( m_characterScaleDelta / 4 );
		if ( w > 0.5 && w < 4 ) {
			m_item_widget.SetSize( w, h );
			int screen_w, screen_h;
			GetScreenSize(screen_w, screen_h);
			float new_x = x - ( m_characterScaleDelta / 8 );
			float new_y = y - ( m_characterScaleDelta / 8 );
			m_item_widget.SetPos( new_x, new_y );
		}
	}

	void UpdateRotation(int mouse_x, int mouse_y, bool is_dragging) {
		vector o = m_characterOrientation;
		o[0] = o[0] + (m_characterRotationY - mouse_y);
		o[1] = o[1] - (m_characterRotationX - mouse_x);
		m_item_widget.SetModelOrientation( o );
		if (!is_dragging) { m_characterOrientation = o; }
	}

    void UpdateList( string classFilter ) {
		lastObjectFilter = classFilter;
        currentClassNames = ""; m_classList.ClearItems(); m_classList.SetSort(0, true);
		TStringArray configs = new TStringArray; configs.Insert(CFG_VEHICLESPATH); configs.Insert(CFG_WEAPONSPATH); configs.Insert(CFG_MAGAZINESPATH);
        string strSearch = m_SearchBox.GetText(); strSearch.ToLower();
		for ( int nConfig = 0; nConfig < configs.Count(); ++nConfig ) {
			string strConfigPath = configs.Get( nConfig );
			int nClasses = g_Game.ConfigGetChildrenCount( strConfigPath );
			for ( int nClass = 0; nClass < nClasses; ++nClass ) {
				string strName;
				g_Game.ConfigGetChildName( strConfigPath, nClass, strName );
				int scope = g_Game.ConfigGetInt( strConfigPath + " " + strName + " scope" );
				if ( scope == 0 ) { continue; }
				string strNameLower = strName;
				strNameLower.ToLower();
				bool foundObj = false;
                if (classFilter == "walls") {
                    if(strNameLower.Contains("wall") || strNameLower.Contains("fence") || strNameLower.Contains("gate") || strNameLower.Contains("platform") || strNameLower.Contains("bridge")) {
                        foundObj = true;
                    }
                }
                else if (classFilter == "castles") { if(strNameLower.Contains("castle")) { foundObj = true; } }
                else if (classFilter == "roads") {
                    if(strNameLower.Contains("roads") || strNameLower.Contains("sidewalk") || strNameLower.Contains("tunnel") || strNameLower.Contains("stream")) { foundObj = true; } }
                else if (classFilter == "ruins") { if(strNameLower.Contains("ruin") || strNameLower.Contains("rubble") || strNameLower.Contains("wreck")) { foundObj = true; } }
                else if (classFilter == "static") { if(strNameLower.Contains("static")) { foundObj = true; } }
                else if (classFilter == "furniture") { if(strNameLower.Contains("furniture")) { foundObj = true; } }
				else if (GetGame().IsKindOf(strNameLower, classFilter) || classFilter == "All") { foundObj = true; }
				if(foundObj) {
                    if (strSearch != "" && (!strNameLower.Contains(strSearch))) { continue; }
					m_classList.AddItem( strName, NULL, 0 ); currentClassNames += strName + "\n";
                }
			}
		}
    }

	static string GetCurrentSelection() {
		if ( m_classList.GetSelectedRow() == -1 ) { return ""; }
        string result; m_classList.GetItemText( m_classList.GetSelectedRow(), 0, result ); return result;
	}
}