class PluginDeveloperSync extends PluginBase
{
	ref Timer m_UpdateTimer;
	ref array<PlayerBase> m_RegisteredPlayers;
	
	ref array<ref SyncedValue> m_PlayerStatsSynced;
	ref array<ref SyncedValue> m_PlayerModsSynced;
	ref array<ref SyncedValue> m_PlayerAgentsSynced;
	
	bool m_StatsUpdateStatus;
	bool m_ModsUpdateStatus;
	bool m_AgentsUpdateStatus;
	
	void PluginDeveloperSync()
	{
		m_RegisteredPlayers 	= new array<PlayerBase>;
		m_PlayerStatsSynced 	= new array<ref SyncedValue>;
		m_PlayerModsSynced 		= new array<ref SyncedValue>;
		m_PlayerAgentsSynced 	= new array<ref SyncedValue>;
		
		m_StatsUpdateStatus 	= false;
		m_ModsUpdateStatus 		= false;
		m_AgentsUpdateStatus 	= false;
	}
	
	//================================================================
	// Init / Update
	//================================================================
	override void OnInit()
	{
		super.OnInit();
		
		if ( GetGame().IsServer() )
		{
			//create timer
			m_UpdateTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		}
	}

	void EnableUpdate( bool state, int type, PlayerBase player )
	{
		//set update by type
		switch ( type )
		{
			case ERPCs.DEV_STATS_UPDATE: 
			{
				m_StatsUpdateStatus = state; 
				break;	
			}
			
			case ERPCs.DEV_MODS_UPDATE: 
			{
				m_ModsUpdateStatus = state; 
				break;	
			}
			
			case ERPCs.DEV_AGENTS_UPDATE: 
			{
				m_AgentsUpdateStatus = state; 
				break;	
			}
		}	
		if ( m_UpdateTimer )
		{
			if ( state && !m_UpdateTimer.IsRunning() )
			{
				//Print("ToggleUpdate() - RUN");
				
				if ( m_StatsUpdateStatus || m_ModsUpdateStatus || m_AgentsUpdateStatus )
				{
					m_UpdateTimer.Run( 1, this, "Update", NULL, true );
					
					//register player
					RegisterPlayer( player );	
				}
			}
			else
			{
				//Print("ToggleUpdate() - STOP");
				
				if ( !m_StatsUpdateStatus && !m_ModsUpdateStatus && !m_AgentsUpdateStatus )
				{
					m_UpdateTimer.Stop();
				
					//unregister player
					UnregisterPlayer( player );
				}
			}
		}
	}
	
	void Update()
	{
		//Stats
		if ( m_StatsUpdateStatus )
		{
			//Multiplayer server
			if ( GetDayZGame().IsMultiplayer() && GetDayZGame().IsServer() )
			{
				for ( int i = 0; i < m_RegisteredPlayers.Count(); i++ )
				{
					if ( m_RegisteredPlayers.Get( i ) )
					{
						SendRPCStats( m_RegisteredPlayers.Get( i ) );		
					}
				}
			}	
			//Local server
			else
			{
				if ( GetDayZGame().IsServer() )
				{
					//update local
					UpdateStatsLocal();	
				}
			}
		}
		
		//Mods
		if ( m_ModsUpdateStatus )
		{
			//Multiplayer server
			if ( GetDayZGame().IsMultiplayer() && GetDayZGame().IsServer() )
			{
				for ( int j = 0; j < m_RegisteredPlayers.Count(); j++ )
				{
					if ( m_RegisteredPlayers.Get( j ) )
					{
						SendRPCMods( m_RegisteredPlayers.Get( j ) );		
					}
				}
			}	
			//Local server
			else
			{
				if ( GetDayZGame().IsServer() )
				{
					//update local
					UpdateModsLocal();	
				}
			}
		}
		
		//Agents
		if ( m_AgentsUpdateStatus )
		{
			//Multiplayer server
			if ( GetDayZGame().IsMultiplayer() && GetDayZGame().IsServer() )
			{
				for ( int k = 0; k < m_RegisteredPlayers.Count(); k++ )
				{
					if ( m_RegisteredPlayers.Get( k ) )
					{
						SendRPCAgents( m_RegisteredPlayers.Get( k ) );		
					}
				}
			}	
			//Local server
			else
			{
				if ( GetDayZGame().IsServer() )
				{
					//update local
					UpdateAgentsLocal();	
				}
			}
		}
	}
	
