class AttRow: ClosableContainer
{
	void AttRow( ContainerBase parent ) { }
	
	override void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver )
	{
		ItemPreviewWidget iw = ItemPreviewWidget.Cast( w.FindAnyWidget("Render") );
		if(!iw)
		{
		  string name = w.GetName();
		  name.Replace("PanelWidget", "Render");
		  iw = ItemPreviewWidget.Cast( w.FindAnyWidget(name) );
		}
		if(!iw)
		{
		  iw = ItemPreviewWidget.Cast( w );
		}
		
		if( !iw.GetItem() )
			return;
		
		ItemBase receiver_item;
		name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		if( receiver_iw )
		{
			receiver_item = ItemBase.Cast( receiver_iw.GetItem() );
		}
		ItemBase item = ItemBase.Cast( iw.GetItem() );
		
		if( m_Entity.GetInventory().CanAddAttachmentEx( item, receiver.GetUserID() ) )
		{
			item.SplitIntoStackMaxClient( m_Entity, receiver.GetUserID() );
			int stack_max = InventorySlots.GetStackMaxForSlotId( receiver.GetUserID() );
			float quantity = item.GetQuantity();
			if( stack_max == 0 || stack_max >= quantity || !item.CanBeSplit() )
			{
				GetGame().GetPlayer().PredictiveTakeEntityToTargetAttachmentEx( m_Entity, item, receiver.GetUserID() );
			}
		}
		else
		{
			if ( receiver_item )
			{
				stack_max = InventorySlots.GetStackMaxForSlotId( receiver_item.GetInventory().GetSlotId() );
				quantity = receiver_item.GetQuantity();
				if ( quantity < stack_max && ( ( ItemBase ) receiver_item ).CanBeCombined( item ) )
				{
					int remaining_quantity = stack_max - quantity;
					( ( ItemBase )receiver_item ).CombineItemsClient( item, true );
				}
				else if( stack_max == 0 && GameInventory.CanSwapEntities( receiver_item, item ) )
				{
					GetGame().GetPlayer().PredictiveSwapEntities( item, receiver_item );
				}
			}
			else
			{
				InventoryLocation inv_loc_src = new InventoryLocation;
				InventoryLocation inv_loc_dst = new InventoryLocation;
				item.GetInventory().GetCurrentInventoryLocation( inv_loc_src );
				m_Entity.GetInventory().FindFreeLocationFor( item, FindInventoryLocationType.ATTACHMENT, inv_loc_dst );
				
				if( inv_loc_dst.IsValid() && inv_loc_dst.GetType() == InventoryLocationType.ATTACHMENT )
				{
					stack_max = InventorySlots.GetStackMaxForSlotId( inv_loc_dst.GetSlot() );
					item.SplitIntoStackMaxClient( m_Entity, receiver.GetUserID() );
					quantity = item.GetQuantity();
					if( stack_max == 0 || stack_max >= quantity || !item.CanBeSplit() )
					{
						GetGame().GetPlayer().PredictiveTakeEntityToTargetAttachmentEx( m_Entity, item, inv_loc_dst.GetSlot() );
					}
				}
			}
		}
	}

	override void UnfocusAll()
	{
		Container cnt = Container.Cast( this );
		for ( int i = 1; i < cnt.Count(); i++ )
		{
			for ( int j = 0; j < ITEMS_IN_ROW; j++ )
			{
				cnt.Get( i ).GetMainPanel().FindAnyWidget( "Cursor" + j ).Show( false );
			}
		}
	}

	override void DraggingOverHeader( Widget w, int x, int y, Widget receiver )
	{
		if( w == NULL )
		{
			return;
		}
		ItemPreviewWidget iw = ItemPreviewWidget.Cast( w.FindAnyWidget("Render") );
		if(!iw)
		{
		  string name = w.GetName();
		  name.Replace("PanelWidget", "Render");
		  iw = ItemPreviewWidget.Cast( w.FindAnyWidget(name) );
		}
		if(!iw)
		{
		  iw = ItemPreviewWidget.Cast( w );
		}
		
		if( !iw || !iw.GetItem() )
		{
			return;
		}
			
		ItemBase receiver_item;
		name = receiver.GetName();
		name.Replace("PanelWidget", "Render");
		ItemPreviewWidget receiver_iw = ItemPreviewWidget.Cast( receiver.FindAnyWidget(name) );
		if(receiver_iw)
		receiver_item = ItemBase.Cast( receiver_iw.GetItem() );
		
		if( receiver_item )
		{
			int stack_max = InventorySlots.GetStackMaxForSlotId( receiver_item.GetInventory().GetSlotId() );
			int quantity = receiver_item.GetQuantity();
			if ( quantity < stack_max && ( ItemBase.Cast( receiver_item ) ).CanBeCombined( ItemBase.Cast( iw.GetItem() ) ) )
			{
				ItemManager.GetInstance().HideDropzones();
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				ColorManager.GetInstance().SetColor( w, ColorManager.COMBINE_COLOR );
			}
			else if( stack_max == 0 && GameInventory.CanSwapEntities( receiver_item, iw.GetItem() ) )
			{
				ItemManager.GetInstance().HideDropzones();
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				ColorManager.GetInstance().SetColor( w, ColorManager.SWAP_COLOR );
			}
			else
			{
				ItemManager.GetInstance().ShowSourceDropzone( iw.GetItem() );
				ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			}
		}
		else
		{
			ItemBase item = ItemBase.Cast( iw.GetItem() );
			InventoryLocation inv_loc_src = new InventoryLocation;
			InventoryLocation inv_loc_dst = new InventoryLocation;
			item.GetInventory().GetCurrentInventoryLocation( inv_loc_src );
			m_Entity.GetInventory().FindFreeLocationFor( item, FindInventoryLocationType.ATTACHMENT, inv_loc_dst );
			
			if( inv_loc_dst.IsValid() )
			{
				ItemManager.GetInstance().HideDropzones();
				ItemManager.GetInstance().GetRootWidget().FindAnyWidget( "LeftPanel" ).FindAnyWidget( "DropzoneX" ).Show( true );
				ColorManager.GetInstance().SetColor( w, ColorManager.GREEN_COLOR );
			}
			else
			{
				ItemManager.GetInstance().ShowSourceDropzone( iw.GetItem() );
				ColorManager.GetInstance().SetColor( w, ColorManager.RED_COLOR );
			}
		}
	}
	
	void OnIconDrag( Widget w )
	{
		ItemManager.GetInstance().SetIsDragging( true );
		string name = w.GetName();
		name.Replace( "PanelWidget", "Render" );
		ItemPreviewWidget ipw = ItemPreviewWidget.Cast( w.FindAnyWidget(name) );
		float icon_x, icon_y, x_content, y_content;
		int m_sizeX, m_sizeY;

		InventoryItem i_item = InventoryItem.Cast( ipw.GetItem() );
		GetGame().GetInventoryItemSize( i_item, m_sizeX, m_sizeY );

		m_Parent.m_Parent.GetMainPanel().FindAnyWidget( "body" ).GetScreenSize( x_content, y_content );
		icon_x = x_content / 10;
		icon_y = x_content / 10;
		w.SetFlags( WidgetFlags.EXACTSIZE );
		w.SetSize( icon_x, icon_y );
		w.SetSize( icon_x * m_sizeX - 1 , icon_y * m_sizeY + m_sizeY - 1 );
		
		if( !ipw.GetItem() )
		{
			return;
		}
		
		name.Replace( "Render", "Col" );
		w.FindAnyWidget( name ).Show( true );
		name.Replace( "Col", "Temperature" );
		w.FindAnyWidget( name ).Show( false );
		name.Replace( "Temperature", "RadialIcon" );
		w.FindAnyWidget( name ).Show( false );
		ItemManager.GetInstance().SetDraggedItem( ipw.GetItem() );
	}
	
	void OnIconDrop( Widget w )
	{
		ItemManager.GetInstance().HideDropzones();
		
		ItemManager.GetInstance().SetIsDragging( false );
		w.ClearFlags( WidgetFlags.EXACTSIZE );
		w.SetSize( 1, 1 );
		string name = w.GetName();
		name.Replace( "PanelWidget", "Col" );
		w.FindAnyWidget( name ).Show( false );
		name.Replace( "Col", "Temperature" );
		w.FindAnyWidget( name ).Show( false );
		name.Replace( "Temperature", "GhostSlot" );
		w.GetParent().FindAnyWidget( name ).Show( true );
		ItemManager.GetInstance().SetDraggedItem( NULL );
	}
	
	void DoubleClick(Widget w, int x, int y, int button)
	{
		if( w == NULL )
		{
			return;
		}
		ItemPreviewWidget iw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if( !iw )
		{
		  string name = w.GetName();
		  name.Replace( "PanelWidget", "Render" );
		  iw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}
		if( !iw )
		{
		  iw = ItemPreviewWidget.Cast( w );
		}
		if( !iw.GetItem() )
		{
			return;
		}
		
		if( m_Entity.GetInventory().GetSlotLock( iw.GetUserID() ) )
		{
			return;
		}
		
		if ( !m_Entity.CanReleaseAttachment( iw.GetItem() ) )
		{
			return;
		}
		
		EntityAI item = iw.GetItem();
		
		InventoryManager manager = InventoryManager.GetInstance();
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if ( player.GetInventory().HasEntityInInventory( item ) && player.GetHumanInventory().CanAddEntityInHands( item ) )
		{
			player.PredictiveTakeEntityToHands( item );
		}
		else
		{
			if (player.GetInventory().CanAddEntityToInventory( item ) && item.GetInventory().CanRemoveEntity())
			{
				player.PredictiveTakeEntityToInventory( FindInventoryLocationType.ANY, InventoryItem.Cast( item ) );
			}
			else
			{
				if( player.GetHumanInventory().CanAddEntityInHands( item ) )
				{
					player.PredictiveTakeEntityToHands( item );
				}
			}
		}
		
		ItemManager.GetInstance().HideTooltip();
		
		name = w.GetName();
		name.Replace( "PanelWidget", "Temperature" );
		w.FindAnyWidget( name ).Show( false );
	}
	
	void Init(int attachments_categories_count, int i, string attachment_category, string config_path_attachment_categories, EntityAI entity, int parent_m_Body_count )
	{
		m_Entity = entity;
		Header he = Header.Cast( this.Get(0) );
		he.SetName( attachment_category );
		
		ref array<string> player_ghosts_slots2 = new array<string>;
		
		string config_path_ghosts_slots3 = config_path_attachment_categories + " " + attachment_category + " attachmentSlots";
		GetGame().ConfigGetTextArray ( config_path_ghosts_slots3, player_ghosts_slots2 );
		
		array<string> slots = player_ghosts_slots2;
		int count = player_ghosts_slots2.Count();
		
		if( parent_m_Body_count < attachments_categories_count + 2 )
		{
			for ( int j = 0; j < (count/7)+1; j++ )
			{
				IconsContainer ic = new IconsContainer(this);
				this.Insert(ic);
				for(int k = 0; k < 7; k++)
				{
					WidgetEventHandler.GetInstance().RegisterOnDropReceived( ic.GetMainPanel().FindAnyWidget("Icon"+k),  this, "OnDropReceivedFromHeader" );
					WidgetEventHandler.GetInstance().RegisterOnDropReceived( ic.GetMainPanel().FindAnyWidget("GhostSlot"+k),  this, "OnDropReceivedFromHeader" );
					WidgetEventHandler.GetInstance().RegisterOnDropReceived( ic.GetMainPanel().FindAnyWidget("PanelWidget"+k),  this, "OnDropReceivedFromHeader" );
					
					WidgetEventHandler.GetInstance().RegisterOnDraggingOver( ic.GetMainPanel().FindAnyWidget("Icon"+k),  this, "DraggingOverHeader" );
					WidgetEventHandler.GetInstance().RegisterOnDraggingOver( ic.GetMainPanel().FindAnyWidget("GhostSlot"+k),  this, "DraggingOverHeader" );
					WidgetEventHandler.GetInstance().RegisterOnDraggingOver( ic.GetMainPanel().FindAnyWidget("PanelWidget"+k),  this, "DraggingOverHeader" );
				}
			}
		}
		
		for ( j = 0; j < count; j++ )
		{
			string slot_name = slots.Get ( j );
			string icon_name2;
			string path = "CfgSlots" + " Slot_" + slot_name;
			GetGame().ConfigGetText(path + " ghostIcon", icon_name2);
			//icon_name2 = "missing";
			
			ImageWidget ghost_icon;
			if ( ghost_icon )
			{
				ghost_icon.LoadImageFile(0,"set:dayz_inventory image:" + icon_name2 );
			}
			
			GetGame().ConfigGetText(path + " name", slot_name);
			int slot_id = InventorySlots.GetSlotIdFromString( slot_name );
			
			ItemPreviewWidget item_preview2 = ItemPreviewWidget.Cast( this.Get((j/7)+1).GetMainPanel().FindAnyWidget("Icon"+j%7).FindAnyWidget("Render"+j%7) );
			ImageWidget image_widget2 = ImageWidget.Cast( item_preview2.GetParent().GetParent().FindAnyWidget("GhostSlot"+j%7) );
			item_preview2.GetParent().Show( true );
			if(!entity.GetInventory())
			return;
			if(entity.GetInventory().FindAttachment(slot_id) == NULL)
			{
				image_widget2.Show(true);
			}
			else
			{
				image_widget2.Show(false);
			}
			
			image_widget2.SetUserID( slot_id );
			item_preview2.SetUserID( slot_id );
			
			image_widget2.LoadImageFile(0,"set:dayz_inventory image:" + icon_name2 );
			item_preview2.SetItem(entity.GetInventory().FindAttachment(slot_id));
			item_preview2.SetModelOrientation( Vector(0, 0, 0) );
			item_preview2.SetView(1);
			
			ItemManager.GetInstance().SetTemperature( entity.GetInventory().FindAttachment( slot_id ), item_preview2 );
			
			Widget item_w = item_preview2;
			EntityAI item = m_Entity.GetInventory().FindAttachment(slot_id);
			if( item == NULL )
			{
				item_w.FindAnyWidget("QuantityPanel"+j%7).Show(false);
			}
			
			item_w.FindAnyWidget("Mounted"+j%7).Show(false);
			
			if ( item_w && item )
			{				
				if( m_Entity.GetInventory().GetSlotLock( slot_id ) && ItemManager.GetInstance().GetDraggedItem() != item )
				{
					item_w.FindAnyWidget("Mounted"+j%7).Show( true );
					item_preview2.GetParent().ClearFlags( WidgetFlags.DRAGGABLE );
				}
				else
				{
					item_w.FindAnyWidget("Mounted"+j%7).Show( false );
					item_preview2.GetParent().SetFlags( WidgetFlags.DRAGGABLE );
				}
				
				if( !m_Entity.CanReleaseAttachment( item ) )
				{
					item_preview2.GetParent().ClearFlags( WidgetFlags.DRAGGABLE );
				}
				
				int has_quantity = QuantityConversions.HasItemQuantity( item );
				Widget quantity_panel = item_w.FindAnyWidget("QuantityPanel"+j%7);
				TextWidget item_quantity = TextWidget.Cast( item_w.FindAnyWidget("Quantity"+j%7) );
				ProgressBarWidget quantity_progress = ProgressBarWidget.Cast( item_w.FindAnyWidget("QuantityBar"+j%7) );
				Widget quantity_stack = item_w.FindAnyWidget("QuantityStackPanel"+j%7);
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
						int max = item.ConfigGetInt("varQuantityMax");
						int count2 = item.ConfigGetInt("count");
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
			
			WidgetEventHandler.GetInstance().RegisterOnDrag( item_preview2.GetParent(),  this, "OnIconDrag" );
			WidgetEventHandler.GetInstance().RegisterOnDrop( item_preview2.GetParent(),  this, "OnIconDrop" );
			WidgetEventHandler.GetInstance().RegisterOnDoubleClick( item_preview2.GetParent(),  this, "DoubleClick" );
			
			if(entity.GetInventory().FindAttachment(slot_id) != NULL)
			{
				ImageWidget image_widget3 = ImageWidget.Cast( item_preview2.GetParent().FindAnyWidget("OutOfReach"+j%7) );
				if ( AttchmentsOutOfReach.IsAttachmentReachable(entity, slot_name) )
				{
					image_widget3.Show(false);
				}
				else
				{
					image_widget3.Show(true);
				}
			}
		}
	}
}
