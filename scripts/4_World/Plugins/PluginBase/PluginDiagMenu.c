enum DiagMenuIDs
{
	DM_SCRIPTS_MENU,
	DM_PLAYER_STATES_MENU,
	DM_PLAYER_STATES_SHOW,
	DM_TRANSFER_VALUES_MENU,
	DM_TRANSFER_VALUES_SHOW,
	DM_PLAYER_CRAFTING_MENU,
	DM_PLAYER_CRAFTING_GENERATE,
	DM_PLAYER_CRAFTING_DEBUG_ACTIONS_ENABLE,
	DM_PLAYER_CRAFTING_DUMP,
	DM_CHEATS_MENU,
	DM_CHEATS_MODIFIERS_ENABLE,
	DM_CHEATS_PLAYER_INSTAKILL,
	DM_CHEATS_INVINCIBILITY,
	DM_CHEATS_STAMINA_DISABLE,
	DM_PLAYER_AGENTS_MENU,
	DM_PLAYER_AGENTS_INJECTS_SHOW,
	DM_SOFT_SKILLS_MENU,
	DM_GUN_PARTICLES,
	DM_SOFT_SKILLS_SHOW_DEBUG,
	DM_SOFT_SKILLS_TOGGLE_STATE,
	DM_SOFT_SKILLS_TOGGLE_MODEL,
	DM_SOFT_SKILLS_SPECIALTY_VALUE,
	DM_ITEM_DEBUG_ACTIONS_SHOW,
	DM_PLAYER_STATS_LOG_ENABLE,
	DM_ACTION_TARGETS_MENU,
	DM_ACTION_TARGETS_DEBUG,
	DM_ACTION_TARGETS_SELPOS_DEBUG,
	DM_LIFESPAN_MENU,
	DM_BLOODY_HANDS,
	DM_LIFESPAN_PLAYTIME_UPDATE,
	DM_PERMANENT_CROSSHAIR,
	DM_TOGGLE_HUD,
	DM_MISC_MENU,
	DM_MELEE_MENU,
	DM_MELEE_DEBUG_ENABLE,
	DM_MELEE_SHOW_TARGETS,
	DM_MELEE_DRAW_TARGETS,
	DM_MELEE_DRAW_RANGE,
	DM_MELEE_BLOCK_STANCE,
	DM_MELEE_FIGHT_SINGLE,
	DM_WEAPON_DEBUG_MENU,
	DM_WEAPON_DEBUG_ENABLE,
	DM_WEAPON_AIM_NOISE,
	DM_WEAPON_ALLOW_RECOIL,
	DM_WEAPON_UNLIMITED,
	DM_ENVIRONMENT_DEBUG_ENABLE,
	DM_DISPLAY_PLAYER_INFO,
	DM_DRAW_CHECKERBOARD,
	DM_BULLET_IMPACT,
};

enum DebugActionType
{
	GENERIC_ACTIONS = 1,
	PLAYER_AGENTS = 2,
	UNLIMITED_AMMO = 4,
};


class PluginDiagMenu extends PluginBase
{
#ifdef DEVELOPER
	ref Timer m_Timer;
	bool m_EnableModifiers 			= true;
	bool m_EnableUnlimitedAmmo		= false;
	bool m_IsInvincible 			= false;
	bool m_ShowCraftingDebugActions = false;
	bool m_LogPlayerStats 			= false;
	bool m_SoftSkillsState 			= false;
	bool m_SoftSkillsDebug 			= false;
	bool m_GunParticlesState		= false;
	bool m_SoftSkillsModel 			= false;
	bool m_BloodyHands	 			= false;
	bool m_PermanentCrossHair		= false;
	bool m_ToggleHud				= false;
	bool m_ShowingWeaponDebug		= false;
	bool m_AimNoiseEnabled			= true;
	int m_DisplayPlayerInfo			= false;
	bool m_ProceduralRecoilEnabled 	= true;
	bool m_StaminaDisabled			= false;
	bool m_MeleeBlockStance			= false;
	bool m_MeleeFight				= false;
	bool m_EnvironmentStats			= false;
	bool m_DrawCheckerboard			= false;
	float m_SpecialtyLevel			= 0;
	float m_LifespanLevel			= 0;
	int  m_DayzPlayerDebugMenu		= -1;
	