	//--- Register / Unregister player
	void RegisterPlayer( PlayerBase player )
	{
		if ( !IsPlayerRegistered( player ) ) 
		{
			m_RegisteredPlayers.Insert( player );
		}
	}

	void UnregisterPlayer( PlayerBase player )
	{
		if ( IsPlayerRegistered( player ) ) 
		{
			m_RegisteredPlayers.RemoveItem( player );
		}
	}

	bool IsPlayerRegistered( PlayerBase player )
	{
		if ( m_RegisteredPlayers.Find( player ) > 0 )
		{
			return true;
		}
		
		return false;
	}
	
	//================================================================
	// RPC
	//================================================================ 
	void OnRPC( PlayerBase player, int rpc_type, ParamsReadContext ctx )
	{
		if ( !GetGame().IsDebug() )
		{
			return;
		}
		
		switch( rpc_type )
		{
			case ERPCs.DEV_STATS_UPDATE:
			{
				EnableUpdate( GetRPCUpdateState( ctx ), ERPCs.DEV_STATS_UPDATE, player ); break;
			}
			
			case ERPCs.DEV_MODS_UPDATE:
			{
				EnableUpdate( GetRPCUpdateState( ctx ), ERPCs.DEV_MODS_UPDATE, player ); break;
			}
			
			case ERPCs.DEV_AGENTS_UPDATE:
			{
				EnableUpdate( GetRPCUpdateState( ctx ), ERPCs.DEV_AGENTS_UPDATE, player ); break;
			}
			
			case ERPCs.DEV_RPC_STATS_DATA:
			{
				OnRPCStats( ctx ); break;
			}
			
			case ERPCs.DEV_RPC_MODS_DATA:
			{
				OnRPCMods( ctx ); break;
			}
			
			case ERPCs.DEV_RPC_AGENTS_DATA:
			{
				OnRPCAgents( ctx ); break;
			}
			
			case ERPCs.DEV_RPC_MODS_ACTIVATE:
			{
				ActivateModifier( GetRPCModifierID( ctx ), player ); break;
			}
			
			case ERPCs.DEV_RPC_MODS_DEACTIVATE:
			{
				DeactivateModifier( GetRPCModifierID( ctx ), player ); break;
			}
			
			case ERPCs.DEV_RPC_MODS_LOCK:
			{
				int id;
				bool lock;
				GetRPCModifierLock( ctx, id, lock );
				LockModifier( id, lock, player ); break;
			}
		}
	}
	
	//get update status from ctx data
	bool GetRPCUpdateState( ParamsReadContext ctx )
	{
		ref Param1<bool> p = new Param1<bool>( 0 );
		if ( ctx.Read( p ) )
		{
			return p.param1;
		}
		
		return false;
	}
	
	//get modifier id from ctx data
	int GetRPCModifierID( ParamsReadContext ctx )
	{
		ref Param1<int> p = new Param1<int>( 0 );
		if ( ctx.Read( p ) )
		{
			return p.param1;
		}
		
		return 0;
	}
	
	//get modifier lock state from lock ctx data
	void GetRPCModifierLock( ParamsReadContext ctx, out int id, out bool lock )
	{
		ref Param2<int, bool> p = new Param2<int, bool>( 0, false );
		if ( ctx.Read( p ) )
		{
			id = p.param1;
			lock = p.param2;
		}
	}

	//============================================
	// STATS
	//============================================	
	//send player stats through RPC
	void SendRPCStats( PlayerBase player )
	{
		//write and send values
		if ( player )
		{
			array<ref Param> rpc_params = new array<ref Param>;
			
			//param count
			float param_count = (float) player.m_PlayerStats.Get().Count();
			rpc_params.Insert( new Param2<string, float>( "PARAM_COUNT", param_count ) );
			
			for ( int i = 0; i < player.m_PlayerStats.Get().Count(); i++ ) 
			{
				string label = player.m_PlayerStats.Get().Get( i ).GetLabel();
				float value = (float) player.m_PlayerStats.Get().Get( i ).Get();
				rpc_params.Insert( new Param2<string, float>( label, value ) );
			}
			
			//send params
			GetDayZGame().RPC( player, ERPCs.DEV_RPC_STATS_DATA, rpc_params, true, player.GetIdentity() );
		}
	}

