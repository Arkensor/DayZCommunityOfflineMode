//-----------------------------
// LIFESPAN plugin
//-----------------------------
/*
Lifespan plugin handles player facial hair, bloody hands, blood type in HUD
*/
enum LifeSpanState
{
	BEARD_NONE		= 0,
	BEARD_MEDIUM	= 1,
	BEARD_LARGE		= 2,	
	COUNT			= 3,
}

class PluginLifespan extends PluginBase
{
	protected static const int LIFESPAN_MIN = 0;
	protected static const int LIFESPAN_MAX = 60; // value in minutes when player achieved maximum age in order to have full beard
	
	protected static const int HANDS_OBJECT_SLOT = 7; // hands object slot index for setting and unsetting bloody hands
	
	protected ref map<PlayerBase, ref LifespanLevel> m_PlayerCurrentLevel;
	protected ref map<string, ref array< ref LifespanLevel>> m_LifespanLevels;
	protected ref map<string, ref BloodyHands> m_BloodyHands;
	protected ref map<PlayerBase, int> m_BloodType;

	void PluginLifespan()
	{
		LoadFromCfg();
		m_PlayerCurrentLevel = new map<PlayerBase, ref LifespanLevel>;
		m_BloodType = new map<PlayerBase, int>;
	}

//-----------------------------
// Player load from config
//-----------------------------
	void LoadFromCfg()
	{
		m_LifespanLevels = new map<string, ref array< ref LifespanLevel>>;
		m_BloodyHands = new map<string, ref BloodyHands>;
		
		const int LIFESPAN_LENGTH = LIFESPAN_MAX - LIFESPAN_MIN;

		string cfg_name = "CfgVehicles";
		int cfg_class_count = GetGame().ConfigGetChildrenCount( cfg_name );
		
		int i, j, k, l, m;
		for ( i = 0; i < cfg_class_count; i++ )
		{
			string cfg_class_name = "";
			GetGame().ConfigGetChildName( cfg_name, i, cfg_class_name );
			
			if ( cfg_class_name != "" && cfg_class_name != "access" )
			{
				if ( GetGame().IsKindOf(cfg_class_name, "SurvivorMale_Base") || GetGame().IsKindOf(cfg_class_name, "SurvivorFemale_Base") )
				{
					string cfg_class_fullname = cfg_name + " " + cfg_class_name;
					int cfg_class_member_count = GetGame().ConfigGetChildrenCount( cfg_class_fullname );

					for ( j = 0; j < cfg_class_member_count; j++ )
					{
						string cfg_class_member_name = "";
						GetGame().ConfigGetChildName( cfg_class_fullname, j, cfg_class_member_name );
						
						string cfg_class_member_fullname = cfg_class_fullname + " " + cfg_class_member_name;

						if ( cfg_class_member_name == "Lifespan" )
						{
							int cfg_class_member_member_count = GetGame().ConfigGetChildrenCount( cfg_class_member_fullname );
							
							for ( k = 0; k < cfg_class_member_member_count; k++ )
							{
								string cfg_class_member_member_name = "";
								GetGame().ConfigGetChildName( cfg_class_member_fullname, k, cfg_class_member_member_name );
								
								string cfg_class_member_member_fullname = cfg_class_member_fullname + " " + cfg_class_member_member_name;
								
								//Print( cfg_class_member_member_name );
								
								if ( cfg_class_member_member_name == "Beard" )
								{
									//Print( cfg_class_name );
									
									ref TStringArray materials = new TStringArray;
									int cfg_class_member_member_variable_count = GetGame().ConfigGetChildrenCount( cfg_class_member_member_fullname );
									//Print( cfg_class_member_member_variable_count );
									
									for ( l = 0; l < cfg_class_member_member_variable_count; l++ )
									{
										string cfg_class_member_member_variable_name = "";
										GetGame().ConfigGetChildName( cfg_class_member_member_fullname, l, cfg_class_member_member_variable_name );
										string cfg_class_member_member_variable_fullname = cfg_class_member_member_fullname + " " + cfg_class_member_member_variable_name;
										
										if ( cfg_class_member_member_variable_name == "mat" )
										{
											GetGame().ConfigGetTextArray( cfg_class_member_member_variable_fullname, materials );

											ref array<ref LifespanLevel> lifespan_levels = new array< ref LifespanLevel>;

											int level_count = materials.Count() / 2;
											for ( m = 0; m < level_count; m++ )
											{
												int tex = m*2;
												int mat = ((m*2)+1);
												if ( mat < materials.Count() )
												{
													float threshold = (((float)m / (float)(level_count - 1)) * (float)LIFESPAN_LENGTH) + (float)LIFESPAN_MIN;
													lifespan_levels.Insert( new LifespanLevel(m, threshold, materials.Get(tex), materials.Get(mat)) );
												}
											}
											
											if ( lifespan_levels.Count() > 0 )
											{
												m_LifespanLevels.Set( cfg_class_name, lifespan_levels );
												//Print( cfg_class_name );
											}
										}
									}
								}	
							}
						}
						else if ( cfg_class_member_name == "BloodyHands" )
						{
							string bloody_material, normal_material;
							string path_normal = cfg_class_member_fullname + " mat_normal";
							string path_bloody = cfg_class_member_fullname + " mat_blood";
							GetGame().ConfigGetText(path_normal, normal_material);
							GetGame().ConfigGetText(path_bloody, bloody_material);
							m_BloodyHands.Set( cfg_class_name, new BloodyHands(normal_material, bloody_material) );
						}
					}
				}
			}
		}
		
		/*for ( int cl = 0; cl < m_LifespanLevels.Count(); cl++ )
		{
			string class_name = m_LifespanLevels.GetKeyByIndex( cl );
			array<LifespanLevel> levels = m_LifespanLevels.GetByIndex( cl );

			Print( class_name );

			for ( int ll = 0; ll < levels.Count(); ll++)
			{
				Print( levels.Get( ll ).GetLevel() );
				Print( levels.Get( ll ).GetThreshold() );
				Print( levels.Get( ll ).GetTextureName() );
				Print( levels.Get( ll ).GetMaterialName() );
			}
		}*/
	}

//-----------------------------
// Support functions
//-----------------------------
	
