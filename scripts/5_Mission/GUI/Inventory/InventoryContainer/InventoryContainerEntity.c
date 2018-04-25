class InventoryContainerEntity: InventoryContainer
{
	protected ref array<EntityAI> m_entities;
	protected ref InventoryViewBase m_cargo_view;
	protected ref map<EntityAI, InventoryViewBase> m_view_grids;	// inventory grids
	protected ref map<EntityAI, ref InventoryViewBase> m_views;			// cargo views
	protected EntityAI m_entity;
	protected ref map<int, Widget> m_inventory_slots_ghost;

	protected ref map<int, ref InventoryViewGridBase> m_inventory_slots;  	//slot_id, grid
	protected ref map<int, bool> m_inventory_slots_state;				//slot_id, state
	protected ref map<EntityAI, int> m_inventory_entities;  			//entity, slot_id

	void InventoryContainerEntity(Widget parent, bool ownedByPlayer, bool isContentGrid = true )
	{
		m_views = new map<EntityAI, ref InventoryViewBase>;
		m_view_grids = new map<EntityAI, InventoryViewBase>;
		m_entities = new array<EntityAI>;
		m_inventory_slots_ghost = new map<int, Widget>;
		m_inventory_slots = new map<int, ref InventoryViewGridBase>;
		m_inventory_slots_state = new map<int, bool>;
		m_inventory_entities = new map<EntityAI, int>;
	}

	void Init(EntityAI entity)
	{
		int i,x,c;
		m_entity = entity;

		// get player's inventory slots
		string config_path_ghosts_slots = "CfgVehicles SurvivorBase InventoryEquipment playerSlots";
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
		for ( i = 0; i < player_ghosts_slots.Count(); i++ )
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
				m_inventory_slots.Set( slot_id, view );
				m_inventory_slots_state.Set( slot_id, false );
			}
		}

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

		InventoryViewGridContainer view_grid = InventoryViewGridContainer.Cast( m_inventory_slots.Get( entity.GetInventory().GetSlotId() ) );
		if ( view_grid )
		{
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
					Widget ghost_icon;
					view_grid = InventoryViewGridContainer.Cast( m_inventory_slots.Get(attachment_slot_id) );
					bool slot_state = m_inventory_slots_state.Get( attachment_slot_id );
					if ( slot_state == false )  // neni zobrazeny tak ho zobraz
					{ 
						view_grid.GetContainerIconGrid().SetItem( InventoryItem.Cast( entity ),  Vector(0, 1, 1), Vector(0,0,0) );
						view_grid.SetEntity( entity );

						ghost_icon = view_grid.GetContainerIcon().FindAnyWidget("GhostIcon");
						ghost_icon.Show(false);

						m_inventory_slots_state.Set( attachment_slot_id, true );

						// expand icon
						if ( entity.GetInventory().GetCargo() || entity.GetSlotsCountCorrect() )
						{
							Widget expand_icons = view_grid.GetContainerIcon().FindAnyWidget("ExpandIcons");
							if ( expand_icons )
							{
								expand_icons.Show( true );
								Widget expand_icon = view_grid.GetContainerIcon().FindAnyWidget("ExpandIcon");
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

					ghost_icon = view_grid.GetContainerIcon().FindAnyWidget("GhostIcon");
					ghost_icon.Show( true );

					m_inventory_slots_state.Set( attachment_slot_id, false );
					m_inventory_entities.RemoveElement(i);

					expand_icons = view_grid.GetContainerIcon().FindAnyWidget("ExpandIcons");
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

		
		return;
		
		/*
		// remove hidden view grids
		// TODO: iba hide/show -> netreba znovu vytvarat
		Widget root_widget;
		c = m_views.Count();
		for (i = 0; i < c; i++)
		{
			view = m_views.GetElement(i);
			root_widget = view.GetRootWidget();
			if ( root_widget && !root_widget.IsVisible() )
			{
				delete m_views.GetElement(i);
				m_views.RemoveElement(i);
				c--;
				i--;
			}
			change = true;
		}

		// update all
		c = m_view_grids.Count();
		for (i = 0; i < c; i++) 
		{
			m_view_grids.GetElement( i ).UpdateItems();
		}
		*/
		

		// SORT -> presunut do initu
		/*
		if ( added_item || true )
		{
			ref array<EntityAI> unsorted_objects = new array<EntityAI>;
			ref array<Param2<EntityAI, Widget>> sorted_objects = new array<Param2<EntityAI, Widget>>;
			array <int> sorted_keys = manager.GetSortedKeys();

			// create table with unsorted objects
			c = m_view_grids.Count();
			for (i = 0; i < c; i++) 
			{
				unsorted_objects.Insert(m_view_grids.GetKey(i));
			}

			for ( i = 0; i < sorted_keys.Count(); i++ )
			{
				int sort_key = sorted_keys.Get(i);
				bool is_found = false;
				for (x = 0; x < unsorted_objects.Count(); x++)  // prejdi vsetky item preview objects
				{
					entity = unsorted_objects.Get(x);
					int sort_id = entity.GetSlotId();
					if ( sort_key == sort_id )
					{
						sorted_objects.Insert(new Param2<EntityAI, Widget>>(entity, NULL));
						is_found = true;
						break;
					}
				}
				
				if ( is_found == false )  // nenasiel attachment, zistime ci existuje ghost, ak ano, tak pridajme ghost
				{
					if ( m_inventory_slots_ghost.Contains( sort_key ) )
					{
						Widget ghost = m_inventory_slots_ghost.Get( sort_key );
						sorted_objects.Insert(new Param2<EntityAI, Widget>>(NULL,ghost));
						ghost.Show( true );
					}
				}
			}
			
			c = sorted_objects.Count();
			for (i = 0; i < c; i++) 
			{
				if ( sorted_objects.Get(i).param1 )
				{
					entity = sorted_objects.Get(i).param1;
					m_view_grids.Get( entity ).UpdateSort();	
				}
				else
				{
					Widget ghost_widget = sorted_objects.Get(i).param2;
					GetContentGridWidget().RemoveChild(ghost_widget);
					GetContentGridWidget().AddChild(ghost_widget);
					ghost_widget.SetSize( manager.GetAttachmentItemSize(), manager.GetAttachmentItemSize() );
				}
			}

			m_contentGrid.Update();
		}
		*/

		if (m_cargo_view)
		{
			m_cargo_view.UpdateItems();
		}
	}
	
	void UpdateViews()
	{
		int c = m_views.Count();
		for (int i = 0; i < c; i++) 
		{
			m_views.GetElement( i ).UpdateItems();
		}
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
	
		// first with cargo -----------------------
		/*
		if ( m_isOwnedByPlayer ) // we dont want to show views for zombie's cargo
		{
			// add new entities from attachments
			c = m_entity.Ptr().GetInventory().AttachmentCount();
			for (i = 0; i < c; i++)
			{
				entity = m_entity.Ptr().GetAttachmentFromIndex(i);

				if (entity && entity.IsInventoryItem() && entity.GetInventory().GetCargo())
				{
					if (m_views.Contains(entity) == false)
					{	
						view = new InventoryView(this, entity, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);
						added_item = true;
						view.CreateLayout();
						m_views.Set( entity, view );
					}
					m_entities.Insert(entity);	
				}
			}
			
			// add new entities from cargo
			if (m_entity.Ptr().GetInventory().GetCargo())
			{
				cargo = m_entity.Ptr().GetInventory().GetCargo();
				c = cargo.GetItemCount();
				
				for (i = 0; i < c; i++)
				{
					entity = cargo.GetItem(i);

					if (entity && entity.IsInventoryItem() && entity.GetInventory().GetCargo())
					{
						if (m_views.Contains(entity) == false)
						{	
							view = new InventoryView(this, entity, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);
							added_item = true;
							view.CreateLayout();
							m_views.Set(entity, view );
						}
						m_entities.Insert(entity);
					}
				}
			}
		}

		// then without cargo -----------------------
		
		// add new entities from attachments
		c = m_entity.Ptr().GetInventory().AttachmentCount();
		for (i = 0; i < c; i++)
		{
			entity = m_entity.Ptr().GetAttachmentFromIndex(i);

			if (entity && entity.IsInventoryItem() )
			{
				if (m_views.Contains(entity) == false)
				{	
					if ( entity.GetInventory().GetCargo() == NULL && entity.GetSlotsCountCorrect() < 1)
					{
						view = new InventoryViewGridContainer(this, entity, 0 );
						added_item = true;
					}
					else
					{
						view = new InventoryView(this, entity, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);
						added_item = true;
					}
					view.CreateLayout();
					m_views.Set(entity, view );	
				}
				m_entities.Insert(entity);
			}
		}
		
		// add new entities from cargo
		if (m_entity.Ptr().GetInventory().GetCargo())
		{
			cargo = m_entity.Ptr().GetInventory().GetCargo();
			c = cargo.GetItemCount();
			
			for (i = 0; i < c; i++)
			{
				entity = cargo.GetItem(i);

				if (entity && entity.IsInventoryItem() && !entity.GetInventory().GetCargo())
				{
					if (m_views.Contains(entity) == false)
					{			
						view = new InventoryViewGridContainer(this, entity, 0 );
						added_item = true;
						view.CreateLayout();
						m_views.Set(entity, view );
					}
					m_entities.Insert(entity);
				}
			}
		}
		
		// remove unnecesary entities
		c = m_views.Count();
		for (i = 0; i < c; i++)
		{
			entity = m_views.GetKey(i);
			if (m_entities.Find(entity) == INDEX_NOT_FOUND)
			{
				delete m_views.GetElement(i);
				m_views.RemoveElement(i);
				c--;
				i--;
				change = true;
			}
		}

		// update all
		c = m_views.Count();
		for (i = 0; i < c; i++) 
		{
			m_views.GetElement( i ).UpdateItems();
		}

		// SORT
		if ( added_item )
		{
			ref array<EntityAI> unsorted_objects = new array<EntityAI>;
			ref array<EntityAI> sorted_objects = new array<EntityAI>;
			array <int> sorted_keys = manager.GetSortedKeys();

			c = m_views.Count();
			for (i = 0; i < c; i++) 
			{
				unsorted_objects.Insert(m_views.GetKey(i));
			}

			for ( i = 0; i < sorted_keys.Count(); i++ )
			{
				int sort_key = sorted_keys.Get(i);
				for (x = 0; x < unsorted_objects.Count(); x++)
				{
					entity = unsorted_objects.Get(x);
					int sort_id = entity.GetSlotId();
					if ( sort_key == sort_id )
					{
						sorted_objects.Insert( entity );
						unsorted_objects.Remove(x);
						break;
					}
				}
			}

			for (x = 0; x < unsorted_objects.Count(); x++)
			{
				sorted_objects.Insert( unsorted_objects.Get(x) );
			}

			c = sorted_objects.Count();
			for (i = 0; i < c; i++) 
			{
				entity = sorted_objects.Get(i);
				m_views.Get( entity ).UpdateSort();
			}		
		}
		
		if (change)
		{
			m_contentGrid.Update();
		}
		
		if (m_cargo_view)
		{
			m_cargo_view.UpdateItems();
		}
		*/
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

	override bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		if (m_entity == NULL) return false;
		
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if (manager.IsDragging() == false || dragged_item == NULL) return false;
		
		if (dragged_item != m_entity && m_entity.GetInventory().CanAddEntityToInventory(dragged_item) ) 
		{
			GetGame().GetPlayer().PredictiveTakeEntityToTargetInventory( m_entity, FindInventoryLocationType.ANY, dragged_item);
		}
		
		manager.Drop();
		return true;
	}
}
