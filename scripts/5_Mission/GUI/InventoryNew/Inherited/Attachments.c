class Attachments
{
	Container m_Parent;
	EntityAI m_Entity;
	ref array<ref IconsContainer> m_Ics;
	int m_FocusedRow, m_FocusedColumn;

	void Attachments( Container parent, EntityAI entity )
	{
		m_Parent = parent;
		m_Entity = entity;
		m_Ics = new array<ref IconsContainer>();
	}

	void UnfocusAll()
	{
		for ( int i = 0; i < m_Ics.Count(); i++ )
		{
			for ( int j = 0; j < 7; j++ )
			{
				m_Ics.Get( i ).GetMainPanel().FindAnyWidget( "Cursor" + j ).Show( false );
			}
		}
	}

	void SetDefaultFocus()
	{
		m_Ics.Get( 0 ).GetMainPanel().FindAnyWidget( "Cursor" + 0 ).Show( true );
	}

	void Select()
	{
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( m_Ics.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Render" + m_FocusedColumn ) );
		EntityAI receiver_item = item_preview.GetItem();
		EntityAI ent = ItemManager.GetInstance().GetSelectedItem();

			if( !ent )
			{
				if( receiver_item )
				{
					ItemManager.GetInstance().SetSelectedItem( receiver_item, m_Ics.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Selected" + m_FocusedColumn ) );
					m_Ics.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Selected" + m_FocusedColumn ).Show( true );
				}
			}
			else
			{


				EntityAI item = ent;
				if( !item )
				{
					return;
				}

				Weapon_Base wpn;
				Magazine mag;
				PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
				if ( Class.CastTo(wpn,  m_Entity ) && Class.CastTo(mag,  item ) )
				{
					if( player.GetWeaponManager().CanAttachMagazine( wpn, mag ) )
					{
						player.GetWeaponManager().AttachMagazine( mag );
					}
				}
				else if (receiver_item)
				{
					if ( ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( item ) ) )
					{
						( ItemBase.Cast( receiver_item ) ).CombineItemsClient( ItemBase.Cast( item ) );
					}
					else if( GameInventory.CanSwapEntities( receiver_item, item ) )
					{
						GetGame().GetPlayer().PredictiveSwapEntities( item, receiver_item );
					}
				}
				else if( m_Entity.GetInventory().CanAddAttachment( item ) )
				{
					GetGame().GetPlayer().PredictiveTakeEntityToTargetAttachment( m_Entity, item );
				}
				else if( ( m_Entity.GetInventory().CanAddEntityInCargo( item ) && (!player.GetInventory().HasEntityInInventory( item ) || !m_Entity.GetInventory().HasEntityInCargo( item )) ) || player.GetHumanInventory().HasEntityInHands( item ) )
				{
					GetGame().GetPlayer().PredictiveTakeEntityToTargetCargo( m_Entity, item );
				}
			ItemManager.GetInstance().SetSelectedItem( NULL, NULL );
			}
	}

	void MoveGridCursor( int direction )
	{
		UnfocusAll();
		if( direction == Direction.UP )
		{
			m_FocusedRow--;
			if( m_FocusedRow < 0 )
			{
				m_FocusedRow  = m_Ics.Count() - 1;
				Container cnt = Container.Cast( m_Parent.GetParent().GetParent() );
				cnt.SetPreviousActive();
				return;
				
			}

			int max_row = m_Ics.Get( m_FocusedRow ).GetColumnCount() - 1;
			if( max_row < m_FocusedColumn )
			{
				m_FocusedColumn = max_row;
			}
		}

		if( direction == Direction.DOWN )
		{
			m_FocusedRow++;
			if( m_FocusedRow == m_Ics.Count() )
			{
				m_FocusedRow = 0 ;
				cnt = Container.Cast( m_Parent.GetParent().GetParent() );
				cnt.SetNextActive();
				return;
			}

			max_row = m_Ics.Get( m_FocusedRow ).GetColumnCount() - 1;
			if( max_row < m_FocusedColumn )
			{
				m_FocusedColumn = max_row;
			}
		}

		if( direction == Direction.RIGHT )
		{
			m_FocusedColumn++;
			if( m_FocusedColumn == m_Ics.Get( m_FocusedRow ).GetColumnCount() )
			{
				m_FocusedColumn = 0;
				
			}
		}

		if( direction == Direction.LEFT )
		{
			m_FocusedColumn--;
			if( m_FocusedColumn < 0 )
			{
				m_FocusedColumn = m_Ics.Get( m_FocusedRow ).GetColumnCount() - 1;
				
			}
		}

		//Container cnt = Container.Cast( m_Body.Get( 1 ) );
		m_Ics.Get( m_FocusedRow ).GetMainPanel().FindAnyWidget( "Cursor" + m_FocusedColumn ).Show( true );
	}

	void Remove()
	{
		for( int i = 0; i < m_Ics.Count(); i++ )
		{
			m_Parent.Remove( m_Ics.Get( i ) );
			delete m_Ics.Get( i ).GetMainPanel();
		}
		m_Ics.Clear();

		m_Parent.Refresh();
		m_Parent.UpdateSpacer();
	}

	void RefreshAtt( )
	{
		if( !m_Entity )
		{
			return;
		}

		ref array<string> attachments_slots = GetItemSlots( m_Entity );
		array<string> slots = attachments_slots;
		int count =  attachments_slots.Count();

		Att( count, slots );
	}

	void Att( int count, array<string> slots )
	{
		Widget ghost_widget;
		int slot_id;

		for ( int i = 0; i < count; i++ )
		{
			string slot_name = slots.Get ( i );
			string path = "CfgSlots" + " Slot_" + slot_name;
			string icon_name2;
			GetGame().ConfigGetText( path + " ghostIcon", icon_name2 );
			//icon_name2 = "missing";

			ImageWidget ghost_icon;
			if ( ghost_icon )
			{
				ghost_icon.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name2 );
			}

			GetGame().ConfigGetText( path + " name", slot_name );
			slot_id = InventorySlots.GetSlotIdFromString( slot_name );

			ItemPreviewWidget item_preview2 = ItemPreviewWidget.Cast( m_Parent.Get( ( i / 7 ) + 1 ).GetMainPanel().FindAnyWidget( "Icon" + i % 7 ).FindAnyWidget( "Render" + i % 7 ) );
			ImageWidget image_widget2 = ImageWidget.Cast( item_preview2.GetParent().GetParent().FindAnyWidget( "GhostSlot" + i % 7 ) );
			if( m_Entity.GetInventory().FindAttachment( slot_id ) == NULL )
			{
				image_widget2.Show( true );
			}
			else
			{
				image_widget2.Show( false );
			}

			item_preview2.GetParent().Show( true );

			image_widget2.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name2 );
			item_preview2.SetItem( m_Entity.GetInventory().FindAttachment( slot_id ) );
			item_preview2.SetModelOrientation( Vector( 0, 0, 0 ) );
			item_preview2.SetView( 1 );

			if( !ItemManager.GetInstance().IsDragging() )
			{
				ItemManager.GetInstance().SetTemperature( m_Entity.GetInventory().FindAttachment( slot_id ), item_preview2 );
			}

			Widget item_w = item_preview2;
			EntityAI item = m_Entity.GetInventory().FindAttachment( slot_id );
			if( item == NULL )
			{
				item_w.FindAnyWidget( "QuantityPanel" + i % 7 ).Show( false );
				item_w.GetParent().FindAnyWidget( "OutOfReach" + i % 7 ).Show( false );
			}

			item_w.FindAnyWidget( "Mounted" + i % 7 ).Show( false );

			if ( item_w && item )
			{
				if( m_Entity.GetInventory().GetSlotLock( slot_id ) && ItemManager.GetInstance().GetDraggedItem() != item )
				{
					item_w.FindAnyWidget( "Mounted" + i % 7 ).Show( true );
				}

				int has_quantity = QuantityConversions.HasItemQuantity( item );
				Widget quantity_panel = item_w.FindAnyWidget( "QuantityPanel" + i % 7 );
				TextWidget item_quantity = TextWidget.Cast( item_w.FindAnyWidget( "Quantity" + i % 7 ) );
				ProgressBarWidget quantity_progress = ProgressBarWidget.Cast( item_w.FindAnyWidget( "QuantityBar" + i % 7 ) );
				Widget quantity_stack = item_w.FindAnyWidget( "QuantityStackPanel" + i % 7 );
				if ( has_quantity == QUANTITY_HIDDEN )
				{
					quantity_panel.Show( false );
				}
				else
				{
					quantity_panel.Show( true );
					if ( has_quantity == QUANTITY_COUNT )
					{
						item_quantity.SetText( QuantityConversions.GetItemQuantityText( item ) );
						quantity_stack.Show( true );
						quantity_progress.Show( false );
					}
					else if ( has_quantity == QUANTITY_PROGRESS )
					{
						float progress_max = quantity_progress.GetMax();
						int max = item.ConfigGetInt( "varQuantityMax" );
						int count2 = item.ConfigGetInt( "count" );
						float quantity = QuantityConversions.GetItemQuantity( InventoryItem.Cast( item ) );
						if ( count2 > 0 )
						{
							max = count2;
						}
						if ( max > 0 )
						{

							float value = Math.Round( ( quantity / max ) * 100 );
							quantity_progress.SetCurrent( value );
						}
						quantity_stack.Show( false );
						quantity_progress.Show( true );
					}
				}
			}

			if( m_Entity.GetInventory().FindAttachment( slot_id ) != NULL )
			{
				ImageWidget image_widget3 = ImageWidget.Cast( item_preview2.GetParent().FindAnyWidget( "OutOfReach" + i % 7 ) );
				Weapon_Base wpn;
				PlayerBase p = PlayerBase.Cast( GetGame().GetPlayer() );
				if ( AttchmentsOutOfReach.IsAttachmentReachable( m_Entity, slot_name ) && ( !Class.CastTo(wpn,  m_Entity ) || p.GetHumanInventory().GetEntityInHands() == m_Entity ) )
				{
					image_widget3.Show( false );
				}
				else
				{
					image_widget3.Show( true );
				}
			}
		}
	}

	void InitAttachmentGrid( )
	{
		ref array<string> attachments_slots = GetItemSlots( m_Entity );

		array<string> slots = attachments_slots;
		int count = attachments_slots.Count();

		int number_of_rows = ( count / 7 );
		if( count % 7 == 0 )
		{
			number_of_rows--;
		}
		for ( int i = 0; i < number_of_rows + 1; i++ )
		{
			IconsContainer ic = new IconsContainer( m_Parent );

			if( i == number_of_rows && count % 7 != 0 )
			{
				ic.SetColumnCount( count % 7 );
			}
			else
			{
				ic.SetColumnCount( 7 );
			}

			m_Ics.Insert( ic );
			m_Parent.Insert( ic );
			for( int j = 0; j < 7; j++ )
			{
				WidgetEventHandler.GetInstance().RegisterOnDropReceived( ic.GetMainPanel().FindAnyWidget( "Icon" + j ), m_Parent, "OnDropReceivedFromHeader2" );
				WidgetEventHandler.GetInstance().RegisterOnDropReceived( ic.GetMainPanel().FindAnyWidget( "GhostSlot" + j ), m_Parent, "OnDropReceivedFromHeader2" );
				WidgetEventHandler.GetInstance().RegisterOnDropReceived( ic.GetMainPanel().FindAnyWidget( "PanelWidget" + j ), m_Parent, "OnDropReceivedFromHeader2" );

				WidgetEventHandler.GetInstance().RegisterOnDraggingOver( ic.GetMainPanel().FindAnyWidget( "Icon" + j ),  m_Parent, "DraggingOverHeader2" );
				WidgetEventHandler.GetInstance().RegisterOnDraggingOver( ic.GetMainPanel().FindAnyWidget( "GhostSlot" + j ), m_Parent, "DraggingOverHeader2" );
				WidgetEventHandler.GetInstance().RegisterOnDraggingOver( ic.GetMainPanel().FindAnyWidget( "PanelWidget" + j ), m_Parent, "DraggingOverHeader2" );
				
				WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( ic.GetMainPanel().FindAnyWidget( "Icon" + j ), m_Parent, "MouseClick2" );
				WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( ic.GetMainPanel().FindAnyWidget( "GhostSlot" + j ), m_Parent, "MouseClick2" );
				WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( ic.GetMainPanel().FindAnyWidget( "PanelWidget" + j ), m_Parent, "MouseClick2" );
			}
		}

		Att( count, slots );

		for ( i = 0; i < count; i++ )
		{
			ItemPreviewWidget item_preview2 = ItemPreviewWidget.Cast( m_Parent.Get( ( i / 7 ) + 1 ).GetMainPanel().FindAnyWidget( "Icon" + i % 7 ).FindAnyWidget( "Render" + i % 7 ) );
			//Print(item_preview2);
			//Print(item_preview2.GetParent());
			WidgetEventHandler.GetInstance().RegisterOnDrag( item_preview2.GetParent(), m_Parent, "OnIconDrag" );
			WidgetEventHandler.GetInstance().RegisterOnDrop( item_preview2.GetParent(), m_Parent, "OnIconDrop" );
			WidgetEventHandler.GetInstance().RegisterOnDoubleClick( item_preview2.GetParent(), m_Parent, "DoubleClick" );
		}
	}

	array<string> GetItemSlots( EntityAI e )
	{
		TStringArray searching_in = new TStringArray;
		searching_in.Insert( CFG_VEHICLESPATH );
		searching_in.Insert( CFG_WEAPONSPATH );
		searching_in.Insert( CFG_MAGAZINESPATH );

		array<string> attachments_slots	= new array<string>;

		for ( int s = 0; s < searching_in.Count(); ++s )
		{
			string cfg_name = searching_in.Get( s );
			string path = cfg_name + " " + e.GetType();

			if ( GetGame().ConfigIsExisting( path ) )
			{
				g_Game.ConfigGetTextArray( path + " attachments", attachments_slots );
				if ( e.IsWeapon() )
				{
					attachments_slots.Insert( "magazine" );
				}
				return attachments_slots;
			}
		}
		if ( e.IsWeapon() )
		{
			attachments_slots.Insert( "magazine" );
		}
		return attachments_slots;
	}
}