	void SynchLifespanVisual( PlayerBase player, int beard_state_visible, bool bloody_hands_visible, bool blood_type_visible, int blood_type )
	{
		SynchBeardVisual( player, beard_state_visible );
		SynchBloodyHandsVisual( player, bloody_hands_visible );
		SynchShowBloodTypeVisual( player, blood_type_visible );
		SynchShowBloodType( player, blood_type );
	}
	
//-----------------------------
// Facial hair
//-----------------------------
	
	void UpdateLifespan( PlayerBase player, bool force_update = false )
	{
		if ( player != NULL )
		{
			// NEW STATS API
			float player_playtime = player.StatGet("playtime");
						
			float player_beard = player_playtime - player.GetLastShavedSeconds();
			player_beard = player_beard / 60.0;
			
			UpdateLifespanLevel( player, player_beard, force_update );
		}
	} 

	void UpdateLifespanParam( PlayerBase player, string hours_survived, bool force_update )
	{
		if ( player != NULL )
		{
			ref TStringArray splitted = new TStringArray;
			hours_survived.Split( ":", splitted );

			if ( splitted.Count() > 1 )
			{
				int hours = splitted.Get(0).ToInt();
				int minutes = splitted.Get(1).ToInt();
				
				float player_playtime = ((hours * 60.0) + minutes) * 60.0;
				float player_beard = player_playtime - player.GetLastShavedSeconds();
				player_beard = player_beard / 60.0;
				
				UpdateLifespanLevel( player, player_beard, force_update );
			}
		}
	}

	protected void UpdateLifespanLevel( PlayerBase player, float player_beard, bool force_update = false )
	{
		if ( m_PlayerCurrentLevel.Contains(player) )
		{
			LifespanLevel current_level = m_PlayerCurrentLevel.Get( player );

			if ( player_beard > current_level.GetThreshold() || force_update )
			{
				LifespanLevel next_level = GetLifespanLevel( player.GetPlayerClass(), player_beard );
			
				if ( next_level != NULL )
				{
					if( false ) // lifespan temporary disabled
					{
						SetPlayerLifespanLevel( player, next_level );
						m_PlayerCurrentLevel.Set( player, next_level );
					}
				}
			}
		}
		else
		{
			if ( m_LifespanLevels.Contains( player.GetPlayerClass() ) )
			{
				LifespanLevel level = GetLifespanLevel( player.GetPlayerClass(), player_beard );

				if ( level != NULL )
				{
					if( false ) // lifespan temporary disabled
					{
						SetPlayerLifespanLevel( player, level );
						m_PlayerCurrentLevel.Set( player, level );
					}
				}
			}
		}
	}

