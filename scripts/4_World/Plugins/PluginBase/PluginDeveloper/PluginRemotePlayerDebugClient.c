enum eRemoteDebugType
{
	NONE,
	DAMAGE_ONLY,
	ALL,
}

class PluginRemotePlayerDebugClient extends PluginBase
{
	const int MAX_SIMULTANIOUS_PLAYERS = 10;
	ref array<ref RemotePlayerStatDebug> m_PlayerDebugStats = new array<ref RemotePlayerStatDebug>;
	ref map<PlayerBase, ref RemotePlayerDamageDebug> m_PlayerDebugDamage = new map<PlayerBase,ref RemotePlayerDamageDebug>;
	
	ref Widget 	m_RootWidget[MAX_SIMULTANIOUS_PLAYERS];
	ref Widget 	m_RootWidgetDamage[MAX_SIMULTANIOUS_PLAYERS];
	ref TextListboxWidget m_StatListWidgets[MAX_SIMULTANIOUS_PLAYERS];
	ref TextListboxWidget m_DamageListWidgets[MAX_SIMULTANIOUS_PLAYERS];
	ref TextWidget m_DistanceWidget[MAX_SIMULTANIOUS_PLAYERS];
	eRemoteDebugType m_DebugType;
	
	override void OnInit()
	{
		#ifndef NO_GUI
		InitWidgets();
		#endif
	}
	
	override void OnUpdate(float delta_time)
	{
		#ifndef NO_GUI
		if( m_DebugType !=0 ) UpdateWidgetsStats();
		#endif
	}
	
	void InitWidgets()
	{
		for(int i = 0; i < MAX_SIMULTANIOUS_PLAYERS; i++)
		{
			m_RootWidget[i] = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_debug_remoteinfo.layout");
			m_RootWidgetDamage[i] = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_debug_remoteinfo_damage.layout");
			m_DamageListWidgets[i] = TextListboxWidget.Cast(m_RootWidgetDamage[i].FindAnyWidget("TextListboxWidget1"));
			m_StatListWidgets[i] = TextListboxWidget.Cast(m_RootWidget[i].FindAnyWidget("TextListboxWidget0"));
			m_DistanceWidget[i] = TextWidget.Cast(m_RootWidget[i].FindAnyWidget("TextWidget0"));
		}
	}
		
	
	void EnableWidgets(bool enable)
	{
		for(int i = 0; i < MAX_SIMULTANIOUS_PLAYERS; i++)
		{
			m_RootWidget[i].Show(enable);
			m_RootWidgetDamage[i].Show(enable);
		}
	}
	
	void UpdateWidgetsStats()
	{
		int i = 0;
		for(; i < m_PlayerDebugStats.Count(); i++)
		{
			RemotePlayerStatDebug rpd = m_PlayerDebugStats.Get(i);
			PlayerBase player = rpd.GetPlayer();
			
			if(player)
			{
				vector pos = player.GetPosition();
				vector screen_pos_stats = GetGame().GetScreenPos(pos + "0 0 0");
				vector screen_pos_damage = GetGame().GetScreenPos(pos + "0 2 0");
				m_RootWidget[i].SetPos(screen_pos_stats[0],screen_pos_stats[1]);
				m_RootWidgetDamage[i].SetPos(screen_pos_damage[0],screen_pos_damage[1]);
				
				if(screen_pos_stats[2] > 0 && screen_pos_stats[0] > 0 && screen_pos_stats[1] > 0)
				{
					m_RootWidget[i].Show(true);
					m_RootWidgetDamage[i].Show(true);
					UpdateStatsWidget(i,rpd);
					UpdateDistanceWidget( i, player );
					UpdateDamageWidget(i, player );
				}
				else
				{
					m_RootWidget[i].Show(false);
					m_RootWidgetDamage[i].Show(false);
				}
				
			}
		}
		for(; i < MAX_SIMULTANIOUS_PLAYERS; i++)
		{
			m_RootWidget[i].Show(false);
			m_RootWidgetDamage[i].Show(false);
		}
	}
	
	
	void UpdateDamageWidget(int index, PlayerBase player)
	{
		array<ref DamageData> damage_list = new array<ref DamageData>;
		if(m_PlayerDebugDamage.Contains(player))
		{
			RemotePlayerDamageDebug value = m_PlayerDebugDamage.Get(player);
			value.GetReversed(damage_list);
			
			m_DamageListWidgets[index].ClearItems();
			if(damage_list.Count() > 0)
			{
				m_DamageListWidgets[index].Show(true);
				for(int i = 0; i < damage_list.Count(); i++)
				{
					float value_global = damage_list.Get(i).GetValueGlobal();
					float value_blood = damage_list.Get(i).GetValueBlood();
					float value_shock = damage_list.Get(i).GetValueShock();
					
					m_DamageListWidgets[index].AddItem( value_global.ToString(),NULL,0,i );
					m_DamageListWidgets[index].SetItem( i, value_blood.ToString(),NULL,1 );
					m_DamageListWidgets[index].SetItem( i, value_shock.ToString(),NULL,2 );
				}
			}
			else
			{
				m_DamageListWidgets[index].Show(false);
			}
		}
	}
	
