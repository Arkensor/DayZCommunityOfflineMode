class InventoryContainerZombie: InventoryContainer
{
	protected ref array<EntityAI> m_entities;
	protected ref InventoryViewBase m_cargo_view;
	protected ref map<EntityAI, InventoryViewBase> m_view_grids;	// inventory grids
	protected ref map<EntityAI, ref InventoryViewBase> m_views;			// cargo views
	protected EntityAI m_entity;
	protected ref map<int, Widget> m_inventory_slots_ghost;

	protected ref map<int, InventoryViewGridBase> m_inventory_slots;  	//slot_id, grid
	protected ref map<int, bool> m_inventory_slots_state;				//slot_id, state
	protected ref map<EntityAI, int> m_inventory_entities;  			//entity, slot_id

	void InventoryContainerZombie(Widget parent, bool ownedByPlayer, bool isContentGrid = true )
	{
		m_views = new map<EntityAI, ref InventoryViewBase>;
		m_view_grids = new map<EntityAI, InventoryViewBase>;
		m_entity = NULL;
		m_entities = new array<EntityAI>;
		m_inventory_slots_ghost = new map<int, Widget>;
		m_inventory_slots = new map<int, InventoryViewGridBase>;
		m_inventory_slots_state = new map<int, bool>;
		m_inventory_entities = new map<EntityAI, int>;
	}

	void Init(EntityAI entity)
	{
		int i,x,c;
		m_entity = entity;

		// get player's inventory slots
		string config_path_ghosts_slots = "CfgVehicles ZombieBase InventoryEquipment playerSlots";
		ref array<string> player_ghosts_slots = new array<string>;
		
		GetGame().ConfigGetTextArray ( config_path_ghosts_slots, player_ghosts_slots );
		
		// get array of all slots
		string config_path_slots = "CfgSlots";
		ref array<int> config_slots	= new array<int>;
		GetGame().ConfigGetIntArray ( config_path_slots, config_slots );

		// add all ghost icons into content grid
		InventoryViewGridBase view;
		Widget ghost_widget;
		InventoryManager manager = InventoryManager.GetInstance();
		for ( i = 0; i < 7; i++ )
		{
			string slot_name = player_ghosts_slots.Get ( i );
			string path = "CfgSlots" + " " + slot_name;
			
			if ( GetGame().ConfigIsExisting( path ) )
			{
				string icon_name;
				GetGame().ConfigGetText(path + " ghostIcon", icon_name);
	
				view = new InventoryViewGridContainer(this, NULL, 0 );
				view.SetOwnerEntity(m_entity);
				view.CreateLayout();
				view.GetGridGhost().Show(true);
				view.GetGridGhost().SetSize( manager.GetAttachmentItemSize(), manager.GetAttachmentItemSize() );
				view.SetGhostIcon(icon_name);

				GetGame().ConfigGetText(path + " name", slot_name);
				int slot_id = InventorySlots.GetSlotIdFromString( slot_name );

				if( slot_name == "Cargo" )
				{
							view.SetAttachmentCategory( "Cargo", entity );
							ImageWidget expand_icons = ImageWidget.Cast( view.GetContainerIcon().FindAnyWidget("ExpandIcons2") );
							if ( expand_icons )
							{
								expand_icons.Show( true );
								ImageWidget expand_icon = ImageWidget.Cast( view.GetContainerIcon().FindAnyWidget("ExpandIcon2") );
								if ( expand_icon )
								{
									expand_icon.Show( true );
								}
							}
				}
				m_inventory_slots.Set( slot_id, view );
				m_inventory_slots_state.Set( slot_id, false );
			}
		}
		
		ShowExpandView( entity );

		// call update to have all functionality performed on init (othervise it's performed on update - there's visual lag)
		m_contentGrid.Update();
		UpdateItemsNewInventory();
	}
	
	override bool HideExpandView( EntityAI entity )
	{
			InventoryViewBase view = m_views.Get( entity );
			if( !view ) { return false; }
			
			SetExpandWidgetsState(  entity, view, false );
			return true;
	}
	
	override void ShowExpandView( EntityAI entity )
	{
		InventoryViewBase view = new InventoryView( this, entity, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS );
		view.CreateLayout();
		m_views.Set( entity, view );
		SetExpandWidgetsState( entity, view, true );
	}
	
	void SetExpandWidgetsState( EntityAI entity, InventoryViewBase view, bool show )
	{
		ImageWidget expand_icon;
		ImageWidget collapse_icon;
		int slot_id = entity.GetInventory().GetSlotId();
		if( entity.IsInherited( ZombieBase ) )
		{
			slot_id = InventorySlots.GetSlotIdFromString( "Cargo" );
		}
		
		InventoryViewGridContainer view_grid = InventoryViewGridContainer.Cast( m_inventory_slots.Get( slot_id ) );
		if ( view_grid )
		{
			ImageWidget expand_icons = ImageWidget.Cast( view_grid.GetContainerIcon().FindAnyWidget("ExpandIcons") );
			Widget container_icon = view_grid.GetContainerIcon();
			if ( container_icon )
			{
				expand_icon = ImageWidget.Cast( container_icon.FindAnyWidget( "ExpandIcon" ) );
				collapse_icon = ImageWidget.Cast( container_icon.FindAnyWidget( "CollapseIcon" ) );
			}
		}
		
		Widget root_widget = view.GetRootWidget();
		if ( root_widget )
		{
				root_widget.Show( show );	
		}

		if ( expand_icon )
		{
			expand_icon.Show( !show );
			collapse_icon.Show( show );
		}
		
		if( !show )
		{
			m_views.Remove( entity );
		}
		
		m_spacer.Update();
		AutoHeightSpacer contentListSpacer;
		m_contentList.GetScript( contentListSpacer );
		contentListSpacer.Update();
	}
	
	override void Ref()
	{
		m_spacer.Update();
		AutoHeightSpacer contentListSpacer;
		m_contentList.GetScript( contentListSpacer );
		contentListSpacer.Update();
	}
	
	override void ToggleExpandView( EntityAI entity )
	{		
		// if entity is in m_views then it's visible, so let's hide it
		if ( !HideExpandView( entity ) )
		{
			ShowExpandView( entity ); //othervise let's create/show view
		} 
	}
	
	void UpdateItemsNewInventory()
	{
		EntityAI entityCargo = ;
		EntityAI entity;
		Cargo cargo;
		int i, j, c, x;
		bool change = false;
		bool added_item = false;
		InventoryViewBase view;

		InventoryManager manager = InventoryManager.GetInstance();

		m_entities.Clear();
		c = m_entity.GetInventory().AttachmentCount();

		// iterate through all player's attachments
		for (i = 0; i < c; i++)
		{
			entity = m_entity.GetInventory().GetAttachmentFromIndex(i);
			if ( entity )
			{
				int attachment_slot_id = entity.GetInventory().GetSlotId();
				bool is_valid_slot = m_inventory_slots_state.Contains( attachment_slot_id );
				if ( is_valid_slot )
				{ 
					InventoryViewGridContainer view_grid;
					ImageWidget ghost_icon;
					view_grid = InventoryViewGridContainer.Cast( m_inventory_slots.Get(attachment_slot_id) );
					bool slot_state = m_inventory_slots_state.Get( attachment_slot_id );
					if ( slot_state == false )  // neni zobrazeny tak ho zobraz
					{ 
						view_grid.GetContainerIconGrid().SetItem( InventoryItem.Cast( entity ),  Vector(0, 1, 1), Vector(0,0,0) );
						view_grid.SetEntity( entity );

						ghost_icon = ImageWidget.Cast( view_grid.GetContainerIcon().FindAnyWidget("GhostIcon") );
						ghost_icon.Show(false);

						m_inventory_slots_state.Set( attachment_slot_id, true );

						// expand icon
						if ( entity.GetInventory().GetCargo() || entity.GetSlotsCountCorrect() )
						{
							ImageWidget expand_icons = ImageWidget.Cast( view_grid.GetContainerIcon().FindAnyWidget("ExpandIcons") );
							if ( expand_icons )
							{
								expand_icons.Show( true );
								ImageWidget expand_icon = ImageWidget.Cast( view_grid.GetContainerIcon().FindAnyWidget("ExpandIcon") );
								if ( expand_icon )
								{
									expand_icon.Show( true );
								}
							}
						}
					}
					m_inventory_entities.Set(entity, attachment_slot_id );
					m_entities.Insert(entity);
				}
			}
		}

		// ak su nejake zobrazene ale uz ich nema hrac u seba -> tak zmazeme
		c = m_inventory_entities.Count();
		for (i = 0; i < c; i++)
		{
			entity = m_inventory_entities.GetKey(i);
			if ( entity )
			{
				attachment_slot_id = entity.GetInventory().GetSlotId();
				slot_state = m_inventory_slots_state.Get(attachment_slot_id);
				bool is = false;
				if ( m_entities.Find(entity) == INDEX_NOT_FOUND ) is = true;

				if (slot_state && m_entities.Find(entity) == INDEX_NOT_FOUND)
				{
					view_grid = InventoryViewGridContainer.Cast( m_inventory_slots.Get(attachment_slot_id) );
					view_grid.RemoveEntity();

					view_grid.GetContainerIconGrid().SetItem( NULL,  Vector(0, 1, 1), Vector(0,0,0) );

					ghost_icon = ImageWidget.Cast( view_grid.GetContainerIcon().FindAnyWidget("GhostIcon") );
					ghost_icon.Show( true );

					m_inventory_slots_state.Set( attachment_slot_id, false );
					m_inventory_entities.RemoveElement(i);

					expand_icons = ImageWidget.Cast( view_grid.GetContainerIcon().FindAnyWidget("ExpandIcons") );
					if ( expand_icons )
					{
						expand_icons.Show( false );
					}
	
					c--;
					i--;
					change = true;
				}
			}
		}

		c = m_views.Count();
		for (i = 0; i < c; i++) 
		{
			m_views.GetElement( i ).UpdateItems();
		}
		
		return;
	}
	
	override void UpdateItems()
	{
		if (m_entity == NULL) return;
		
		EntityAI entity;
		Cargo cargo;
		int i, j, c, x;
		bool change = false;
		bool added_item = false;

		m_entities.Clear();
		InventoryViewBase view ;

		InventoryManager manager = InventoryManager.GetInstance();

		UpdateItemsNewInventory();
		return;
		
	}
	
	// ScriptedWidgetEventHandler override	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if (m_entity == NULL || dragged_item == NULL) return false;
		
		if (manager.IsDragging() == false) return false;
		
		if (dragged_item && dragged_item.GetInventory().CanRemoveEntity() == false)
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
		}
		else if (dragged_item == m_entity)
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_DRAG);
		} 	
		else if ( m_entity.GetInventory().CanAddEntityToInventory(dragged_item) ) 
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
		}
		else
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
		}
		return true;
	}
	
	void CargoClick()
	{
		ToggleExpandView(m_entity);
		ImageWidget expand_icon;
		ImageWidget collapse_icon;
		InventoryViewGridContainer view_grid = InventoryViewGridContainer.Cast( m_inventory_slots.Get( 15 ) );
		if ( view_grid )
		{
			ImageWidget expand_icons = ImageWidget.Cast( view_grid.GetContainerIcon().FindAnyWidget("ExpandIcons") );
			Widget container_icon = view_grid.GetContainerIcon();
			if ( container_icon )
			{
				expand_icon = ImageWidget.Cast( container_icon.FindAnyWidget( "ExpandIcon" ) );
				collapse_icon = ImageWidget.Cast( container_icon.FindAnyWidget( "CollapseIcon" ) );
			}
		}
	}
	
	override bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		if (m_entity == NULL) return false;
		
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if (manager.IsDragging() == false || dragged_item == NULL) return false;
		
		if (dragged_item != m_entity && m_entity.GetInventory().CanAddEntityToInventory(dragged_item) ) 
		{
			GetGame().GetPlayer().PredictiveTakeEntityToTargetInventory(m_entity, FindInventoryLocationType.ANY, dragged_item);
		}
		
		manager.Drop();
		return true;
	}
}
