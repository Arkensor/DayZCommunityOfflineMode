class HudDebugWinCharAgents extends HudDebugWinBase
{	
	protected TextListboxWidget	m_WgtAgents;

	//============================================
	// HudDebugWinCharAgents
	//============================================
	void HudDebugWinCharAgents( Widget widget_root )
	{	
		m_WgtRoot = widget_root;
		m_WgtAgents = TextListboxWidget.Cast( m_WgtRoot.FindAnyWidget( "txl_CharAgents_Values" ) );
		
		FitWindowByContent( m_WgtAgents );
	}

	void ~HudDebugWinCharAgents()
	{	
		SetUpdate( false );
	}

	//============================================
	// GetWinType
	//============================================
	override int GetType()
	{
		return HudDebug.HUD_WIN_CHAR_AGENTS;
	}
	
	//============================================
	// Update
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
				player.RPCSingleParam( ERPCs.DEV_AGENTS_UPDATE, params, true );
			}
		}
		//else set directly
		else
		{
			if ( developer_sync )
			{
				developer_sync.EnableUpdate( state, ERPCs.DEV_AGENTS_UPDATE, player );	
			}
		}
	}

	override void Update()
	{
		super.Update();
		
		//Print("Update()");
		
		//refresh notifiers
		SetAgents();
	}
	
	//============================================
	// Show / Hide
	//============================================
	override void Show()
	{
		super.Show();
		
		//Print("Show()");
		
		SetUpdate( true );
	}

	override void Hide()
	{
		super.Hide();
		
		//Print("Hide()");
		
		SetUpdate( false );
	}
	
	void SetAgents()
	{
		PluginDeveloperSync developer_sync = PluginDeveloperSync.Cast( GetPlugin( PluginDeveloperSync ) );
		
		//clear window
		ClearAgents();
			
		//set agents	
		if ( developer_sync.m_PlayerAgentsSynced.Count() > 0 )
		{
			for ( int i = 0; i < developer_sync.m_PlayerAgentsSynced.Count(); i++ )
			{
				AddAgent( developer_sync.m_PlayerAgentsSynced.Get( i ).GetName(), developer_sync.m_PlayerAgentsSynced.Get( i ).GetValue().ToString() );
			}
		}
		
		//fit to screen
		FitWindow();
	}
	
	void AddAgent( string title, string value )
	{
		int index = m_WgtAgents.AddItem( title, NULL, 0 );
		m_WgtAgents.SetItem( index, value, NULL, 1 );
	}

	void ClearAgents()
	{
		m_WgtAgents.ClearItems();
	}

	void FitWindow()
	{
		FitWindowByContent( m_WgtAgents );
	}
}
