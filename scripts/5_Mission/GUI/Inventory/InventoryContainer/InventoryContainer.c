bool CanShowItemInInventory(PlayerBase player, Object obj)
{
	if ( player == NULL || obj == NULL || player == obj )
	{
		return false;
	}
			
	if ( player.GetInventory().IsPlaceholderEntity(obj) )
	{
		return false; // noproxy: ignore body placeholder
	}
	
	// obj is NOT InventoryItem, Building, PlayerBase, ZombieBase, AnimalBase, Car
	if ( !obj.IsInherited(InventoryItem) && !obj.IsInherited(Building) && !obj.IsInherited(PlayerBase) && !obj.IsInherited(ZombieBase) && !obj.IsInherited(AnimalBase) && !obj.IsInherited(Car)  )
	{
		return false;
	}
	
	if (obj.GetParent())
		return false; // noproxy: ignore owned items
	
	// obj is PlantBase, DiggedSoil
	if ( obj.IsKindOf("PlantBase") || obj.IsKindOf("DiggedSoil") )
	{
		return false;
	}
	
	// Is player Restrained
	if ( obj.IsInherited(PlayerBase) )
	{
		PlayerBase obj_player = PlayerBase.Cast( obj );

		if ( obj_player.GetPlayerState() == EPlayerStates.ALIVE && !obj_player.IsRestrained() )
		{
			return false;
		}
	}
	
	// Check only for non TentBase objects if they have locked inventory
	if ( !obj.IsInherited(TentBase) && GetGame().IsInventoryTreeLocked(EntityAI.Cast( obj ), EntityAI.Cast( player ) ) )
	{
		return false;
	}
	
	if ( !GetGame().IsObjectAccesible(EntityAI.Cast( obj ), Man.Cast( player )) )
	{
		return false;
	}
	
 	return true;
}

class InventoryContainer: ScriptedWidgetEventHandler
{
	protected bool m_isOwnedByPlayer;
	protected bool m_isContentGrid;
	protected Widget m_root;
	protected Widget m_button;
	protected TextWidget m_header;
	protected Widget m_header_bar;
	protected Widget m_minimize_widget;
	protected Widget m_maximize_widget;
	protected Widget m_contentList;
	protected Widget m_contentGrid;
	protected AutoHeightSpacer m_spacer;
	
	void InventoryContainer(Widget parent, bool ownedByPlayer, bool isContentGrid = true )
	{
		m_isOwnedByPlayer = ownedByPlayer;
		m_isContentGrid = isContentGrid;
		m_root = CreateLayout(parent);
		m_root.GetScript(m_spacer);
		m_contentList = m_root.FindAnyWidget("ContentList");
		m_contentGrid = m_root.FindAnyWidget("ContentGrid");
		m_button = m_root.FindAnyWidget("ButtonFrame"); 
		m_header = TextWidget.Cast( m_root.FindAnyWidget("Header") ); 
		m_header_bar = m_root.FindAnyWidget("HeaderBar");
		m_minimize_widget = m_root.FindAnyWidget("MinimizeWidget");
		m_maximize_widget = m_root.FindAnyWidget("MaximizeWidget");

		if (m_contentGrid) m_contentGrid.Show(false);
		if (m_button) m_button.SetHandler(this);
		if (m_header) m_header.SetHandler(this);
		if (m_header_bar) m_header_bar.SetHandler(this);
		if (m_minimize_widget) m_minimize_widget.SetHandler(this);	
		if (m_maximize_widget) m_maximize_widget.SetHandler(this);

		if (m_spacer) m_spacer.Update();

		if ( m_minimize_widget ) m_minimize_widget.Show( false );
		if ( m_maximize_widget ) m_maximize_widget.Show( false );

		if ( IsHistoryExpanded() || m_contentList.IsVisible() || m_contentGrid.IsVisible() )
		{
			if ( m_minimize_widget )
			{
				m_minimize_widget.Show( true );
				m_maximize_widget.Show( false );
			}
			if (m_contentList) m_contentList.Show( true );
			if ( m_isContentGrid )
			{
				if (m_contentGrid) m_contentGrid.Show( true );
			}
		}
		else
		{
			if ( m_minimize_widget )
			{
				m_minimize_widget.Show( false );
				m_maximize_widget.Show( true );
			}
			if (m_contentList) m_contentList.Show( false );
			if (m_contentGrid) m_contentGrid.Show( false );
		}
	}
	
