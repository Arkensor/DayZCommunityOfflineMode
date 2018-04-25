// #include "scripts/GUI/Inventory/InventoryManager.c"

// #include "scripts/GUI/Inventory/InventoryView/InventoryView.c"
// #include "scripts/GUI/Inventory/InventoryView/InventoryViewBase.c"
// #include "scripts/GUI/Inventory/InventoryView/InventoryViewProperties.c"
// #include "scripts/GUI/Inventory/InventoryView/InventoryViewGrid.c"
// #include "scripts/GUI/Inventory/InventoryView/InventoryViewHands.c"

// #include "scripts/GUI/Inventory/InventoryCombinationFlags.c"
// #include "scripts/GUI/Inventory/InventoryDragFromType.c"
// #include "scripts/GUI/Inventory/InventoryDragToType.c"
// #include "scripts/GUI/Inventory/InventoryQuickbar.c"

// #include "scripts/GUI/Inventory/InventoryContainer/InventoryContainer.c"
// #include "scripts/GUI/Inventory/InventoryContainer/InventoryContainerEntity.c"
// #include "scripts/GUI/Inventory/InventoryContainer/InventoryContainerHands.c"
// #include "scripts/GUI/Inventory/InventoryContainer/InventoryContainerVicinity.c"

//TODO modify note methods below, if needed
void EnfReadNote(string text)
{
	text.Replace("\n", "<br>");
	
	InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().FindMenu(MENU_INVENTORY) );
	if (menu)
	{	
		NoteMenu note_menu = NoteMenu.Cast( menu.EnterScriptedMenu(MENU_NOTE) );
		if (note_menu)
		{
			//note_menu.InitRead(text);
		}
	}
}

void EnfWriteNote(Paper paper, InventoryItem pen)
{
	InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().FindMenu(MENU_INVENTORY) );
	if (menu)
	{	
		NoteMenu note_menu = NoteMenu.Cast( menu.EnterScriptedMenu(MENU_NOTE) );
		if (note_menu)
		{
			//note_menu.InitWrite(paper, pen, 0); //0 = default handwriting
		}
	}
}

class InventoryMenu extends UIScriptedMenu
{
	const int TOOLTIP_TIME = 1.0;
	const int PLAYER_STATUS_COUNT = 8;
	private int m_characterRotationX;
	private int m_characterRotationY;
	private int m_characterScaleDelta;
	private bool m_isCharacterRotating;
	private vector m_characterOrientation;
	
	private EntityAI m_target;
	private EntityAI m_itemInHands;
	private InventoryItem m_tooltip_target;
	private Widget m_tooltip_widget;

	private ref array<Object> m_ground_objects;
	private ref array<Object> m_ground_zombie_objects;
	private ref array<ref InventoryContainerEntity> m_ground_objects_containers;
	private ref array<ref InventoryContainerZombie> m_ground_zombie_objects_containers;

	private ref Timer m_refresh_timer;
	
	private ref ContextMenu m_context_menu;

	private PlayerPreviewWidget m_character_panel_widget;	
	private Widget	m_character_panel_area_widget;
	private Widget	m_left_panel;
	private Widget	m_right_panel;
	private Widget	m_center_panel;
	private Widget	m_left_panel_content;
	private Widget	m_right_panel_content;
	private Widget	m_center_panel_content;
	private Widget	m_center_panel_border;
	private Widget	m_left_panel_border;
	private Widget	m_right_panel_border;
	private Widget	m_vicinity_panel;
	private Widget	m_quickbar_widget;
	private ref InventoryContainerHands m_hands_container;
	ref InventoryContainerVicinity m_ground_container;
	ref InventoryContainerEntity m_equipment_container;
	private ref InventoryContainerEntity m_target_container;
	private ref InventoryQuickbar m_quickbar;
	private ref InventoryManager m_drag_manager;
	