	protected LifespanLevel GetLifespanLevel( string player_class, float age = 0 )
	{
		array< ref LifespanLevel> lifespan_levels = m_LifespanLevels.Get( player_class );
		
		if ( lifespan_levels != NULL )
		{
			for ( int i = lifespan_levels.Count() - 1; i >= 0; i-- )
			{
				LifespanLevel level = lifespan_levels.Get( i );
				
				if ( age >= level.GetThreshold() )
				{
					return level;
				}
			}
			
			if ( lifespan_levels.Count() > 0 )
			{
				return lifespan_levels.Get( 0 );
			}
		}
		
		return NULL;
	}

	protected void SetPlayerLifespanLevel( PlayerBase player, LifespanLevel level )
	{
		/*
		Print( "set level" );
		
		Print( "level " + level.GetLevel() );
		Print( "treshold " + level.GetThreshold() );
		Print( "texture name " + level.GetTextureName() );
		Print( "material name " + level.GetMaterialName() );
		*/
		player.SetFaceTexture( level.GetTextureName() );
		player.SetFaceMaterial( level.GetMaterialName() );
		
		switch(level.GetLevel())
		{
			case LifeSpanState.BEARD_NONE:
			{
				player.SetLifeSpanStateVisible(LifeSpanState.BEARD_NONE);
				break;
			}
			case LifeSpanState.BEARD_MEDIUM:
			{
				player.SetLifeSpanStateVisible(LifeSpanState.BEARD_MEDIUM);
				break;
			}
			
			case LifeSpanState.BEARD_LARGE:
			{
				player.SetLifeSpanStateVisible(LifeSpanState.BEARD_LARGE);
				break;
			}
								
			default:
			{
				Print("Evil error");
				break;
			}
		}	
	}
	
	void SynchBeardVisual( PlayerBase player, int state )
	{
		float player_beard; 
		
		switch(state)
		{
			case LifeSpanState.BEARD_NONE:
			{
				player_beard = LIFESPAN_MIN;
				break;
			}
			case LifeSpanState.BEARD_MEDIUM:
			{
				player_beard = LIFESPAN_MAX/2;
				break;
			}
			
			case LifeSpanState.BEARD_LARGE:
			{
				player_beard = LIFESPAN_MAX;
				break;
			}
								
			default:
			{
				Print("Evil error");
				break;
			}
		}
		
		UpdateLifespanLevel( player, player_beard, true );
	}
	
//-----------------------------
// Bloody hands
//-----------------------------
	
	void UpdateBloodyHandsVisibility( PlayerBase player, bool show )
	{
		if ( CanMakeHandsBloody( player ) )
		{
			SetBloodyHands( player, show );			
		}
	}
	
	void SynchBloodyHandsVisual( PlayerBase player, bool has_bloody_hands )
	{
		UpdateBloodyHandsVisibility( player, has_bloody_hands );
	}

	void SetBloodyHands( PlayerBase player, bool show )
	{
		player.SetBloodyHands( show );
		
		if ( show )
		{
			SetHandsMaterial( player, BloodyHands.MATERIAL_TYPE_BLOODY );
		}
		else
		{
			SetHandsMaterial( player, BloodyHands.MATERIAL_TYPE_NORMAL );
		}
	}

	protected void SetHandsMaterial( PlayerBase player, int material_type )
	{
		string player_class = player.GetPlayerClass();
		int slot_id;
		EntityAI eai;

		if ( m_BloodyHands.Contains(player_class))
		{	
			slot_id = InventorySlots.GetSlotIdFromString("Gloves");
			eai = player.GetInventory().FindPlaceholderForSlot( slot_id );
			
			if (eai)
			{
				eai.SetObjectMaterial( 0, m_BloodyHands.Get(player_class).GetMaterial(material_type) );
			}	
		}
	}

	bool CanMakeHandsBloody( PlayerBase player )
	{
		if ( !player.FindAttachmentBySlotName( "Gloves" ) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
//-----------------------------
// Blood type in HUD
//-----------------------------
	
	void UpdateBloodTypeVisibility( PlayerBase player, bool show )
	{
		player.SetBloodTypeVisible( show );
		
		if ( player.m_HUD )
		{
			player.m_HUD.UpdateBloodName();
		}
	}
	
	void UpdateBloodType( PlayerBase player, int blood_type )
	{
		player.SetBloodType( blood_type );
	}
	
	void SynchShowBloodTypeVisual( PlayerBase player, bool show )
	{
		UpdateBloodTypeVisibility( player, show );
	}

	void SynchShowBloodType( PlayerBase player, int blood_type )
	{
		UpdateBloodType( player, blood_type );
	}
}