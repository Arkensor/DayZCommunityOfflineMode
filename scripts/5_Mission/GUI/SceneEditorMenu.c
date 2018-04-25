class SceneEditorMenu extends UIScriptedMenu
{	
//---------------------------------------------------------------------------------
// >> Public Scope
	static const int POPUP_ID_SCENE_MANAGER		= 0;
	static const int POPUP_ID_SCENE_SETTINGS	= 1;
	static const int POPUP_ID_SCENE_NEW			= 2;
	static const int POPUP_ID_SCENE_RENAME		= 3;
	static const int POPUP_ID_SCENE_DELETE		= 4;
	static const int POPUP_ID_NOTIFY			= 5;
	static const int POPUP_ID_EDITOR_SETTINGS	= 6;
	static const int POPUP_ID_INIT_SCRIPT		= 7;
	static const int POPUP_ID_POSITION_MANAGER		= 8;
	static const int POPUP_ID_PRESET_NEW = 9;
	static const int POPUP_ID_PRESET_RENAME = 10;
	static const int POPUP_ID_CONFIGS = 11;
	const string CONST_DEFAULT_PRESET_PREFIX = "[Default]";	
	
	// Render specific Preset Items
	void RenderPresets()
	{
		m_PresetsTextListbox.ClearItems();

		int i;
		TBoolArray preset_params;

		// load fixed presets list
		TStringArray presets_array = m_ConfigDebugProfileFixed.GetPresets();
		for ( i = 0; i < presets_array.Count(); i++ )
		{
			m_PresetsTextListbox.AddItem( "["+presets_array.Get(i)+"]", new PresetParams ( presets_array.Get(i), true, false), 0);
		}

		// load custom presets list
		TStringArray custom_presets_array = m_ConfigDebugProfile.GetPresets();
		for ( i = 0; i < custom_presets_array.Count(); i++ )
		{
			m_PresetsTextListbox.AddItem( custom_presets_array.Get(i), new PresetParams ( custom_presets_array.Get(i),false, false), 0);
		}

		string default_preset = m_ConfigDebugProfile.GetDefaultPreset();
		if ( default_preset != "" )
		{
			// if is fixed
			int index = GetPresetIndexByName( default_preset );
			if ( IsPresetFixed( default_preset) )
			{
				default_preset = "[" + default_preset + "]";
			}
			PresetParams preset_params_array;
			if( index > -1 && index < m_PresetsTextListbox.GetNumItems() )
			{
				m_PresetsTextListbox.GetItemData( index, 0, preset_params_array );
				m_PresetsTextListbox.SetItem( index, default_preset + CONST_DEFAULT_PRESET_PREFIX, preset_params_array, 0 );
			}
		}
	}
	
	bool IsPresetFixed( string preset_name )
	{
		int preset_index = GetPresetIndexByName( preset_name);
		PresetParams item_params_array;
		if ( preset_index > -1 && preset_index < m_PresetsTextListbox.GetNumItems() )
		{
			m_PresetsTextListbox.GetItemData( preset_index, 0, item_params_array );
			return item_params_array.param2;
		}
		return false;
	}
	
	int GetPresetIndexByName( string preset_name )
	{
		int i;
		for ( i = 0; i < m_PresetsTextListbox.GetNumItems(); i++ )
		{
			PresetParams item_params_array;
			m_PresetsTextListbox.GetItemData( i, 0, item_params_array );

			if ( item_params_array.param1 == preset_name )
			{
				return i;
			}
		}
		return -1;
	}
	
	void NewPreset( string preset_name )
	{
		m_ConfigDebugProfile.PresetAdd( preset_name );
		RefreshLists();
	}

	void DeletePreset()
	{
		if ( GetCurrentPresetIndex() != -1 )
		{
			bool result = m_ConfigDebugProfile.PresetRemove( GetCurrentPresetName() );
			RefreshLists();
		}
	}
	
	void SetDefaultPreset( int preset_index )
	{
		// remove previous default parameter
		string default_preset = m_ConfigDebugProfile.GetDefaultPreset();
		if ( default_preset != "" )
		{
			int index = GetPresetIndexByName( default_preset );
			// if is fixed
			if ( IsPresetFixed( default_preset) )
			{
				default_preset = "[" + default_preset + "]";
			}
			PresetParams prev_preset_params_array;
			if( index > -1 && index < m_PresetsTextListbox.GetNumItems() )
			{
				m_PresetsTextListbox.GetItemData( index, 0, prev_preset_params_array );
				prev_preset_params_array.param3 = false; // remove DEFAULT
				m_PresetsTextListbox.SetItem( index, default_preset, prev_preset_params_array, 0 );
			}
		}

		// set preset on preset_index to default
		// if is fixed
		string preset_name = GetCurrentPresetName();
		if ( IsPresetFixed( preset_name) )
		{
			preset_name = "[" + preset_name + "]";
		}
		// set new default preset
		PresetParams preset_params_array;
		index = GetCurrentPresetIndex();
		if ( index > -1 && index < m_PresetsTextListbox.GetNumItems() )
		{
			m_PresetsTextListbox.GetItemData( index, 0, preset_params_array );
			preset_params_array.param3 = true; // DEFAULT
			m_PresetsTextListbox.SetItem( index, preset_name + CONST_DEFAULT_PRESET_PREFIX, preset_params_array, 0 );
		}
		// store preset
		m_ConfigDebugProfile.SetDefaultPreset( GetCurrentPresetName() );
	}
	
	void RefreshLists()
	{
		RenderPresets();
		RenderPresetItems();
	}
	
	void RenamePreset( string new_preset_name )
	{
		if ( GetCurrentPresetIndex() != -1 )
		{
			bool result = m_ConfigDebugProfile.PresetRename( GetCurrentPresetName(), new_preset_name );
			RefreshLists();
		}
	}
	
	// Render specific Preset Items
	void RenderPresetItems()
	{
		// load preset items list
		int i;
		m_PresetItemsTextListbox.ClearItems();
		if ( GetCurrentPresetIndex() != -1 )
		{
			bool isFixed = IsCurrentPresetFixed();
			TStringArray preset_array = new TStringArray;

			if ( isFixed )
			{
				 m_ConfigDebugProfileFixed.GetPresetItems( GetCurrentPresetName(), preset_array );
			}
			else
			{
				m_ConfigDebugProfile.GetPresetItems( GetCurrentPresetName(), preset_array );
			}

			if ( preset_array )
			{
				for ( i = 0; i < preset_array.Count(); i++)
				{
					m_PresetItemsTextListbox.AddItem( preset_array.Get(i), NULL, 0);
				}
			}
		}
	}
	
	string GetCurrentPresetName()
	{
		int index = GetCurrentPresetIndex();
		// load preset items list
		if ( index > -1 && index < m_PresetsTextListbox.GetNumItems() )
		{
			PresetParams item_params_array;
			m_PresetsTextListbox.GetItemData( index, 0, item_params_array );
			return item_params_array.param1;
		}
		return "";
	}
	
	string GetCurrentItemName()
	{
		if ( GetCurrentItemIndex() != -1 )
		{
			string item_name;
			m_PresetItemsTextListbox.GetItemText( GetCurrentItemIndex(), 0, item_name );
			return item_name;
		}
		return "";
	}
	
	string GetCurrentObjectName()
	{
		int selected_row_index = m_ClWgtLbxClassesList.GetSelectedRow();
		if ( selected_row_index != -1 )
		{
			string item_name;
			m_ClWgtLbxClassesList.GetItemText( selected_row_index, 0, item_name );
			return item_name;
		}
		return "";
	}
		
	int GetCurrentPresetIndex()
	{
		return m_PresetsTextListbox.GetSelectedRow();
	}

	int GetCurrentItemIndex()
	{
		return m_PresetItemsTextListbox.GetSelectedRow();
	}
	
	bool IsCurrentPresetFixed()
	{
		int index = GetCurrentPresetIndex(); 
		if ( index > -1 && index < m_PresetsTextListbox.GetNumItems() )
		{
			PresetParams item_params_array;
			m_PresetsTextListbox.GetItemData( index, 0, item_params_array );
			return item_params_array.param2;
		}
		return -1;
	}
	
	void AddItemToPreset()
	{
		int selected_row_index = m_ClWgtLbxClassesList.GetSelectedRow();
		if ( selected_row_index != -1 && GetCurrentPresetIndex() != -1 )
		{
			string item_name;
			m_ClWgtLbxClassesList.GetItemText( selected_row_index, 0, item_name );
			m_ConfigDebugProfile.ItemAddToPreset( GetCurrentPresetName(), item_name);
			RenderPresetItems();
		}
	}
	
	void RemoveItemFromPreset()
	{
		if ( GetCurrentItemIndex() != -1 && GetCurrentPresetIndex() != -1 )
		{
			m_ConfigDebugProfile.ItemRemoveFromPreset( GetCurrentPresetName(), GetCurrentItemIndex() );
			RenderPresetItems();
		}
	}
	
	/// spawn preset items into inventory
	void SetPreset( bool clear_inventory, string preset_name)
	{
		int i;
		if ( GetCurrentPresetIndex() != -1 )
		{
			bool is_preset_fixed = IsCurrentPresetFixed();
			TStringArray preset_array = new TStringArray;

			if ( is_preset_fixed )
			{
				m_ConfigDebugProfileFixed.GetPresetItems( preset_name, preset_array );
			}
			else
			{
				m_ConfigDebugProfile.GetPresetItems( preset_name, preset_array );
			}

			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			if ( clear_inventory )
			{
				m_Developer.ClearInventory(player);
			}

			for ( i = 0; i < preset_array.Count(); i++)
			{
				float health = 100;
				int quantity = -1;
				if ( is_preset_fixed )
				{
					health = m_ConfigDebugProfileFixed.GetItemHealth( preset_name, i );
					quantity = m_ConfigDebugProfileFixed.GetItemQuantity( preset_name, i );
				}
				else
				{
					health = m_ConfigDebugProfile.GetItemHealth( preset_name, i );
					quantity = m_ConfigDebugProfile.GetItemQuantity( preset_name, i );
				}
				
				m_Developer.SpawnEntityInInventory(player, preset_array.Get(i), health, quantity);
			}
		}
	}
	
	void ItemMoveUp()
	{
		int new_index = GetCurrentItemIndex() - 1;
		if ( GetCurrentItemIndex() != -1 && GetCurrentPresetIndex() != -1 && new_index > -1)
		{
			m_ConfigDebugProfile.SwapItem( GetCurrentPresetName(), GetCurrentItemIndex(), new_index );
			RenderPresetItems();
			m_PresetItemsTextListbox.SelectRow (new_index);
		}
	}

	void ItemMoveDown()
	{
		int new_index = GetCurrentItemIndex() + 1;
		if ( GetCurrentItemIndex() != -1 && GetCurrentPresetIndex() != -1 && new_index < m_PresetItemsTextListbox.GetNumItems() )
		{
			m_ConfigDebugProfile.SwapItem( GetCurrentPresetName(), GetCurrentItemIndex(), new_index );
			RenderPresetItems();
			m_PresetItemsTextListbox.SelectRow (new_index);
		}
	}
	
	void SaveProfileSpawnDistance()
	{
		if ( m_ConfigDebugProfile && m_SpawnDistanceEditBox )
		{
			m_ConfigDebugProfile.SetSpawnDistance(  m_SpawnDistanceEditBox.GetText().ToFloat() );
		}
	}
		
	// Overrided Parent Functions
	//============================================
	// UseMouse (override)
	//============================================
	override bool UseMouse()
	{
		return true;
	}

	//============================================
	// UseKeyboard (override)
	//============================================
	override bool UseKeyboard()
	{
		return true;
	}
	
	// System Events
	//============================================
	// SceneEditorMenu
	//============================================
	void SceneEditorMenu()
	{
		m_ModuleSceneManager = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
		m_ModuleSceneManager.OnUIEditorOpened();
		
		m_Popups		= new map<int, ref UIPopupScript>;
		m_SlObjectsList	= new map<int, SceneObject>;
		m_OpenedPopups	= new TIntArray;
		m_ClClassesList	= new TStringArray;
	}

	//============================================
	// ~SceneEditorMenu
	//============================================
	void ~SceneEditorMenu()
	{
		PopupCloseAll();
		
		if ( IsModuleExist(PluginSceneManager) )
		{
			m_ModuleSceneManager.OnUIEditorClosed();
			m_ModuleSceneManager = NULL;
		}
		
		m_NotifyFadeTimer.Stop();
	}
	
	//============================================
	// Init
	//============================================
	override Widget Init()
	{
		// Create Main layout menu
		Widget wgt = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_scene_editor.layout");
		m_WgtPnlWrapper = wgt.FindAnyWidget("pnl_presets_wrapper_outer");
		m_SlWgtLoadedScene		= TextWidget.Cast( wgt.FindAnyWidget("txt_left_label_loaded_scene") );
		// Find ListTextBoxWidget for objects list
		m_SlWgtLbxObjectsList	= TextListboxWidget.Cast(wgt.FindAnyWidget("txtlist_left_items") );
		// Find Edit Box for shearching in object list
		m_SlWgtEbxFilter		= EditBoxWidget.Cast(wgt.FindAnyWidget("edit_left_search_item") );
		// Find Select Button for selecting in object list
		m_SlWgtSelect			= ButtonWidget.Cast(wgt.FindAnyWidget("btn_left_select") );
		m_SlWgtFocus			= ButtonWidget.Cast(wgt.FindAnyWidget("btn_left_focus") );
		// Find Popup main panel
		m_WgtPopupsMain			= wgt.FindAnyWidget("pnl_popups");
		// Find Poups backgroudn
		m_WgtPopupsBg			= wgt.FindAnyWidget("pnl_popup_bg");
		// Find Edit Box for shearching in class list
		m_ClWgtEbxFilter		= EditBoxWidget.Cast(wgt.FindAnyWidget("edit_left_search_class") );
		// Find ListTextBoxWidget for class list
		m_ClWgtLbxClassesList	= TextListboxWidget.Cast(wgt.FindAnyWidget("txtlist_left_classes") );
		// Find Buttons
		m_WgtBtnSceneManager	= ButtonWidget.Cast(wgt.FindAnyWidget("btn_top_scene_manager") );
		m_WgtBtnPositionManager	= ButtonWidget.Cast(wgt.FindAnyWidget("btn_top_position_manager") );
		m_WgtBtnSceneSettings	= ButtonWidget.Cast(wgt.FindAnyWidget("btn_top_settings") );
		m_ClWgtButtonAddAtt		= ButtonWidget.Cast(wgt.FindAnyWidget("btn_left_cl_add_attachment") );
		m_WgtBtnSceneSave		= ButtonWidget.Cast(wgt.FindAnyWidget("btn_top_save_scene") );
		m_WgtBtnEditorSettings	= ButtonWidget.Cast(wgt.FindAnyWidget("btn_top_editor_settings") );
		m_WgtBtnEditInitScript	= ButtonWidget.Cast(wgt.FindAnyWidget("btn_right_prop_pos_iscr_value") );
		m_WgtBtnDeleteRuler		= ButtonWidget.Cast(wgt.FindAnyWidget("btn_delete_ruler") );
		m_WgtBtnLeftPresets		= ButtonWidget.Cast(wgt.FindAnyWidget("btn_left_presets") );	
			
		// Find Widgets for properties
		m_PrWgtClassName		= TextWidget.Cast(wgt.FindAnyWidget("txt_right_prop_class_value") );
		m_PrWgtPoxX				= EditBoxWidget.Cast(wgt.FindAnyWidget("ebx_right_prop_pos_x_value") );
		m_PrWgtPoxY				= EditBoxWidget.Cast(wgt.FindAnyWidget("ebx_right_prop_pos_y_value") );
		m_PrWgtPoxZ				= EditBoxWidget.Cast(wgt.FindAnyWidget("ebx_right_prop_pos_z_value") );
		m_PrWgtDir				= EditBoxWidget.Cast(wgt.FindAnyWidget("ebx_right_prop_pos_dir_value") );
		m_PrWgtDmg				= EditBoxWidget.Cast(wgt.FindAnyWidget("ebx_right_prop_pos_hlt_value") );
		m_PrWgtAttRoot			= wgt.FindAnyWidget("pnl_right_inspector_attachments");
		m_PrWgtAttTitle			= wgt.FindAnyWidget("pnl_att_title");
		m_PrWidgetsAttachments	= new array<ref UIPropertyAttachment>;
		
		// Notify
		m_NotifyWgtPanel		= wgt.FindAnyWidget("pnl_notify");
		m_NotifyWgtPanel.SetAlpha(0.0);
		m_NotifyFadeTimer		= new WidgetFadeTimer;
		
		// Register Poups
		m_Popups.Insert(POPUP_ID_SCENE_MANAGER,		new UIPopupScriptSceneManager(wgt.FindAnyWidget("pnl_popup_scene_manager")));
		m_Popups.Insert(POPUP_ID_POSITION_MANAGER,		new UIPopupScriptPositionManager(wgt.FindAnyWidget("pnl_popup_position_manager")));
		m_Popups.Insert(POPUP_ID_SCENE_SETTINGS,	new UIPopupScriptSceneSettings(wgt.FindAnyWidget("pnl_popup_settings")) );
		m_Popups.Insert(POPUP_ID_SCENE_NEW, 		new UIPopupScriptSceneNew(wgt.FindAnyWidget("pnl_popup_scene_new")) );
		m_Popups.Insert(POPUP_ID_SCENE_RENAME, 		new UIPopupScriptSceneRename(wgt.FindAnyWidget("pnl_popup_scene_rename")) );
		m_Popups.Insert(POPUP_ID_SCENE_DELETE, 		new UIPopupScriptSceneDelete(wgt.FindAnyWidget("pnl_popup_scene_delete")) );
		m_Popups.Insert(POPUP_ID_NOTIFY,	 		new UIPopupScriptNotify(wgt.FindAnyWidget("pnl_popup_notify")) );
		m_Popups.Insert(POPUP_ID_EDITOR_SETTINGS,	new UIPopupScriptEditorSettings(wgt.FindAnyWidget("pnl_popup_editor_settings")));
		m_Popups.Insert(POPUP_ID_INIT_SCRIPT,		new UIPopupScriptInitScript(wgt.FindAnyWidget("pnl_popup_init_script")));
		m_Popups.Insert(POPUP_ID_PRESET_NEW,		new UIPopupScriptPresetNew(wgt.FindAnyWidget("pnl_popup_preset_new")));
		m_Popups.Insert(POPUP_ID_PRESET_RENAME,		new UIPopupScriptPresetRename(wgt.FindAnyWidget("pnl_popup_preset_rename")));
		m_Popups.Insert(POPUP_ID_CONFIGS,		new UIPopupScriptConfigs(wgt.FindAnyWidget("pnl_popup_configs")));
		
		m_PresetsTextListbox = TextListboxWidget.Cast( wgt.FindAnyWidget("pnl_presets") );
		m_PresetItemsTextListbox = TextListboxWidget.Cast( wgt.FindAnyWidget("pnl_preset_items") );
		m_ConfigDebugProfileFixed	= PluginConfigDebugProfileFixed.Cast( GetPlugin(PluginConfigDebugProfileFixed) );
		m_ConfigDebugProfile		= PluginConfigDebugProfile.Cast( GetPlugin(PluginConfigDebugProfile) );
		m_PresetAddItemtButton = ButtonWidget.Cast( wgt.FindAnyWidget("btn_add_to_preset") );
		m_PresetRemoveItemButton = ButtonWidget.Cast( wgt.FindAnyWidget("btn_remove_from_preset") );
		m_SpawnOnGroundButton = ButtonWidget.Cast( wgt.FindAnyWidget("btn_spawn_on_ground") );
		m_SpawnInInventoryButton = ButtonWidget.Cast( wgt.FindAnyWidget("btn_spawn_in_inventory") );
		m_SpawnAsAttachmentButton = ButtonWidget.Cast( wgt.FindAnyWidget("btn_spawn_as_attachment") );
		m_UpButton = ButtonWidget.Cast( wgt.FindAnyWidget("btn_up") );
		m_DownButton = ButtonWidget.Cast( wgt.FindAnyWidget("btn_down") );
		m_Developer					= PluginDeveloper.Cast( GetPlugin(PluginDeveloper) );
		m_QuantityEditBox = EditBoxWidget.Cast( wgt.FindAnyWidget("txt_quantity_value") );
		m_DamageEditBox = EditBoxWidget.Cast( wgt.FindAnyWidget("txt_damage_value") );
		m_SpawnDistanceEditBox = EditBoxWidget.Cast( wgt.FindAnyWidget("txt_distance_value") );
		m_PresetNewButton = ButtonWidget.Cast( wgt.FindAnyWidget("btn_new") );
		m_PresetDeleteButton = ButtonWidget.Cast( wgt.FindAnyWidget("btn_delete") );
		m_PresetRenameButton = ButtonWidget.Cast( wgt.FindAnyWidget("btn_rename") );
		m_PresetSetDefaultButton = ButtonWidget.Cast( wgt.FindAnyWidget("btn_default") );
		m_CopyToClipboardButton = ButtonWidget.Cast( wgt.FindAnyWidget("btn_copy_to_clipboard") );
		m_ConfigsButton = EditBoxWidget.Cast( wgt.FindAnyWidget("btn_top_configs") );
	  m_SpawnDistanceEditBox.SetText( m_ConfigDebugProfile.GetSpawnDistance().ToString() );
		
		RenderPresets();
		
		UpdateListObjects();
		UpdateListClasses();
		
		PopupHideAll();
		
		return wgt;
	}
	
	override bool OnDoubleClick( Widget w, int x, int y, int button )
	{
		if( w == m_PresetItemsTextListbox || w == m_PresetsTextListbox || w == m_ClWgtLbxClassesList )
		{
			SaveProfileSpawnDistance();
			
			//float distance = m_SpawnDistanceEditBox.GetText().ToFloat();
			if ( m_SelectedObjectIsPreset )
			{
				SetPreset( true, m_SelectedObject);
				return true;
			}
			else
			{
				float health = 100;
				int quantity = -1;
				if(GetCurrentItemIndex() != -1)
				{
					health = m_ConfigDebugProfile.GetItemHealth( GetCurrentPresetName(), GetCurrentItemIndex() );
					quantity = m_ConfigDebugProfile.GetItemQuantity( GetCurrentPresetName(), GetCurrentItemIndex() );
				}
				m_Developer.SpawnEntityInInventory( PlayerBase.Cast( GetGame().GetPlayer() ), m_SelectedObject, health, quantity);
				return true;
			}
		}
		return false;
	}
	//============================================
	// OnClick
	//============================================
	override bool OnClick(Widget w, int x, int y, int button)
	{	
		super.OnClick(w, x, y, button);

		int row_index;
		
		if ( w == m_WgtBtnSceneManager )
		{
			PopupOpen(POPUP_ID_SCENE_MANAGER, Param.Cast( NULL ) );
			
			return true;
		}
		else if( w  == m_ConfigsButton )
		{
			PopupOpen( POPUP_ID_CONFIGS, NULL );
			return true;
		}
		else if( w == m_PresetsTextListbox )
		{
			RenderPresetItems();
			m_SelectedObjectIsPreset = true;
			m_SelectedObject = GetCurrentPresetName();
			return true;
		}
		else if ( w == m_PresetSetDefaultButton )
		{
			if ( GetCurrentPresetName() != "" )
			{
				SetDefaultPreset( GetCurrentPresetIndex() );
			}
			return true;
		}
		else if ( w == m_PresetNewButton )
		{
			PopupOpen( POPUP_ID_PRESET_NEW, NULL );
			RefreshLists();
			return true;
		}
		else if ( w == m_PresetRenameButton )
		{
			PopupOpen( POPUP_ID_PRESET_RENAME, NULL );
			return true;
		}
		else if( w == m_CopyToClipboardButton )
		{
			GetGame().CopyToClipboard( m_SelectedObject );
			return true;
		}
		else if ( w == m_PresetDeleteButton )
		{
			if ( GetCurrentPresetName() != "" )
			{
				DeletePreset();
			}
			return true;
		}
		else if( w == m_SpawnOnGroundButton || w == m_SpawnAsAttachmentButton || w == m_SpawnInInventoryButton )
		{
			SaveProfileSpawnDistance();

			float distance = m_SpawnDistanceEditBox.GetText().ToFloat();
			float health = 100;
			int quantity = -1;
			if( GetCurrentItemIndex() != -1 )
			{
				health = m_ConfigDebugProfile.GetItemHealth( GetCurrentPresetName(), GetCurrentItemIndex() );
				quantity = m_ConfigDebugProfile.GetItemQuantity( GetCurrentPresetName(), GetCurrentItemIndex() );
			}

			switch ( w )
			{
				case m_SpawnOnGroundButton:
				{
					if ( m_SelectedObjectIsPreset )
					{
                        SetPreset( true, m_SelectedObject );
					}
					else
					{
                        EntityAI oObj = g_Game.CreateObject( m_SelectedObject, g_Game.GetPlayer().GetPosition(), false );
                        
                        if ( oObj.IsInherited( ItemBase ) )
                        {
                            ItemBase oItem = ( ItemBase ) oObj;
                            SetupSpawnedItem( oItem, health, quantity );
                        }
					}
					
					break;
				}

				case m_SpawnAsAttachmentButton:
				{
					Man player = GetGame().GetPlayer();
					
					vector rayStart = GetGame().GetCurrentCameraPosition();
					vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 1.5;		
					vector hitPos;
					vector hitNormal;
					int hitComponentIndex;		
					ref set<Object> hitObjects = new set<Object>;
					DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, hitObjects, NULL, player);
							
					Object target = NULL;
					if( hitObjects.Count() )
						target = hitObjects.Get(0);

					if ( target != NULL && target.IsInherited(EntityAI) )
					{
						EntityAI att_parent = EntityAI.Cast( target );
						m_Developer.SpawnEntityAsAttachment( PlayerBase.Cast( player ), att_parent, m_SelectedObject, health, quantity);
					}
					else
					{
						m_Developer.SpawnEntityAsAttachment( PlayerBase.Cast( player ), player, m_SelectedObject, health, quantity);
					}
					break;
				}

				case m_SpawnInInventoryButton:
				{
					EntityAI oInvItem = g_Game.GetPlayer().GetInventory().CreateInInventory( m_SelectedObject );
					
					oInvItem.SetHealth( health );
					
					break;
				}
			}
			return true;
		}
		else if( w == m_PresetItemsTextListbox )
		{
			m_SelectedObjectIsPreset = false;
			m_SelectedObject = GetCurrentItemName();
			
			if( GetCurrentItemIndex() != -1 )
			{
				float item_health = m_ConfigDebugProfile.GetItemHealth( GetCurrentPresetName(), GetCurrentItemIndex() );
				int item_quantity = m_ConfigDebugProfile.GetItemQuantity( GetCurrentPresetName(), GetCurrentItemIndex() );
				
				m_DamageEditBox.SetText( item_health.ToString() );
				m_QuantityEditBox.SetText( item_quantity.ToString() );
			}
			
			return true;
		}
		else if( w == m_UpButton )
		{
			ItemMoveUp();
			return true;
		}
		else if( w == m_DownButton )
		{
			ItemMoveDown();
			return true;
		}
		else if( w == m_WgtBtnLeftPresets )
		{
			if( m_WgtPnlWrapper.IsVisible() )
			{
				m_WgtPnlWrapper.Show(false);
				m_WgtBtnLeftPresets.SetText("Presets >>");
			}
			else
			{
				m_WgtPnlWrapper.Show(true);
				m_WgtBtnLeftPresets.SetText("Presets <<");
			}
			
			return true;
		}
		else if ( w == m_PresetAddItemtButton )
		{
			AddItemToPreset();
			return true;
		}
		else if ( w == m_PresetRemoveItemButton )
		{
			RemoveItemFromPreset();
			return true;
		}
		else if ( w == m_WgtBtnPositionManager )
		{
			m_PopupScriptPositionManager = UIPopupScriptPositionManager.Cast( PopupOpen( POPUP_ID_POSITION_MANAGER, Param.Cast( NULL ) ) );
			return true;
		}
		else if ( w == m_WgtBtnSceneSettings )
		{
			PopupOpen(POPUP_ID_SCENE_SETTINGS, NULL);
			return true;
		}
		else if ( w == m_SlWgtSelect )
		{
			row_index = m_SlWgtLbxObjectsList.GetSelectedRow();
			
			if ( m_SlObjectsList.Count() > 0 && m_SlObjectsList.Count() > row_index )
			{
				m_ModuleSceneManager.SelectObject(m_SlObjectsList.GetElement(row_index));
			}
			
			return true;
		}
		else if ( w == m_SlWgtFocus )
		{
			m_ModuleSceneManager.SelectedObjectFocus();
			
			return true;
		}
		else if ( w == m_WgtBtnSceneSave )
		{		
			m_ModuleSceneManager.SceneSave();
			
			return true;
		}
		else if ( w == m_WgtBtnEditorSettings )
		{
			PopupOpen(POPUP_ID_EDITOR_SETTINGS, NULL);
			
			return true;
		}
		else if ( w == m_WgtBtnEditInitScript )
		{
			Param2<int, SceneObject> param = new Param2<int, SceneObject>( m_ModuleSceneManager.GetSelectedSceneObjectIndex(), m_ModuleSceneManager.GetSelectedSceneObject() );
			PopupOpen(POPUP_ID_INIT_SCRIPT, param);
			
			return true;
		}
		else if ( w == m_WgtBtnDeleteRuler )
		{
			m_ModuleSceneManager.RulerDelete();
			
			return true;
		}
		
		bool ret = ComponentsOnClick(w, x, y, button);
		
		return ret;
	}
	
	//============================================
	// OnChange
	//============================================
	override bool OnChange(Widget w, int x, int y, bool finished)
	{	
		super.OnChange(w, x, y, finished);

		if (w == m_ClWgtEbxFilter)
		{
			UpdateListClasses();
			return true;
		}
		else if (w == m_SlWgtEbxFilter)
		{
			UpdateListObjects();
			return true;
		}
		else if ( w == m_PrWgtPoxX && finished )
		{
			m_ModuleSceneManager.SelectedObjectSetPosX(m_PrWgtPoxX.GetText().ToFloat());
		}
		else if ( w == m_PrWgtPoxY && finished )
		{
			m_ModuleSceneManager.SelectedObjectSetPosY(m_PrWgtPoxY.GetText().ToFloat());
		}
		else if ( w == m_PrWgtPoxZ && finished )
		{
			m_ModuleSceneManager.SelectedObjectSetPosZ(m_PrWgtPoxZ.GetText().ToFloat());
		}
		else if ( w == m_PrWgtDir && finished )
		{
			m_ModuleSceneManager.SelectedObjectSetRot(m_PrWgtDir.GetText().ToFloat());
			return true;
		}
		else if ( w == m_PrWgtDmg && finished )
		{		
			m_ModuleSceneManager.SelectedObjectSetDamage(m_PrWgtDmg.GetText().ToFloat());
			return true;
		}
		else if ( w == m_QuantityEditBox )
		{
			m_ConfigDebugProfile.SetItemQuantity( GetCurrentPresetName(), GetCurrentItemIndex(), m_QuantityEditBox.GetText().ToInt() );
			return true;
		}
		else if ( w == m_DamageEditBox )
		{
			m_ConfigDebugProfile.SetItemHealth( GetCurrentPresetName(), GetCurrentItemIndex(), m_DamageEditBox.GetText().ToFloat() );
			return true;
		}
		
		bool ret = ComponentsOnChange(w, x, y, finished);
		
		return false;
	}

	//============================================
	// OnMouseWheel
	//============================================
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		super.OnMouseWheel(w, x, y, wheel);
		
		m_ModuleSceneManager.OnMouseWheel(wheel);
		return true;
	}
	
	//============================================
	// OnItemSelected
	//============================================
	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn)
	{	
		super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn);

		if ( w == m_ClWgtLbxClassesList )
		{
			string selected_class_name;
			m_ClWgtLbxClassesList.GetItemText( m_ClWgtLbxClassesList.GetSelectedRow(), 0, selected_class_name );
			m_ModuleSceneManager.SelectClassName(selected_class_name);
			
			SceneObject obj_selected = m_ModuleSceneManager.GetSelectedSceneObject();
			m_SelectedObject = GetCurrentObjectName();
			m_SelectedObjectIsPreset = false;
		}
		else if ( w == m_SlWgtLbxObjectsList )
		{
			int row_index = m_SlWgtLbxObjectsList.GetSelectedRow();
			
			if ( m_SlObjectsList.Count() > 0 && m_SlObjectsList.Count() > row_index )
			{
				m_ModuleSceneManager.SelectObject(m_SlObjectsList.GetElement(row_index));
			}
		}
		
		if( m_PopupScriptPositionManager != NULL )
		{
			m_PopupScriptPositionManager.OnItemSelected(w, x, y, row, column, oldRow, oldColumn);
		}
		
		return true;
	}
	
	// Scripted Events
	//============================================
	// PopupOpen
	//============================================
	UIPopupScript PopupOpen(int popup_id, Param param)
	{	
		// Open background image (black transparent) under popups
		if ( m_OpenedPopups.Count() == 0 )
		{
			m_WgtPopupsMain.Show(true);
			m_WgtPopupsBg.Show(true);
		}
		else
		{
			int popup_curr_id = m_OpenedPopups.Get(m_OpenedPopups.Count() - 1);
			m_Popups.Get(popup_curr_id).Show(false);
		}
		
		m_OpenedPopups.Insert(popup_id);
		
		UIPopupScript popup = m_Popups.Get(popup_id);
		
		popup.Show(true);
		popup.OnOpen(param);
		
		return popup;
	}

	//============================================
	// PopupBack
	//============================================
	UIPopupScript PopupBack()
	{
		if ( m_OpenedPopups.Count() > 0 )
		{
			int popup_curr_id = m_OpenedPopups.Get(m_OpenedPopups.Count() - 1);
		
			m_Popups.Get(popup_curr_id).Show(false);
			m_Popups.Get(popup_curr_id).OnClose();
		
			m_OpenedPopups.Remove(m_OpenedPopups.Count() - 1);	
			
			if ( m_OpenedPopups.Count() > 0 )
			{
				int ppp_id = m_OpenedPopups.Get(m_OpenedPopups.Count() - 1);
				m_Popups.Get(ppp_id).Show(true);
				m_Popups.Get(ppp_id).OnOpen(NULL);
				
				return m_Popups.Get(ppp_id);
			}
		}	
		
		m_WgtPopupsMain.Show(false);
		m_WgtPopupsBg.Show(false);
		
		return NULL;
	}
	
	//============================================
	// PopupCloseAll
	//============================================
	void PopupCloseAll()
	{
		if ( m_OpenedPopups.Count() > 0 )
		{
			int popup_curr_id = m_OpenedPopups.Get(m_OpenedPopups.Count() - 1);
		
			m_Popups.Get(popup_curr_id).Show(false);
			m_Popups.Get(popup_curr_id).OnClose();
			
			m_OpenedPopups.Clear();
		}
	}



	//============================================
	// ToggleVisibility (Show/Hide of editor)
	//============================================
	void ToggleVisibility()
	{
		m_WgtRoot.Show( m_ModuleSceneManager.IsOpened() );
	}
	
	//============================================
	// Refresh
	//============================================
	override void Refresh()
	{
		UpdateListObjects();
		
		string class_name = "n/a";
		string pos_x = "n/a";
		string pos_y = "n/a";
		string pos_z = "n/a";
		string rot = "n/a";
		string hlt = "n/a";
		
		// Clear attachments
		for ( int i = 0; i < m_PrWidgetsAttachments.Count(); ++i )
		{
			m_PrWidgetsAttachments.Get(i).Hide();
		}	
		
		if ( m_ModuleSceneManager.GetSelectedSceneObject() )
		{
			SceneObject obj = m_ModuleSceneManager.GetSelectedSceneObject();
			vector v = obj.GetPosition();
			
			class_name	= obj.GetTypeName();		
			pos_x		= v[0].ToString();
			pos_y		= v[1].ToString();
			pos_z		= v[2].ToString();
			rot			= obj.GetRotation().ToString();
			hlt			= obj.GetHealth().ToString();
			
			ref TStringArray attachments_slots = obj.GetConfigAttachments();
			
			float prop_h = 0.03;
			float prop_count = attachments_slots.Count();
			float prop_root_h = prop_h * (prop_count + 1);
			float line_h = 1.0 / (prop_count + 1);
			
			m_PrWgtAttRoot.SetSize(1, prop_root_h);
			m_PrWgtAttTitle.SetSize(1, line_h);
			
			
			EntityAI e = m_ModuleSceneManager.GetSelectedSceneObject().GetObject();
			
			map<string, ref TStringArray> attachments_in_slots = GetItemNamesForSlots(attachments_slots);
			
			
			for ( int j = 0; j < attachments_in_slots.Count(); ++j )
			{				
				TStringArray attachments_in_slot = attachments_in_slots.GetElement(j);
			
				UIPropertyAttachment ui_prop = GetFreeUIPropertyAttchament();
				
				ui_prop.Show(m_ModuleSceneManager.GetSelectedSceneObject().GetObject(), attachments_in_slots.GetKey(j), attachments_in_slot);
				ui_prop.SetPos( 0, (1 + j) * line_h );
				ui_prop.SetSize( 1, line_h );
			}
			
			m_WgtBtnEditInitScript.Enable( true );
		}
		else
		{
			m_WgtBtnEditInitScript.Enable( false );
		}
		
		m_PrWgtClassName.SetText(class_name);
		m_PrWgtPoxX.SetText(pos_x);
		m_PrWgtPoxY.SetText(pos_y);
		m_PrWgtPoxZ.SetText(pos_z);
		m_PrWgtDir.SetText(rot);
		m_PrWgtDmg.SetText(hlt);
		
		m_SlWgtLoadedScene.SetText("Loaded Scene: "+m_ModuleSceneManager.SceneGetName());
		
		//Ruler
		if ( m_ModuleSceneManager.IsRulerActivated() )
		{
			m_WgtBtnDeleteRuler.SetColor( 0xFF5DE028 );
		}
		else
		{
			m_WgtBtnDeleteRuler.SetColor( 0xFFFFFFFF );
		}
		
	}

	void SceneEditorCommand( Param params )
	{
		Param2<int, Param> p = Param2<int, Param>.Cast( params );
		int cmd_id = p.param1;
		
		switch ( cmd_id )
		{
			case PluginSceneManager.SCENE_EDITOR_CMD_REFRESH:
				Refresh();	
				break;
				
			case PluginSceneManager.SCENE_EDITOR_CMD_SAVE:
				m_NotifyWgtPanel.SetAlpha(1.0);
				m_NotifyFadeTimer.FadeOut(m_NotifyWgtPanel, 2, true);
				break;	
		}
	}