	void ~InventoryContainer()
	{
		SaveHistory();
		delete m_root;
	}

	Widget CreateLayout(Widget parent)
	{
		if (m_isOwnedByPlayer)
		{
			return GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryContainerRight.layout", parent);
		}
		else
		{
			return GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryContainer.layout", parent);
		}
	}

	bool IsOwnedByPlayer() {
		return m_isOwnedByPlayer;
	}	
	
	bool isContentGrid()
	{
		return m_isContentGrid;
	}
	
	void UpdateItems() {
	}
	
	Widget GetRootWidget() {
		return m_root;
	}
	
	Widget GetContentListWidget() {
		return m_contentList;
	}
	
	Widget GetContentGridWidget() {
		return m_contentGrid;
	}
	
	void SetHeaderText(string text)
	{
		if (m_header) m_header.SetText(text);
	}

	bool IsListView()
	{
		return false;
	}
	
	void OnItemDoubleClick(InventoryViewBase view, InventoryItem item)
	{
		Print("OnItemDoubleClick");
		InventoryManager manager = InventoryManager.GetInstance();
		Man player = GetGame().GetPlayer();
		
		if (m_isOwnedByPlayer)
		{
			manager.TakeToHands(item);
		}
		else
		{
			bool found = false;
			if( item.GetInventory().CanRemoveEntity())
			{
				InventoryLocation il = new InventoryLocation;
				found = player.GetInventory().FindFreeLocationFor(item,FindInventoryLocationType.ANY,il);
				if (found)
				{
					if ( il.GetType() == FindInventoryLocationType.ATTACHMENT )
					{
						if ( il.GetParent() != player )
							found = false;
					}
				}
			}
			if ( found )
			{
				player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, item);
			}
			else
			{
				manager.TakeToHands(item);
			}
		}

		InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().FindMenu(MENU_INVENTORY) );
		if ( menu )
		{
			menu.RefreshQuickbar();
		}
	}
	
	void Ref()
	{
		
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if ( w == m_header_bar || w == m_header  || w == m_minimize_widget || w == m_maximize_widget )
		{
			if ( m_contentList.IsVisible() || m_contentGrid.IsVisible() )
			{
				m_contentList.Show( false );
				m_contentGrid.Show( false );
				m_minimize_widget.Show( false );
				m_maximize_widget.Show( true );
			}	
			else
			{
				m_contentList.Show( true );
				if ( m_isContentGrid )
				{
					m_contentGrid.Show( true );
				}
				
				m_minimize_widget.Show( true );
				m_maximize_widget.Show( false );
			}
			Widget container_root = m_root;
			AutoHeightSpacer spacer;
			container_root.GetScript(spacer);
			spacer.Update();
		}
		return true;
	}
	
	// history saving
	protected static const int EXPANDED_HISTORY_SIZE = 20;
	protected static int s_expanded_history[EXPANDED_HISTORY_SIZE];
	protected static int s_expanded_history_index = 0;

	bool IsHistoryExpanded()
	{
		// if (m_entity.IsNull()) return false;
		
		// int id = m_entity.Ptr().GetID();
		
		// for (int i = 0; i < EXPANDED_HISTORY_SIZE; i++)
		// {
		// 	if (s_expanded_history[i] == id) 
		// 	{
		// 		return true;
		// 	}
		// }
		
		// return false;
	}

	void ClearHistory()
	{
		// for (int i = 0; i < EXPANDED_HISTORY_SIZE; i++)
		// {
		// 	s_expanded_history[i] = -1;
		// }
		// s_expanded_history_index = 0;
	}

	void SaveHistory()
	{
		// if (m_entity.IsNull()) return;
		
		// if  ( m_body && m_body_spacer && m_content_spacer )
		// {
		// 	if (m_body_spacer.MaxHeight == 0)
		// 	{
		// 		s_expanded_history[s_expanded_history_index] = m_entity.Ptr().GetID();
		// 		s_expanded_history_index++;
		// 	}		
		// }
	}


	void ToggleExpandView( EntityAI entity )
	{
	}
	
	bool HideExpandView( EntityAI entity )
	{	
	}
	
	void ShowExpandView( EntityAI entity )
	{
	}
}
