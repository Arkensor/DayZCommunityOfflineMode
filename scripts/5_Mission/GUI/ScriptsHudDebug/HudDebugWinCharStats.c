class HudDebugWinCharStats extends HudDebugWinBase
{
	TextListboxWidget				m_WgtValues;
	
	//============================================
	// Constructor
	//============================================
	void HudDebugWinCharStats(Widget widget_root)
	{	
		m_WgtValues = TextListboxWidget.Cast( widget_root.FindAnyWidget("txl_CharStats_Values") );
		
		FitWindow();
	}

	//============================================
	// Destructor
	//============================================
	void ~HudDebugWinCharStats()
	{	
		SetUpdate( false );
	}


	//============================================
	// GetWinType
	//============================================
	override int GetType()
	{
		return HudDebug.HUD_WIN_CHAR_STATS;
	}
	
	//============================================
	// Show 
	//============================================
	override void Show()
	{
		super.Show();
		
		//Print("Show()");
		
		SetUpdate( true );
	}

	//============================================
	// Hide 
	//============================================
	override void Hide()
	{
		super.Hide();
		
		//Print("Hide()");
		
		SetUpdate( false );
	}

	//============================================
	// SetUpdate
	//============================================
	void SetUpdate( bool state )
	{
		//Disable update on server (PluginDeveloperSync)
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		PluginDeveloperSync developer_sync = PluginDeveloperSync.Cast( GetPlugin( PluginDeveloperSync ) );
		
		//if client, send RPC
		if ( GetGame().IsClient() )
		{
			ref Param1<bool> params = new Param1<bool>( state );
			if ( player )
			{
				player.RPCSingleParam( ERPCs.DEV_STATS_UPDATE, params, true );
			}
		}
		//else set directly
		else
		{
			if ( developer_sync )
			{
				developer_sync.EnableUpdate( state, ERPCs.DEV_STATS_UPDATE, player );	
			}
		}
	}


	override void Update()
	{
		super.Update();
		
		//Print("Update()");
		
		//refresh notifiers
		SetValues();
	}
	
	void SetValues()
	{
		PluginDeveloperSync developer_sync = PluginDeveloperSync.Cast( GetPlugin( PluginDeveloperSync ) );
		
		//clear window
		ClearValues();
			
		if ( developer_sync.m_PlayerStatsSynced.Count() > 0 )
		{
			//set
			for ( int i = 0; i < developer_sync.m_PlayerStatsSynced.Count(); i++ )
			{
				AddValue( developer_sync.m_PlayerStatsSynced.Get( i ).GetName(), developer_sync.m_PlayerStatsSynced.Get( i ).GetValue().ToString() );
			}
		}
		
		//fit to screen
		FitWindow();		
	}

	void AddValue( string title, string value )
	{
		int index = m_WgtValues.AddItem( title, NULL, 0 );
		m_WgtValues.SetItem( index, value, NULL, 1 );
	}

	void ClearValues()
	{
		m_WgtValues.ClearItems();
	}

	void FitWindow()
	{
		FitWindowByContent( m_WgtValues );
	}
}