	override void OnInit()
	{
		if( GetGame().IsMultiplayer() && GetGame().IsServer() ) return; //(only client/local)

		m_Timer = new Timer();

		m_Timer.Run(0.5, this, "Update", NULL, true);
		
		//---------------------------------------------------------------
		// LEVEL 0
		//---------------------------------------------------------------
 		DiagMenu.RegisterMenu(DiagMenuIDs.DM_SCRIPTS_MENU,"Script", "");
		
		
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_CHEATS_MENU,"Cheats", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_CHEATS_MODIFIERS_ENABLE, "lalt+2", "Tick Modifiers", "Cheats");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_CHEATS_INVINCIBILITY, "lalt+1", "Invincibility(IDDQD)", "Cheats");
				DiagMenu.SetValue(DiagMenuIDs.DM_CHEATS_MODIFIERS_ENABLE, true);
				DiagMenu.RegisterBool(DiagMenuIDs.DM_CHEATS_PLAYER_INSTAKILL, "", "Kill Player", "Cheats");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_CHEATS_STAMINA_DISABLE, "", "Disable stamina", "Cheats");
			
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_PLAYER_CRAFTING_MENU,"Crafting", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_CRAFTING_GENERATE, "", "Generate Cache", "Crafting");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_CRAFTING_DEBUG_ACTIONS_ENABLE, "lalt+3", "Debug Insta Crafting", "Crafting");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_CRAFTING_DUMP, "", "Dump recipes to file", "Crafting");
			
			
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_PLAYER_STATES_MENU,"Player States", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_STATES_SHOW, "lalt+6", "Show States", "Player States");

			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_TRANSFER_VALUES_MENU, "TransferValues", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_TRANSFER_VALUES_SHOW, "", "ShowValues", "TransferValues");
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_PLAYER_AGENTS_MENU, "Player Agents", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_AGENTS_INJECTS_SHOW, "lalt+5", "Allow Inject Actions", "Player Agents");
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_SOFT_SKILLS_MENU, "Soft Skills", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool( DiagMenuIDs.DM_SOFT_SKILLS_SHOW_DEBUG, "", "Show Debug", "Soft Skills" );
				DiagMenu.RegisterBool( DiagMenuIDs.DM_SOFT_SKILLS_TOGGLE_STATE, "", "Enable SoftSkills", "Soft Skills" );
				DiagMenu.RegisterBool( DiagMenuIDs.DM_SOFT_SKILLS_TOGGLE_MODEL, "", "Enable linear model", "Soft Skills" );
				DiagMenu.RegisterRange( DiagMenuIDs.DM_SOFT_SKILLS_SPECIALTY_VALUE, "", "Set specialty value", "Soft Skills", "-1, 1, 0, 0.01" );
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_LIFESPAN_MENU, "Lifespan", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool( DiagMenuIDs.DM_BLOODY_HANDS, "", "Bloody hands", "Lifespan" );
				DiagMenu.RegisterRange( DiagMenuIDs.DM_LIFESPAN_PLAYTIME_UPDATE, "", "Playtime in seconds", "Lifespan", "0, 3600, 0, 300" );
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_MISC_MENU, "Misc", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_ITEM_DEBUG_ACTIONS_SHOW, "lalt+4", "Item Debug Actions", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_BULLET_IMPACT, "ralt+0", "BulletImpact", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PLAYER_STATS_LOG_ENABLE, "", "Log Player Stats", "Misc");
				DiagMenu.RegisterMenu(DiagMenuIDs.DM_ACTION_TARGETS_MENU, "Action Targets", "Misc");
					//---------------------------------------------------------------
					// LEVEL 3
					//---------------------------------------------------------------
					DiagMenu.RegisterBool(DiagMenuIDs.DM_ACTION_TARGETS_DEBUG, "", "Show Debug", "Action Targets");
					DiagMenu.RegisterBool(DiagMenuIDs.DM_ACTION_TARGETS_SELPOS_DEBUG, "", "Show selection pos debug", "Action Targets");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_PERMANENT_CROSSHAIR, "", "Enable permanent crosshair", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_TOGGLE_HUD, "", "Toggle HUD on/off", "Misc", true);
				DiagMenu.RegisterRange(DiagMenuIDs.DM_DISPLAY_PLAYER_INFO, "", "Display Player Info", "Misc", "0,2,0,1");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_ENVIRONMENT_DEBUG_ENABLE, "", "Show Environment stats", "Misc");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_DRAW_CHECKERBOARD, "", "Draw Checkerboard on screen", "Misc");
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_MELEE_MENU, "Melee", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool(DiagMenuIDs.DM_MELEE_DEBUG_ENABLE, "", "Enable Melee Debug", "Melee");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_MELEE_SHOW_TARGETS, "", "Show targets", "Melee");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_MELEE_DRAW_TARGETS, "", "Draw targets", "Melee");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_MELEE_DRAW_RANGE, "", "Draw range", "Melee");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_MELEE_BLOCK_STANCE, "", "Block stance", "Melee");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_MELEE_FIGHT_SINGLE, "", "Single Hit (cont)", "Melee");
			//---------------------------------------------------------------
			// LEVEL 1
			//---------------------------------------------------------------
			DiagMenu.RegisterMenu(DiagMenuIDs.DM_WEAPON_DEBUG_MENU, "Weapon", "Script");
				//---------------------------------------------------------------
				// LEVEL 2
				//---------------------------------------------------------------
				DiagMenu.RegisterBool( DiagMenuIDs.DM_GUN_PARTICLES, "", "Enable gun particles", "Weapon", true );
				DiagMenu.RegisterBool(DiagMenuIDs.DM_WEAPON_DEBUG_ENABLE, "", "Show Debug", "Weapon");
				DiagMenu.RegisterBool(DiagMenuIDs.DM_WEAPON_AIM_NOISE, "", "Weapon Sway", "Weapon");
				DiagMenu.SetValue(DiagMenuIDs.DM_WEAPON_AIM_NOISE, true);
				DiagMenu.RegisterBool(DiagMenuIDs.DM_WEAPON_ALLOW_RECOIL, "", "Procedural Recoil", "Weapon");
				DiagMenu.SetValue(DiagMenuIDs.DM_WEAPON_ALLOW_RECOIL, true);
				DiagMenu.RegisterBool(DiagMenuIDs.DM_WEAPON_UNLIMITED, "lalt+9", "Unlitimited Ammo", "Weapon");
	}
	
	void Update(float deltaT)
	{
		//---------------------------------------------
		// every system can handle their own actions locally(within the system itself),
		// this section is for systems which don't have client side representation,
		// or don't have their own tick, or for whatever other reason you prefer to put the logic here
		// PUT ONLY METHOD CALLS HERE, NO CODE !!
		// PUT ONLY METHOD CALLS HERE, NO CODE !!
		//---------------------------------------------
			
		CheckModifiers();//should modifiers be ticking
		CheckKillPlayer();//should i kill myself ?
		CheckCraftingDebugActions();
		CheckItemDebugActions();
		CheckAgentInjectActions();
		CheckGenerateRecipeCache();
		CheckDayzPlayerMenu();
		CheckSoftSkillsDebug();
		CheckSoftSkillsState();
		CheckSoftSkillsModel();
		CheckGunParticles();
		CheckSpecialtyLevel();
		CheckBloodyHands();
		CheckLifeSpanPlaytimeUpdate();
		CheckPermanentCrossHairUpdate();
		CheckToggleHud();
		CheckShowWeaponDebug();
		CheckInvincibility();
		CheckStamina();
		CheckAimNoise();
		CheckProceduralRecoil();
		CheckUnlimitedAmmo();
		CheckCraftingDump();
		CheckMeleeBlockStance();
		CheckMeleeFight();
		CheckEnvironmentStats();
		CheckDisplayMenu();
		CheckDrawCheckerboard();
		CheckBulletImpact();
	}
	//---------------------------------------------
	void CheckModifiers()
	{
		if( DiagMenu.GetBool(DiagMenuIDs.DM_CHEATS_MODIFIERS_ENABLE) )
		{
			if(!m_EnableModifiers)
			{
				SendModifiersRPC(true);
				m_EnableModifiers = true;
			}
		}
		else
		{
			if(m_EnableModifiers)
			{
				SendModifiersRPC(false);
				m_EnableModifiers = false;
			}
		}
	}
	//---------------------------------------------
	void CheckUnlimitedAmmo()
	{
		if( DiagMenu.GetBool(DiagMenuIDs.DM_WEAPON_UNLIMITED) )
		{
			if(!m_EnableUnlimitedAmmo)
			{
				SendUnlimitedRPC(true);
				m_EnableUnlimitedAmmo = true;
			}
		}
		else
		{
			if(m_EnableUnlimitedAmmo)
			{
				SendUnlimitedRPC(false);
				m_EnableUnlimitedAmmo = false;
			}
		}
	}
	//---------------------------------------------
	void CheckGenerateRecipeCache()
	{
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_CRAFTING_GENERATE) )
		{
			PluginRecipesManager plugin_recipes_manager;
			Class.CastTo(plugin_recipes_manager, GetPlugin(PluginRecipesManager));
			plugin_recipes_manager.CallbackGenerateCache();
			DiagMenu.SetValue(DiagMenuIDs.DM_PLAYER_CRAFTING_GENERATE, false);
		}
	}
	//---------------------------------------------
	void CheckKillPlayer()
	{
		if( DiagMenu.GetBool(DiagMenuIDs.DM_CHEATS_PLAYER_INSTAKILL) )
		{
			SendKillPlayerRPC();
			DiagMenu.SetValue(DiagMenuIDs.DM_CHEATS_PLAYER_INSTAKILL, false);//to prevent constant RPC calls, switch back to false
		}
	}
	//---------------------------------------------
	void CheckItemDebugActions()
	{
		if( DiagMenu.GetBool(DiagMenuIDs.DM_ITEM_DEBUG_ACTIONS_SHOW) )
		{
			if( (ItemBase.GetDebugActionsMask() & DebugActionType.GENERIC_ACTIONS) == 0 )
			{
				int test = ItemBase.GetDebugActionsMask();
				int mask = (ItemBase.GetDebugActionsMask() | DebugActionType.GENERIC_ACTIONS);
				ItemBase.SetDebugActionsMask(mask);
				SendDebugActionsRPC();
			}
		}
		else
		{
			if( (ItemBase.GetDebugActionsMask() & DebugActionType.GENERIC_ACTIONS) )
			{
				int mask2 = (ItemBase.GetDebugActionsMask() & (~DebugActionType.GENERIC_ACTIONS));
				ItemBase.SetDebugActionsMask(mask2);
				SendDebugActionsRPC();
			}
		}
	}

	void CheckBulletImpact()
	{
		if( DiagMenu.GetBool(DiagMenuIDs.DM_BULLET_IMPACT) )
		{
			PlayerBase pl_player = PlayerBase.Cast(GetGame().GetPlayer() );
			pl_player.SpawnBulletHitReaction();
			DiagMenu.SetValue(DiagMenuIDs.DM_BULLET_IMPACT, false);
		}
		else
		{
			
		}
	}
	
	//---------------------------------------------
	void CheckAgentInjectActions()
	{
		if( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_AGENTS_INJECTS_SHOW) )
		{
			if( (ItemBase.GetDebugActionsMask() & DebugActionType.PLAYER_AGENTS) == 0 )
			{
				int mask = (ItemBase.GetDebugActionsMask() | DebugActionType.PLAYER_AGENTS);
				ItemBase.SetDebugActionsMask(mask);	
				SendDebugActionsRPC();
			}
		}
		else
		{
			if( ItemBase.GetDebugActionsMask() & DebugActionType.PLAYER_AGENTS )
			{
				int mask2 = (ItemBase.GetDebugActionsMask() & (~DebugActionType.PLAYER_AGENTS));
				ItemBase.SetDebugActionsMask(mask2);
				SendDebugActionsRPC();
			}
		}
	}
	//---------------------------------------------
	/*
	void CheckAgentInjectActions()
	{
		if( DiagMenu.GetBool(DM_PLAYER_AGENTS_INJECTS_SHOW) )
		{
			if( !ItemBase.IsShowInjectActions() )
			{
				ItemBase.SetShowInjectActions(true);		
			}
		}
		else
		{
			if( ItemBase.IsShowInjectActions() )
			{
				ItemBase.SetShowInjectActions(false);		
			}
		}
	}
	*/
	
	//---------------------------------------------
	void CheckCraftingDump()
	{
		if( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_CRAFTING_DUMP) )
		{
			PluginRecipesManager plugin_recipes_manager;
			Class.CastTo(plugin_recipes_manager, GetPlugin(PluginRecipesManager));
			plugin_recipes_manager.GenerateHumanReadableRecipeList();
			DiagMenu.SetValue(DiagMenuIDs.DM_PLAYER_CRAFTING_DUMP, false);
		}
	}
	//---------------------------------------------
	
	
	//---------------------------------------------
	void CheckCraftingDebugActions()
	{
		if( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_CRAFTING_DEBUG_ACTIONS_ENABLE) )
		{
			if(!m_ShowCraftingDebugActions)
			{
				SendDebugCraftingRPC(true);
				PluginRecipesManager plugin_recipes_manager;
				Class.CastTo(plugin_recipes_manager, GetPlugin(PluginRecipesManager));
				plugin_recipes_manager.SetEnableDebugCrafting(true);
				m_ShowCraftingDebugActions = true;
			}
		}
		else
		{
			if(m_ShowCraftingDebugActions)
			{
				SendDebugCraftingRPC(false);
				PluginRecipesManager plugin_recipes_manager2;
				Class.CastTo(plugin_recipes_manager2, GetPlugin(PluginRecipesManager));
				plugin_recipes_manager2.SetEnableDebugCrafting(false);
				m_ShowCraftingDebugActions = false;
			}
		}
	}
	//---------------------------------------------
	void CheckInvincibility()
	{
		if( DiagMenu.GetBool(DiagMenuIDs.DM_CHEATS_INVINCIBILITY) )
		{
			if(!m_IsInvincible)	
			{
				SendInvincibilityRPC(true);
				m_IsInvincible = true;
			}
		}
		else
		{
			if(m_IsInvincible)	
			{
				SendInvincibilityRPC(false);
				m_IsInvincible = false;
			}
		}
	}

	//---------------------------------------------
	void CheckStamina()
	{
		if( DiagMenu.GetBool(DiagMenuIDs.DM_CHEATS_STAMINA_DISABLE) )
		{
			if(!m_StaminaDisabled)	
			{
				m_StaminaDisabled = true;
			}
		}
		else
		{
			if(m_StaminaDisabled)	
			{
				m_StaminaDisabled = false;
			}
		}
	}

	//---------------------------------------------
	void CheckLogPlayerStats()
	{
		if( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_STATS_LOG_ENABLE) )
		{
			if(!m_LogPlayerStats)	
			{
				SendLogPlayerStatsRPC(true);
				m_LogPlayerStats = true;
			}
		}
		else
		{
			if(m_LogPlayerStats)	
			{
				SendLogPlayerStatsRPC(false);
				m_LogPlayerStats = false;
			}
		}
	}

	//---------------------------------------------
	void CheckSoftSkillsDebug()
	{
		PlayerBase player;
		SoftSkillManagerDebug m_DebugWindow;

		if( DiagMenu.GetBool( DiagMenuIDs.DM_SOFT_SKILLS_SHOW_DEBUG ) )
		{
			if(!m_SoftSkillsDebug)	
			{
				Class.CastTo(player, GetGame().GetPlayer());
				player.GetSoftSkillManager().CreateDebugWindow( true );
				SoftSkillsShowDebugRPC(true);
				m_SoftSkillsDebug = true;
			}
		}
		else
		{
			if(m_SoftSkillsDebug)	
			{
				Class.CastTo(player, GetGame().GetPlayer());
				player.GetSoftSkillManager().CreateDebugWindow( false );
				SoftSkillsShowDebugRPC(false);
				m_SoftSkillsDebug = false;
			}
		}
	}

	//---------------------------------------------
	void CheckSoftSkillsState()
	{
		if( DiagMenu.GetBool( DiagMenuIDs.DM_SOFT_SKILLS_TOGGLE_STATE ) )
		{
			if(!m_SoftSkillsState)	
			{
				SoftSkillsToggleStateRPC(true);
				m_SoftSkillsState = true;
			}
		}
		else
		{
			if(m_SoftSkillsState)	
			{
				SoftSkillsToggleStateRPC(false);
				m_SoftSkillsState = false;
			}
		}
	}
		
	//---------------------------------------------
	void CheckGunParticles()
	{
		if( DiagMenu.GetBool( DiagMenuIDs.DM_GUN_PARTICLES ) )
		{
			if(!m_GunParticlesState)
			{
				GunParticlesToggleStateRPC(false);
				m_GunParticlesState = true;
			}
		}
		else
		{
			if(m_GunParticlesState)
			{
				GunParticlesToggleStateRPC(true);
				m_GunParticlesState = false;
			}
		}
	}

	//---------------------------------------------
	void CheckSoftSkillsModel()
	{
		if( DiagMenu.GetBool( DiagMenuIDs.DM_SOFT_SKILLS_TOGGLE_MODEL ) )
		{
			if(!m_SoftSkillsModel)	
			{
				SoftSkillsToggleModelRPC(true);
				m_SoftSkillsModel = true;
			}
		}
		else
		{
			if(m_SoftSkillsModel)	
			{
				SoftSkillsToggleModelRPC(false);
				m_SoftSkillsModel = false;
			}
		}
	}

	//---------------------------------------------
	void CheckSpecialtyLevel()
	{
		float specialty_value = DiagMenu.GetRangeValue( DiagMenuIDs.DM_SOFT_SKILLS_SPECIALTY_VALUE );

		if( m_SpecialtyLevel != specialty_value )
		{
			SoftSkillsSetSpecialtyRPC( specialty_value );
			m_SpecialtyLevel = specialty_value;
		}
	}

	//---------------------------------------------
	void CheckBloodyHands()
	{
		if( DiagMenu.GetBool( DiagMenuIDs.DM_BLOODY_HANDS ) )
		{
			if(!m_BloodyHands)	
			{
				LifespanBloodyHandsRPC(true);
				m_BloodyHands = true;
			}
		}
		else
		{
			if(m_BloodyHands)	
			{
				LifespanBloodyHandsRPC(false);
				m_BloodyHands = false;
			}
		}
	}

	//---------------------------------------------
	void CheckLifeSpanPlaytimeUpdate()
	{
		float lifespan_level = DiagMenu.GetRangeValue( DiagMenuIDs.DM_LIFESPAN_PLAYTIME_UPDATE );

		if( m_LifespanLevel != lifespan_level )
		{
			LifespanPlaytimeUpdateRPC( lifespan_level );
			m_LifespanLevel = lifespan_level;
		}
	}
	//---------------------------------------------	
	void CheckPermanentCrossHairUpdate()
	{
		PluginPermanentCrossHair crosshair;
		if( DiagMenu.GetBool( DiagMenuIDs.DM_PERMANENT_CROSSHAIR ) )
		{
			if(!m_PermanentCrossHair)	
			{
				Class.CastTo(crosshair, GetPlugin( PluginPermanentCrossHair ));
				crosshair.SwitchPermanentCrossHair(true);
				m_PermanentCrossHair = true;
			}
		}
		else
		{
			if(m_PermanentCrossHair)	
			{
				Class.CastTo(crosshair, GetPlugin( PluginPermanentCrossHair ));
				crosshair.SwitchPermanentCrossHair(false);
				m_PermanentCrossHair = false;
			}
		}
	}

	//---------------------------------------------	
	void CheckToggleHud()
	{
		Hud hud;
			
		if( DiagMenu.GetBool( DiagMenuIDs.DM_TOGGLE_HUD ) )
		{
			if(!m_ToggleHud)	
			{
				hud = GetGame().GetMission().GetHud();
				hud.Show( m_ToggleHud );
				GetGame().GetMission().HideCrosshairVisibility();
				m_ToggleHud = true;
			}
		}
		else
		{
			if(m_ToggleHud)	
			{
				hud = GetGame().GetMission().GetHud();
				hud.Show( m_ToggleHud );
				GetGame().GetMission().RefreshCrosshairVisibility();
				m_ToggleHud = false;
			}
		}
	}

	//---------------------------------------------	
	void CheckShowWeaponDebug()
	{
		PlayerBase player;
		if( DiagMenu.GetBool( DiagMenuIDs.DM_WEAPON_DEBUG_ENABLE ) )
		{
			if(!m_ShowingWeaponDebug)	
			{

				Class.CastTo(player, GetGame().GetPlayer());
				player.ShowWeaponDebug(true);
				m_ShowingWeaponDebug = true;
			}
		}
		else
		{
			if(m_ShowingWeaponDebug)	
			{
				Class.CastTo(player, GetGame().GetPlayer());
				player.ShowWeaponDebug(false);
				m_ShowingWeaponDebug = false;
			}
		}
	}
	//---------------------------------------------	
	void CheckProceduralRecoil()
	{
		DayZPlayerImplement player;
		if( DiagMenu.GetBool( DiagMenuIDs.DM_WEAPON_ALLOW_RECOIL ) )
		{
			if(!m_ProceduralRecoilEnabled)
			{
				Class.CastTo(player, GetGame().GetPlayer());
				player.GetAimingModel().SetProceduralRecoilEnabled(true);
				m_ProceduralRecoilEnabled = true;
			}
		}
		else
		{
			if(m_ProceduralRecoilEnabled)	
			{
				Class.CastTo(player, GetGame().GetPlayer());
				player.GetAimingModel().SetProceduralRecoilEnabled(false);
				m_ProceduralRecoilEnabled = false;
			}
		}
	}
	//---------------------------------------------	
	void CheckAimNoise()
	{
		DayZPlayerImplement player;
		if( DiagMenu.GetBool( DiagMenuIDs.DM_WEAPON_AIM_NOISE ) )
		{
			if(!m_AimNoiseEnabled)
			{

				Class.CastTo(player, GetGame().GetPlayer());
				player.GetAimingModel().SetAimNoiseAllowed(true);
				m_AimNoiseEnabled = true;
			}
		}
		else
		{
			if(m_AimNoiseEnabled)	
			{
				Class.CastTo(player, GetGame().GetPlayer());
				player.GetAimingModel().SetAimNoiseAllowed(false);
				m_AimNoiseEnabled = false;
			}
		}
	}

	//---------------------------------------------
	void CheckDayzPlayerMenu()
	{
		int	actMenuValue = DiagMenu.GetValue(DayZPlayerConstants.DEBUG_SHOWDEBUGPLUGIN);
		// Print("Debug Menu:" + actMenuValue.ToString());
		if (actMenuValue != m_DayzPlayerDebugMenu)
		{
			if (g_Plugins != NULL)
			{
				PluginDayzPlayerDebug pd;
				Class.CastTo(pd, g_Plugins.GetPluginByType(PluginDayzPlayerDebug));
				if (pd != NULL)
				{
					pd.ToggleDebugWindowSetMode(actMenuValue);
					//Print("DayzPlayerDebug - switched to:" + actMenuValue.ToString());
				}
			}
			m_DayzPlayerDebugMenu = actMenuValue;
		}

	}
	//---------------------------------------------
	
	void CheckDisplayMenu()
	{
		PluginRemotePlayerDebugClient plugin_remote_client;
		int value = DiagMenu.GetRangeValue( DiagMenuIDs.DM_DISPLAY_PLAYER_INFO );
		
		if( value != 0 )
		{
			if( m_DisplayPlayerInfo != value )
			{
				plugin_remote_client = PluginRemotePlayerDebugClient.Cast( GetPlugin(PluginRemotePlayerDebugClient) );
				plugin_remote_client.RequestPlayerInfo( PlayerBase.Cast(GetGame().GetPlayer()), value );
				
				m_DisplayPlayerInfo = value;
			}
		}
		else
		{
			if( m_DisplayPlayerInfo != 0 )	
			{
				plugin_remote_client = PluginRemotePlayerDebugClient.Cast( GetPlugin(PluginRemotePlayerDebugClient) );
				plugin_remote_client.RequestPlayerInfo( PlayerBase.Cast(GetGame().GetPlayer()), 0 );
				
				m_DisplayPlayerInfo = 0;
			}
		}
	}

	
	
	//---------------------------------------------
	void CheckMeleeBlockStance()
	{
		if( DiagMenu.GetBool(DiagMenuIDs.DM_MELEE_DEBUG_ENABLE) && DiagMenu.GetBool(DiagMenuIDs.DM_MELEE_BLOCK_STANCE) )
		{
			if(!m_MeleeBlockStance)
			{
				SendMeleeBlockStanceRPC(true);
				m_MeleeBlockStance = true;
			}
		}
		else
		{
			if(m_MeleeBlockStance)
			{
				SendMeleeBlockStanceRPC(false);
				m_MeleeBlockStance = false;
			}
		}
	}

	//---------------------------------------------	
	void CheckMeleeFight()
	{
		if( DiagMenu.GetBool(DiagMenuIDs.DM_MELEE_DEBUG_ENABLE) && DiagMenu.GetBool(DiagMenuIDs.DM_MELEE_FIGHT_SINGLE) )
		{
			SendMeleeFightRPC(DiagMenu.GetBool(DiagMenuIDs.DM_MELEE_FIGHT_SINGLE));
			m_MeleeFight = true;
		}
		else
		{
			if(m_MeleeFight)
			{
				SendMeleeFightRPC(false);
				m_MeleeFight = false;
			}
		}
	}
	
	void CheckEnvironmentStats()
	{
		if(DiagMenu.GetBool(DiagMenuIDs.DM_ENVIRONMENT_DEBUG_ENABLE))
		{
			if(!m_EnvironmentStats)
				m_EnvironmentStats = true;
		}
		else
		{
			if(m_EnvironmentStats)
				m_EnvironmentStats = false;
		}
	}
	
	void CheckDrawCheckerboard()
	{
		if(DiagMenu.GetBool(DiagMenuIDs.DM_DRAW_CHECKERBOARD))
		{
			if(!m_DrawCheckerboard) m_DrawCheckerboard = true;
		}
		else
		{
			if(m_DrawCheckerboard) m_DrawCheckerboard = false;
		}
	}
	
	//---------------------------------------------
	void SendDebugActionsRPC()
	{
		CashedObjectsParams.PARAM1_INT.param1 = ItemBase.GetDebugActionsMask();
		int mask = CashedObjectsParams.PARAM1_INT.param1;
		if( GetGame() && GetGame().GetPlayer() ) GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_ITEM_DEBUG_ACTIONS, CashedObjectsParams.PARAM1_INT, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendKillPlayerRPC()
	{
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_KILL_PLAYER, NULL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendInvincibilityRPC(bool enable)
	{
		CashedObjectsParams.PARAM1_BOOL.param1 = enable;
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_ENABLE_INVINCIBILITY, CashedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendLogPlayerStatsRPC(bool enable)
	{
		CashedObjectsParams.PARAM1_BOOL.param1 = enable;
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_LOG_PLAYER_STATS, CashedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendModifiersRPC(bool enable)
	{
		CashedObjectsParams.PARAM1_BOOL.param1 = enable;
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_DISABLE_MODIFIERS, CashedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendUnlimitedRPC(bool enable)
	{
		CashedObjectsParams.PARAM1_BOOL.param1 = enable;
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_RPC_UNLIMITED_AMMO, CashedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendDebugCraftingRPC(bool enable)
	{
		CashedObjectsParams.PARAM1_BOOL.param1 = enable;
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_CRAFTING_DEBUG, CashedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SoftSkillsShowDebugRPC(bool enable)
	{
		CashedObjectsParams.PARAM1_BOOL.param1 = enable;
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_SOFT_SKILLS_DEBUG_WINDOW, CashedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SoftSkillsToggleStateRPC(bool enable)
	{
		CashedObjectsParams.PARAM1_BOOL.param1 = enable;
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_SOFT_SKILLS_TOGGLE_STATE, CashedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void GunParticlesToggleStateRPC(bool enable)
	{
		CashedObjectsParams.PARAM1_BOOL.param1 = enable;
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_GUN_PARTICLES_TOGGLE, CashedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SoftSkillsToggleModelRPC(bool enable)
	{
		CashedObjectsParams.PARAM1_BOOL.param1 = enable;
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_SOFT_SKILLS_TOGGLE_MODEL, CashedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SoftSkillsSetSpecialtyRPC( float specialty_value )
	{
		CashedObjectsParams.PARAM1_FLOAT.param1 = specialty_value;
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_SOFT_SKILLS_SET_SPECIALTY, CashedObjectsParams.PARAM1_FLOAT, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void LifespanBloodyHandsRPC(bool enable)
	{
		CashedObjectsParams.PARAM1_BOOL.param1 = enable;
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_LIFESPAN_BLOODY_HANDS, CashedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void LifespanPlaytimeUpdateRPC( float playtime )
	{
		CashedObjectsParams.PARAM1_FLOAT.param1 = playtime;
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.RPC_LIFESPAN_PLAYTIME_UPDATE, CashedObjectsParams.PARAM1_FLOAT, true, GetGame().GetPlayer().GetIdentity() );
 	}
	//---------------------------------------------
	void SendMeleeBlockStanceRPC(bool enable)
	{
		CashedObjectsParams.PARAM1_BOOL.param1 = enable;
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_RPC_MELEE_BLOCK_STANCE, CashedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
	}
	//---------------------------------------------
	void SendMeleeFightRPC(bool enable)
	{
		CashedObjectsParams.PARAM1_BOOL.param1 = enable;
		GetGame().RPCSingleParam( GetGame().GetPlayer(),ERPCs.DEV_RPC_MELEE_FIGHT, CashedObjectsParams.PARAM1_BOOL, true, GetGame().GetPlayer().GetIdentity() );
	}
	//---------------------------------------------
	void OnRPC(PlayerBase player, int rpc_type, ParamsReadContext ctx)
	{
		switch(rpc_type)
		{
			case ERPCs.DEV_RPC_UNLIMITED_AMMO:
				ctx.Read(CashedObjectsParams.PARAM1_BOOL);
				bool enabled = CashedObjectsParams.PARAM1_BOOL.param1;
				if(enabled)
				{			
					ItemBase.SetDebugActionsMask( ItemBase.GetDebugActionsMask() | DebugActionType.UNLIMITED_AMMO );
				}
				else
				{
					ItemBase.SetDebugActionsMask( ItemBase.GetDebugActionsMask() & (~DebugActionType.UNLIMITED_AMMO) );			
				}
			break;
			
			case ERPCs.RPC_DISABLE_MODIFIERS:
				ctx.Read(CashedObjectsParams.PARAM1_BOOL);
				bool enable = CashedObjectsParams.PARAM1_BOOL.param1;
				player.SetModifiers( enable );
			break;
			
			case ERPCs.RPC_KILL_PLAYER:
				player.SetHealth("","", -1);
			break;

			case ERPCs.RPC_ENABLE_INVINCIBILITY:
				ctx.Read(CashedObjectsParams.PARAM1_BOOL);
				bool value = CashedObjectsParams.PARAM1_BOOL.param1;
				player.SetAllowDamage(!value);
			break;

			case ERPCs.RPC_ITEM_DEBUG_ACTIONS:
				ctx.Read(CashedObjectsParams.PARAM1_INT);
				int mask = CashedObjectsParams.PARAM1_INT.param1;
				ItemBase.SetDebugActionsMask(mask);
			break;

			case ERPCs.RPC_LOG_PLAYER_STATS:
				ctx.Read(CashedObjectsParams.PARAM1_BOOL);
				player.GetPlayerStats().SetAllowLogs(CashedObjectsParams.PARAM1_BOOL.param1);
			break;

			case ERPCs.RPC_SOFT_SKILLS_TOGGLE_STATE:
				ctx.Read(CashedObjectsParams.PARAM1_BOOL);
				player.GetSoftSkillManager().SetSoftSkillsState(CashedObjectsParams.PARAM1_BOOL.param1);
			break;

			case ERPCs.RPC_SOFT_SKILLS_DEBUG_WINDOW:
				ctx.Read(CashedObjectsParams.PARAM1_BOOL);
				bool show = CashedObjectsParams.PARAM1_BOOL.param1;
				
				if( show )
				{
					player.GetSoftSkillManager().StartSynchTimer();
				}
				else
				{
					player.GetSoftSkillManager().StopSynchTimer();
					player.GetSoftSkillManager().ResetDebugWindow();
				}
				
			break;
		
			case ERPCs.RPC_GUN_PARTICLES_TOGGLE:
				ctx.Read(CashedObjectsParams.PARAM1_BOOL);
				PrtTest.m_GunParticlesState = CashedObjectsParams.PARAM1_BOOL.param1;
			break;

			case ERPCs.RPC_CRAFTING_DEBUG:
				ctx.Read(CashedObjectsParams.PARAM1_BOOL);
				PluginRecipesManager plugin_recipes_manager;
				Class.CastTo(plugin_recipes_manager, GetPlugin(PluginRecipesManager));
				plugin_recipes_manager.SetEnableDebugCrafting(CashedObjectsParams.PARAM1_BOOL.param1);
			break;

			case ERPCs.RPC_SOFT_SKILLS_TOGGLE_MODEL:
				ctx.Read(CashedObjectsParams.PARAM1_BOOL);
				player.GetSoftSkillManager().SetLinearState(CashedObjectsParams.PARAM1_BOOL.param1) ;
			break;

			case ERPCs.RPC_SOFT_SKILLS_SET_SPECIALTY:
				ctx.Read( CashedObjectsParams.PARAM1_FLOAT );
				float specialty = CashedObjectsParams.PARAM1_FLOAT.param1;
				SoftSkillsManager soft_skill_manager = player.GetSoftSkillManager();
				soft_skill_manager.SetSpecialtyLevel( specialty );
				soft_skill_manager.SynchSpecialtyLevel();
				player.GetStatSpecialty().Set( specialty );
			break;

			case ERPCs.RPC_LIFESPAN_BLOODY_HANDS:
				ctx.Read( CashedObjectsParams.PARAM1_BOOL );
				bool bloody_hands = CashedObjectsParams.PARAM1_BOOL.param1;
				PluginLifespan lifespan_bloody_hands;
				Class.CastTo(lifespan_bloody_hands, GetPlugin( PluginLifespan ));
				lifespan_bloody_hands.UpdateBloodyHandsVisibility( player, bloody_hands );
			break;

			case ERPCs.RPC_LIFESPAN_PLAYTIME_UPDATE:
				ctx.Read( CashedObjectsParams.PARAM1_FLOAT );
				//reset players playtime
				float playtime = player.StatGet("playtime");
				float opposite_playtime = playtime * ( -1 );
				player.StatUpdate("playtime", opposite_playtime );
				//set new playtime
				float playtime_update = CashedObjectsParams.PARAM1_FLOAT.param1;
				player.StatUpdate("playtime", playtime_update );
				player.SetLastShavedSeconds( 0 );
				//update lifespan
				PluginLifespan module_lifespan_update;
				Class.CastTo(module_lifespan_update, GetPlugin( PluginLifespan ));
				module_lifespan_update.UpdateLifespan( player, true );
			break;
			case ERPCs.DEV_RPC_MELEE_BLOCK_STANCE:
				ctx.Read(CashedObjectsParams.PARAM1_BOOL);
				bool blockStanceEnabled = CashedObjectsParams.PARAM1_BOOL.param1;
				player.SetMeleeBlockingStance(blockStanceEnabled);
			break;
			case ERPCs.DEV_RPC_MELEE_FIGHT:
				ctx.Read(CashedObjectsParams.PARAM1_BOOL);
				bool fightEnabled = CashedObjectsParams.PARAM1_BOOL.param1;
				player.SetMeleeFight(fightEnabled);
			break;
		}
		
	}
#endif
}