	//Display player stats
	void OnRPCStats( ParamsReadContext ctx )
	{
		//clear values
		m_PlayerStatsSynced.Clear();
		
		ref Param2<string, float> p = new Param2<string, float>( "", 0 );
		
		//get param count
		int param_count = 0;
		if ( ctx.Read(p) )
		{
			param_count = (int) p.param2;
		}
		
		//read values and set 
		for ( int i = 0; i < param_count; i++ )
		{
			ctx.Read(p);
			m_PlayerStatsSynced.Insert( new SyncedValue( p.param1, p.param2, false ) );
		}
	}

	//Update data on local
	void UpdateStatsLocal()
	{
		PlayerBase player = PlayerBase.Cast( GetDayZGame().GetPlayer() );
		
		//clear values
		m_PlayerStatsSynced.Clear();
		
		//set values
		for ( int i = 0; i < player.m_PlayerStats.Get().Count(); i++ ) 
		{
			string label = player.m_PlayerStats.Get().Get( i ).GetLabel();
			float value = player.m_PlayerStats.Get().Get( i ).Get();
			m_PlayerStatsSynced.Insert( new SyncedValue( label, value, false ) );
		}

	}

	//============================================
	// MODS
	//============================================	
	//send player mods through RPC
	void SendRPCMods( PlayerBase player )
	{
		//write and send values
		if ( player )
		{
			array<ref Param> rpc_params = new array<ref Param>;
			
			//get modifiers
			ModifiersManager mods_manager = player.GetModifiersManager();
			
			//param count
			map<int, string> modifiers = new map<int, string>;
			mods_manager.DbgGetModifiers( modifiers );
			
			float param_count = ( float ) modifiers.Count();
			rpc_params.Insert( new Param3<string, float, bool>( "PARAM_COUNT", param_count, false ) );
			
			//set modifiers
			for ( int i = 0; i < modifiers.Count(); ++i ) 
			{
				int key = modifiers.GetKey( i );
				string value = modifiers.Get( key );
				bool state = IsModifierLocked( key, player );
				rpc_params.Insert( new Param3<string, float, bool>( value, key, state ) );
			}
			
			//send params
			GetDayZGame().RPC( player, ERPCs.DEV_RPC_MODS_DATA, rpc_params, true, player.GetIdentity() );
		}
	}

	//Display player modifiers
	void OnRPCMods( ParamsReadContext ctx )
	{
		//clear values
		m_PlayerModsSynced.Clear();
		
		ref Param3<string, float, bool> p = new Param3<string, float, bool>( "", 0, false );
		
		//get param count
		int param_count = 0;
		if ( ctx.Read(p) )
		{
			param_count = (int) p.param2;
		}
		
		//read values and set 
		for ( int i = 0; i < param_count; i++ )
		{
			ctx.Read(p);
			m_PlayerModsSynced.Insert( new SyncedValue( p.param1, p.param2, p.param3 ) );
		}
	}

	//Update data on local
	void UpdateModsLocal()
	{
		PlayerBase player = PlayerBase.Cast( GetDayZGame().GetPlayer() );
		
		if ( player )
		{
			//clear values
			m_PlayerModsSynced.Clear();
			
			//get active/inactive modifiers
			ModifiersManager mods_manager = player.GetModifiersManager();
			
			map<int, string> modifiers = new map<int, string>;
			mods_manager.DbgGetModifiers( modifiers );
			
			//set active modifiers
			for ( int i = 0; i < modifiers.Count(); i++ ) 
			{
				int key = modifiers.GetKey( i );
				string value = modifiers.Get( key );
				bool state = IsModifierLocked( key );
				m_PlayerModsSynced.Insert( new SyncedValue( value, key, state ) );
			}
		}
	}
	
	//Activates modifier with given ID
	void ActivateModifier( int id, PlayerBase player = NULL )
	{
		if ( !player )
		{
			player = PlayerBase.Cast( GetDayZGame().GetPlayer() );
		}
		ModifiersManager mods_manager = player.GetModifiersManager();
		mods_manager.ActivateModifier( Math.AbsInt( id ) );
	}

	//Activates modifier with given ID
	void DeactivateModifier( int id, PlayerBase player = NULL )
	{
		if ( !player )
		{
			player = PlayerBase.Cast( GetDayZGame().GetPlayer() );
		}
		ModifiersManager mods_manager = player.GetModifiersManager();
		mods_manager.DeactivateModifier( Math.AbsInt( id ) );
	}

