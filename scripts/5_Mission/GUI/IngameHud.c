class IngameHud extends Hud
{
	protected const float FADE_IN_TIME = 0.3;
	protected const float FADE_OUT_TIME = 0.3;
	protected const float HIDE_MENU_TIME = 5;

	protected ref map<int, string>	m_StatesWidgetNames;
	protected ref map<int, ImageWidget>	m_StatesWidgets;  // [key] ImageWidget

	protected ref array<ImageWidget> m_TendencyStatusCritical;	//array of icons that are blinking due to critical tendency status
	protected const float 	TENDENCY_BLINK_TIME = 0.25;
	
	protected ref map<int, string>	m_BadgesWidgetNames;
	protected ref map<int, bool>	m_BadgesWidgetDisplay;
	protected ref map<int, ImageWidget>	m_BadgesWidgets;  // [key] ImageWidget

	protected ref map<int, string>	m_VehicleGearTable;

	protected Widget m_HudPanelWidget;
	protected Widget m_quickbar_widget;
	protected ref InventoryQuickbar m_quickbar;
	
	protected TextWidget m_VehiclePanelSpeedValue;
	protected TextWidget m_VehiclePanelGearValue;
	
	protected Widget m_VehiclePanel;
	protected Widget m_Notifiers;
	protected Widget m_Badges;
	protected ref Timer m_hide_timer;
	protected ref Timer m_vehicle_timer;
//	protected ref Timer m_zeroing_and_weaponmode_timer;
	protected ref WidgetFadeTimer m_fade_timer_crosshair;
	protected ref WidgetFadeTimer m_fade_timer_action_cursor;
//	protected ref WidgetFadeTimer m_FadeTimerWeaponMode;
//	protected ref WidgetFadeTimer m_FadeTimerZeroing;
	protected ref WidgetFadeTimer m_fade_timer_zeroing_old;
	protected ref WidgetFadeTimer m_fade_timer_magazine_name;
	protected ref WidgetFadeTimer m_fade_timer_weapon_name;
	protected ref WidgetFadeTimer m_fade_timer_weapon_stats;
	protected ref WidgetFadeTimer m_fade_timer_quickbar;
	protected ref WidgetFadeTimer m_fade_timer_walkie_talkie;
	protected ref WidgetFadeTimer m_fade_timer_walkie_talkie_text;
	protected ProgressBarWidget m_stamina;
	protected Widget m_stamina_background;
	protected Widget m_specializationPanel;
	protected Widget m_specializationIcon;
	protected Widget m_presence;
	protected Widget m_stanceProne;
	protected Widget m_stanceStand;
	protected Widget m_stanceCrouch;
	protected Widget m_stanceCar;
	protected Widget m_stancePanel;
	protected Widget m_PresenceLevel0;
	protected Widget m_PresenceLevel1;
	protected Widget m_PresenceLevel2;
	protected Widget m_PresenceLevel3;
	protected Widget m_PresenceLevel4;
//	protected TextWidget m_Zeroing;
//	protected Widget m_WeaponStats;
//	protected TextWidget m_WeaponMode;
//	protected TextWidget m_ZeroingOld;
//	protected TextWidget m_WeaponName;
//	protected TextWidget m_MagazineName;
	protected ref Timer myTimer;
	protected ref array<ref WidgetFadeTimer> m_fade_timers = new array<ref WidgetFadeTimer>;
	protected Widget m_MouseSingleAction;
	protected Widget m_MouseContinuousAction;
	protected Widget m_RadialProgressBar1;
	protected Widget m_RadialProgressBarCrossHair;
	protected Widget m_RadialProgressBar;
	protected Widget m_CursorWidget;
	protected ImageWidget m_CursorIcon;
	protected Widget m_WalkieTalkie;
	
	protected Widget m_ActionWidget;
	protected TextWidget m_ActionDesc;
	protected TextWidget m_ActionItemDesc;
	protected Widget m_ActionItemQuantityWrapper
	protected ImageWidget m_ActionHealthMark;
	protected ProgressBarWidget m_ActionQuantityBar;
	protected Widget m_ActionQuantityBox;
	protected TextWidget m_ActionQuantityBoxText;
	protected ImageWidget m_ActionIcon;
	protected TextWidget m_ActionButtonText;

	protected TextWidget m_BloodType;
	protected Widget m_ActionIconFrame;
	protected Widget m_ActionMultipleItemsFrame;
	
	protected Widget m_ActionTarget;
	
	// CrossHairs
	protected ImageWidget m_PermanentCrossHair;
	
	protected bool m_HudState;
	protected bool m_QuickbarState;
	protected bool m_Faded;
	protected bool m_ZeroingKeyPressed;
		
	void IngameHud()
	{
		m_fade_timer_crosshair = new WidgetFadeTimer;
		m_fade_timer_action_cursor = new WidgetFadeTimer;
//		m_FadeTimerWeaponMode = new WidgetFadeTimer;
//		m_FadeTimerZeroing = new WidgetFadeTimer;
//		 m_fade_timer_zeroing_old = new WidgetFadeTimer;
//		 m_fade_timer_magazine_name = new WidgetFadeTimer;
//		 m_fade_timer_weapon_name = new WidgetFadeTimer;
//		 m_fade_timer_weapon_stats = new WidgetFadeTimer;
		 m_fade_timer_quickbar = new WidgetFadeTimer;
		 m_fade_timer_walkie_talkie = new WidgetFadeTimer;
 		 m_fade_timer_walkie_talkie_text = new WidgetFadeTimer;
		m_hide_timer = new Timer(CALL_CATEGORY_GUI);
		m_vehicle_timer = new Timer( CALL_CATEGORY_GAMEPLAY );
		//m_zeroing_and_weaponmode_timer = new Timer( CALL_CATEGORY_GAMEPLAY );

		m_StatesWidgets = new map<int, ImageWidget>; // [key] widgetName
		m_StatesWidgetNames = new map<int, string>;
		
		m_TendencyStatusCritical = new array<ImageWidget>;
		
		m_BadgesWidgets = new map<int, ImageWidget>; // [key] widgetName
		m_BadgesWidgetNames = new map<int, string>;
		m_BadgesWidgetDisplay = new map<int, bool>;

		m_VehicleGearTable = new map<int, string>;
		m_VehicleGearTable.Set( -1, "" );
		m_VehicleGearTable.Set( (int)CarGear.REVERSE, "R" );
		m_VehicleGearTable.Set( (int)CarGear.NEUTRAL, "N" );
		m_VehicleGearTable.Set( (int)CarGear.FIRST, "1" );
		m_VehicleGearTable.Set( (int)CarGear.SECOND, "2" );
		m_VehicleGearTable.Set( (int)CarGear.THIRD, "3" );
		m_VehicleGearTable.Set( (int)CarGear.FOURTH, "4" );
		m_VehicleGearTable.Set( (int)CarGear.FIFTH, "5" );
		m_VehicleGearTable.Set( (int)CarGear.SIXTH, "6" );
		m_VehicleGearTable.Set( (int)CarGear.SEVENTH, "7" );
		m_VehicleGearTable.Set( (int)CarGear.EIGTH, "8" );
	}
	
	override void Init( Widget hud_panel_widget )
	{
		m_HudPanelWidget = hud_panel_widget;
		m_HudPanelWidget.Show( true );

		// quickbar
		m_quickbar_widget = m_HudPanelWidget.FindAnyWidget("QuickbarGrid");
		m_quickbar_widget.Show( false );
		
		// TEMPORARY HACK!!! player is not present when Hud is being initialized 
		myTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		myTimer.Run( 1, this, "InitQuickbar" );
		
		m_CursorWidget = m_HudPanelWidget.FindAnyWidget("CursorIcons");
		m_CursorWidget.Show(true);
		Class.CastTo(m_CursorIcon, m_HudPanelWidget.FindAnyWidget("Cursor"));

		// Permanent CrossHairs
		Class.CastTo(m_PermanentCrossHair, m_HudPanelWidget.FindAnyWidget("PermanentCrossHair"));
		
		m_WalkieTalkie = m_HudPanelWidget.FindAnyWidget("WalkieTalkie");
		
		// panels
		Class.CastTo(m_stamina, m_HudPanelWidget.FindAnyWidget("StaminaBar"));
		m_specializationPanel = m_HudPanelWidget.FindAnyWidget("SpecializationPanel");
		m_specializationIcon = m_HudPanelWidget.FindAnyWidget("SpecializationIcon");
		m_presence = m_HudPanelWidget.FindAnyWidget("PresencePanel");
		m_Badges = hud_panel_widget.FindAnyWidget("BadgesPanel");
		m_Notifiers = m_HudPanelWidget.FindAnyWidget("NotifiersPanel");
		m_VehiclePanel = m_HudPanelWidget.FindAnyWidget("VehiclePanel");
		Class.CastTo(m_VehiclePanelSpeedValue, m_HudPanelWidget.FindAnyWidget("SpeedValue0"));
		Class.CastTo(m_VehiclePanelGearValue, m_HudPanelWidget.FindAnyWidget("SpeedValue1"));
		//Class.CastTo(m_Zeroing, m_HudPanelWidget.FindAnyWidget("Zeroing"));
		//Class.CastTo(m_WeaponMode, m_HudPanelWidget.FindAnyWidget("WeaponMode"));
		//m_WeaponStats = m_HudPanelWidget.FindAnyWidget("WeaponStats");
		//Class.CastTo(m_ZeroingOld, m_HudPanelWidget.FindAnyWidget("ZeroingOld"));
		//Class.CastTo(m_WeaponName, m_HudPanelWidget.FindAnyWidget("WeaponName"));
		//Class.CastTo(m_MagazineName, m_HudPanelWidget.FindAnyWidget("MagazineName"));
		m_stamina_background = m_HudPanelWidget.FindAnyWidget("StaminaBackground");
		m_stamina_background.Show(true);
		m_stanceProne = m_HudPanelWidget.FindAnyWidget("StanceProne");
		m_stanceCrouch = m_HudPanelWidget.FindAnyWidget("StanceCrouch");
		m_stanceStand = m_HudPanelWidget.FindAnyWidget("StanceStand");
		m_stanceCar = m_HudPanelWidget.FindAnyWidget("StanceCar");
		m_stancePanel = m_HudPanelWidget.FindAnyWidget("StancePanel");
		m_ActionTarget = m_HudPanelWidget.FindAnyWidget("ActionTargetsCursorWidget");
		Class.CastTo(m_BloodType, m_HudPanelWidget.FindAnyWidget("BloodType") );
		
		// state notifiers
		m_StatesWidgetNames.Clear();
		m_StatesWidgets.Clear();
		m_StatesWidgetNames.Set( NTFKEY_THIRSTY, "Thirsty" );
		m_StatesWidgetNames.Set( NTFKEY_HUNGRY, "Hungry" );
		m_StatesWidgetNames.Set( NTFKEY_SICK, "Health" );
		m_StatesWidgetNames.Set( NTFKEY_BACTERIA, "Bacteria" );
		m_StatesWidgetNames.Set( NTFKEY_BLEEDISH, "Blood" );
		m_StatesWidgetNames.Set( NTFKEY_FEVERISH, "Temperature" );

		#ifndef NO_GUI
			m_Timer = new Timer( CALL_CATEGORY_GAMEPLAY );
			m_Timer.Run(1, this, "RefreshQuickbar", NULL, true );
			//m_Timer.Run(1, this, "CheckHudElementsVisibility", NULL, true ); //modify duration if needed, currently on 1s "update"
		#endif

			m_Notifiers.Show( true );
			// m_Notifiers.SetAlpha( 0 );
			m_Badges.Show( true );
			// m_Badges.SetAlpha( 0 );

			int i = 0;
			int key = 0;
			for ( i = 0; i < m_StatesWidgetNames.Count(); i++ )
			{
				string widget_name = m_StatesWidgetNames.GetElement(i);
				key = m_StatesWidgetNames.GetKey(i);
				ImageWidget w;
				Class.CastTo(w,  m_Notifiers.FindAnyWidget( String( "Icon" + widget_name ) ) );
				m_StatesWidgets.Set( key, w );
				w.Show( true );
				// clear all arrows
				for ( int x = 1; x < 4; x++ )
				{
					Class.CastTo(w,  m_Notifiers.FindAnyWidget( String( widget_name + "ArrowUp" + x.ToString() ) ) );
					w.Show( false );
					Class.CastTo(w,  m_Notifiers.FindAnyWidget( String( widget_name + "ArrowDown" + x.ToString() ) ) );
					w.Show( false );
				}
			}

			// badges
			m_BadgesWidgetNames.Clear();
			m_BadgesWidgets.Clear();
			m_BadgesWidgetDisplay.Clear();
			m_BadgesWidgetNames.Set( NTFKEY_FRACTURE, "Fracture" );
			m_BadgesWidgetNames.Set( NTFKEY_STUFFED, "Stomach" );
			m_BadgesWidgetNames.Set( NTFKEY_SICK, "Pill" );
			m_BadgesWidgetNames.Set( NTFKEY_BLEEDISH, "Wetness" );
			m_BadgesWidgetNames.Set( NTFKEY_FEVERISH, "Skull" );
			// NTFKEY_SICK
			// NTFKEY_BLEEDISH
			// NTFKEY_FRACTURE
			// NTFKEY_STUFFED
			// NTFKEY_WETNESS iconDrops

			for ( i = 0; i < m_BadgesWidgetNames.Count(); i++ )
			{
				string badge_name = m_BadgesWidgetNames.GetElement(  i);
				key = m_BadgesWidgetNames.GetKey( i );
				ImageWidget badge_widget;
				Class.CastTo(badge_widget,  m_Badges.FindAnyWidget( badge_name ) );
				m_BadgesWidgets.Set( key, badge_widget );
				badge_widget.Show( false );
				m_BadgesWidgetDisplay.Set( key, false );
			}

			m_PresenceLevel0 = hud_panel_widget.FindAnyWidget("Presence0");
			m_PresenceLevel1 = hud_panel_widget.FindAnyWidget("Presence1");
			m_PresenceLevel2 = hud_panel_widget.FindAnyWidget("Presence2");
			m_PresenceLevel3 = hud_panel_widget.FindAnyWidget("Presence3");
			m_PresenceLevel4 = hud_panel_widget.FindAnyWidget("Presence4");
			m_PresenceLevel0.Show( false );
			m_PresenceLevel1.Show( false );
			m_PresenceLevel2.Show( false );
			m_PresenceLevel3.Show( false );
			m_PresenceLevel4.Show( false );
		
		/*
		else
		{
			m_stamina_background.Show( false );
			m_stamina.Show( false );
			m_presence.Show( false );
			m_stanceProne.Show( false );
			m_stanceStand.Show( false );
			m_stanceCrouch.Show( false );
			m_Badges.Show( false );
			m_Notifiers.Show( false );
		}
		*/
		#ifndef NO_GUI
			m_vehicle_timer.Run(0.5, this, "RefreshVehicleHud", NULL, true );
		#endif
		
//		#ifndef NO_GUI
//		m_zeroing_and_weaponmode_timer.Run(0.1, this, "RefreshZeroingAndWeaponMode", NULL, true );
//		#endif
		
		SetLeftStatsVisibility( true );
		ToggleHud( g_Game.GetProfileOption( EDayZProfilesOptions.HUD ) );
		ToggleQuickBar( g_Game.GetProfileOption( EDayZProfilesOptions.QUICKBAR ) );
	}

	override void Show( bool show )
	{
		m_HudPanelWidget.Show( show );
	}

	override void ShowWalkieTalkie( bool show )
	{
		m_fade_timer_walkie_talkie.Stop();
		m_fade_timer_walkie_talkie_text.Stop();
		m_WalkieTalkie.Show( show );
	}
	
	override void ShowWalkieTalkie( int fadeOutSeconds )
	{
			m_WalkieTalkie.Show(true);
			m_fade_timer_walkie_talkie.Stop();
			m_fade_timer_walkie_talkie_text.Stop();
			m_fade_timer_walkie_talkie.FadeOut( m_WalkieTalkie.FindAnyWidget("Icon") , fadeOutSeconds );
			m_fade_timer_walkie_talkie_text.FadeOut( m_WalkieTalkie.FindAnyWidget("Text") , fadeOutSeconds );
	}
	
	override void SetWalkieTalkieText( string text )
	{
		TextWidget txt;
		Class.CastTo(txt, m_WalkieTalkie.FindAnyWidget("Text"));
		txt.SetText(text);
	}
	
	override void SetCursorIcon( string icon )
	{
		if ( icon.Length() == 0 || icon == CursorIcons.None )
			m_CursorIcon.Show(false);
		else
		{
			m_CursorIcon.LoadImageFile( 0, icon );
			m_CursorIcon.Show(true);
		}
	}

	override void ShowCursor()
	{
		m_CursorWidget.Show( true );
	}

	override void RefreshQuantity( EntityAI item_to_refresh )
	{
		InventoryMenuNew inventory;
		MissionGameplay mission;

		UIManager manager = GetGame().GetUIManager();
		Class.CastTo(inventory, manager.FindMenu( MENU_INVENTORY ));
		Class.CastTo(mission, GetGame().GetMission());
		if(	mission )
		{
			inventory = mission.GetInventory();
		}
		
		if( inventory )
		{
			inventory.RefreshQuantity( item_to_refresh );
		}
	}
	
	override void RefreshItemPosition( EntityAI item_to_refresh )
	{
		InventoryMenuNew inventory;
		MissionGameplay mission;

		UIManager manager = GetGame().GetUIManager();
		Class.CastTo(inventory, manager.FindMenu( MENU_INVENTORY ));
		Class.CastTo(mission, GetGame().GetMission());
		if(	mission )
		{
			inventory = mission.GetInventory();
		}
		
		if( inventory )
		{
			inventory.RefreshItemPosition( item_to_refresh );
		}
	}
	
	override void HideCursor()
	{
		m_CursorWidget.Show( false );
	}
	
	override void SetCursorIconScale( string type, float percentage )
	{
		float x, y;
		m_CursorIcon.FindAnyWidget( type ).GetParent().GetSize( x, y );
		m_CursorIcon.FindAnyWidget( type ).GetParent().SetSize( x * percentage, y * percentage );
	}
	
	override void SetCursorIconOffset( string type, float x, float y )
	{
		m_CursorIcon.FindAnyWidget( type ).GetParent().SetPos( x, y );
	}
	
	override void SetCursorIconSize( string type, float x, float y )
	{
		m_CursorIcon.FindAnyWidget( type ).GetParent().SetSize( x, y );
	}

#ifdef DEVELOPER
	// Debug Crosshair
	override void SetPermanentCrossHair( bool show )
	{
		m_PermanentCrossHair.Show(show);
	}
#endif
	
	override void DisplayNotifier( int key, int tendency, int status )
	{
		ImageWidget w;
		Class.CastTo(w,  m_Notifiers.FindAnyWidget( String( "Icon" + m_StatesWidgetNames.Get( key ) ) ) );
		w.Show( true );

		//tendency status
		//1 - white (fine)
		//2 - yellow (attention)
		//3 - red (alert)
		//4 - red blinking (critical)
		float alpha = w.GetAlpha();
			
		switch( status )
		{
			case 1:
				w.SetColor( ARGB( 255, 255, 255, 255 ) );	//white
				m_TendencyStatusCritical.RemoveItem( w );	//remove from blinking group
				break;
			case 2:
				w.SetColor( ARGB( 255, 255, 255, 0 ) );		//yellow
				m_TendencyStatusCritical.RemoveItem( w );	//remove from blinking group
				break;
			case 3:
				w.SetColor( ARGB( 255, 255, 0, 0 ) );		//red
				m_TendencyStatusCritical.RemoveItem( w );	//remove from blinking group
				break;
			case 4:
				if ( m_TendencyStatusCritical.Find( w ) < 0 )
				{
					m_TendencyStatusCritical.Insert( w );	//add to blinking group
				}
				break;				
			default:
				w.SetColor( ARGB( 255, 255, 255, 255 ) );	//white
				m_TendencyStatusCritical.RemoveItem( w );	//remove from blinking group
				break;
		}
		w.SetAlpha( alpha );
		//
		
		// tendency arrows
		string arrow_name = "ArrowUp";
		if ( tendency < 0 )
		{
			arrow_name = "ArrowDown";
		}
		tendency = Math.AbsInt( tendency );

		for ( int x = 1; x < 4; x++ )
		{ 
			Class.CastTo(w,  m_Notifiers.FindAnyWidget( String(  m_StatesWidgetNames.Get( key ) + "ArrowUp" + x.ToString() ) ) );
			w.Show( false );
			Class.CastTo(w,  m_Notifiers.FindAnyWidget( String(  m_StatesWidgetNames.Get( key ) + "ArrowDown" + x.ToString() ) ) );
			w.Show( false );
		}

		for ( int i = 1; i < ( tendency + 1) ; i++ )
		{
			string widget_name = m_StatesWidgetNames.Get( key ) + arrow_name + i.ToString() ;
			Class.CastTo(w,  m_Notifiers.FindAnyWidget( widget_name ) );
			w.Show( true );
		}
	}
	
	override void DisplayBadge( int key, bool show )
	{
		m_BadgesWidgetDisplay.Set( key, show );

		int x = 0;
		for ( int i = 0; i < m_BadgesWidgetDisplay.Count(); i++ )
		{
			int badge_key = m_BadgesWidgetDisplay.GetKey( i );
			string badge_name = m_BadgesWidgetNames.Get( badge_key );
			ImageWidget badge_widget
			Class.CastTo(badge_widget,  m_Badges.FindAnyWidget( badge_name ) );
			if ( badge_widget )
			{
				if ( m_BadgesWidgetDisplay.Get( badge_key ) == true )
				{
					badge_widget.SetPos ( x*0.2, 0.0, true);
					badge_widget.Show( true );
					x = x + 1;
				}
				else
				{
					badge_widget.Show( false );
				}
			}
		}
	}
	
	override void SetStamina( int value , int range )
	{
		//PlayerBase player = GetGame().GetPlayer();
		/*if( !player.m_PlayerStats )
		{
			return;
		}*/
		float sx, sy;
		float max = STAMINA_MAX;
		float percentage =  range / max;
		m_stamina.SetCurrent( ( value / range ) * 100 );
		m_stamina.GetSize( sx, sy );
		m_stamina.SetSize( percentage, sy );
 		m_stamina_background.SetSize( 1-percentage, sy);
	}

/*	
	void RefreshZeroingAndWeaponModeOld()
	{
		PlayerBase player;
		Class.CastTo(player,  GetGame().GetPlayer() );
		
		if ( player != NULL )
		{
			EntityAI entity = player.GetHumanInventory().GetEntityInHands();
			
			if ( entity != NULL && entity.IsWeapon() )
			{	
				float zeroing = player.GetCurrentZeroing();
				
				if( m_ZeroingKeyPressed )
				{
					m_ZeroingKeyPressed = false;
					m_Faded = false;
				}
				
				if( !m_Faded )
				{
					m_fade_timer_zeroing_old.FadeOut( m_WeaponStats, 5 );
					m_fade_timer_magazine_name.FadeOut( m_WeaponName, 5 );
					m_fade_timer_weapon_name.FadeOut( m_MagazineName, 5 );
					m_fade_timer_weapon_stats.FadeOut( m_ZeroingOld, 5 );
					m_Faded = true;
				}
				
				if( entity.GetInventory().AttachmentCount() == 0 )
				{
					m_MagazineName.SetText( "" );
				}
				
				for ( int i = 0; i < entity.GetInventory().AttachmentCount(); i++ )
				{
					EntityAI attachment = entity.GetInventory().GetAttachmentFromIndex( i );
					if ( !attachment ) continue;
					
					if ( attachment.IsMagazine() )
					{
						m_MagazineName.SetText( attachment.GetDisplayName() );
					}
					else
					{
						m_MagazineName.SetText("");
					}
				}
				
				m_WeaponName.SetText( entity.GetDisplayName() );
				m_WeaponStats.Show( true );
				m_ZeroingOld.SetText( zeroing.ToString() );
			}
			else
			{
				m_Faded = false;
				m_WeaponStats.Show( false );
			}
		}
	}
	
	void RefreshZeroingAndWeaponMode()
	{
		if( !m_FadeTimerWeaponMode || !m_FadeTimerZeroing || !m_WeaponMode || !m_Zeroing )
			return;
			
		PlayerBase player;
		Class.CastTo(player,  GetGame().GetPlayer() );
		
		if ( player != NULL )
		{
			EntityAI entity = player.GetHumanInventory().GetEntityInHands();
			
			if ( entity != NULL && entity.IsWeapon() )
			{	
				float zeroing = player.GetCurrentZeroing();
				string weaponMode = player.GetCurrentWeaponMode();
				
				if( m_ZeroingKeyPressed )
				{
					m_ZeroingKeyPressed = false;
					m_Faded = false;
				}
				
				if( !m_Faded )
				{
					m_FadeTimerWeaponMode.FadeOut(m_WeaponMode, 5);
					m_FadeTimerZeroing.FadeOut(m_Zeroing, 5);
					m_Faded = true;
				}
				
				m_Zeroing.Show( true );
				m_Zeroing.SetText( zeroing.ToString() );
				m_WeaponMode.Show( true );
				m_WeaponMode.SetText( weaponMode );
			}
			else
			{
				m_Faded = false;
				m_WeaponMode.Show( false );
				m_Zeroing.Show( false );
			}
		}
	}
*/

	bool KeyPress(int key)
	{
	}
	
	void ZeroingKeyPress()
	{
		m_ZeroingKeyPressed = true;
	}
	
	override void DisplayStance( int stance )
	{
		PlayerBase player;
		Class.CastTo(player,  GetGame().GetPlayer() );
		Car car;
		if ( !Class.CastTo(car, player.GetDrivingVehicle()) )
		{
			if( stance == 1 )
			{
				m_stanceStand.Show(true);
				m_stanceCrouch.Show(false);
				m_stanceProne.Show(false);
				m_stanceCar.Show( false );
			}
			if( stance == 2 )
			{
				m_stanceStand.Show(false);
				m_stanceCrouch.Show(true);
				m_stanceProne.Show(false);
				m_stanceCar.Show( false );
			}
			if( stance == 3 )
			{
				m_stanceStand.Show(false);
				m_stanceCrouch.Show(false);
				m_stanceProne.Show(true);
				m_stanceCar.Show( false );
			}
		}
		else
		{
			m_stanceStand.Show(false);
			m_stanceCrouch.Show(false);
			m_stanceProne.Show(false);
			m_stanceCar.Show( true );
		}
	}
	
	override void DisplayPresence()
	{
		PlayerBase player;
		Class.CastTo(player, GetGame().GetPlayer() );
		if ( player != NULL )
		{
			// TODO: rrada
			//float presence_level = player.GetPresenceInAI();
			float presence_level = 0; // tmp until we have working the GetPresenceInAI() method above
			m_PresenceLevel0.Show( false );
			m_PresenceLevel1.Show( false );
			m_PresenceLevel2.Show( false );
			m_PresenceLevel3.Show( false );
			m_PresenceLevel4.Show( false );

			if ( presence_level > 0 )
			{
				m_PresenceLevel0.Show( true );
			}
			if ( presence_level > 0.2 )
			{
				m_PresenceLevel1.Show( true );
			}
			if ( presence_level > 0.4 )
			{
				m_PresenceLevel2.Show( true );
			}
			if ( presence_level > 0.6 )
			{
				m_PresenceLevel3.Show( true );
			}
			if ( presence_level > 0.8 )
			{
				m_PresenceLevel4.Show( true );
			}
		}
	}
	
	void RefreshVehicleHud()
	{
		PlayerBase player;
		Class.CastTo(player,  GetGame().GetPlayer() );
		if ( player != NULL )
		{
			Car car;
			if ( Class.CastTo(car, player.GetDrivingVehicle()) )
			{
				m_VehiclePanel.Show( true );
				float rpm_value = ( car.GetEngineRPM() * 100 ) ;
				Widget RPMPointer = m_HudPanelWidget.FindAnyWidget("RpmPointer");
				RPMPointer.SetRotation( 0, 0, rpm_value, true );
				TextWidget VehiclePanelRPMValue;
				TextWidget VehiclePanelSpeedValue;
				Class.CastTo(VehiclePanelRPMValue,  m_HudPanelWidget.FindAnyWidget("RpmLabel") );
				Class.CastTo(VehiclePanelSpeedValue,  m_HudPanelWidget.FindAnyWidget("SpeedValue") );
				VehiclePanelSpeedValue.SetText( Math.Floor( car.GetSpeedometer() ).ToString() );

				int engaged_gear = car.GetController().GetGear();
				int prev_gear = engaged_gear - 1;
				int next_gear = engaged_gear + 1;

				if ( engaged_gear == CarGear.NEUTRAL )
				{
					prev_gear = CarGear.REVERSE;
				}
				else if ( engaged_gear == CarGear.REVERSE )
				{
					prev_gear = -1;
					next_gear = CarGear.NEUTRAL;
				}
				
				Class.CastTo(VehiclePanelSpeedValue,  m_HudPanelWidget.FindAnyWidget("Current") );
				VehiclePanelSpeedValue.SetText( m_VehicleGearTable.Get( engaged_gear ) );
							
				Class.CastTo(VehiclePanelSpeedValue,  m_HudPanelWidget.FindAnyWidget("Next") );
				VehiclePanelSpeedValue.SetText( m_VehicleGearTable.Get( next_gear ) );

				Class.CastTo(VehiclePanelSpeedValue,  m_HudPanelWidget.FindAnyWidget("Prev") );
				VehiclePanelSpeedValue.SetText( m_VehicleGearTable.Get( prev_gear ) );

				m_stamina_background.Show( false );
				m_stamina.Show( false );
			}
			else
			{
				if( m_VehiclePanel )
				{
					m_VehiclePanel.Show( false );
				}
			}
		}
	}
	
	void InitQuickbar()
	{
		if (m_quickbar == NULL)
		{
			m_quickbar = new InventoryQuickbar( m_quickbar_widget );
			ShowQuickbar();
		}
	}
	
	InventoryQuickbar GetQuickbar()
	{
		return m_quickbar;
	}
	
	void ShowQuickbar()
	{
#ifdef PLATFORM_XBOX
		return;
#else
#ifdef PLATFORM_PS4
		return;
#endif
#endif
		
		PlayerBase player;
		Class.CastTo(player,  GetGame().GetPlayer() );
		if(!player)
		{
			return;
		}

		if ( m_quickbar_widget && player.GetQuickBarSize() != 0 )
		{
				InventoryGrid quickbarGrid;
				m_quickbar_widget.GetScript(quickbarGrid);
				Widget child = quickbarGrid.GetRoot().GetChildren();
				float alpha = quickbarGrid.GetRoot().GetAlpha();
				RefreshQuickbar();

				m_fade_timers.Clear();
				
				m_quickbar_widget.Show( true );
				m_QuickbarState = true;
				
				while (child)
				{
					child.Show(true);
					child.GetChildren().Show(true);
					child.SetAlpha(alpha);
					child.GetChildren().SetAlpha(1);
					child = child.GetSibling();
				}
		}
	}
	
	void HideQuickbar( bool ignore_state = false , bool instant_hide = false )
	{
		if ( m_quickbar_widget )
		{
			InventoryGrid quickbarGrid;
			m_quickbar_widget.GetScript(quickbarGrid);
			
			if( !instant_hide )
			{
				Widget child = quickbarGrid.GetRoot().GetChildren();
				while (child)
				{
					WidgetFadeTimer fade_timer_quickbar = new WidgetFadeTimer;
					fade_timer_quickbar.FadeOut(child, 5, true);
					m_fade_timers.Insert(fade_timer_quickbar);
					child = child.GetSibling();
				}
			}
			else
			{
				m_quickbar_widget.Show( false );
			}
			
			if ( !ignore_state )
			{
				m_QuickbarState = false;
			}
		}
	}
	
	void ToggleQuickBar( bool show )
	{
		if ( show )
		{
			ShowQuickbar();
		}
		else
		{
			HideQuickbar( false, true );
		}
	}
	
	void ToggleHud( bool show )
	{
		//You can add more widgets to toggle here
		SetLeftStatsVisibility( show );
		m_Badges.Show( show );
		m_Notifiers.Show( show );
		m_HudState = show;

		//! save it to profiles
		g_Game.SetProfileOption( EDayZProfilesOptions.HUD, show );
	}
	
	bool GetQuickBarState()
	{
		return m_QuickbarState;
	}
	
	bool GetHudState()
	{
		return m_HudState;
	}
	
	void SetLeftStatsVisibility(bool visible)
	{
		m_stamina_background.Show(visible);
		m_stamina.Show(visible);
		m_stancePanel.Show(visible);
		m_presence.Show(visible);
	}

	void SetSpecialtyMeterVisibility( bool visible )
	{
		PlayerBase player;
		Class.CastTo(player,  GetGame().GetPlayer() );
		m_specializationPanel.Show( visible );
		float x = player.GetSoftSkillManager().GetSpecialtyLevel() / 2;
		float y = - 0.15; // taken from SpecializationPanel => SpecializationIcon in dayz_z_hud.layout
		m_specializationIcon.SetPos( x, y, true );		
	}
	
	void CheckHudElementsVisibility()
	{
		if (m_HudState != g_Game.GetProfileOption( EDayZProfilesOptions.HUD))
		{
			ToggleHud( g_Game.GetProfileOption( EDayZProfilesOptions.HUD ) );
		}
		if (m_QuickbarState != (g_Game.GetProfileOption( EDayZProfilesOptions.QUICKBAR)))
		{
			ToggleQuickBar( g_Game.GetProfileOption( EDayZProfilesOptions.QUICKBAR ) );
		}
	}
	
	override void RefreshQuickbar( bool itemChanged = false )
	{		
		if( itemChanged )
		{
			UIManager manager = GetGame().GetUIManager();
		
			InventoryMenuNew inventory = InventoryMenuNew.Cast(manager.FindMenu( MENU_INVENTORY ));
			MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());

			
			if(	mission )
			{
				inventory = mission.GetInventory();
			}
		
			if( inventory )
			{
				inventory.RefreshQuickbar();
			}
		}
	
		if ( m_quickbar ) 
		{
			m_quickbar.UpdateItems( m_quickbar_widget );
		}
	}
	
	Widget GetHudPanelWidget()
	{
		return m_HudPanelWidget;
	}
	
	override void UpdateBloodName()
	{
		PlayerBase player;
		Class.CastTo(player, GetGame().GetPlayer() );
		
		if( player )
		{
			string blood_name = BloodTypes.GetBloodTypeName( player.GetBloodType() );
			bool blood_type_visible = player.HasBloodTypeVisible();
			
			if( blood_type_visible )
			{
				m_BloodType.Show( blood_type_visible );
				m_BloodType.SetText( blood_name );	
			}
			else
			{
				m_BloodType.Show( blood_type_visible );
				m_BloodType.SetText( "" );
			}
		}
	}

	protected int m_LastTime;
	protected float m_BlinkTime;
	override void Update()
	{
		super.Update();
		
		m_ActionTarget.Update();
		
		//
		//modifiers - tendency status (critical)
		if ( m_BlinkTime > TENDENCY_BLINK_TIME )
		{
			//get color & alpha
			if ( m_TendencyStatusCritical.Count() > 0 )
			{
				for ( int i = 0; i < m_TendencyStatusCritical.Count(); ++i )
				{
					
					ImageWidget widget = m_TendencyStatusCritical.Get( i );
					int alpha = widget.GetAlpha() * 255;
					int color;
					int colorA = ARGB( alpha, 255, 255, 255 );
					int colorB = ARGB( alpha, 255, 0, 0 );
					
					//set new color to different color
					if ( widget.GetColor() == colorA )
					{
						color = colorB;
					}
					else
					{
						color = colorA;
					}

					widget.SetColor( color );
				}
			}
			
			m_BlinkTime = 0;	//reset
		}
		else
		{
			int current_time = GetGame().GetTime();
			float delta_time = ( current_time - m_LastTime ) / 1000;		//in seconds
			m_BlinkTime += delta_time;
		}
		
		//update time
		m_LastTime = GetGame().GetTime();
		//
	}
}