//---------------------------------------------------------------------------------
// >> Protected Scope
	protected Widget				m_WgtRoot;
	protected PluginSceneManager	m_ModuleSceneManager;

//---------------------------------------------------------------------------------
// >> protected Scope	

	// Top Panel
	protected ButtonWidget			m_WgtBtnSceneManager;
	protected ButtonWidget			m_WgtBtnSceneSettings;
	protected ButtonWidget			m_WgtBtnSceneSave;
	protected ButtonWidget			m_WgtBtnEditorSettings;
	protected ButtonWidget			m_WgtBtnDeleteRuler;
	
	// Popups
	protected Widget					m_WgtPopupsMain;
	protected Widget					m_WgtPopupsBg;
	protected ref TIntArray					m_OpenedPopups;
	protected ref map<int, ref UIPopupScript>	m_Popups;
	
	// Scene Object List
	protected TextWidget			m_SlWgtLoadedScene;
	protected string				m_SlSelectedClass;
	protected ref map<int, SceneObject>	m_SlObjectsList;
	protected TextListboxWidget		m_SlWgtLbxObjectsList;
	protected EditBoxWidget			m_SlWgtEbxFilter;
	protected ButtonWidget			m_SlWgtSelect;
	protected ButtonWidget			m_SlWgtFocus;
	
	// Config Class List
	protected string				m_ClSelectedClass;
	protected ref TStringArray	m_ClClassesList;
	protected EditBoxWidget			m_ClWgtEbxFilter;	
	protected TextListboxWidget		m_ClWgtLbxClassesList;
	protected ButtonWidget			m_ClWgtButtonAddAtt;
	
	// Properties
	protected TextWidget			m_PrWgtClassName;
	protected EditBoxWidget			m_PrWgtPoxX;
	protected EditBoxWidget			m_PrWgtPoxY;
	protected EditBoxWidget			m_PrWgtPoxZ;
	protected EditBoxWidget			m_PrWgtDir;
	protected EditBoxWidget			m_PrWgtDmg;
	protected ButtonWidget			m_WgtBtnEditInitScript;
	protected Widget				m_PrWgtAttRoot;
	protected Widget				m_PrWgtAttTitle;
	
	protected ref array<ref UIPropertyAttachment> m_PrWidgetsAttachments;
	
	// Notify
	protected ref WidgetFadeTimer m_NotifyFadeTimer;
	protected Widget				m_NotifyWgtPanel;
	
	//Other
	protected TextListboxWidget m_PresetsTextListbox;
	protected TextListboxWidget m_PresetItemsTextListbox;
	protected ButtonWidget m_PresetAddItemtButton;
	protected ButtonWidget m_PresetRemoveItemButton;
	protected ButtonWidget m_SpawnOnGroundButton;
	protected ButtonWidget m_SpawnInInventoryButton;
	protected ButtonWidget m_SpawnAsAttachmentButton;
	protected ButtonWidget m_UpButton;
	protected ButtonWidget m_DownButton;
	protected ButtonWidget m_PresetNewButton;
	protected ButtonWidget m_PresetDeleteButton;
	protected ButtonWidget m_PresetRenameButton;
	protected ButtonWidget m_PresetSetDefaultButton;
	protected ButtonWidget m_CopyToClipboardButton;
	protected EditBoxWidget m_DamageEditBox;
	protected EditBoxWidget m_QuantityEditBox;
	protected EditBoxWidget m_SpawnDistanceEditBox;
	protected EditBoxWidget m_ConfigsButton;
	protected Widget m_WgtPnlWrapper;
	protected ButtonWidget m_WgtBtnLeftPresets;
	protected ButtonWidget m_WgtBtnPositionManager;
	
	protected string m_SelectedObject;
	protected bool m_SelectedObjectIsPreset;
	protected PluginDeveloper m_Developer;
	protected UIPopupScriptPositionManager m_PopupScriptPositionManager;
	protected PluginConfigDebugProfileFixed m_ConfigDebugProfileFixed;
	protected PluginConfigDebugProfile m_ConfigDebugProfile;
		
	//---- Functions
	protected void 					RefreshByLocalProfile();
	
	//--------------------------------------------
	// UpdateListObjects
	//--------------------------------------------
	private void UpdateListObjects()
	{
		m_SlObjectsList = GetFiltredSceneObjects(m_SlWgtEbxFilter.GetText(), m_SlObjectsList);
		
		m_SlWgtLbxObjectsList.ClearItems();
		
		int row = -1;
		SceneObject selected_object = m_ModuleSceneManager.GetSelectedSceneObject();
		
		for ( int i = 0; i < m_SlObjectsList.Count(); ++i )
		{
			SceneObject scene_obj = m_SlObjectsList.GetElement(i);
			
			if ( selected_object != NULL && selected_object == scene_obj )
			{
				row = i;
			}
			
			m_SlWgtLbxObjectsList.AddItem(scene_obj.GetTypeName(), NULL, 0);
		}	
		
		m_SlWgtLbxObjectsList.SelectRow(row);
	}
	
	//--------------------------------------------
	// UpdateListClasses
	//--------------------------------------------
	protected void UpdateListClasses()
	{
		m_ClClassesList = GetFiltredConfigClasses(m_ClWgtEbxFilter.GetText(), m_ClClassesList);
		
		m_ClWgtLbxClassesList.ClearItems();
		
		for ( int i = 0; i < m_ClClassesList.Count(); ++i )
		{		
			m_ClWgtLbxClassesList.AddItem(m_ClClassesList.Get(i), NULL, 0);
		}
	}
	
	//--------------------------------------------
	// GetFiltredSceneObjects
	//--------------------------------------------
	protected map<int, SceneObject> GetFiltredSceneObjects( string search_string, map<int, SceneObject> array_ret )
	{	
		array<ref SceneObject> scene_objects =  m_ModuleSceneManager.GetSceneObjectsAll();
				
		search_string.ToLower();
		
		array_ret.Clear();
		
		if( scene_objects != NULL )
		{
			for ( int i=0; i < scene_objects.Count(); ++i )
			{
				SceneObject sc_obj = scene_objects.Get(i);
				
				string obj_name = sc_obj.GetTypeName();
				
				obj_name.ToLower();
				
				if ( obj_name.Contains(search_string))
				{
					array_ret.Insert(i, sc_obj);
				}
			}
		}
		
		return array_ret;
	}
	
	//--------------------------------------------
	// GetFiltredConfigClasses
	//--------------------------------------------
	TStringArray GetFiltredConfigClasses( string search_string, TStringArray array_ret )
	{	
		TStringArray searching_in = new TStringArray;
		searching_in.Insert(CFG_VEHICLESPATH);
		searching_in.Insert(CFG_WEAPONSPATH);
		searching_in.Insert(CFG_MAGAZINESPATH);
		
		array_ret.Clear();
		
		search_string.ToLower();
		
		for ( int s = 0; s < searching_in.Count(); ++s )
		{
			string config_path = searching_in.Get(s);
			
			int objects_count = g_Game.ConfigGetChildrenCount(config_path);
			for (int i = 0; i < objects_count; i++)
			{
				string childName;
				g_Game.ConfigGetChildName(config_path, i, childName);

				int scope = g_Game.ConfigGetInt(config_path + " " + childName + " scope");
				if ( scope == 0 )
				{
					continue;
				}
				
				string nchName = childName;
				nchName.ToLower();

				if ( nchName.Contains(search_string))
				{
					array_ret.Insert(childName);
				}
			}
		}
		
		return array_ret;
	}
	
	//--------------------------------------------
	// PopupClose
	//--------------------------------------------
	private void PopupClose(int popup_id)
	{
	}

	//--------------------------------------------
	// PopupCloseAll
	//--------------------------------------------
	private void PopupHideAll()
	{
		for ( int i = 0; i < m_Popups.Count(); ++i )
		{
			m_Popups.Get(i).Show(false);
		}
		
		m_WgtPopupsMain.Show(false);
		m_WgtPopupsBg.Show(false);
	}
	
	//--------------------------------------------
	// ComponentsOnClick
	//--------------------------------------------
	private bool ComponentsOnClick(Widget w, int x, int y, int button)
	{
		for ( int i = 0; i < m_Popups.Count(); ++i )
		{
			if ( m_Popups.Get(i).OnClick(w, x, y, button) )
			{
				return true;
			}
		}
		
		for ( int j = 0; j < m_PrWidgetsAttachments.Count(); ++j )
		{
			if ( m_PrWidgetsAttachments.Get(j).OnClick(w, x, y, button) )
			{
				return true;
			}
		}
		
		return false;
	}
	
	//--------------------------------------------
	// ComponentsOnChange
	//--------------------------------------------
	private bool ComponentsOnChange(Widget w, int x, int y, bool finished)
	{
		for ( int i = 0; i < m_Popups.Count(); ++i )
		{
			if ( m_Popups.Get(i).OnChange(w, x, y, finished) )
			{
				return true;
			}
		}
		
		return false;
	}
	
	//--------------------------------------------
	// GetFreeUIPropertyAttchament
	//--------------------------------------------
	UIPropertyAttachment GetFreeUIPropertyAttchament()
	{
		for ( int i = 0; i < m_PrWidgetsAttachments.Count(); ++i )
		{
			UIPropertyAttachment ui_comp = m_PrWidgetsAttachments.Get(i);
			
			if ( !ui_comp.IsVisible() )
			{
				return ui_comp;
			}
		}
		
		UIPropertyAttachment ui_prop = new UIPropertyAttachment(m_PrWgtAttRoot);
		m_PrWidgetsAttachments.Insert(ui_prop);
		
		return ui_prop;
	}
	
	//--------------------------------------------
	// GetItemNamesForSlots
	//--------------------------------------------
	private map<string, ref TStringArray> GetItemNamesForSlots(TStringArray slots)
	{
		TStringArray searching_in = new TStringArray;
		searching_in.Insert(CFG_VEHICLESPATH);
		searching_in.Insert(CFG_WEAPONSPATH);
		searching_in.Insert(CFG_MAGAZINESPATH);
		
		map<string, ref TStringArray> array_ret = new map<string, ref TStringArray>;
		
		for ( int m = 0; m < slots.Count(); ++m )
		{
			array_ret.Insert(slots.Get(m), new TStringArray);
		}
		
		TStringArray inv_slots = new TStringArray;
		string inv_slot;
		string childName;
		
		for ( int s = 0; s < searching_in.Count(); ++s )
		{
			string config_path = searching_in.Get(s);
			
			int objects_count = g_Game.ConfigGetChildrenCount(config_path);
			for (int i = 0; i < objects_count; i++)
			{
				g_Game.ConfigGetChildName(config_path, i, childName);
				
				g_Game.ConfigGetTextArray(config_path + " " + childName + " inventorySlot", inv_slots);
				
				if ( inv_slots.Count() > 0 )
				{
					for ( int j = 0; j < inv_slots.Count(); ++j )
					{
						inv_slot = "";
						inv_slot = inv_slots.Get(j);
						
						for ( int k = 0; k < slots.Count(); ++k )
						{
							string finding_slot_type = slots.Get(k);
							
							if ( inv_slot == finding_slot_type )
							{
								array_ret.Get(finding_slot_type).Insert(childName);
							}
						}
					}
				}
				else
				{				
					inv_slot = "";
					g_Game.ConfigGetText(config_path + " " + childName + " inventorySlot", inv_slot);
					
					if ( inv_slot != "" )
					{
						for ( int l = 0; l < slots.Count(); ++l )
						{
							string finding_slot_type_2 = slots.Get(l);
							
							if ( inv_slot == finding_slot_type_2 )
							{
								array_ret.Get(finding_slot_type_2).Insert(childName);
							}
						}
					}
				}
			}
		}
		
		return array_ret;
	}
}
