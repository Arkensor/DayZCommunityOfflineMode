//! int  time of the logout end 
//: string  uid of the player
typedef Param2<int, string> LogoutInfo; 

class MissionServer extends MissionBase
{
	ref array<Man> m_Players;
	ref map<PlayerBase, ref LogoutInfo> m_LogoutPlayers;
	
	int m_currentPlayer;
	int m_top = -1;
	int m_bottom = -1;
	int m_shoes = -1;
	int m_skin = -1;
	
	PlayerBase m_player;
	MissionBase m_mission;
	PluginAdditionalInfo m_moduleDefaultCharacter;
	
	ref TStringArray topsArray;
	ref TStringArray pantsArray;
	ref TStringArray shoesArray;
	ref TStringArray backpackArray  = {"TaloonBag_Blue","TaloonBag_Green","TaloonBag_Orange","TaloonBag_Violet"}; //only for testing equips, will remove eventually
	
	void MissionServer()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.UpdatePlayersStats, 30000, true);
	
		int debugMonitorEnable = GetGame().ServerConfigGetInt("enableDebugMonitor");
		GetGame().SetDebugMonitorEnabled(debugMonitorEnable);
		
		UpdatePlayersStats();
		m_Players = new array<Man>;
		m_LogoutPlayers = new map<PlayerBase, ref LogoutInfo>;
		
		InitEquipArrays();
	}

	void ~MissionServer()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.UpdatePlayersStats);
	}

	void InitEquipArrays()
	{
		topsArray = new TStringArray;
		pantsArray = new TStringArray;
		shoesArray = new TStringArray;
		
		string root_path = "cfgCharacterCreation";
		
		g_Game.ConfigGetTextArray(root_path + " top", topsArray);
		g_Game.ConfigGetTextArray(root_path + " bottom", pantsArray);
		g_Game.ConfigGetTextArray(root_path + " shoe", shoesArray);
	}
	
	override void OnInit()
	{
		/*
		// example
		Print("This is Server mission");
		
		Print("vytvaram triggeraaaaaa");
		vector player_pos = "2558 16 2854";
		g_Game.CreateObject("ItemTrigger", player_pos, false);
		//SpawnItems();*/
	}
	
	override void OnUpdate(float timeslice)
	{
		UpdateDummyScheduler();
		TickScheduler(timeslice);
		UpdateLogoutPlayers();
	}

	override bool IsServer()
	{
		return true;
	}	
	
	void UpdatePlayersStats()
	{
		PluginLifespan module_lifespan;
		Class.CastTo(module_lifespan, GetPlugin( PluginLifespan ));
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
			
		for ( int i = 0; i < players.Count(); i++ )
		{
			PlayerBase player;
			Class.CastTo(player, players.Get(i));

			// NEW STATS API
			player.StatUpdateByTime("playtime");
			player.StatUpdateByPosition("dist");

			module_lifespan.UpdateLifespan( player );
		}
	}
	
	// check if logout finished for some players
	void UpdateLogoutPlayers()
	{
		for ( int i = 0; i < m_LogoutPlayers.Count(); i++ )
		{
			LogoutInfo info = m_LogoutPlayers.GetElement(i);
			
			if (GetGame().GetTime() >= info.param1)
			{
				PlayerIdentity identity;
				PlayerBase player = m_LogoutPlayers.GetKey(i);
				if (player)
				{
					identity = player.GetIdentity();
				}
				PlayerDisconnected(player, identity, info.param2);
					
				m_LogoutPlayers.RemoveElement(i);
				i--;
			}
		}
	}
	
	override void OnEvent(EventType eventTypeId, Param params) 
	{
		PlayerIdentity identity;
		PlayerBase player;
		int counter = 0;
		
		switch(eventTypeId)
		{
		case PreloadEventTypeID:
			PreloadEventParams preloadParams;
			Class.CastTo(preloadParams, params);
			
			OnPreloadEvent(preloadParams.param1, preloadParams.param2, preloadParams.param3, preloadParams.param4, preloadParams.param5);
			break;

		case ClientNewEventTypeID:
			ClientNewEventParams newParams;
			Class.CastTo(newParams, params);
			
			player = OnClientNewEvent(newParams.param1, newParams.param2, newParams.param3);
			if (!player)
			{
				Debug.Log("ClientNewEvent: Player is empty");
				return;
			}
			identity = newParams.param1;
			InvokeOnConnect(player,identity );
			break;
			
		case ClientReadyEventTypeID:
			ClientReadyEventParams readyParams;
			Class.CastTo(readyParams, params);
			
			identity = readyParams.param1;
			Class.CastTo(player, readyParams.param2);
			if (!player)
			{
				Debug.Log("ClientReadyEvent: Player is empty");
				return;
			}
			
			OnClientReadyEvent(identity, player);
			InvokeOnConnect(player, identity);
			break;
					
		case ClientRespawnEventTypeID:
			ClientRespawnEventParams respawnParams;
			Class.CastTo(respawnParams, params);
			
			identity = respawnParams.param1;
			Class.CastTo(player, respawnParams.param2);
			if (!player)
			{
				Debug.Log("ClientRespawnEvent: Player is empty");
				return;
			}
			
			OnClientRespawnEvent(identity, player);
			break;
		
		case ClientDisconnectedEventTypeID:
			ClientDisconnectedEventParams discoParams;
			Class.CastTo(discoParams, params);		
			
			identity = discoParams.param1;
			Class.CastTo(player, discoParams.param2);			
			int discTime = discoParams.param3;
			bool authFailed = discoParams.param4;

			if (!player)
			{
				Debug.Log("ClientDisconnectenEvent: Player is empty");
				return;
			}
						
			InvokeOnDisconnect(player);	
			OnClientDisconnectedEvent(identity, player, discTime, authFailed);	
			break;
			
		case LogoutCancelEventTypeID:
			LogoutCancelEventParams logoutCancelParams;
				
			Class.CastTo(logoutCancelParams, params);				
			Class.CastTo(player, logoutCancelParams.param1);
			
			identity = player.GetIdentity();
			if (identity)
			{
				Print("[Logout]: Player " + identity.GetId() + " cancelled"); 
			}
			else
			{
				Print("[Logout]: Player cancelled"); 
			}
			m_LogoutPlayers.Remove(player);
			break;
		}
	}
	
	void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		Debug.Log("InvokeOnConnect:"+this.ToString(),"Connect");
		if( player ) player.OnConnect();
	}

	void InvokeOnDisconnect(PlayerBase player)
	{
		Debug.Log("InvokeOnDisconnect:"+this.ToString(),"Connect");
		if( player ) player.OnDisconnect();
	}

	void OnPreloadEvent(PlayerIdentity identity, out bool useDB, out vector pos, out float yaw, out int queueTime)
	{
		if (GetHive())
		{
			// Preload data on client by character from database
			useDB = true;
		}
		else
		{
			// Preload data on client without database
			useDB = false;
			pos = "1189.3 0.0 5392.48";
			yaw = 0;
			queueTime = 5;
		}
	}

	//saves them for the whole mission
	void ProcessLoginData(ParamsReadContext ctx, out int top, out int bottom, out int shoes, out int skin)
	{
		ref Param1<int> topParam = new Param1<int>(-1);
		if (ctx.Read(topParam))
		{
			top = topParam.param1;
		}
		
		ref Param1<int> bottomParam = new Param1<int>(-1);
		if (ctx.Read(bottomParam))
		{
			bottom = bottomParam.param1;
		}
		
		ref Param1<int> shoesParam = new Param1<int>(-1);
		if (ctx.Read(shoesParam))
		{
			shoes = shoesParam.param1;
		}
		
		ref Param1<int> skinParam = new Param1<int>(-1);
		if (ctx.Read(skinParam))
		{
			skin = skinParam.param1;
		}
	}
	
	//
	PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{		
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player
		Class.CastTo(m_player, playerEnt);
		
		GetGame().SelectPlayer(identity, m_player);
		
		return m_player;
		//moduleDefaultCharacter.FileDelete(moduleDefaultCharacter.GetFileName());
	}
	
	void EquipCharacter()
	{
		//Creates random starting clothes - fallback
		if (m_top == -1 || m_bottom == -1 || m_shoes == -1 || m_skin == -1)
		{
			EntityAI item = m_player.GetInventory().CreateInInventory(topsArray.GetRandomElement());
			EntityAI item2 = m_player.GetInventory().CreateInInventory(pantsArray.GetRandomElement());
			EntityAI item3 = m_player.GetInventory().CreateInInventory(shoesArray.GetRandomElement());
			
			StartingEquipSetup(m_player, false);
		}
		//Creates clothes from DayZIntroScene's m_demoUnit
		else
		{
			item = m_player.GetInventory().CreateInInventory(topsArray.Get(m_top));
			item2 = m_player.GetInventory().CreateInInventory(pantsArray.Get(m_bottom));
			item3 = m_player.GetInventory().CreateInInventory(shoesArray.Get(m_shoes));
			
			StartingEquipSetup(m_player, true);
		}
	}
	
	void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
	}
	
	PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
	{
		string characterName;
		// get login data for new character
		// note: ctx can be filled on client in StoreLoginData()
		ProcessLoginData(ctx, m_top, m_bottom, m_shoes, m_skin);
		
		if (m_top == -1 || m_bottom == -1 || m_shoes == -1 || m_skin == -1)
		{
			characterName = GetGame().CreateRandomPlayer();
		}
		else
		{
			characterName = GetGame().ListAvailableCharacters().Get(m_skin);
		}
		
		if (CreateCharacter(identity, pos, ctx, characterName))
		{
			EquipCharacter();
		}		
		return m_player;
	}
	
	void OnClientReadyEvent(PlayerIdentity identity, PlayerBase player)
	{
		GetGame().SelectPlayer(identity, player);
	}	
	
	void OnClientRespawnEvent(PlayerIdentity identity, PlayerBase player)
	{
		if (GetHive() && player)
		{
			GetHive().CharacterKill(player);
		}
	}
	
	void OnClientDisconnectedEvent(PlayerIdentity identity, PlayerBase player, int queueTime, bool authFailed)
	{
		// TODO: get out of vehicle
		
		// using database and no saving if authorization failed
		if (GetHive() && !authFailed && queueTime > 0)
		{
			// player alive
			if (player.GetPlayerState() == EPlayerStates.ALIVE && !player.IsRestrained())
			{
				if (!m_LogoutPlayers.Contains(player))
				{
					// wait for some time before logout and save
					LogoutInfo params = new LogoutInfo(GetGame().GetTime() + queueTime * 1000, identity.GetId());
					m_LogoutPlayers.Insert(player, params);
					Print("[Logout]: New player " + identity.GetId() + " with logout time " + queueTime.ToString());
				}		
				return;
			}
		}
		PlayerDisconnected(player, identity, identity.GetId());
	}

	void PlayerDisconnected(PlayerBase player, PlayerIdentity identity, string uid)
	{
		// Note: At this point, identity can be already deleted
		if (!player)
		{
			Print("[Logout]: Skipping player " + uid + ", already removed");
			return;
		}
		
		Print("[Logout]: Player " + uid + " finished");

		if (GetHive())
		{
			// save player
			player.Save();
			
			// unlock player in DB	
			GetHive().CharacterExit(player);		
		}
		
		HandleBody(player);
		
		// remove player from server
		GetGame().DisconnectPlayer(identity, uid);
	}
	
	void HandleBody(PlayerBase player)
	{
		if (player.IsAlive() && !player.IsRestrained())
		{
			// remove the body
			player.Delete();	
		}
		else if (/* TODO if player.IsUnconscious() || */ player.IsRestrained())
		{
			// kill character
			player.SetHealth("", "", 0.0);	
		}		
	}
	
	
	void TickScheduler(float timeslice)
	{
		GetGame().GetWorld().GetPlayerList(m_Players);//REWORK.V: this is kind of bad, as we construct this array on every single tick: programmers are fine with this, but lets see if we can construct our own static array, will also fix duplicate ticks when players disconnect

		if(m_currentPlayer >= m_Players.Count() )
		{
			m_currentPlayer = 0;
		} 
		//actual code for player ticks
		if( m_Players.Count() != 0 )
		{

			PlayerBase currentPlayer = PlayerBase.Cast(m_Players.Get(m_currentPlayer));
			currentPlayer.OnTick();
			m_currentPlayer++;
			
		}
		
	}
}
