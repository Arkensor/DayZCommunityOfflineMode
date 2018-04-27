class WelcomeManager extends UIScriptedMenu
{
	void WelcomeManager()
	{
	}	
	
	void ~WelcomeManager()
	{
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		Close();
		
		return true;
	}
}

class MissionGameplay extends MissionBase
{
	int		m_life_state;
	float 	m_volume_sound;
	float 	m_volume_speechEX;
	float 	m_volume_music;
	float 	m_volume_VOIP;
	float 	m_volume_radio;
	bool	m_initialized;
	bool	m_invUpdateThisFrame;
	protected bool		m_IsOpenPauseMenu;
	protected UIManager	m_UIManager;
	
	Widget			m_hud_root_widget;
	ImageWidget m_microphone_icon;
	
	private ref InventoryManager 	m_drag_manager;
	private ref WidgetCache 		m_Widgets_Cache;
	private ref WidgetEventHandler 	m_widget_event_handler;
	
	InventoryMenu 					m_inventory_menu;
	ref InventoryMenuNew			m_inventory_menu_new;
	ref Chat 						m_chat;
	ref ActionMenu					m_actionMenu;
	ref IngameHud					m_hud;
	ref HudDebug					m_hud_debug;
	ref LogoutMenu					m_logout;
	ref DebugMonitor				m_debugMonitor;
	
	ref Timer						m_chat_channel_hide_timer;
	ref WidgetFadeTimer				m_chat_channel_fade_timer;
	
	Widget							m_chat_channel_area;
	TextWidget						m_chat_channel_text;
	NoteMenu 						m_note;
	
	protected ref Timer				m_ToggleHudTimer;
	protected const int 			HOLD_LIMIT_TIME	= 300; //ms
	protected int					m_ActionDownTime;
	protected int					m_ActionUpTime;
	
	//For freecam and utils
    PlayerBase m_oPlayer;
	Camera m_oCamera;
	bool m_bDebugMonitor = false;
	bool m_bGodMode = false;
	bool m_bWelcomeSeen = false;
	private ref WelcomeManager m_oWelcomeManager = new WelcomeManager;

	//For keyhandler
	bool m_IsCtrlHolding = false;
	bool m_IsWinHolding = false;
	bool m_IsLeftAltHolding = false;
	bool m_IsRightAltHolding = false;
	bool m_IsLeftShiftHolding = false;
	bool m_IsRightShiftHolding = false;
	
	void MissionGameplay()
	{
		m_initialized = false;
		m_hud_root_widget = NULL;
		m_chat = new Chat;
		m_actionMenu = new ActionMenu;
		m_life_state = -1;
		m_hud = new IngameHud;
		m_chat_channel_fade_timer = new WidgetFadeTimer;
		m_chat_channel_hide_timer = new Timer(CALL_CATEGORY_GUI);
		
		m_ToggleHudTimer = new Timer(CALL_CATEGORY_GUI);

		m_drag_manager = new InventoryManager;
		m_widget_event_handler = new WidgetEventHandler;
		g_Game.m_loadingScreenOn = true;
	}
	
	void ~MissionGameplay()
	{
		DestroyInventory();
		
		//GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.UpdateDebugMonitor);
	#ifndef NO_GUI
		if (g_Game && g_Game.GetUIManager() && g_Game.GetUIManager().ScreenFadeVisible())
		{
			g_Game.GetUIManager().ScreenFadeOut(0);
		}
	#endif
	}
	
	void SpawnPlayer()
	{
		m_oPlayer = PlayerBase.Cast( GetGame().CreatePlayer( NULL, GetGame().CreateRandomPlayer(), "0 0 0", 0, "NONE") );
		
		GetGame().SelectPlayer( NULL, m_oPlayer );
		
		EntityAI item = m_oPlayer.GetInventory().CreateInInventory( "AviatorGlasses" );
		
		item = m_oPlayer.GetInventory().CreateInInventory( "MilitaryBeret_UN" );

		item = m_oPlayer.GetInventory().CreateInInventory( "M65Jacket_Black" );
		
		item = m_oPlayer.GetInventory().CreateInInventory( "PlateCarrierHolster" );
		
		item = m_oPlayer.GetInventory().CreateInInventory( "TacticalGloves_Black" );
		
		item = m_oPlayer.GetInventory().CreateInInventory( "HunterPants_Autumn" );
		
		item = m_oPlayer.GetInventory().CreateInInventory( "MilitaryBoots_Black" );
		
		item = m_oPlayer.GetInventory().CreateInInventory( "AliceBag_Camo" );

		// item = m_oPlayer.GetInventory().CreateInInventory( "UMP45" );
		// item.GetInventory().CreateAttachment( "Mag_UMP_25Rnd" );
		// item.GetInventory().CreateAttachment( "ReflexOptic" );
		// item.GetInventory().CreateAttachment( "PistolSuppressor" );
		// m_oPlayer.GetInventory().CreateInInventory( "Mag_UMP_25Rnd" );
		// m_oPlayer.GetInventory().CreateInInventory( "Mag_UMP_25Rnd" );
		// m_oPlayer.GetInventory().CreateInInventory( "Mag_UMP_25Rnd" );
		
		item = m_oPlayer.GetInventory().CreateInInventory( "AKM" );
		
		item.GetInventory().CreateAttachment( "AK74_Hndgrd" );
		item.GetInventory().CreateAttachment( "AK_Suppressor" );
		item.GetInventory().CreateAttachment( "AK_WoodBttstck" );
		item.GetInventory().CreateAttachment( "Mag_AKM_30Rnd" );
		m_oPlayer.GetInventory().CreateInInventory( "Mag_AKM_30Rnd" );
		m_oPlayer.GetInventory().CreateInInventory( "Mag_AKM_30Rnd" );
		m_oPlayer.GetInventory().CreateInInventory( "Mag_AKM_30Rnd" );
		
		m_oPlayer.LocalTakeEntityToHands( item );
		
		m_oPlayer.SetQuickBarEntityShortcut( item, 0, true );

		m_oPlayer.SetAllowDamage( false );
		
		// Cherno downtown 
		// vector position = "6623.335938 23.480206 2418.818848"; 
		// vector direction = "0.520912 0.000000 0.853610";
		
		// Cherno outer city
		// vector position = "6564.447754 110.249420 3403.791748";
		// vector direction = "0.614116 0.000000 -0.789216";
		
		// Airfield
		vector position = "4603.797363 342.474487 10492.90722";
		vector direction = "0.893051 0.000000 -0.449954";
		
		m_oPlayer.SetPosition( position );
		m_oPlayer.SetDirection( direction );
	}
	