	void UpdateDistanceWidget(int index, PlayerBase other_player)
	{
		float distance = vector.Distance(GetGame().GetCurrentCameraPosition(), other_player.GetPosition());
		m_DistanceWidget[index].SetText(distance.ToString() +"m.");
	}
	
	void UpdateStatsWidget(int index, RemotePlayerStatDebug rpd)
	{
		array<string> names = new array<string>;
		array<string> values = new array<string>;
		rpd.SerializeNames(names, m_DebugType);
		rpd.SerializeValues(values,m_DebugType);
		m_StatListWidgets[index].ClearItems();
		
		for(int i = 0; i < names.Count(); i++)
		{

			m_StatListWidgets[index].AddItem( names.Get(i),NULL,0,i );
			m_StatListWidgets[index].SetItem( i, values.Get(i),NULL,1 );
		}
		

	}

	void RequestPlayerInfo(PlayerBase player, int type)
	{
		if(type == 0)
		{
			EnableWidgets(false);
		}
		/*
		else
		{
			EnableWidgets(true);
		}
		*/
		ScriptRPC rpc = new ScriptRPC();
		m_DebugType = type;
		rpc.Write(type);
		rpc.Send( player, ERPCs.DEV_REQUEST_PLAYER_DEBUG, true, player.GetIdentity() );
	}
	
	void MergeDamage( array<ref RemotePlayerDamageDebug> delta )
	{
		for(int i = 0; i < delta.Count();i++)
		{
		
			RemotePlayerDamageDebug value_delta = delta.Get(i);
			PlayerBase player_delta = value_delta.GetPlayer();
			
			if( m_PlayerDebugDamage.Contains(null) )
			{
				//cleans up garbage
				m_PlayerDebugDamage.Remove(null);
			}
			
			if( m_PlayerDebugDamage.Contains(player_delta) )
			{
				RemotePlayerDamageDebug value_local = m_PlayerDebugDamage.Get(player_delta);
				array<ref DamageData> damage_list = new array<ref DamageData>;
				value_delta.GetReversed(damage_list);
				
				for(int x = 0; x < damage_list.Count(); x++)
				{
					value_local.InsertDamageObject( damage_list.Get(x) );
				}
			}
			else
			{
				m_PlayerDebugDamage.Insert(player_delta, value_delta);
			}
		}
	}
	
	void DebugDamage(PlayerBase player)
	{
		for(int i = 0; i < m_PlayerDebugStats.Count(); i++)
		{
			player = m_PlayerDebugStats.Get(i).GetPlayer();
			
			PrintString("player:" + player.ToString() );
			m_PlayerDebugStats.Get(i).Debug();
			PrintString("-------------------------------------");
		}
		
		for(i = 0; i < m_PlayerDebugDamage.Count(); i++)
		{
			PrintString("-------------------------------------");
			if( m_PlayerDebugDamage.GetElement(i) ) m_PlayerDebugDamage.GetElement(i).Debug();
			PrintString("-------------------------------------");
		}
		
		for(i = 0; i < m_PlayerDebugDamage.Count();i++)
		{
			PrintString("------------Debug Damage Start--------------");
			PlayerBase player2 = m_PlayerDebugDamage.GetKey(i);
			RemotePlayerDamageDebug obj = m_PlayerDebugDamage.Get(player2);
			obj.Debug();
			PrintString("-------------Debug Damage End-------------");
		}
	}
	
	void OnRPC(ParamsReadContext ctx)
	{
		ctx.Read(m_PlayerDebugStats);
		
		array<ref RemotePlayerDamageDebug> player_damage = new array<ref RemotePlayerDamageDebug>;
		
		ctx.Read(player_damage);
		PlayerBase player;
		for(int x = 0; x < player_damage.Count(); x++)
		{
			RemotePlayerDamageDebug value = player_damage.Get(x);
			if(value)
			{
				player = value.GetPlayer();
				PrintString("debug damage: "+x.ToString() + "," + player.ToString());
				value.Debug();

				int xs = 1 + 1;
			}
		}
		
		if(player_damage.Count() > 0 ) MergeDamage(player_damage);
	}
}