	void InventoryMenu()
	{
		m_characterRotationX = 0;
		m_characterRotationY = 0;
		m_characterScaleDelta = 1;
		m_isCharacterRotating = false;
		
		m_context_menu = new ContextMenu;
		m_refresh_timer = new Timer(CALL_CATEGORY_GUI);
		m_drag_manager = new InventoryManager;

		m_ground_objects = new array<Object>;
		m_ground_zombie_objects = new array<Object>;
		m_ground_objects_containers = new array<ref InventoryContainerEntity>;
		m_ground_zombie_objects_containers = new array<ref InventoryContainerZombie>;
	}

	void ~InventoryMenu()
	{
		s_ground_container_list_switch = false;
		s_equipment_container_list_switch = false;
		s_target_container_list_switch = false;
		
		if (m_ground_container != NULL)
		{
			s_ground_container_list_switch = m_ground_container.IsListView();
		}
			
		if (m_target_container != NULL)
		{
			s_target_container_list_switch = m_target_container.IsListView();
		}
		
		if (m_equipment_container != NULL)
		{
			s_equipment_container_list_switch = m_equipment_container.IsListView();
		}

		GetGame().GetDragQueue().RemoveCalls(this);
	}
	
#ifdef PLATFORM_XBOX
	bool UseMouse()
	{
		return true;
	}
#endif
	
	override bool UseKeyboard() 
	{
		return false;
	}