	InventoryMenuNew GetInventory()
	{
		return m_inventory_menu_new;
	}
	
	override void OnInit()
	{
		if (m_initialized)
		{
			return;
		}

		m_UIManager = GetGame().GetUIManager();
			
		g_Game.m_loadingScreenOn = true;
		m_initialized = true;

		// init hud ui
		if (!m_hud_root_widget)
		{
			m_hud_root_widget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_hud.layout");
			
			m_hud_root_widget.Show(false);
			m_chat.Init(m_hud_root_widget.FindAnyWidget("ChatFrameWidget"));
			m_actionMenu.Init( TextWidget.Cast( m_hud_root_widget.FindAnyWidget("DefaultActionWidget") ) );
			m_hud.Init( m_hud_root_widget.FindAnyWidget("HudPanel") );
			m_microphone_icon = ImageWidget.Cast( m_hud_root_widget.FindAnyWidget("mic") );

			// notification window
			m_notification_widget = new NotificationMessage( m_hud_root_widget );
			
			// chat channel
			m_chat_channel_area = m_hud_root_widget.FindAnyWidget("ChatChannelPanel");
			m_chat_channel_text = TextWidget.Cast( m_hud_root_widget.FindAnyWidget("ChatChannelText") );
		}
		
		// init hud ui
		
		if ( GetGame().IsDebug() )
		{
			m_hud_debug = new HudDebug;
			
			if ( !m_hud_debug.IsInitialized() )
			{
				m_hud_debug.Init( GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_hud_debug.layout") );
				
				Debug.SetEnabledLogs(PluginConfigDebugProfile.GetInstance().GetLogsEnabled());
			}
		}

		//AIBehaviourHL.RegAIBehaviour("zombie2",AIBehaviourHLZombie2,AIBehaviourHLDataZombie2);
		//RegBehaviour("zombie2",AIBehaviourHLZombie2,AIBehaviourHLDataZombie2);
		
		m_Widgets_Cache = new WidgetCache;
		
		m_volume_sound = GetGame().GetSoundScene().GetSoundVolume();
		m_volume_speechEX = GetGame().GetSoundScene().GetSpeechExVolume();
		m_volume_music = GetGame().GetSoundScene().GetMusicVolume();
		m_volume_VOIP = GetGame().GetSoundScene().GetVOIPVolume();
		m_volume_radio = GetGame().GetSoundScene().GetRadioVolume();
		
		SpawnPlayer();
	}

	UIManager GetUIManager()
	{
		return m_UIManager;
	}

	override void OnMissionStart()
	{
		//does not display HUD until player is fully loaded
		m_hud_root_widget.Show(true);
		GetUIManager().ShowUICursor(false);
		g_Game.SetMissionState( DayZGame.MISSION_STATE_GAME );
		
		CreateDebugMonitor();
		m_debugMonitor.Hide();
	}
	
	void InitInventory()
	{
		if (m_inventory_menu_new == NULL)
		{
			m_inventory_menu_new = InventoryMenuNew.Cast( GetUIManager().EnterScriptedMenu(MENU_INVENTORY, NULL) );
		}
	}

	WidgetCache GetWidgetCache()
	{
		return m_Widgets_Cache;
	}
	
	void TickScheduler(float timeslice)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
				
		if( player) player.OnTick();
	}
	
	
	override void OnMissionFinish()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if ( player != NULL )
		{
			int player_last_shaved_seconds = player.GetLastShavedSeconds();
			GetGame().SetProfileString( "lastShavedSeconds", player_last_shaved_seconds.ToString() );
		}
		Print("OnMissionFinish");
		GetUIManager().HideDialog();
		DestroyAllMenus();
		
		m_chat.Destroy();
		delete m_hud_root_widget;
		
