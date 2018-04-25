class VicinityContainer: CollapsibleContainer
{
	ref VicinityIconsContainer m_VicinityIconsContainer;
	ref map<EntityAI, ref Container> m_ShowedItems = new ref map<EntityAI, ref Container>;
	ref map<int, ref Container> m_ShowedItemsIDs = new ref map<int, ref Container>;
	ref array<Object> m_ShowedItemIcons = new array<Object>;

	void VicinityContainer( ContainerBase parent )
	{
		m_VicinityIconsContainer = new VicinityIconsContainer( this );
		m_Body.Insert( m_VicinityIconsContainer );
		RecomputeOpenedContainers();
		Header h = Header.Cast( m_Body.Get(0) );
		h.SetName("VICINITY");
		LoadDefaultState();
	}
	
	override void RefreshItemPosition( EntityAI item_to_refresh )
	{
		for ( int i = 0; i < m_ShowedItems.Count(); i++ )
		{
			Container container = m_ShowedItems.GetElement( i );
			container.RefreshItemPosition( item_to_refresh );
		}
	}
	
	override void RefreshQuantity( EntityAI item_to_refresh )
	{
		for ( int i = 0; i < m_ShowedItems.Count(); i++)
		{
			m_ShowedItems.GetElement( i ).RefreshQuantity( item_to_refresh );
		}
	}

	override void Select()
	{
		if( m_FocusedContainer.IsInherited( ItemWithCargo ) )
		{
			ItemWithCargo iwc = ItemWithCargo.Cast( m_FocusedContainer );
			if( iwc )
			{
				iwc.Select();
			}
		}
		else
		{
			m_VicinityIconsContainer.Select( m_FocusedColumn );
		}
	}
	
	override void EquipItem( )
	{
		if( m_FocusedContainer.IsInherited( ItemWithCargo ) )
		{
			ItemWithCargo iwc = ItemWithCargo.Cast( m_FocusedContainer );
			if( iwc )
			{
				iwc.EquipItem();
			}
		}
		else
		{
			m_VicinityIconsContainer.EquipItem( m_FocusedColumn );
		}
	}
	
	override void TransferItem( )
	{
		if( m_FocusedContainer.IsInherited( ItemWithCargo ) )
		{
			ItemWithCargo iwc = ItemWithCargo.Cast( m_FocusedContainer );
			if( iwc )
			{
				iwc.TransferItem();
			}
		}
		else
		{
			m_VicinityIconsContainer.TransferItem( m_FocusedColumn );
		}
	}
	
	override void Combine( )
	{
		if( m_FocusedContainer.IsInherited( ItemWithCargo ) )
		{
			ItemWithCargo iwc = ItemWithCargo.Cast( m_FocusedContainer );
			if( iwc )
			{
				iwc.Combine();
			}
		}
		else
		{
			m_VicinityIconsContainer.Combine( m_FocusedColumn );
		}
	}

	override void MoveGridCursor( int direction )
	{
		if( !m_FocusedContainer || !m_FocusedContainer.IsActive() )
		{
			return;
		}
		if( m_FocusedContainer && (m_FocusedContainer.IsInherited( ItemWithCargo ) || m_FocusedContainer.IsInherited( ItemWithCargoAndAttachments )) )
		{
			ItemWithCargo iwc = ItemWithCargo.Cast( m_FocusedContainer );
			ItemWithCargoAndAttachments iwca = ItemWithCargoAndAttachments.Cast( m_FocusedContainer );
			if( iwc )
			{
				iwc.MoveGridCursor(direction);
			}
			else if ( iwca )
			{
				iwca.MoveGridCursor( direction );
			}
		}
		else
		{
			m_FocusedContainer.UnfocusAll();

			if( direction == Direction.RIGHT )
			{
				m_FocusedColumn++;
				if( m_FocusedColumn == ITEMS_IN_ROW )
				m_FocusedColumn = 0;
			}
			else if( direction == Direction.LEFT )
			{
				m_FocusedColumn--;
				if( m_FocusedColumn < 0 )
				m_FocusedColumn = ITEMS_IN_ROW - 1;
			}
			else if( direction == Direction.UP )
			{
				m_FocusedRow--;
				if( m_FocusedRow < 0 )
				{
					m_FocusedRow = 0 ;
					LeftArea left_area = LeftArea.Cast( GetParent() );
					left_area.SetPreviousActive();
					return;
				}				
			}
			else if( direction == Direction.DOWN )
			{
				m_FocusedRow++;
				if( m_FocusedRow == m_VicinityIconsContainer.Count() )
				{
					m_FocusedRow = 0 ;
					left_area = LeftArea.Cast( GetParent() );
					left_area.SetNextActive();
					return;
				}				
			}
			
			m_VicinityIconsContainer.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Cursor" + m_FocusedColumn ).Show( true );
		}
	}


	/*bool IsActive()
	{
		for ( int i = 1; i < m_Body.Count(); i++ )
		{
			Container cont = m_Body[i];
			if( cont.IsActive() )
			{
				return true;
			}
		}
		return false;
	}

	void SetActive( bool active )
	{
		if( active )
		{
			m_VicinityIconsContainer.SetActive( true );
			activeIndex = 1;
		}
		else
		{
			for ( int i = 1; i < m_Body.Count(); i++ )
			{
				Container cont = m_Body[i];
				if( cont.IsActive() )
				{
					cont.SetActive( false );
					activeIndex = 1;
				}
			}
		}
	}*/

	/*int activeIndex = 1;
	void SetNextActive()
	{
		Container active = m_Body[activeIndex];
		if( !active.IsActive() )
		{
			return;
		}
		active.SetActive( false );
		Container next;
		if( ++activeIndex < m_Body.Count() )
		{
			next = m_Body[activeIndex];
		}
		while( next && !next.GetMainPanel().IsVisible() )
		{
			if( ++activeIndex < m_Body.Count() )
			{
				next = m_Body[activeIndex];
			}
		}
		if( next )
		{
			next.SetActive( true );
		}
		else
		{
			activeIndex = 1;
			Container first = m_Body[activeIndex];
			first.SetActive( true );
		}
	}

	void SetPreviousActive()
	{
		Container active = m_Body[activeIndex];
		active.SetActive( false );
		Container prev;
		if( --activeIndex > 0 )
		{
			prev = m_Body[activeIndex];
		}
		while( prev && !prev.GetMainPanel().IsVisible() )
		{
			if( --activeIndex > 0 )
			{
				prev = m_Body[activeIndex];
			}
		}
		if(prev)
		{
			prev.SetActive( true );
		}
		else
		{
			activeIndex = m_Body.Count() - 1;
			Container first = m_Body[activeIndex];
			first.SetActive( true );
		}
	}

	*/
	/*void LoadDefaultState()
	{
		m_Hidden = !ItemManager.GetInstance().GetDefaultHeaderOpenState( "VICINITY" );

		if( m_Hidden )
		{
			OnHide();
			Print(GetMainPanel().GetName());
			/*for ( int i = 1; i < m_Body.Count(); i++ )
			{
				m_Body.Get( i ).OnHide();
			}*/
	/*	}
		else
		{
			OnShow();
			/*for ( i = 1; i < m_Body.Count(); i++ )
			{
				m_Body.Get( i ).OnShow();
			}*/
		/*}

		//GetMainPanel().FindAnyWidget("opened").Show(!m_Hidden);
		//GetMainPanel().FindAnyWidget("closed").Show(m_Hidden);
	}*/

	void TraverseShowedItems()
	{
		map<string, bool> serialized_types = new map<string, bool>();
		for ( int i = 0; i < m_ShowedItems.Count(); i++ )
		{
			EntityAI entity = m_ShowedItems.GetKey( i );
			Container container = m_ShowedItems.GetElement( i );
			string type = entity.GetType();
			if( container.IsInherited( ItemWithCargo ) )
			{
				if( !serialized_types.Contains( type ) )
				{
					ItemWithCargo item_with_cargo = ItemWithCargo.Cast( container );
					serialized_types.Insert( type, item_with_cargo.IsOpened() );
				}
			}
		}

		if( serialized_types.Count() > 0 )
		{
			FileSerializer file = new FileSerializer();
			if ( file.Open( "inventory_state.save", FileMode.APPEND ) )
			{
				file.Write( serialized_types.Count() );

				for ( i = 0; i < serialized_types.Count(); i++ )
				{
					file.Write( serialized_types.GetKey( i ) );
					file.Write( serialized_types.GetElement( i ) );
				}
			}
			file.Close();
		}
	}

	override void DraggingOverHeader( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !ipw )
		{
		  string name = w.GetName();
		  name.Replace( "PanelWidget", "Render" );
		  ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		if( !ipw )
		{
		  ipw = ItemPreviewWidget.Cast( w );
		}
		if( !ipw || !ipw.IsInherited( ItemPreviewWidget ) || !ipw.GetItem() )
		{
			return;
		}

		if( GetGame().GetPlayer().CanDropEntity( ipw.GetItem() ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		}
		else
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			ItemManager.GetInstance().ShowSourceDropzone( ipw.GetItem() );
		}
	}

	void DraggingOverIcon( Widget w, int x, int y, Widget receiver )
	{
		string name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		EntityAI receiver_item = receiver_iw.GetItem();

		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !ipw )
		{
			name = w.GetName();
			name.Replace( "PanelWidget", "Render" );
			ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		if( !ipw )
		{
			ipw = ItemPreviewWidget.Cast( w );
		}

		if( !ipw || !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return;
		}

		if( !receiver_item.IsInherited( ItemBase ) || !ipw.GetItem() )
		{
			return;
		}

		if ( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( ipw.GetItem() ) ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.COMBINE_COLOR );
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		}
		else if( GameInventory.CanSwapEntities( receiver_item, ipw.GetItem() ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.SWAP_COLOR );
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		}
		else if( GetGame().GetPlayer().CanDropEntity( ipw.GetItem() ) )
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			ItemManager.GetInstance().HideDropzones();
			ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
		}
		else
		{
			ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			ItemManager.GetInstance().ShowSourceDropzone( ipw.GetItem() );
		}
	}

	void OnDropReceivedFromIcon( Widget w, int x, int y, Widget receiver )
	{
		string name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		EntityAI receiver_item = receiver_iw.GetItem();

		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !ipw )
		{
			name = w.GetName();
			name.Replace( "PanelWidget", "Render" );
			ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		if( !ipw )
		{
			ipw = ItemPreviewWidget.Cast( w );
		}

		if( !ipw.IsInherited( ItemPreviewWidget ) )
		{
			return;
		}
		
		EntityAI item = ipw.GetItem();
		if( !receiver_item.IsInherited( ItemBase ) || item == NULL || m_ShowedItemIcons.Find( item ) != -1 )
		{
			return;
		}
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		Magazine mag = Magazine.Cast( item );
		bool handled_by_controller = false;
		if( mag )
		{
			Weapon_Base wpn = Weapon_Base.Cast( mag.GetHierarchyParent() );
			if( wpn )
			{
				if (player.GetHumanInventory().GetEntityInHands() == wpn )
				{
					vector m4[4];
					Math3D.MatrixIdentity4(m4);
					GameInventory.PrepareDropEntityPos(player, mag, m4);
					InventoryLocation il_mag_next = new InventoryLocation;
					il_mag_next.SetGround(mag, m4);
					player.GetWeaponManager().DetachMagazine(il_mag_next);
					handled_by_controller = true;
				}
			}
		}
		if(!handled_by_controller )
		{
			if ( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( item ) ) )
			{
				( ItemBase.Cast( receiver_item ) ).CombineItemsClient( ItemBase.Cast( item ) );
			}
			else if( GameInventory.CanSwapEntities( receiver_item, item ) )
			{
				player.PredictiveSwapEntities( item, receiver_item );
			}
			else
			{
				if( player.CanDropEntity( item ) )
				{
					player.PredictiveDropEntity( item );
				}
			}
		}
		ItemManager.GetInstance().SetIsDragging( false );
		ItemManager.GetInstance().PrepareTooltip( item );

		InventoryMenuNew menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
		if ( menu )
		{
			menu.RefreshQuickbar();
		}
	}
	
	void OnLeftPanelDropReceived( Widget w, int x, int y, Widget receiver )
	{
		OnDropReceivedFromHeader( w, x, y, receiver );
	}

	override void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !ipw )
		{
			string name = w.GetName();
			name.Replace( "PanelWidget", "Render" );
			ipw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		if( !ipw )
		{
			ipw = ItemPreviewWidget.Cast( w );
		}

		EntityAI item = ipw.GetItem();
		if( !ipw.IsInherited( ItemPreviewWidget ) || item == NULL || m_ShowedItemIcons.Find( item ) != -1 )
		{
			return;
		}
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		Magazine mag = Magazine.Cast( item );
		bool handled_by_controller = false;
		if( mag )
		{
			Weapon_Base wpn = Weapon_Base.Cast( mag.GetHierarchyParent() );
			if( wpn )
			{
				vector m4[4];
				Math3D.MatrixIdentity4( m4 );
				GameInventory.PrepareDropEntityPos( player, mag, m4 );
				InventoryLocation il_mag_next = new InventoryLocation;
				il_mag_next.SetGround( mag, m4 );
				player.GetWeaponManager().DetachMagazine( il_mag_next );
				handled_by_controller = true;
			}
		}
		if( !handled_by_controller && player.CanDropEntity( item ) )
		{
			player.PredictiveDropEntity( item );
		}

		InventoryMenuNew menu = InventoryMenuNew.Cast( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
		if ( menu )
		{
			menu.RefreshQuickbar();
		}
	}

	void OnDropReceivedFromEmptySpace( Widget w, int x, int y, Widget receiver )
	{
		OnDropReceivedFromHeader( w, x, y, receiver );
	}

	override void UpdateInterval()
	{

		// GetObjectsInVicinity
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if(!player)
		return;
		vector pos = player.GetPosition();
		vector dir = player.GetDirection();
		ref array<Object> objects = new array<Object>;
		ref array<Object> objects_in_cone = new array<Object>;
		ref array<Cargo> proxyCargos = new array<Cargo>;
		GetGame().GetObjectsAtPosition( pos, 1.0, objects, proxyCargos );
		objects_in_cone = ActionTargets.GetVicinityObjects();

		for ( int i = 0; i < objects_in_cone.Count(); i++ )
		{
			Object object = objects_in_cone.Get( i );
			if( object && objects.Find( object ) == INDEX_NOT_FOUND )
			{
				objects.Insert( object );
			}
		}

		//GetItemsShowableInInventory
		array<Object> showable_items = new array<Object>;
		m_ShowedItemIcons.Clear();

		for( i = 0; i < objects.Count(); i++ )
		{
			Object obj = objects.Get( i );
			bool showable_item = !objects.Get( i ).IsAnyInherited( { ScriptedEntity, Building, Camera, PlantSuper, ZombieBase, PASReceiver, DayZAnimal } );
			if ( player.GetInventory().IsPlaceholderEntity(obj) )
				continue; // noproxy: ignore body placeholder
			if (obj.GetParent())
				continue; // noproxy: ignore owned items

			// Temporary solution for making GardenBase objects visible in vicinity
			if (!showable_item )
				showable_item = objects.Get( i ).IsAnyInherited( { GardenBase } );

			if( showable_item )
			{
				if( obj.IsInherited( EntityAI ) && obj != GetGame().GetPlayer() )
				{
					showable_items.Insert( obj );
				}

				if( obj.IsInherited( EntityAI ) && obj != GetGame().GetPlayer() && !obj.IsInherited( DayZInfected ) && !obj.IsInherited( PlayerBase ) )
				{
					m_ShowedItemIcons.Insert( obj );
				}
			}
		}

		if( m_Parent.m_Parent.GetMainPanel().IsVisible() && !this.m_Hidden )
		{
			{ //ShowCargoItems
				ref map<EntityAI, ref Container> new_showed_items = new ref map<EntityAI, ref Container>;
				ref map<int, ref Container> showed_items_IDs = new ref map<int, ref Container>;
				for ( i = 0; i < showable_items.Count(); i++ )
				{
					EntityAI entity = EntityAI.Cast( showable_items.Get( i ) );
					if( m_ShowedItems.Contains( entity ) == false )
					{
						string config = "CfgVehicles " + entity.GetType() + " GUIInventoryAttachmentsProps";

						if( GetGame().ConfigIsExisting( config ) )
						{
							AttachmentCategoriesContainer ac = new AttachmentCategoriesContainer( m_Parent );
							ac.SetEntity( entity );
							new_showed_items.Insert( entity, ac );
							showed_items_IDs.Insert( entity.GetID(), ac);
						}
						else if ( entity.GetSlotsCountCorrect() > 0 && entity.GetInventory().GetCargo() )
						{
							if( entity.IsInherited( DayZInfected ) )
							{
								if( entity.IsAlive() )
								{
									continue;
								}
								ZombieContainer zmb_cnt = new ZombieContainer( m_Parent );
								zmb_cnt.SetEntity( entity );
								new_showed_items.Insert( entity, zmb_cnt );
								showed_items_IDs.Insert( entity.GetID(), zmb_cnt );

							} else
							{
								ItemWithCargoAndAttachments iwca = new ItemWithCargoAndAttachments( this );
								iwca.SetEntity( entity );
								new_showed_items.Insert( entity, iwca );
								showed_items_IDs.Insert( entity.GetID(), iwca );
							}
						}
						else if( entity.GetInventory().GetCargo() )
						{
							{
								ItemWithCargo iwc = new ItemWithCargo( this );
								iwc.SetEntity( entity );
								new_showed_items.Insert( entity, iwc );
								showed_items_IDs.Insert( entity.GetID(), iwc );
							}
						}
						else if( entity.GetSlotsCountCorrect() > 0 )
						{
							if( entity.HasEnergyManager() )
							{
								ItemWithElectricManager iwem = new ItemWithElectricManager( this );
								iwem.SetEntity( entity );
								new_showed_items.Insert( entity, iwem );
								showed_items_IDs.Insert( entity.GetID(), iwem );
							}
							else
							{
								if ( entity.IsInherited( PlayerBase ) )
								{
									if( entity.IsAlive() )
									{
										continue;
									}
									PlayerContainer plyr_cnt = new PlayerContainer( m_Parent );
									plyr_cnt.SetPlayer( PlayerBase.Cast( entity ) );
									( Container.Cast( m_Parent ) ).m_Body.Insert( plyr_cnt );
									( Container.Cast( m_Parent ) ).Refresh();
									new_showed_items.Insert( entity, plyr_cnt );
									showed_items_IDs.Insert( entity.GetID(), plyr_cnt);
								} else
								{
									ItemWithCargoAndAttachments iwcas = new ItemWithCargoAndAttachments( this );
									iwcas.SetEntity( entity );
									new_showed_items.Insert( entity, iwcas );
									showed_items_IDs.Insert( entity.GetID(), iwcas );
								}
							}
						}
					}
					else
					{
						new_showed_items.Insert( entity, m_ShowedItems.Get( entity ) );
						showed_items_IDs.Insert( entity.GetID(), m_ShowedItemsIDs.Get( entity.GetID() ) );
					}
				}

				for ( i = 0; i < m_ShowedItems.Count(); i++ )
				{
					EntityAI ent = m_ShowedItems.GetKey( i );
					m_ShowedItems.GetElement( i ).UpdateInterval();
					if( new_showed_items.Contains( ent ) == false )
					{
						Container con = m_ShowedItems.GetElement( i );
						( Container.Cast( con.m_Parent ) ).Remove( con );
						RecomputeOpenedContainers();
					}
				}
				m_ShowedItems = new_showed_items;
				m_ShowedItemsIDs = showed_items_IDs;
			}

			m_VicinityIconsContainer.ShowItemsInContainers( m_ShowedItemIcons );
		}
	}

	void ToggleContainer( Container conta )
	{
		Container cont = conta;

		if( cont && cont.IsInherited( ClosableContainer ) )
		{
			ClosableContainer c = ClosableContainer.Cast( cont );
			if( c.IsOpened() )
			{
				c.Close();
			}
			else
			{
				c.Open();
			}
		}
	}

	override void CollapseButtonOnMouseButtonDown(Widget w)
	{
		super.CollapseButtonOnMouseButtonDown(w);
		ItemManager.GetInstance().SetDefaultHeaderOpenState( "VICINITY", !m_Hidden );
		m_Spacer.Update();
	}
}