	override Widget Init()
	{
		if (layoutRoot) return layoutRoot;
		
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_inventory.layout");

		PlayerBase player = PlayerBase.Cast( g_Game.GetPlayer() );

		// context menu
		m_context_menu.Init(layoutRoot);

		// tooltip
		m_tooltip_widget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_inventory_tooltip.layout", layoutRoot);
		m_tooltip_widget.Show( false );

		// player preview
		m_character_panel_area_widget = layoutRoot.FindAnyWidget("CharacterPanel");

		m_character_panel_widget = PlayerPreviewWidget.Cast( layoutRoot.FindAnyWidget("CharacterPanelWidget") );
		m_character_panel_widget.SetPlayer(player);
		vector pivot;
		pivot[0] = 0;
		pivot[1] = -1;
		pivot[2] = 0.605;
		m_character_panel_widget.SetModelPosition( pivot );

		m_character_panel_widget.SetSize( 1.34, 1.34 );  // default scale
		UpdateScale();
		UpdatePlayerPreviewAnim(player.GetHumanInventory().GetEntityInHands());
		
		// player status
		m_Player_Statuses = new array<TextWidget>;
		m_Player_Statuses.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget("Status0") ) );
		m_Player_Statuses.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget("Status1") ) );
		m_Player_Statuses.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget("Status2") ) );
		m_Player_Statuses.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget("Status3") ) );
		m_Player_Statuses.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget("Status4") ) );
		m_Player_Statuses.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget("Status5") ) );
		m_Player_Statuses.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget("Status6") ) );
		m_Player_Statuses.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget("Status7") ) );

		// player status
		m_Player_StatusesBackground = new array<ImageWidget>;
		m_Player_StatusesBackground.Insert( ImageWidget.Cast( layoutRoot.FindAnyWidget("StatusBackground0") ) );
		m_Player_StatusesBackground.Insert( ImageWidget.Cast( layoutRoot.FindAnyWidget("StatusBackground1") ) );
		m_Player_StatusesBackground.Insert( ImageWidget.Cast( layoutRoot.FindAnyWidget("StatusBackground2") ) );
		m_Player_StatusesBackground.Insert( ImageWidget.Cast( layoutRoot.FindAnyWidget("StatusBackground3") ) );
		m_Player_StatusesBackground.Insert( ImageWidget.Cast( layoutRoot.FindAnyWidget("StatusBackground4") ) );
		m_Player_StatusesBackground.Insert( ImageWidget.Cast( layoutRoot.FindAnyWidget("StatusBackground5") ) );
		m_Player_StatusesBackground.Insert( ImageWidget.Cast( layoutRoot.FindAnyWidget("StatusBackground6") ) );
		m_Player_StatusesBackground.Insert( ImageWidget.Cast( layoutRoot.FindAnyWidget("StatusBackground7") ) );

		float w,h;
		m_left_panel = layoutRoot.FindAnyWidget("LeftPanel");	
		m_right_panel = layoutRoot.FindAnyWidget("RightPanel");
		m_center_panel = layoutRoot.FindAnyWidget("CenterPanel");
		m_left_panel_content = m_left_panel.FindAnyWidget("Content");	
		m_right_panel_content = m_right_panel.FindAnyWidget("Content");
		m_center_panel_content = m_center_panel.FindAnyWidget("Content");

		m_quickbar_widget = layoutRoot.FindAnyWidget("QuickbarGrid");	

		m_center_panel_border = layoutRoot.FindAnyWidget("CenterPanelBorder");
		m_left_panel_border = layoutRoot.FindAnyWidget("LeftPanelBorder");
		m_right_panel_border = layoutRoot.FindAnyWidget("RightPanelBorder");
		m_vicinity_panel = layoutRoot.FindAnyWidget("VicinityPanel");
		
		m_left_panel_content.GetScreenSize(w,h);	
		
		InventoryManager manager = InventoryManager.GetInstance();
		manager.RecalculateIconSize(w);
		
		return layoutRoot;
	}
	
	void InitContainers(EntityAI target)
	{
		float w = 64;
		float h = 64;
		PlayerBase player = PlayerBase.Cast( g_Game.GetPlayer() );
		
		if (m_target && m_target != target)
		{
			m_target = NULL;
			m_target_container = NULL;
		}

			if (target && !m_target)
		{
			if (target.IsInherited( InventoryItem) || target.IsInherited( Building) || target.IsInherited( PlayerBase) || target.IsInherited( Car) || target.IsInherited( AnimalBase) )
			{
				m_target = target;
			}
		}

		//-----
		if (m_hands_container == NULL)
		{
			m_hands_container = new InventoryContainerHands(m_center_panel_content, true, true );
			m_hands_container.Init();
			m_hands_container.SetHeaderText("EMPTY HANDS");
			m_hands_container.UpdateItems();
		}	
		
		if (/*target == NULL && */m_ground_container == NULL)
		{
			m_ground_container = new InventoryContainerVicinity(m_left_panel_content, false, true );
			m_ground_container.Init();
			m_ground_container.SetHeaderText("#container_vicinity");
			m_ground_container.UpdateItems();
		}
		
		if (m_target && m_ground_container)
		{
			m_ground_container = NULL;
		}
			
		if (m_target_container == NULL && m_target)
		{
			m_target_container = new InventoryContainerEntity(m_left_panel_content, false,true);
			m_target_container.Init(m_target);
			m_target_container.SetHeaderText(m_target.GetDisplayName());
			m_target_container.UpdateItems();
		}
		
		if (m_equipment_container == NULL)
		{
			m_equipment_container = new InventoryContainerEntity(m_right_panel_content, true,true);
			m_equipment_container.Init(GetGame().GetPlayer());
			m_equipment_container.SetHeaderText("#container_inventory");
			m_equipment_container.UpdateItems();	
		}
		
		if (m_quickbar == NULL)
		{
			m_quickbar = new InventoryQuickbar( m_quickbar_widget );
			m_quickbar.UpdateItems( m_quickbar_widget );
		}
		
		InventoryView.ClearHistory();
	}
	
	void RefreshPlayerStatus()
	{

			// clear table
			string ordered_notifiers[PLAYER_STATUS_COUNT];
			for ( int i = 0; i < PLAYER_STATUS_COUNT; i++ )
			{
				ordered_notifiers[i]="";
				m_Player_Statuses.Get(i).SetText( "" );
				m_Player_StatusesBackground.Get( i ).Show( false );
			}

			// sort keys (use index+key as key in temporary static table - we can use sort function )
			// create table for sorting
			PluginPlayerStatus module_player_status = PluginPlayerStatus.Cast( GetPlugin(PluginPlayerStatus) );
			if ( ! module_player_status ) return;
			
			for ( i = 0; i < module_player_status.m_NotifiersLabel.Count(); i++ )
			{
				int index = module_player_status.m_NotifiersIndexColor.GetByIndex(i).Get(0); 
				string key = index.ToString() + module_player_status.m_NotifiersLabel.GetKeyByIndex(i).ToString(); //string(index+key)
				ordered_notifiers[i] = key;
			}
			// sort
			Sort(ordered_notifiers, PLAYER_STATUS_COUNT);
			// remove index, so we have sorted table containing keys (sorted by index)
			for ( i = 0; i < PLAYER_STATUS_COUNT; i++ )
			{
				string index_key = ordered_notifiers[i];
				int length =  index_key.Length(  ) - 1;
				if ( length > 0 )
				{
					ordered_notifiers[i] = index_key.Substring( 1, length);
				}
			}
			
			// Print("------------------------ sort");
			// for ( i = 0; i < PLAYER_STATUS_COUNT; i++ )  { Print( ordered_notifiers[i] ); }
			// Print("------------------------ ");

			// render statuses
			int status_text_widget_index = 0;
			for ( i = 0; i < PLAYER_STATUS_COUNT; i++ )
			{
				int modifier_key  = ordered_notifiers[i].ToInt();
				
				if ( modifier_key )
				{
					string label = module_player_status.m_NotifiersLabel.Get( modifier_key ).Get(0);
					int color = module_player_status.m_NotifiersIndexColor.Get( modifier_key ).Get(1);
					if ( label && color )
					{
						TextWidget player_status_widget = m_Player_Statuses.Get( status_text_widget_index );
						ImageWidget player_status_widget_background = m_Player_StatusesBackground.Get( status_text_widget_index );
						player_status_widget.SetText( label );
						player_status_widget_background.SetColor( color );
						player_status_widget_background.Show( true );
						status_text_widget_index = status_text_widget_index + 1;
					}
				}
			}

		}
	
	void Blur(float intensity)
	{
		Material matColors = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/gauss");
		if (matColors)
			matColors.SetParam("Intensity", intensity);
	}

	// menu overloads
	override void OnShow()
	{
		super.OnShow();
		PPEffects.SetBlurInventory(1);
		PlayerBase player = PlayerBase.Cast( g_Game.GetPlayer() );
		m_refresh_timer.Run(0.5, this, "PerformRefreshInventory", NULL, true);
		
		// refresh ingame hud quickbar
		Mission mission = GetGame().GetMission();
		if ( mission )
		{
			IngameHud hud = IngameHud.Cast( mission.GetHud() );
			if ( hud )
			{
				hud.HideQuickbar( true, true );
				hud.SetLeftStatsVisibility( false );
				hud.SetSpecialtyMeterVisibility( true );
			}
		}
		RefreshQuickbar();
		ShowQuickbar();
			
		PerformRefreshInventory();
		Refresh();
		if( m_equipment_container ) m_equipment_container.UpdateViews();
	}

	override void OnHide()
	{
		super.OnHide();
		
		PPEffects.SetBlurInventory(0);	
		m_refresh_timer.Stop();
		HidePanelBorders();
		
		InventoryManager manager = InventoryManager.GetInstance();	
		if ( manager )
		{
			manager.HideTooltip();
		}

		Mission mission = GetGame().GetMission();
		if ( mission )
		{
			IngameHud hud = IngameHud.Cast( mission.GetHud() );
			if ( hud )
			{
				hud.SetLeftStatsVisibility(true);
				hud.SetSpecialtyMeterVisibility( false );
				if ( hud.GetQuickBarState() )
				{
					hud.ShowQuickbar();
				}
			}
		}
		HideQuickbar();
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		return false;
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonDown(w, x, y, button);
		
		if ( w == m_character_panel_area_widget )
		{
			GetGame().GetDragQueue().Call(this, "UpdateRotation");
			g_Game.GetMousePos(m_characterRotationX, m_characterRotationY);
		}
		return false;
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonUp(w, x, y, button);
		
		m_isCharacterRotating = false;

		return false;
	}

	override bool OnMouseWheel(Widget  w, int  x, int  y, int wheel)
	{
		super.OnMouseWheel(w, x, y, wheel);
		
		if ( w == m_character_panel_area_widget )
		{
			GetGame().GetDragQueue().Call(this, "UpdateScale");
			m_characterScaleDelta = wheel ;
		}
		return false;
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		super.OnDoubleClick(w, x, y, button);
		
		if (w && w.GetTypeID() == ItemPreviewWidgetTypeID && button == MouseState.LEFT)
		{
			Man player = GetGame().GetPlayer();
			ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w );
			EntityAI item = ipw.GetItem();
			EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();

			if (item && item.IsInventoryItem() && item.GetHierarchyRootPlayer() == player && item_in_hands != item && GetGame().GetPlayer().GetHumanInventory().CanAddEntityInHands( item ) )
			{
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(player.PredictiveTakeEntityToHands, item );
				return true;
			}
			else if (item && player.GetInventory().CanAddEntityToInventory(item) &&  item.GetHierarchyRootPlayer() != player)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(player.PredictiveTakeEntityToInventory, item);
				return true;
			} 
			else if (item && item.IsInventoryItem())
			{
				//InspectItem(item);
				return true;
			}
		}
		return false;
	}
	
	void UpdateRotation(int mouse_x, int mouse_y, bool is_dragging)
	{
		vector o = m_characterOrientation;
		o[1] = o[1] - (m_characterRotationX - mouse_x);
		m_character_panel_widget.SetModelOrientation(o);

		if (!is_dragging)
		{	
			m_characterOrientation = o;
		}
	}

	void UpdateScale()
	{
		float w, h;
		m_character_panel_widget.GetSize(w,h);
		w = w + ( m_characterScaleDelta / 25 );
		h = h + ( m_characterScaleDelta / 25 );
		if ( w > 0.62 && w < 3 )
		{
			m_character_panel_widget.SetSize( w, h );
		}
	}

	void UpdatePlayerPreviewAnim(EntityAI inHands)
	{
		if (m_character_panel_widget == NULL || m_itemInHands == inHands)	return;
		m_itemInHands = inHands;		
		
		m_character_panel_widget.SetItemInHands( InventoryItem.Cast( inHands ) );
		
		// Mirek: Switch move was related to legacy animation system
		if (inHands == NULL)
		{	
//			m_character_panel_widget.SwitchMove("menu_idleUnarmed0");
		}
		else if (inHands.IsKindOf("Pistol"))
		{
//			m_character_panel_widget.SwitchMove("menu_idlePistol0");
		}
		else if (inHands.IsKindOf("Rifle"))
		{
//			m_character_panel_widget.SwitchMove("menu_idlerifleLong0");
		}
		else if (inHands.IsKindOf("KnifeBase"))
		{
//			m_character_panel_widget.SwitchMove("menu_idleKnife0");
		}
		else if (inHands.IsKindOf("BaseballBat"))
		{
//			m_character_panel_widget.SwitchMove("menu_idleBat0");
		}
		else if (inHands.IsKindOf("Chainsaw"))
		{
//			m_character_panel_widget.SwitchMove("menu_idleChainsaw0");
		}
		else if (inHands.IsKindOf("MeleeItemBase"))
		{
//			m_character_panel_widget.SwitchMove("menu_idleHatchet0");
		}	
		else
		{
			m_character_panel_widget.SetItemInHands(NULL);
//			m_character_panel_widget.SwitchMove("menu_idleUnarmed0");
		}
	}
	
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		super.OnFocusLost(w, x, y);
		
		m_isCharacterRotating = false;
		
		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		super.OnMouseEnter(w, x, y);
		
		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		super.OnMouseLeave(w, enterW, x, y);
		
		return false;
	}

	override bool OnSelect(Widget w, int x, int y)
	{
		super.OnSelect(w, x, y);
		
		return false;
	}
	
	override void Refresh()
	{
		Man player = g_Game.GetPlayer();
		if (m_target && GetGame().IsObjectAccesible(m_target, player) == false)
		{
			InitContainers(NULL);
			return;
		}

		UpdatePlayerPreviewAnim(player.GetHumanInventory().GetEntityInHands());
	}

	void RefreshQuickbar()
	{
		if ( m_quickbar )
		{
			m_quickbar.UpdateItems( m_quickbar_widget );
		}
	}
	
	void ShowQuickbar()
	{
		if ( m_quickbar_widget )
		{
			m_quickbar_widget.Show( true );
		}
	}

	void HideQuickbar()
	{
		if ( m_quickbar_widget )
		{
			m_quickbar_widget.Show( false );
		}
	}

	void HidePanelBorders()
	{
		if ( m_center_panel_border )
		{
			// m_center_panel_border.SetColor(0x00FFFFFF);
			m_left_panel_border.SetColor(0x00FFFFFF);
			m_right_panel_border.SetColor(0x00FFFFFF);
			m_center_panel_content.SetColor(0x00FFFFFF);
			m_center_panel.SetColor(0x00FFFFFF);
		}
	}

	void ShowHandsPanelBorder()
	{
		if ( m_center_panel )
		{
			m_center_panel.SetColor(0x3FFFFFFF);
		}
	}
	
	void ShowRightPanelBorder()
	{
		if ( m_right_panel_border )
		{
			m_right_panel_border.SetColor(0x3FFFFFFF);
		}
	}

	void ShowLeftPanelBorder()
	{
		if ( m_left_panel_border )
		{
			m_left_panel_border.SetColor(0x3FFFFFFF);
		}
	}

	void PerformRefreshInventory()
	{
		Man player = GetGame().GetPlayer();
		if( player == NULL)
		{
			return;
		}
		
		if (player.GetHumanInventory().CanOpenInventory() == false)
		{
			GetGame().GetUIManager().GetMenu().UnlockControls();
			GetGame().GetUIManager().HideMenu(MENU_INVENTORY);
			return;
		}

		if (m_target_container) 
		{
			bool reset_condition = false;
			
			if ( m_target )
			{
				if ( !GetGame().IsObjectAccesible(m_target, player) || !CanShowItemInInventory(PlayerBase.Cast( GetGame().GetPlayer() ), m_target) )
				{
					reset_condition = true;
				}
			}
			
			if ( reset_condition )
			{
				InitContainers(NULL);
			}
			else
			{
				m_target_container.UpdateItems();
			}
		}
		
		if (m_hands_container) m_hands_container.UpdateItems();
		if (m_ground_container) m_ground_container.UpdateItems();
		if (m_equipment_container) m_equipment_container.UpdateItems();
		

		
		Object obj;
		Cargo cargo;
		int i,c;
		InventoryContainerEntity container;
		InventoryContainerZombie zombie_container;

		// find objects near player
		vector pos = player.GetPosition();
		vector dir = player.GetDirection();
		ref array<Object> objects = new array<Object>;
		ref array<Cargo> proxyCargos = new array<Cargo>;
		GetGame().GetObjectsAtPosition(pos + (dir * 0.75), 1.0, objects, proxyCargos);

		// remove container objects which are not anymore in list of nearby objects
		c = m_ground_objects.Count();
		for ( i = 0; i < c; i++)
		{
			obj = m_ground_objects.Get(i);
			int find_index = objects.Find( obj );
			if ( find_index == -1 )
			{
				m_ground_objects.Remove( i );
				m_ground_objects_containers.Remove( i );
				c--;
				i--;
			}
			else
			{
				container = m_ground_objects_containers.Get(i);
				container.UpdateItems();
				objects.Remove(find_index);
			}
		}
		
		c = m_ground_zombie_objects.Count();
		for ( i = 0; i < c; i++)
		{
			obj = m_ground_zombie_objects.Get(i);
			int find_index2 = objects.Find( obj );
			if ( find_index2 == -1 )
			{
				m_ground_zombie_objects.Remove( i );
				m_ground_zombie_objects_containers.Remove( i );
				c--;
				i--;
			}
			else
			{
				zombie_container = m_ground_zombie_objects_containers.Get(i);
				zombie_container.UpdateItems();
				objects.Remove(find_index);
			}
		}

		// create containers for nearby objects
		c = objects.Count();
		for ( i = 0; i < c; i++)
		{
			obj = objects.Get(i);
			if ( CanShowItemInInventory(PlayerBase.Cast( player ), obj) )
			{
				if ( obj.IsInherited(PlayerBase) || obj.IsInherited(ZombieBase) )
				{
					int container_index = m_ground_zombie_objects.Find( obj );
					if ( container_index < 0 )
					{
						InventoryContainerZombie zombie_container2 = new InventoryContainerZombie( m_left_panel_content, false, true);//show_viewgrid );
						zombie_container2.Init(EntityAI.Cast( obj ));
						zombie_container2.SetHeaderText( obj.GetDisplayName() );
						zombie_container2.UpdateItems();
						
						m_ground_zombie_objects.Insert( obj );
						m_ground_zombie_objects_containers.Insert( zombie_container2 );
					}
				}
			}
		}
		
		RefreshQuickbar();
		RefreshPlayerStatus();

		Widget widget_under_cursor = GetWidgetUnderCursor();
		if ( widget_under_cursor == NULL )
		{
			m_drag_manager.HideTooltip();
		}
	}

	override ContextMenu GetContextMenu()
	{
		return m_context_menu;
	}

	Widget GetCenterPanel()
	{
		return m_center_panel;
	}

	// ScriptedWidgetEventHandler override	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		super.OnDraggingOver(w, x, y, reciever);
		
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		HidePanelBorders();

		if (dragged_item && !(player.GetHumanInventory().GetEntityInHands() != dragged_item || player.GetHumanInventory().CanRemoveEntityInHands()))
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
			return true;
		}
		else if( reciever.GetParent() == m_left_panel_content.GetChildren() && dragged_item )
		{
			if(dragged_item.GetHierarchyParent() == player)
			{
				if (player.GetInventory().CanAddEntityToInventory(dragged_item))
				{
					manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
				}
			}
			else
			{
				InventoryContainerZombie zombie_container;
				int c = m_ground_zombie_objects_containers.Count();
				for ( int i = 0; i < c; i++)
				{
					zombie_container = m_ground_zombie_objects_containers.Get(i);
					if(zombie_container.GetContentGridWidget() == reciever)
					{
						EntityAI ent = EntityAI.Cast( m_ground_zombie_objects.Get(i) );
						if(ent.GetInventory().CanAddEntityToInventory(dragged_item))
						{
							//Print(dragged_item.GetOwner());
							manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
						}
					}
				}
			}
		}
		else if( reciever.GetName() == "GridGhost" && dragged_item )
		{
			if (player.GetInventory().CanAddEntityToInventory(dragged_item))
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			}
		}
		else if (reciever == m_character_panel_area_widget && dragged_item)
		{
			if (player.GetInventory().CanAddEntityToInventory(dragged_item))
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			} 	
			else
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
			}
			ShowRightPanelBorder();
		}
		else if (reciever == m_left_panel.GetChildren() && dragged_item)
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			ShowLeftPanelBorder();
		}
		else if (reciever == m_left_panel_border && dragged_item)
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			ShowLeftPanelBorder();
		}
		else if (reciever == m_vicinity_panel && dragged_item)
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			ShowLeftPanelBorder();
		}
		else if (reciever == m_right_panel.GetChildren() && dragged_item)
		{
			if (player.GetInventory().CanAddEntityToInventory(dragged_item))
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			} 	
			else
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
			}
			ShowRightPanelBorder();
		}
		else if (reciever == m_right_panel_border && dragged_item)
		{
			if (player.GetInventory().CanAddEntityToInventory(dragged_item))
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			} 	
			else
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
			}
			ShowRightPanelBorder();
		}
		else
		{
			
			if ( reciever.GetName() == "HeaderBgHands" )
			{
				ShowHandsPanelBorder();
				return false;
			}
			else
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_DRAG);
			}
		}

		return true;	
	}

	override bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		super.OnDropReceived(w, x, y, reciever);
		
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );	

		if (dragged_item && (player.GetHumanInventory().GetEntityInHands() != dragged_item || player.GetHumanInventory().CanRemoveEntityInHands()))
		{
			if( reciever.GetName() == "GridGhost")
			{
				if (player.GetInventory().CanAddEntityToInventory(dragged_item))
				{
					player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, dragged_item);
				}
			}
			else if( reciever.GetParent() == m_left_panel_content.GetChildren() && dragged_item )
			{
				if(dragged_item.GetHierarchyParent() == player)
				{
					if( player.GetInventory().CanAddEntityToInventory( dragged_item ))
					{
						player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, dragged_item);
					}
				}
				else
				{
					InventoryContainerZombie zombie_container;
					int c = m_ground_zombie_objects_containers.Count();
					for ( int i = 0; i < c; i++)
					{
						zombie_container = m_ground_zombie_objects_containers.Get(i);
						if(zombie_container.GetContentGridWidget() == reciever)
						{
								EntityAI ent = EntityAI.Cast( m_ground_zombie_objects.Get(i) );
								if(	ent.GetInventory().CanAddEntityToInventory(dragged_item) )
								{
									player.PredictiveTakeEntityToTargetInventory(ent, FindInventoryLocationType.ANY, dragged_item);
								}
						}
					}
				}
			
			}
			else if (reciever == m_character_panel_area_widget && dragged_item)
			{
				if (player.GetInventory().CanAddEntityToInventory(dragged_item))
				{
					player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, dragged_item);
				}
			}
			else if (reciever == m_left_panel.GetChildren() && dragged_item)
			{
				manager.DropItem(dragged_item);
			}
			else if (reciever == m_left_panel_border && dragged_item)
			{
				manager.DropItem(dragged_item);
			}
			else if (reciever == m_vicinity_panel && dragged_item)
			{
				manager.DropItem(dragged_item);
			}
			else if (reciever == m_right_panel.GetChildren() && dragged_item)
			{
				if (player.GetInventory().CanAddEntityToInventory(dragged_item))
				{
					player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, dragged_item);
				}
			}
			else if (reciever == m_right_panel_border && dragged_item)
			{
				if (player.GetInventory().CanAddEntityToInventory(dragged_item))
				{
					player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, dragged_item);
				}
			}
		}

		HidePanelBorders();

		manager.Drop();
		return true;
	}

	// drag & drop stuff
	override bool OnDrag(Widget w, int x, int y)
	{
		super.OnDrag(w, x, y);
		
		return false;
	}

	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		super.OnDragging(w, x, y, reciever);
		
		return false;
	}

	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		super.OnDrop(w, x, y, reciever);
		
		return false;
	}
	
	// PLAYER STATUS
	private ref array<TextWidget> m_Player_Statuses;
	private ref array<ImageWidget> m_Player_StatusesBackground;

	// containers list switch history
	static bool s_ground_container_list_switch;
	static bool s_equipment_container_list_switch;
	static bool s_target_container_list_switch;
}
