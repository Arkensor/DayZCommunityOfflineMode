class HandsPreview: ContainerBase
{
	ref Icon icon;
	protected EntityAI item_has_attachments_initialized;

	void HandsPreview( ContainerBase parent )
	{
		
	}

	void RefreshQuantity( EntityAI item_to_refresh )
	{
		if( icon )
		{
			icon.RefreshQuantity();
		}
	}

	void SelectCombine()
	{
		icon.SelectCombine();
	}

	void SelectSwap()
	{
		icon.SelectSwap();
	}
	
	Icon GetIcon()
	{
		return icon;
	}
	
	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.HandsPreview;
	}

	void RemoveItem()
	{
		delete icon;
	}

	override void UpdateInterval()
	{
		//TODO: create layout for HandsPreview and set ARGB there
		m_ParentWidget.SetColor( ARGB( 230, 20, 20, 20 ) );
		bool show_combine_swap = ItemManager.GetInstance().IsDragging();

		#ifndef PLATFORM_XBOX
		if( icon && !icon.IsDragged() )
		{
			icon.GetMainPanel().FindAnyWidget( "Combine" ).Show(  show_combine_swap );
			icon.GetMainPanel().FindAnyWidget( "Swap" ).Show( show_combine_swap );
		}
		#endif

		PlayerBase p = PlayerBase.Cast( GetGame().GetPlayer() );
		if(!p)
		return;
		EntityAI item = p.GetHumanInventory().GetEntityInHands();

		if ( ( !item && icon ) || ( icon && item != icon.GetObject() ) )
		{
			RemoveItem();
			( HandsContainer.Cast( m_Parent ) ).DestroyAtt();
			( HandsContainer.Cast( m_Parent ) ).DestroyCargo();
			item_has_attachments_initialized = NULL;
		}
		else
		if( item && !icon )
		{
			icon = new Icon( this, true );
			icon.Refresh();
			if( icon )
			{
				Weapon_Base wpn;
				if ( Class.CastTo(wpn,  item ) )
				{
					int mi = wpn.GetCurrentMuzzle();
					icon.GetMainPanel().FindAnyWidget( "AmmoIcon" ).Show(  wpn.IsChamberFull( mi )  );
				}

				icon.Init( item );
				icon.FullScreen();
				if( item != item_has_attachments_initialized )
				{
					( HandsContainer.Cast( m_Parent ) ).DestroyAtt();
					( HandsContainer.Cast( m_Parent ) ).DestroyCargo();
					( HandsContainer.Cast( m_Parent ) ).ShowAtt( item );
					item_has_attachments_initialized = item;
				}
			}
		}
		else if ( icon )
		{
			icon.Refresh();
			icon.FullScreen();
		}
		
		HandsContainer parent = HandsContainer.Cast( m_Parent );
		Header header = parent.m_CollapsibleHeader;
		
		if( item )
		{
			string display_name = item.GetDisplayName();
			display_name.ToUpper();
			header.SetName( display_name );
		}
		else
		{
			header.SetName( "HANDS EMPTY" );
		}
	}
}
