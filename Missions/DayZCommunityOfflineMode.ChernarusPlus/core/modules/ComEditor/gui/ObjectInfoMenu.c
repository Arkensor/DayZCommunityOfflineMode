
// Added editingPos bool, set OnMouseEnter and OnMouseLeave to prevent text updating while typing.
// Adjusted OnChange and OnMouseWheel to allow scaling.
// - Brandon10x15.

class ObjectInfoMenu extends PopupMenu
{
	static EditBoxWidget infoPosX;
	static EditBoxWidget infoPosY;
	static EditBoxWidget infoPosZ;

	static EditBoxWidget infoPosYaw;
	static EditBoxWidget infoPosPitch;
	static EditBoxWidget infoPosRoll;
	static EditBoxWidget infoPosScale;

	static TextListboxWidget listBox;

	static bool editingPos = false;

	private ref WidgetStore widgetStore;

	void ObjectInfoMenu()
	{

	}

	void ~ObjectInfoMenu()
	{
		GetGame().GetCallQueue( CALL_CATEGORY_GUI ).Remove( Update );
	}

	static void UpdateObjectList()
	{
		listBox.ClearItems();

		auto objects = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).m_Objects;

		foreach( Object obj : objects )
		{
			listBox.AddItem(obj.GetType(), obj, 0); // store object ref into list?
		}
	}

	Object GetSelectedRowObject()
	{
		if ( listBox.GetSelectedRow() != -1 )
		{
			Object result;
			listBox.GetItemData( listBox.GetSelectedRow(), 0, result );
			return result;
		}

		return NULL;
	}

	override void Init( )
	{

		widgetStore = new WidgetStore( layoutRoot );

		listBox = widgetStore.GetListboxWidget("object_editor_info_list");

		infoPosX = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_pos_x" ));
		infoPosY = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_pos_y" ));
		infoPosZ = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_pos_z" ));
		infoPosYaw = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_yaw_input" ));
		infoPosPitch = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_pitch_input" ));
		infoPosRoll = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_roll_input" ));
		infoPosScale = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_scale_input" ));
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
		if ( w.GetName() == "object_editor_info_list")
		{
			Object selected = GetSelectedRowObject();
			if ( selected )
			{
				ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).SelectObject( selected );
			}
		}

		return false;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w.GetName() == "object_editor_info_export")
		{
			ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).ExportScene();
		}
		if ( w.GetName() == "object_editor_info_save")
		{
			ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).SaveScene();
		}
		if ( w.GetName() == "object_editor_info_clear")
		{
			auto objects = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).m_Objects;
			foreach(auto obj : objects)
			{
				obj.SetPosition(vector.Zero);
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(GetGame().ObjectDelete, 100, false, obj);
			}
			objects.Clear();
			UpdateObjectList();
		}
		if ( w.GetName() == "object_editor_info_dumplods" )
		{
			string toCopy = "";
			array<LOD> lods = new array<LOD>;
			Object object = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).m_SelectedObject;
			object.GetLODS(lods);

			foreach( LOD lod : lods )
			{
				toCopy = toCopy + object.GetLODName( lod ) + "\n";
				array<Selection> selections = new array<Selection>;
				lod.GetSelections( selections );
				foreach ( Selection selection : selections )
				{
					toCopy = toCopy + " " + selection.GetName() + "\n";
				}
			}
			GetGame().CopyToClipboard(toCopy);
			COM_Message("Dumped LODs to clipboard");
		}
		if ( w.GetName() == "object_editor_info_load")
		{
			objects = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).m_Objects;

			foreach( Object obj2 : objects )
			{
				GetGame().ObjectDelete( obj2 );
			}
			objects.Clear();
			ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).LoadScene();
			UpdateObjectList();
		}
		return false;
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		if ( !GetSelectedObject() )
		{
			return false;
		}
		EditBoxWidget editWidget = EditBoxWidget.Cast(w);

		vector orientation = GetSelectedObject().GetOrientation();
		vector position = GetSelectedObject().GetPosition();
		float scale = GetSelectedObject().GetScale();

		bool up = wheel < 0;
		int value = 1;
		if ( up ) value = -1;

		bool changed = false;
		int posChange = -1, yprChange = -1, scaleChange = -1;

		if (editWidget == infoPosX) { posChange = 0; }
		else if (editWidget == infoPosY) { posChange = 1; }
		else if (editWidget == infoPosZ) { posChange = 2; }
		else if (editWidget == infoPosYaw) { yprChange = 0; }
		else if (editWidget == infoPosPitch) { yprChange = 1; }
		else if (editWidget == infoPosRoll) { yprChange = 2; }
		else if (editWidget == infoPosScale) { scaleChange = 0; }

        if(posChange != -1) {
        	changed = true;
            position[posChange] = position[posChange] + value;
			GetSelectedObject().SetPosition( position );
			COM_ForceTargetCollisionUpdate( GetSelectedObject() );
			infoPosX.SetText(position[0].ToString());
			infoPosY.SetText(position[1].ToString());
			infoPosZ.SetText(position[2].ToString());
		}
        if(yprChange != -1) {
        	changed = true;
            orientation[yprChange] = orientation[yprChange] + value;
			GetSelectedObject().SetOrientation( orientation );
			COM_ForceTargetCollisionUpdate( GetSelectedObject() );
			infoPosYaw.SetText(orientation[0].ToString());
			infoPosPitch.SetText(orientation[1].ToString());
			infoPosRoll.SetText(orientation[2].ToString());
		}
        if(scaleChange != -1) {
        	scale = scale + (value / 10);
            if(Math.AbsFloat(scale) < 0.001) { scale = 0.001; }
			infoPosScale.SetText(scale.ToString());
		}
		if((changed && scale.ToString() != "1") || scaleChange != -1)
		{
        	ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).ScaleObject(GetSelectedObject(), scale);
        }
		return false;
	}

	override bool OnChange( Widget w, int x, int y, bool finished ) // finished = press enter
	{
		if ( !w.IsInherited( EditBoxWidget ))
		{
			return false;
		}

		vector position = GetSelectedObject().GetPosition();
		vector orientation = GetSelectedObject().GetOrientation();
		float scale = GetSelectedObject().GetScale();

		EditBoxWidget editWidget = EditBoxWidget.Cast(w);
		string text = editWidget.GetText();

		bool changed = false;
		int posChange = -1, yprChange = -1, scaleChange = -1;

		if (editWidget == infoPosX) { posChange = 0; }
		else if (editWidget == infoPosY) { posChange = 1; }
		else if (editWidget == infoPosZ) { posChange = 2; }
		else if (editWidget == infoPosYaw) { yprChange = 0; }
		else if (editWidget == infoPosPitch) { yprChange = 1; }
		else if (editWidget == infoPosRoll) { yprChange = 2; }
		else if (editWidget == infoPosScale) { scaleChange = 0; }

		float value = text.ToFloat();

        if(posChange != -1) {
        	changed = true;
            position[posChange] = value;
			GetSelectedObject().SetPosition( position );
			COM_ForceTargetCollisionUpdate( GetSelectedObject() );
		}
        if(yprChange != -1) {
        	changed = true;
            orientation[yprChange] = value;
			GetSelectedObject().SetOrientation( orientation );
			COM_ForceTargetCollisionUpdate( GetSelectedObject() );
		}
        if(scaleChange != -1) {
        	scale = value;
            if(Math.AbsFloat(scale) < 0.001) { scale = 0.001; }
		}
		if((changed && scale.ToString() != "1") || scaleChange != -1)
		{
        	ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).ScaleObject(GetSelectedObject(), scale);
        }
