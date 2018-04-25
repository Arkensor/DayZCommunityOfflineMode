class AttachmentCategoriesContainer: CollapsibleContainer
{
	protected ref map<int, ref Widget> m_inventory_slots;
	protected EntityAI m_Entity;

	void AttachmentCategoriesContainer( ContainerBase parent )
	{
		m_inventory_slots = new ref map<int, ref Widget>;
	}

	void SetEntity( EntityAI entity )
	{
		m_Entity = entity;
		InitGhostSlots( entity );
		( Container.Cast( m_Parent ) ).m_Body.Insert( this );
		m_Parent.Refresh();
		SetHeaderName( entity );
	}

	void SetHeaderName( EntityAI entity )
	{
		Header h = Header.Cast( m_Body.Get(0) );
		h.SetName( entity.GetDisplayName() );
	}

	override void UpdateInterval()
	{
		if( m_Entity )
		{
			InitGhostSlots( m_Entity );
		}
	}
	
	/*void SetLastActive()
	{
		Container cont = Container.Cast( m_Body[m_Body.Count() - 1] );
		cont.SetActive( true );
	}*/
	
	void LoadAttachmentCategoriesIcon( IconsContainer items_cont, string icon_name, int slot_number )
	{
		ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( items_cont.GetMainPanel().FindAnyWidget( "Icon"+ slot_number ) );
		ImageWidget image_widget = ImageWidget.Cast( item_preview.FindAnyWidget( "GhostSlot" + slot_number ) );
		image_widget.Show( true );
		image_widget.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name );
		if( m_Body.Count() > ( slot_number + 2 ) )
		{
			ClosableContainer c = ClosableContainer.Cast( m_Body.Get( slot_number + 2 ) );
			if( c.IsOpened() )
				item_preview.FindAnyWidget( "RadialIcon" + slot_number ).Show( true );
			else
				item_preview.FindAnyWidget( "RadialIcon" + slot_number ).Show( false );
		}
	}

	int GetAttachmentCategoriesCount( string config_path )
	{
		return GetGame().ConfigGetChildrenCount( config_path );
	}

	private IconsContainer GetIconsContainer()
	{
		AttachmentCategoriesIconsContainer items_cont;

		if(m_Body.Count() == 1)
		{
			items_cont = new AttachmentCategoriesIconsContainer(this);
			m_Body.Insert(items_cont);
		}
		else
		{
			items_cont = AttachmentCategoriesIconsContainer.Cast( m_Body.Get(1) );
		}

		return items_cont.GetIconsContainer();
	}

	string GetAttachmentCategory( string config_path_attachment_categories, int i )
	{
		string attachment_category;
		GetGame().ConfigGetChildName(config_path_attachment_categories, i, attachment_category);
		return attachment_category;
	}

	string GetIconName( string config_path_attachment_categories, string attachment_category )
	{
		string icon_path = config_path_attachment_categories+ " " + attachment_category + " icon";
		string icon_name;
		GetGame().ConfigGetText(icon_path, icon_name);
		return icon_name;
	}

	void MouseClick( Widget w )
	{
		ClosableContainer c = ClosableContainer.Cast( m_Body.Get( w.GetUserID() + 2 ) );
		if(c.IsOpened())
		{
			c.Close();
		}
		else
		{
			c.Open();
		}

		if(m_Body.Count() > ( w.GetUserID() + 2))
		{
			if(c.IsOpened())
				w.GetParent().FindAnyWidget( "RadialIcon" + w.GetUserID() ).Show( true );
			else
				w.GetParent().FindAnyWidget( "RadialIcon" + w.GetUserID() ).Show( false );
		}
	}

	void InitGhostSlots( EntityAI entity )
	{
		string type = entity.GetType();
		string config_path_attachment_categories = "CfgVehicles " + type + " GUIInventoryAttachmentsProps";

		int attachments_categories_count = GetAttachmentCategoriesCount( config_path_attachment_categories );

		ref IconsContainer items_cont = GetIconsContainer();

		for (int i = 0; i < attachments_categories_count; i++)
		{
			string attachment_category = GetAttachmentCategory( config_path_attachment_categories, i );
			string icon_name = GetIconName( config_path_attachment_categories, attachment_category);

			if(items_cont)
			{
				int slot_number = i;
				ItemPreviewWidget item_preview = ItemPreviewWidget.Cast( items_cont.GetMainPanel().FindAnyWidget( "Icon"+ slot_number ) );
				//WidgetEventHandler.GetInstance().RegisterOnDrag( item_preview.GetParent(),  this, "OnIconDrag" );
				ImageWidget image_widget = ImageWidget.Cast( item_preview.FindAnyWidget( "GhostSlot" + slot_number ) );
				image_widget.Show( true );
				image_widget.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name );
				if( m_Body.Count() > ( slot_number + 2 ) )
				{
					ClosableContainer c = ClosableContainer.Cast( m_Body.Get( slot_number + 2 ) );
					if( c.IsOpened() )
						item_preview.FindAnyWidget( "RadialIcon" + slot_number ).Show( true );
					else
						item_preview.FindAnyWidget( "RadialIcon" + slot_number ).Show( false );
				}
				
				WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( items_cont.GetMainPanel().FindAnyWidget( "PanelWidget"+i ),  this, "MouseClick" );
				WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( items_cont.GetMainPanel().FindAnyWidget( "GhostSlot"+i ),  this, "MouseClick" );
				items_cont.GetMainPanel().FindAnyWidget( "PanelWidget"+i ).SetUserID( i );
				items_cont.GetMainPanel().FindAnyWidget( "GhostSlot"+i ).SetUserID( i );
				AttRow ar;
				if( m_Body.Count() < attachments_categories_count + 2 )
				{
					ar = new AttRow(this);
				}
				else
				{
					ar = AttRow.Cast( m_Body.Get(i+2) );
				}
				ar.Init(attachments_categories_count, i, attachment_category, config_path_attachment_categories, entity,m_Body.Count() );
				if( m_Body.Count() < attachments_categories_count + 2 )
				{
					this.Insert(ar);
				}
			}
		}
		
		if( m_Body.Count() < attachments_categories_count + 3 )
		{
			if( entity.GetInventory().GetCargo() )
			{
				ItemWithCargo iwc = new ItemWithCargo( this );
				iwc.SetEntity( entity );
				
				ref IconsContainer items_cont2 = GetIconsContainer();
				if(items_cont2)
				{
					int slot_number2 = i;
					ItemPreviewWidget item_preview2 = ItemPreviewWidget.Cast( items_cont2.GetMainPanel().FindAnyWidget( "Icon"+ slot_number2 ) );
					ImageWidget image_widget2 = ImageWidget.Cast( item_preview2.FindAnyWidget( "GhostSlot" + slot_number2 ) );
				WidgetEventHandler.GetInstance().RegisterOnDrag( item_preview2.FindAnyWidget( "PanelWidget"+ slot_number2 ),  this, "OnIconDrag" );
					//Print(item_preview2.GetParent().GetName());
					image_widget2.Show( true );
					image_widget2.LoadImageFile( 0, "set:dayz_inventory image:" + icon_name );
					if( m_Body.Count() > ( slot_number2 + 2 ) )
					{
						ClosableContainer c2 = ClosableContainer.Cast( m_Body.Get( slot_number2 + 2 ) );
						if( c2.IsOpened() )
							item_preview2.FindAnyWidget( "RadialIcon" + slot_number2 ).Show( true );
						else
							item_preview2.FindAnyWidget( "RadialIcon" + slot_number2 ).Show( false );
					}
					
					WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( items_cont2.GetMainPanel().FindAnyWidget( "PanelWidget"+i ),  this, "MouseClick" );
					WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( items_cont2.GetMainPanel().FindAnyWidget( "GhostSlot"+i ),  this, "MouseClick" );
					items_cont2.GetMainPanel().FindAnyWidget( "PanelWidget"+i ).SetUserID( i );
					items_cont2.GetMainPanel().FindAnyWidget( "GhostSlot"+i ).SetUserID( i );
				}
			}
		}
		else
		{
			ItemWithCargo iwc2 = ItemWithCargo.Cast( m_Body.Get(attachments_categories_count + 2 ) );
			//iwc2.SetEntity( entity );
			iwc2.UpdateInterval();
		}
		//m_Parent.Refresh();
	}
	
	void OnIconDrag( Widget w )
	{
		string name = w.GetName();
		name.Replace( "PanelWidget", "RadialIcon" );
		ClosableContainer c = ClosableContainer.Cast( m_Body.Get( w.GetUserID() + 2 ) );
		w.GetParent().FindAnyWidget( name ).Show( false );
	}
	
	void OnIconDrop( Widget w )
	{
		ItemManager.GetInstance().HideDropzones();
		
		string name = w.GetName();
		name.Replace( "PanelWidget", "RadialIcon" );
		ClosableContainer c = ClosableContainer.Cast( m_Body.Get( w.GetUserID() + 2 ) );
		w.GetParent().FindAnyWidget( name ).Show( c.IsOpened() );
	}

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
		if( m_Entity.GetInventory().CanAddAttachment( iw.GetItem() ) )
		{
			GetGame().GetPlayer().PredictiveTakeEntityToTargetAttachment( m_Entity, iw.GetItem() );
		}
		else if ( m_Entity.GetInventory().CanAddEntityToInventory( iw.GetItem() ))
		{
			GetGame().GetPlayer().PredictiveTakeEntityToTargetInventory( m_Entity, FindInventoryLocationType.ANY, iw.GetItem() );
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
		if( m_Entity.GetInventory().CanAddAttachment( iw.GetItem() ) || m_Entity.GetInventory().CanAddEntityToInventory( iw.GetItem() ) )
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
	
	override void CollapseButtonOnMouseButtonDown(Widget w)
	{
		super.CollapseButtonOnMouseButtonDown(w);
		m_RootSpacer.Update();
		(LeftArea.Cast( m_Parent ) ).UpdateSpacer();
	}
}