	//Activates modifier with given ID
	void LockModifier( int id, bool lock, PlayerBase player = NULL )
	{
		if ( !player )
		{
			player = PlayerBase.Cast( GetDayZGame().GetPlayer() );
		}
		ModifiersManager mods_manager = player.GetModifiersManager();
		mods_manager.SetModifierLock( Math.AbsInt( id ), lock );
	}
	
	bool IsModifierLocked( int id, PlayerBase player = NULL )
	{
		if ( !player )
		{
			player = PlayerBase.Cast( GetDayZGame().GetPlayer() );
		}
		ModifiersManager mods_manager = player.GetModifiersManager();	
		
		return mods_manager.GetModifierLock( Math.AbsInt( id ) );
	}
	
	//============================================
	// AGENTS
	//============================================	
	//send player mods through RPC
	void SendRPCAgents( PlayerBase player )
	{
		//write and send values
		if ( player )
		{
			array<ref Param> rpc_params = new array<ref Param>;
			
			//get agent pool data
			array<ref Param> agent_pool = new array<ref Param>;
			player.m_AgentPool.GetDebugObject( agent_pool );
			
			//agents count
			ref Param1<int> p1 = Param1<int>.Cast( agent_pool.Get( 0 ) );
			int agents_count = p1.param1;
			//param count
			float param_count = (float) agents_count;
			rpc_params.Insert( new Param2<string, float>( "PARAM_COUNT", param_count ) );
			
			//set synced data
			for ( int i = 0; i < agents_count; i++ ) 
			{
				ref Param2<string, string> p2 = Param2<string, string>.Cast( agent_pool.Get( i + 1 ) );
				rpc_params.Insert( new Param2<string, float>( p2.param1, p2.param2.ToFloat() ) );
			}
			
			//send params
			GetDayZGame().RPC( player, ERPCs.DEV_RPC_AGENTS_DATA, rpc_params, true, player.GetIdentity() );
		}
	}

	//Display player modifiers
	void OnRPCAgents( ParamsReadContext ctx )
	{
		//clear values
		m_PlayerAgentsSynced.Clear();
		
		ref Param2<string, float> p = new Param2<string, float>( "", 0 );
		
		//get param count
		int param_count = 0;
		if ( ctx.Read(p) )
		{
			param_count = (int) p.param2;
		}
		
		//read values and set 
		for ( int i = 0; i < param_count; i++ )
		{
			ctx.Read(p);
			m_PlayerAgentsSynced.Insert( new SyncedValue( p.param1, p.param2, false ) );
		}
	}

	//Update data on local
	void UpdateAgentsLocal()
	{
		PlayerBase player = PlayerBase.Cast( GetDayZGame().GetPlayer() );
		
		if ( player )
		{
			//clear values
			m_PlayerAgentsSynced.Clear();
			
			//get agent pool data
			array<ref Param> agent_pool = new array<ref Param>;
			player.m_AgentPool.GetDebugObject( agent_pool );
			
			//get agents count 
			Param1<int> p1 = Param1<int>.Cast( agent_pool.Get( 0 ) );
			int agents_count = p1.param1;
			
			//set synced data
			for ( int i = 0; i < agents_count; i++ ) 
			{
				ref Param2<string, string> p2 = Param2<string, string>.Cast( agent_pool.Get( i + 1 ) );
				m_PlayerAgentsSynced.Insert( new SyncedValue( p2.param1, p2.param2.ToFloat(), false ) );
			}
		}
	}
	
	//================================================================
	// FOCUS
	//================================================================ 
	void CaptureFocus()
	{
		GetGame().GetInput().ChangeGameFocus( 1 );
		GetGame().GetUIManager().ShowUICursor( true );
	}
	
	void ReleaseFocus()
	{
		GetGame().GetInput().ChangeGameFocus( -1 );
		GetGame().GetUIManager().ShowUICursor( false );
	}
	
	void ToggleFocus()
	{
		if ( GetGame().GetInput().HasGameFocus() )
		{
			GetGame().GetInput().ChangeGameFocus( 1 );
			GetGame().GetUIManager().ShowUICursor( true );
		}
		else
		{
			GetGame().GetInput().ResetGameFocus();
			GetGame().GetUIManager().ShowUICursor( false );
		}
	}
}