//		Print("float value = text.ToFloat() = " + value);

/*
		bool check = false; //?????????????

        if ( editWidget == infoPosPitch )
        {
        	for(int i = 0; i < text.Length(); i++ )
        	{
        		string token;
        		string character = text.Get(i);
        		int result = character.ParseStringEx(token);

        		if ( result == 4 || text.Get(i) == "." )
        		{
        			newText = newText + token;
        			check = true; // lol wtf why this crashing. come back to this
        		}
        	}

        	if ( check ) // idk why this bool is causing unknown module crash
        	{
        		orientation[1] = newText.ToFloat();
        		GetSelectedObject().SetOrientation(orientation);
        	}
        	else
        	{
        		editWidget.SetText( newText );
        	}

            return false;
        }

        */

        return false;
    }

    override bool OnMouseEnter(Widget w, int x, int y) {
		if ( !w.IsInherited( EditBoxWidget )) { return false; }
		EditBoxWidget editWidget = EditBoxWidget.Cast(w);
		if (editWidget == infoPosYaw || editWidget == infoPosPitch || editWidget == infoPosRoll || editWidget == infoPosX || editWidget == infoPosY || editWidget == infoPosZ || editWidget == infoPosScale)
		{
			editingPos = true;
		}
		return false;
	}
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y) {
		if ( !w.IsInherited( EditBoxWidget )) { return false; }
		EditBoxWidget editWidget = EditBoxWidget.Cast(w);
		if (editWidget == infoPosYaw || editWidget == infoPosPitch || editWidget == infoPosRoll || editWidget == infoPosX || editWidget == infoPosY || editWidget == infoPosZ || editWidget == infoPosScale)
		{
			editingPos = false;
			infoPosX.SetText( GetSelectedObject().GetPosition()[0].ToString() );
            infoPosY.SetText( GetSelectedObject().GetPosition()[1].ToString() );
            infoPosZ.SetText( GetSelectedObject().GetPosition()[2].ToString() );
            infoPosYaw.SetText( GetSelectedObject().GetOrientation()[0].ToString() );
            infoPosPitch.SetText( GetSelectedObject().GetOrientation()[1].ToString() );
            infoPosRoll.SetText( GetSelectedObject().GetOrientation()[2].ToString() );
            infoPosScale.SetText( GetSelectedObject().GetScale().ToString() );
		}
		SetFocus( layoutRoot );
		return false;
	}

    override void OnShow()
    {
    	UpdateObjectList();
    	GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( Update, 100, true );
    }

    override void OnHide()
    {
    	//GetGame().GetUpdateQueue( CALL_CATEGORY_GUI ).Remove( Update );
    }

	void Update()
	{
		Object selectedObject = GetSelectedObject();
		string text = "No object Selected";
		TStringArray paths = new TStringArray;

		bool selected = false;

		Widget focusedWidget = GetFocus();

		if ( selectedObject )
		{
			infoPosX.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosY.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosZ.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosYaw.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosPitch.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosRoll.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosScale.ClearFlags( WidgetFlags.IGNOREPOINTER );

			text = selectedObject.GetType();
		} else
		{
			infoPosX.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosY.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosZ.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosYaw.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosPitch.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosRoll.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosScale.SetFlags( WidgetFlags.IGNOREPOINTER );
		}

		TextWidget selectedObjectWidget = TextWidget.Cast(layoutRoot.FindAnyWidget( "object_editor_info_select_input" ));
		selectedObjectWidget.SetText( text );
		// SetFlags(int flags, bool immedUpdate = true);
	}

	Object GetSelectedObject()
	{
		return ObjectEditor.Cast(COM_GetModuleManager().GetModule( ObjectEditor )).m_SelectedObject;
	}
}