		if (m_debugMonitor)
			m_debugMonitor.Hide();
		g_Game.GetUIManager().ShowUICursor(false);
		g_Game.SetMissionState( DayZGame.MISSION_STATE_FINNISH );
	}
	
	override void OnUpdate(float timeslice)
	{
		Man player = GetGame().GetPlayer();
		PlayerBase playerPB = PlayerBase.Cast(player);
		TickScheduler(timeslice);
		UpdateDummyScheduler();//for external entities
		UIScriptedMenu menu = m_UIManager.GetMenu();
		InventoryMenuNew inventory = InventoryMenuNew.Cast( m_UIManager.FindMenu(MENU_INVENTORY) );
		//m_inventory_menu_new = inventory;
		InspectMenuNew inspect = InspectMenuNew.Cast( m_UIManager.FindMenu(MENU_INSPECT) );
		Input input = GetGame().GetInput();
		
		if( m_bGodMode )
		{
			player.SetHealth( player.GetMaxHealth( "", "" ) );
			player.SetHealth( "","Blood", player.GetMaxHealth( "", "Blood" ) );
		}
		
		UpdateDebugMonitor();
		
		//TODO should be switchable
		if (playerPB && playerPB.enterNoteMenuRead)
		{
			playerPB.enterNoteMenuRead = false;
			Paper paper = Paper.Cast(playerPB.GetItemInHands());
			m_note = NoteMenu.Cast( GetUIManager().EnterScriptedMenu(MENU_NOTE, menu) ); //NULL means no parent
			m_note.InitRead(paper.m_AdvancedText);
		}
		
		if (playerPB && playerPB.enterNoteMenuWrite)
		{
			playerPB.enterNoteMenuWrite = false;
			m_note = NoteMenu.Cast( GetUIManager().EnterScriptedMenu(MENU_NOTE, menu) ); //NULL means no parent
			m_note.InitWrite(playerPB.m_paper,playerPB.m_writingImplement,playerPB.m_Handwriting);
		}
		
		if (inspect && player.GetHumanInventory().CanOpenInventory() == false)
		{
			m_UIManager.CloseMenu(MENU_INSPECT);
		}

		//Quick Reload Weapon
		if ( GetGame().IsDebug() )
		{
			if ( input.GetActionDown( UAQuickReload, false ) )
			{
				if ( !GetGame().IsInventoryOpen() && !playerPB.GetActionManager().FindActionTarget().GetObject() )
				{
					EntityAI entity_hands = playerPB.GetHumanInventory().GetEntityInHands();
					
					if ( entity_hands && entity_hands.IsWeapon() )
					{
						playerPB.QuickReloadWeapon( entity_hands );
					}
				}
			}
		}
		
#ifdef PLATFORM_XBOX		
		//Switch beween weapons in quickslots 
		if( input.GetActionDown( UAUIRadialMenuPick, false ) )
		{
			if ( !GetGame().IsInventoryOpen() )
			{
				EntityAI entity_in_hands = playerPB.GetHumanInventory().GetEntityInHands();
				EntityAI quickbar_entity;
				int quickbar_index = 0;
				
				if ( entity_in_hands )
				{			
					int quickbar_entity_hands_index = playerPB.FindQuickBarEntityIndex( entity_in_hands );
					
					if ( quickbar_entity_hands_index > -1 && quickbar_entity_hands_index < MAX_QUICKBAR_SLOTS_COUNT - 1 )	//(0->8)
					{
						quickbar_index = quickbar_entity_hands_index + 1;
					}
				}

				//find next weapon
				for ( int iter = 0; iter < MAX_QUICKBAR_SLOTS_COUNT; ++iter )
				{
					quickbar_entity = playerPB.GetQuickBarEntity( quickbar_index );
					
					if ( quickbar_entity && ( quickbar_entity.IsWeapon() || ( quickbar_entity.IsMeleeWeapon() && !quickbar_entity.IsMagazine() ) )
					{
						break;
					}
					
					quickbar_index += 1;
					if ( quickbar_index > MAX_QUICKBAR_SLOTS_COUNT - 1 )
					{
						quickbar_index = 0;	//reset
					}
				}
				
				//swap
				int slot_id;
				if ( quickbar_index > -1 )
				{
					slot_id = quickbar_index + 1;
					if ( slot_id == MAX_QUICKBAR_SLOTS_COUNT )
					{
						slot_id = 0;
					}
					
					playerPB.RadialQuickBarSingleUse( slot_id );
				}
			}
		}
#endif

#ifdef PLATFORM_PS4		
		//Switch beween weapons in quickslots 
		if( input.GetActionDown( UAUIRadialMenuPick, false ) )
		{
			if ( !GetGame().IsInventoryOpen() )
			{
				EntityAI entity_in_hands = playerPB.GetHumanInventory().GetEntityInHands();
				EntityAI quickbar_entity;
				int quickbar_index = 0;
				
				if ( entity_in_hands )
				{			
					int quickbar_entity_hands_index = playerPB.FindQuickBarEntityIndex( entity_in_hands );
					
					if ( quickbar_entity_hands_index > -1 && quickbar_entity_hands_index < MAX_QUICKBAR_SLOTS_COUNT - 1 )	//(0->8)
					{
						quickbar_index = quickbar_entity_hands_index + 1;
					}
				}

				//find next weapon
				for ( int iter = 0; iter < MAX_QUICKBAR_SLOTS_COUNT; ++iter )
				{
					quickbar_entity = playerPB.GetQuickBarEntity( quickbar_index );
					
					if ( quickbar_entity && ( quickbar_entity.IsWeapon() || ( quickbar_entity.IsMeleeWeapon() && !quickbar_entity.IsMagazine() ) )
					{
						break;
					}
					
					quickbar_index += 1;
					if ( quickbar_index > MAX_QUICKBAR_SLOTS_COUNT - 1 )
					{
						quickbar_index = 0;	//reset
					}
				}
				
				//swap
				int slot_id;
				if ( quickbar_index > -1 )
				{
					slot_id = quickbar_index + 1;
					if ( slot_id == MAX_QUICKBAR_SLOTS_COUNT )
					{
						slot_id = 0;
					}
					
					playerPB.RadialQuickBarSingleUse( slot_id );
				}
			}
		}
#endif

#ifdef PLATFORM_XBOX		
		//Gestures
		if(input.GetActionDown(UAUIGesturesOpen, false))
		{
			//open gestures menu
			if ( !GetUIManager().IsMenuOpen( MENU_GESTURES ) )
			{
				GesturesMenu.OpenMenu();
			}
		}
		
		if(input.GetActionUp(UAUIGesturesOpen, false))
		{
			//close gestures menu
			if ( GetUIManager().IsMenuOpen( MENU_GESTURES ) )
			{
				GesturesMenu.CloseMenu();
			}
		}
#endif	

#ifdef PLATFORM_PS4		
		//Gestures
		if(input.GetActionDown(UAUIGesturesOpen, false))
		{
			//open gestures menu
			if ( !GetUIManager().IsMenuOpen( MENU_GESTURES ) )
			{
				GesturesMenu.OpenMenu();
			}
		}
		
		if(input.GetActionUp(UAUIGesturesOpen, false))
		{
			//close gestures menu
			if ( GetUIManager().IsMenuOpen( MENU_GESTURES ) )
			{
				GesturesMenu.CloseMenu();
			}
		}
#endif		

#ifdef PLATFORM_XBOX		
		//Radial quickbar
		if(input.GetActionDown(UAUIQuickbarRadialOpen, false))
		{
			//open gestures menu
			if ( !GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
			{
				RadialQuickbarMenu.OpenMenu();
			}
		}
		
		if(input.GetActionUp(UAUIQuickbarRadialOpen, false))
		{
			//close gestures menu
			if ( GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
			{
				RadialQuickbarMenu.CloseMenu();
			}
		}
		
		//Radial Quickbar from inventory
		if( GetGame().GetInput().GetActionUp( UAUIQuickbarRadialInventoryOpen, false ) )
		{
			//close radial quickbar menu
			if ( GetGame().GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
			{
				RadialQuickbarMenu.CloseMenu();
				RadialQuickbarMenu.SetItemToAssign( NULL );
			}
		}		
#endif

#ifdef PLATFORM_PS4		
		//Radial quickbar
		if(input.GetActionDown(UAUIQuickbarRadialOpen, false))
		{
			//open gestures menu
			if ( !GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
			{
				RadialQuickbarMenu.OpenMenu();
			}
		}
		
		if(input.GetActionUp(UAUIQuickbarRadialOpen, false))
		{
			//close gestures menu
			if ( GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
			{
				RadialQuickbarMenu.CloseMenu();
			}
		}
		
		//Radial Quickbar from inventory
		if( GetGame().GetInput().GetActionUp( UAUIQuickbarRadialInventoryOpen, false ) )
		{
			//close radial quickbar menu
			if ( GetGame().GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
			{
				RadialQuickbarMenu.CloseMenu();
				RadialQuickbarMenu.SetItemToAssign( NULL );
			}
		}		
#endif
		
		if (player && m_life_state == EPlayerStates.ALIVE)
		{
			// enables HUD on spawn
			if (m_hud_root_widget)
			{
				m_hud_root_widget.Show(true);
			}
			
		#ifndef NO_GUI
			// fade out black screen
			if ( GetUIManager().ScreenFadeVisible() )
			{
				 GetUIManager().ScreenFadeOut(0.5);
			}
		#endif
		
			if( input.GetActionDown(UAGear, false ) )
			{
				if( !inventory )
				{
					if( GetGame().IsOldInventory() )
					{
						ShowInventory();
					}
					else
					{
						ShowInventory2();
					}
				}
				else if( menu == inventory )
				{
					HideInventory();
				}
			}

			if (input.GetActionDown(UAChat, false))
			{
				if (menu == NULL)
				{
					m_chat_channel_hide_timer.Stop();
					m_chat_channel_fade_timer.Stop();
					m_chat_channel_area.Show(false);
					m_UIManager.EnterScriptedMenu(MENU_CHAT_INPUT, NULL);
				}
			}
			
			if ( input.GetActionDown( UAUIQuickbarToggle, false) )
			{
				SetActionDownTime( GetGame().GetTime() );
				bool hud_state = m_hud.GetHudState();
				m_ToggleHudTimer.Run( 0.3, m_hud, "ToggleHud", new Param1<bool>( !hud_state ) );
			}
			
			if ( input.GetActionUp( UAUIQuickbarToggle, false) )
			{
				SetActionUpTime( GetGame().GetTime() );
				
				if ( GetHoldActionTime() < HOLD_LIMIT_TIME )
				{
					if ( menu == NULL )
					{
						if ( m_hud.GetQuickBarState() )
						{
							m_hud.HideQuickbar( false, true );
						}
						else
						{
							m_hud.ShowQuickbar();
						}
					}
				}
				
				m_ToggleHudTimer.Stop();
			}
			
			if ( g_Game.GetInput().GetActionDown( UAZeroingUp, false) || g_Game.GetInput().GetActionDown( UAZeroingDown, false) || g_Game.GetInput().GetActionDown( UAToggleWeapons, false) )
			{
				m_hud.ZeroingKeyPress();
			}
			
			if ( menu == NULL )
			{			
				if (input.GetActionDown(UANextAction, false))
				{
					m_actionMenu.NextAction();
				}
				
				if (input.GetActionDown(UAPrevAction, false))
				{
					m_actionMenu.PrevAction();
				}
			}
			
			//hologram rotation
			if (menu == NULL && playerPB.IsPlacingLocal())
			{
				if (input.GetActionUp(UANextAction, false))
				{
					playerPB.GetHologramLocal().SubtractProjectionRotation(15);
				}
				
				if (input.GetActionUp(UAPrevAction, false))
				{
					playerPB.GetHologramLocal().AddProjectionRotation(15);
				}
			}
		}
		
		// life state check
		if (player)
		{
			int life_state = player.GetPlayerState();
			
			// life state changed
			if (m_life_state != life_state)
			{
				m_life_state = life_state;
				
				if (m_life_state != EPlayerStates.ALIVE)
				{
					CloseAllMenus();
					if (m_life_state == EPlayerStates.DEAD)
					{
						// GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(SimulateDeath,true);
						GetGame().PlayMission( "$CurrentDir:\\missions\\Arkensor_DayZSP.ChernarusPlus" );
					}
				}
				else if (m_life_state == EPlayerStates.ALIVE)
				{
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(SimulateDeath,false);
				}
			}
		}
		
		if (menu && !menu.UseKeyboard() && menu.UseMouse())
		{
			int i;
			for (i = 0; i < 5; i++)
			{
				input.DisableKey(i | INPUT_DEVICE_MOUSE);
				input.DisableKey(i | INPUT_ACTION_TYPE_DOWN_EVENT | INPUT_DEVICE_MOUSE);
				input.DisableKey(i | INPUT_ACTION_TYPE_DOUBLETAP | INPUT_DEVICE_MOUSE);
			}
			
			for (i = 0; i < 6; i++)
			{
				input.DisableKey(i | INPUT_DEVICE_MOUSE_AXIS);
			}
		}
		
		if (!m_UIManager.IsDialogVisible())
		{
			if ( menu )
			{
				if ( IsPaused() )
				{
					if(input.GetActionDown(UAUIBack, false) || input.GetActionDown(UAUIMenu, false))
					{
						Continue();
					}
				}
				else if ( menu == inventory )
				{
					if(input.GetActionDown(UAUIBack, false))
					{
						if( ItemManager.GetInstance().GetSelectedItem() == NULL )
						{
							HideInventory();
						}
					}
				}
				else if(input.GetActionDown(UAUIBack, false))
				{
					m_UIManager.Back();
				}
			}
			else if (input.GetActionDown(UAUIMenu, false))
			{
				Pause();
			}
		}
	}
	
	vector GetCursorPos()
	{
		vector rayStart = GetGame().GetCurrentCameraPosition();
		vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 10000;		
		vector hitPos;
		vector hitNormal;
		int hitComponentIndex;		
		DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, NULL, NULL, m_oPlayer);
		
		return hitPos;
	}
	
	string GetRandomChildFromBaseClass( string strConfigName, string strBaseClass )
	{
		string child_name = "";
		int count = GetGame().ConfigGetChildrenCount ( strConfigName );
		array<string> class_names = new array<string>;

		for (int p = 0; p < count; p++)
		{
			GetGame().ConfigGetChildName ( strConfigName, p, child_name );
			
			if ( GetGame().IsKindOf(child_name, strBaseClass ) )
			{
				class_names.Insert(child_name);
			}
		}
		
		return class_names.GetRandomElement()
	}
	
	// ---------- Keyhandler part
	
	bool CTRL()
	{
		return m_IsCtrlHolding;
	}
	
	bool WIN()
	{
		return m_IsWinHolding;
	}
	
	bool SHIFT()
	{
		return ( m_IsLeftShiftHolding || m_IsRightShiftHolding );
	}
	
	bool ALT()
	{
		return ( m_IsLeftAltHolding || m_IsRightAltHolding );
	}
	
	override void OnKeyPress(int key)
	{
		if( !m_bWelcomeSeen )
		{
			m_bWelcomeSeen = true;
			
			GetGame().GetUIManager().ShowDialog( "DayZ OfflineMode", "Enjoy the DayZ Community Offline Mode.\n \n \nThe current game build is still ALPHA!\nCrashes and bugs can occur.\nNot all 0.63 features are impletemented yet!\nProvide feedback on the game via feedback.bistudio.com\n \n \nCreated by Arkensor.\nContributions from DuhOneZ", 76, DBT_OK, DBB_OK, DMT_INFO, m_oWelcomeManager );
		}
		
		super.OnKeyPress(key);
		
		m_hud.KeyPress(key);
		
		if( GetGame().GetUIManager().GetMenu() )
		{
			return;
		}
		
		switch( key )
		{
			case KeyCode.KC_LCONTROL:
			{
				m_IsCtrlHolding = true;
				break;
			}
			
			case KeyCode.KC_LWIN:
			{
				m_IsWinHolding = true;
				break;
			}
			
			case KeyCode.KC_LMENU:
			{
				m_IsLeftAltHolding = true;
				break;	
			}
			
			case KeyCode.KC_RMENU:
			{
				m_IsRightAltHolding = true;
				break;
			}
			
			case KeyCode.KC_LSHIFT:
			{
				m_IsLeftShiftHolding = true;
				break:
			}
			
			case KeyCode.KC_RSHIFT:
			{
				m_IsRightShiftHolding = true;
				break:
			}
			
			//Gestures [.]
			case KeyCode.KC_PERIOD:
			{
				//open gestures menu
				if ( !GetUIManager().IsMenuOpen( MENU_GESTURES ) )
				{
					//TODO reconnect when appropriate
					GesturesMenu.OpenMenu();
				}
				
				break;
			}
			
			//Radial Quickbar [,]
			case KeyCode.KC_COMMA:
			{
				//open radial quickbar menu
				if ( !GetGame().GetUIManager().IsMenuOpen( MENU_RADIAL_QUICKBAR ) )
				{
					//TODO reconnect when appropriate
					RadialQuickbarMenu.OpenMenu();
				}
				
				break;
			}
			
			case KeyCode.KC_DELETE:
			{
				if ( g_Game.GetUIManager().GetMenu() == NULL )
				{
					g_Game.GetUIManager().ShowUICursor(true);
					g_Game.GetUIManager().EnterScriptedMenu(MENU_SCENE_EDITOR, NULL);
				}
				else if ( g_Game.GetUIManager().IsMenuOpen(MENU_SCENE_EDITOR) )
				{
					g_Game.GetUIManager().ShowUICursor(false);
					g_Game.GetUIManager().Back();
				}
				
				break;
			}
			
			case KeyCode.KC_T:
			{	
				vector hitPos = GetCursorPos();

				float distance = vector.Distance( m_oPlayer.GetPosition(), hitPos );
				
				if ( distance < 5000 )
				{
					EntityAI oVehicle = m_oPlayer.GetDrivingVehicle();

					if( oVehicle )
					{
						m_oPlayer.MessageStatus("Get out of the vehicle first!");
					}
					else
					{
						m_oPlayer.SetPosition( hitPos );
						m_oPlayer.MessageStatus("Teleported!");
					}
				}
				else
				{
					m_oPlayer.MessageStatus( "Distance for teleportation is too far!" );
				}	

				break;
			}
			
			case KeyCode.KC_O:
			{
				if( CTRL() )
				{
					GetGame().CreateObject( "Animal_CanisLupus_Grey", GetCursorPos(), false, true );
				}
				else if( SHIFT() )
				{
					GetGame().CreateObject( GetRandomChildFromBaseClass( "cfgVehicles", "AnimalBase" ), GetCursorPos(), false, true );
				}
				else
				{
					GetGame().CreateObject( GetRandomChildFromBaseClass( "cfgVehicles", "DayZInfected" ), GetCursorPos(), false, true );
				}
				
				break;
			}

			case KeyCode.KC_R:
			{
				EntityAI oWeapon = m_oPlayer.GetHumanInventory().GetEntityInHands();
				
				if( oWeapon )
				{
					Magazine oMag = ( Magazine ) oWeapon.GetAttachmentByConfigTypeName( "DefaultMagazine" );
					
					if( oMag && oMag.IsMagazine() )
					{
						oMag.LocalSetAmmoMax();
					}					
					
					Object oSupressor = ( Object ) oWeapon.GetAttachmentByConfigTypeName( "SuppressorBase" );
					
					if( oSupressor )
					{
						oSupressor.SetHealth( oSupressor.GetMaxHealth( "", "" ) );
					}
				}

				break;
			}
			
			case KeyCode.KC_P:
			{
				Print( m_oPlayer.GetPosition().ToString() );
				Print( m_oPlayer.GetDirection().ToString() );
				
				Print( "POS X:" + m_oPlayer.GetPosition()[0] + " Y:" + m_oPlayer.GetPosition()[2] + " Z:" + m_oPlayer.GetPosition()[1] );
				m_oPlayer.MessageStatus( "POS X:" + m_oPlayer.GetPosition()[0] + " Y:" + m_oPlayer.GetPosition()[2] + " Z:" + m_oPlayer.GetPosition()[1] );

				m_oPlayer.MessageStatus( "Player position and direction vector were written to the game logs too." );
				
				break;
			}
			
			case KeyCode.KC_D:
			{
				if( CTRL() )
				{
					GetGame().OpenURL( "https://github.com/Arkensor/DayZCommunityOfflineMode" );
				}
				
				break;
			}
			
			case KeyCode.KC_B:
			{
				Print( m_debugMonitor );
				
				if( m_bDebugMonitor )
				{
					m_debugMonitor.Hide();
					m_bDebugMonitor = false;
				}
				else
				{
					m_debugMonitor.Show();
					m_bDebugMonitor = true;
				}
				
				break;
			}
			
			
			case KeyCode.KC_K:
			{
				GetGame().PlayMission( "$CurrentDir:\\missions\\Arkensor_DayZSP.ChernarusPlus" );
				break;
			}
			
			
			case KeyCode.KC_N:
			{
				EntityAI oCar = EntityAI.Cast( GetGame().CreateObject( "CivilianSedan", GetCursorPos(), false, true ) );
				
				oCar.GetInventory().CreateAttachment( "CivSedanWheel" );
				oCar.GetInventory().CreateAttachment( "CivSedanWheel" );
				oCar.GetInventory().CreateAttachment( "CivSedanWheel" );
				oCar.GetInventory().CreateAttachment( "CivSedanWheel" );
				oCar.GetInventory().CreateAttachment( "CarBattery" );
				oCar.GetInventory().CreateAttachment( "CarRadiator" );
				oCar.GetInventory().CreateAttachment( "EngineBelt" );
				oCar.GetInventory().CreateAttachment( "SparkPlug" );
				oCar.GetInventory().CreateAttachment( "CivSedanHood" );
				oCar.GetInventory().CreateAttachment( "CivSedanTrunk" );
				oCar.GetInventory().CreateAttachment( "CivSedanDoors_Driver" );
				oCar.GetInventory().CreateAttachment( "CivSedanDoors_CoDriver" );
				oCar.GetInventory().CreateAttachment( "CivSedanDoors_BackLeft" );
				oCar.GetInventory().CreateAttachment( "CivSedanDoors_BackRight" );
				
				oCar.SetAllowDamage( false );
				
				break;
			}

			case KeyCode.KC_INSERT:
			{	
				if ( m_oCamera )
				{
					GetGame().SelectPlayer( NULL, m_oPlayer );
					
					m_oPlayer.SetPosition( GetCursorPos() );
					
					m_oCamera.SetActive( false );
					
					GetGame().ObjectDelete( m_oCamera );
					
					m_oCamera = NULL;
				}
				else
				{
					GetGame().SelectPlayer( NULL, NULL );
					
					m_oCamera = g_Game.CreateObject( "FreeDebugCamera", m_oPlayer.GetPosition(), true );
					
					m_oCamera.SetActive( true );
				}
				
				break;
			}
			
			case KeyCode.KC_END:
			{
				if ( m_bGodMode )
				{
					m_oPlayer.MessageStatus( "God mode disabled." );
					m_oPlayer.SetAllowDamage( true );
					m_bGodMode = false;
				}
				else
				{
					m_oPlayer.MessageStatus( "God mode enabled." );
					m_oPlayer.SetAllowDamage( false );
					m_bGodMode = true;
				}
				
				break;
			}		
			
			case KeyCode.KC_HOME:
			{
				DamageSystem.ExplosionDamage( NULL, NULL, "Dummy_Heavy", GetCursorPos() );
				Particle.Play( ParticleList.EXPLOSION_TEST, GetCursorPos() );

				break;
			}
		}	
	}
	
	override void OnKeyRelease(int key)
	{
		super.OnKeyRelease(key);
				
		switch( key )
		{
			case KeyCode.KC_LCONTROL:
			{
				m_IsCtrlHolding = false;
				break;
			}
			
			
			case KeyCode.KC_LWIN:
			{
				m_IsWinHolding = false;
				break;
			}
			
			
			case KeyCode.KC_LMENU:
			{
				m_IsLeftAltHolding = false;
				break;
			}
						
			
			case KeyCode.KC_RMENU:
			{
				m_IsRightAltHolding = false;
				break;
			}
			
			case KeyCode.KC_LSHIFT:
			{
				m_IsLeftShiftHolding = false;
				break:
			}
			
			case KeyCode.KC_RSHIFT:
			{
				m_IsRightShiftHolding = false;
				break:
			}
		}
	}
	
	override void OnEvent(EventType eventTypeId, Param params)
	{
		super.OnEvent(eventTypeId, params);
		InventoryMenuNew menu;
		Man player = GetGame().GetPlayer();
		
		switch(eventTypeId)
		{
		case ChatMessageEventTypeID:
			Print( "ChatMessageEventTypeID: " );
			Print( ChatMessageEventTypeID );
			ChatMessageEventParams chat_params = ChatMessageEventParams.Cast( params );
			if (chat_params.param1 == CCSystem && GetProfileValueBool(SYSTEM_CHAT_MSG, true) == false) break;
			if (chat_params.param1 == CCGlobal && GetProfileValueBool(GLOBAL_CHAT_MSG, true) == false) break;
			if (chat_params.param1 == CCItemTransmitter && GetProfileValueBool(RADIO_CHAT_MSG, true) == false) break;
			if (chat_params.param1 == CCDirect && GetProfileValueBool(DIRECT_CHAT_MSG, true) == false) break;
			
			if (m_life_state == EPlayerStates.ALIVE)
			{
				m_chat.Add(chat_params);
			}
			break;
			
		case SQFConsoleEventTypeID:
			SQFConsoleEventParams sqf_console_params = SQFConsoleEventParams.Cast( params );
			if (sqf_console_params.param1)
			{
				//Pause();
				//Continue();
				g_Game.GetUIManager().ShowUICursor(true);
			}
			else
			{
				//Continue();
				g_Game.GetUIManager().ShowUICursor(false);
			}
			break;
			
		case ChatChannelEventTypeID:
			ChatChannelEventParams cc_params = ChatChannelEventParams.Cast( params );
			ChatInputMenu chatMenu = ChatInputMenu.Cast( GetUIManager().FindMenu(MENU_CHAT_INPUT) );
			if (chatMenu)
			{
				chatMenu.UpdateChannel();
			}
			else
			{
				m_chat_channel_text.SetText(ChatInputMenu.GetChannelName(cc_params.param1));
				m_chat_channel_fade_timer.FadeIn(m_chat_channel_area, 0.5, true);
				m_chat_channel_hide_timer.Run(2, m_chat_channel_fade_timer, "FadeOut", new Param3<Widget, float, bool>(m_chat_channel_area, 0.5, true));
			}
			break;
			
		case WindowsResizeEventTypeID:

			if(!GetGame().IsOldInventory())
			{
				if( m_inventory_menu_new != NULL )
				{
					m_inventory_menu_new.Reset();
				}
				else
				{
					WidgetCache.GetInstance().Reset();
				}
			}
			CloseAllMenus();
			DestroyAllMenus();
			
			break;
			
		case VONStateEventTypeID:
			VONStateEventParams vonStateParams = VONStateEventParams.Cast( params );
			if (vonStateParams.param1)
			{
				m_microphone_icon.Show(true);
			}
			else
			{
				m_microphone_icon.Show(false);
			}
			break;
			
		case SetFreeCameraEventTypeID:
			SetFreeCameraEventParams set_free_camera_event_params = SetFreeCameraEventParams.Cast( params );
			PluginDeveloper plugin_developer = PluginDeveloper.Cast( GetPlugin(PluginDeveloper) );
			plugin_developer.OnSetFreeCameraEvent( PlayerBase.Cast( player ), set_free_camera_event_params.param1 );
			break;
		}
	}
	
	override void OnItemUsed(InventoryItem item, Man owner)
	{
		if (item && GetUIManager().GetMenu() == NULL)
		{
			if (item.IsInherited(ItemBook))
			{
				BookMenu bookMenu = BookMenu.Cast( GetUIManager().EnterScriptedMenu(MENU_BOOK, NULL) );
				if (bookMenu)
				{
					bookMenu.ReadBook(item);
				}
			}
			else if (item.IsInherited(ItemMap))
			{
				GetUIManager().EnterScriptedMenu(MENU_MAP, NULL);
			}
		}
	}
	
	void CloseAllMenus()
	{
		HideInventory();
		GetUIManager().CloseAll();
		m_IsOpenPauseMenu = false;
	}
	
	void DestroyAllMenus()
	{
		GetUIManager().HideDialog();
		DestroyInventory();
		GetUIManager().CloseAll();
		m_chat.Clear();
	}
	
	void ShowInventory()
	{
		bool init = false;
		UIScriptedMenu menu = GetUIManager().GetMenu();
		
		if (menu == NULL && GetGame().GetPlayer().GetHumanInventory().CanOpenInventory())
		{
			if (m_inventory_menu == NULL)
			{
				m_inventory_menu = InventoryMenu.Cast( GetUIManager().EnterScriptedMenu(MENU_INVENTORY, NULL) );
			}
			else if ( GetUIManager().FindMenu(MENU_INVENTORY) == NULL )
			{
				GetUIManager().ShowScriptedMenu(m_inventory_menu, NULL);
			}
			init = true;
		}
		
		if (menu && menu == m_inventory_menu)
		{
			init = true;
		}
		
		if (m_inventory_menu && init)
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			vector rayStart = GetGame().GetCurrentCameraPosition();
			vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 1.5;
			vector hitPos;
			vector hitNormal;
			int hitComponentIndex;
			ref set<Object> hitObjects = new set<Object>;
			DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, hitObjects, NULL, player);
			
			Object target = NULL;
			if( hitObjects.Count() )
				target = hitObjects.Get(0);
				
			m_inventory_menu.InitContainers(EntityAI.Cast(target));

			//player.GetInventoryActionHandler().DeactiveAction();
			//player.GetCraftingManager().CancelInventoryCraft();
		}
	}
	
	void ShowInventory2()
	{
		bool init = false;
		UIScriptedMenu menu = GetUIManager().GetMenu();
		
		if (menu == NULL && GetGame().GetPlayer().GetHumanInventory().CanOpenInventory())
		{
			if (m_inventory_menu_new == NULL)
			{
				m_inventory_menu_new = InventoryMenuNew.Cast( GetUIManager().EnterScriptedMenu(MENU_INVENTORY, NULL) );
			}
			else if ( GetUIManager().FindMenu(MENU_INVENTORY) == NULL )
			{
				GetUIManager().ShowScriptedMenu(m_inventory_menu_new, NULL);
			}
			init = true;
		}
		
		if (menu && menu == m_inventory_menu_new)
		{
			init = true;
		}
		
		if (m_inventory_menu_new && init)
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			vector rayStart = GetGame().GetCurrentCameraPosition();
			vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 1.5;
			vector hitPos;
			vector hitNormal;
			int hitComponentIndex;
			ref set<Object> hitObjects = new set<Object>;
			DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, hitObjects, NULL, player);
			
			Object target = NULL;
			if( hitObjects.Count() )
				target = hitObjects.Get(0);
				
			m_inventory_menu_new.InitContainers(EntityAI.Cast(target));
		}
	}
	
	void HideInventory()
	{
		InventoryMenuNew inventory = InventoryMenuNew.Cast( GetUIManager().FindMenu(MENU_INVENTORY) );
		if (inventory)
		{
			GetUIManager().HideScriptedMenu(inventory);
		}
	}
	
	void DestroyInventory()
	{
		if (m_inventory_menu)
		{
			m_inventory_menu.Close();
			m_inventory_menu = NULL;
		}
		if (m_inventory_menu_new)
		{
			m_inventory_menu_new.Close();
			m_inventory_menu_new = NULL;
		}
	}
	
	override Hud GetHud()
	{
		return m_hud;
	}
	
	HudDebug GetHudDebug()
	{
		return m_hud_debug;
	}
	
	override void RefreshCrosshairVisibility()
	{
		GetHudDebug().RefreshCrosshairVisibility();
	}
	
	override void HideCrosshairVisibility()
	{
		GetHudDebug().HideCrosshairVisibility();
	}
	
	override bool IsPaused()
	{
		//return GetGame().GetUIManager().IsMenuOpen(MENU_INGAME);
		return m_IsOpenPauseMenu;
	}
	
	override void Pause()
	{
		if ( IsPaused() )
		{
			return;
		} 
		
		CloseAllMenus();
		
		// open ingame menu
		GetUIManager().EnterScriptedMenu(MENU_INGAME, NULL);
		m_IsOpenPauseMenu = true;
	}
	
	override void Continue()
	{
		if ( !IsPaused() )
		{
			return;
		}
		
		GetUIManager().CloseMenu(MENU_INGAME);
		m_IsOpenPauseMenu = false;
	}
	
	override bool IsMissionGameplay()
	{
		return false;
	}
	
	override void AbortMission()
	{
		#ifdef BULDOZER
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(g_Game.RequestExit, IDC_MAIN_QUIT);
		#else
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().AbortMission);
		#endif
	}
	
	override void CreateLogoutMenu(UIMenuPanel parent)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		// do not show logout screen if player's dead
		if (!player || player.IsDamageDestroyed())
		{
			// exit the game immediately
			AbortMission();
			return;
		}
		
		if (parent)
			m_logout = LogoutMenu.Cast(parent.EnterScriptedMenu(MENU_LOGOUT));
		
		if (m_logout)
			m_logout.SetTimeLoading();
	}
	
	override void StartLogoutMenu(int time)
	{
		if (m_logout)
		{
			if (time > 0)
			{
				// character will be deleted from server int "time" seconds
				m_logout.SetTime(time);
				m_logout.Show();
				
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_logout.UpdateTime, 1000, true);
			}
			else
			{
				// no waiting time -> player is most likely dead
				m_logout.Exit();
			}
		}
	}
	
	override void CreateDebugMonitor()
	{
		if (!m_debugMonitor)
		{
			m_debugMonitor = new DebugMonitor();
			m_debugMonitor.Init();
		}
	}
	
	void UpdateDebugMonitor()
	{
		if (!m_debugMonitor) return;
		
		if (m_oPlayer)
		{
			// DebugMonitorValues values = m_oPlayer.GetDebugMonitorValues();
			
			// if (values)
			// {
				// m_debugMonitor.SetHealth( values.GetHealth() );
				// m_debugMonitor.SetBlood( values.GetBlood() );
				// m_debugMonitor.SetLastDamage( values.GetLastDamage() );
				// m_debugMonitor.SetPosition( m_oPlayer.GetPosition() );
			// }
			// else
			// {
				m_debugMonitor.SetHealth( m_oPlayer.GetHealth( "","" ) );
				m_debugMonitor.SetBlood(  m_oPlayer.GetHealth( "","Blood" ) );
				m_debugMonitor.SetLastDamage( "from https://lloyd.cat" );
				m_debugMonitor.SetPosition( m_oPlayer.GetPosition() );
			// }
		}
	}
	
	void SetActionDownTime( int time_down )
	{
		m_ActionDownTime = time_down;
	}
	
	void SetActionUpTime( int time_up )
	{
		m_ActionUpTime = time_up;
	}
	
	int GetActionDownTime()
	{
		return m_ActionDownTime;
	}
	
	int GetActionUpTime()
	{
		return m_ActionUpTime;
	}
	
	float GetHoldActionTime()
	{
		float hold_action_time = GetActionUpTime() - GetActionDownTime();
		return hold_action_time;
	}
	
	void SimulateDeath(bool state)
	{
		if (g_Game.GetMissionState() != DayZGame.MISSION_STATE_GAME)	 return;
		//TODO turn on/off death effects (audio, visuals, controls)
		
		//TODO prepsat 
		//controlls
		/*if (state == true) 	GetGame().GetUIManager().GetMenu().LockControls();
		else 				GetGame().GetUIManager().GetMenu().UnlockControls();*/
		LockControls(state);
		
		//video?
		
		//audio?
		if (state == true)
		{
			GetGame().GetSoundScene().SetSoundVolume(0,1);
			GetGame().GetSoundScene().SetSpeechExVolume(0,1);
			GetGame().GetSoundScene().SetMusicVolume(0,1);
			GetGame().GetSoundScene().SetVOIPVolume(0,1);
			GetGame().GetSoundScene().SetRadioVolume(0,1);
		}
		else
		{
			GetGame().GetSoundScene().SetSoundVolume(m_volume_sound,1);
			GetGame().GetSoundScene().SetSpeechExVolume(m_volume_speechEX,1);
			GetGame().GetSoundScene().SetMusicVolume(m_volume_music,1);
			GetGame().GetSoundScene().SetVOIPVolume(m_volume_VOIP,1);
			GetGame().GetSoundScene().SetRadioVolume(m_volume_radio,1);
		}
	}
	
	void LockControls(bool state)
	{
		if (state == true)
		{
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_MOUSE);
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_KEYBOARD);
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_GAMEPAD);
			
			if (GetGame().GetUIManager()) 	GetGame().GetUIManager().ShowUICursor(true);
		}
		else
		{
			GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_MOUSE);
			GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_KEYBOARD);
			GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_GAMEPAD);
			
			if (GetGame().GetUIManager())
			{
				if (GetGame().GetUIManager().GetMenu())
				{
					GetGame().GetUIManager().ShowUICursor(true);
				}
				else
				{
					GetGame().GetUIManager().ShowUICursor(false);
				}
			}
		}
	}
}
