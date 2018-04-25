
// *************************************************************************************
// ! PluginDayzPlayerWeaponsDebugUserData
// *************************************************************************************
class PluginDayzPlayerWeaponsDebugUserData
{
	void PluginDayzPlayerWeaponsDebugUserData(int pAction, int pActionType)
	{
		m_iAction = pAction;
		m_iActionT = pActionType;
	}
		

	int		GetAction()
	{
		return m_iAction;
	}

	int		GetActionType()
	{
		return m_iActionT;
	}


	protected int 	m_iAction;
	protected int 	m_iActionT;
}


// *************************************************************************************
// ! PluginDayzPlayerDebug_AbilityConfig
// *************************************************************************************
class PluginDayzPlayerDebug_AbilityConfig
{
	void PluginDayzPlayerDebug_AbilityConfig(string pName, int pAction, int pActionT) 
	{ 
		m_Name = pName; 
		m_iAction = pAction; 
		m_iActionT = pActionT; 
	}

	string		m_Name;
	int				m_iAction;
	int				m_iActionT;
};



// *************************************************************************************
// ! PluginDayzPlayerDebug  
// *************************************************************************************
class PluginDayzPlayerDebug_Weapons 
{	
	Widget 									m_MainWnd;

	TextListboxWidget				m_Selector;
	ButtonWidget						m_ButtonStart;
	TextWidget							m_WidgetActionRunning;	
	TextWidget							m_WidgetActionEvents;
	Weapon_Base							m_CurrentWeapon;
	EditBoxWidget 						m_SetInitStateI;
	ButtonWidget						m_SetInitStateB;

	// command handler properties
	int 								m_CH_WeapAction			= -1;
	int 								m_CH_WeapActionType		= -1;
	int 								m_CH_LastWeapAction		= -1;
	int 								m_CH_LastWeapActionType	= -1;
	//---------------------------------------------------
	// 

	//!
	ref array<ref PluginDayzPlayerDebug_AbilityConfig> m_AbilityConfig =
	{ 
		PluginDayzPlayerDebug_AbilityConfig("Rifle - Reload Mag Bullet", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_BULLET),
		PluginDayzPlayerDebug_AbilityConfig("Rifle - Reload NoMag Buller", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_NOMAGAZINE_BULLET),
		PluginDayzPlayerDebug_AbilityConfig("Rifle - Reload Mag NoBullet", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_NOBULLET),
		PluginDayzPlayerDebug_AbilityConfig("Rifle - Reload NoMag NoBullet", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_NOMAGAZINE_NOBULLET),
		PluginDayzPlayerDebug_AbilityConfig("Rifle - Reload Mag NoBullet lock", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_NOBULLET_OPEN),
		PluginDayzPlayerDebug_AbilityConfig("Rifle - Reload NoMag NoBullet lock", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_NOMAGAZINE_NOBULLET_OPEN),

		PluginDayzPlayerDebug_AbilityConfig("Pistol Closed - Reload Mag Bullet", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_MAGAZINE_BULLET_CLOSED),
		PluginDayzPlayerDebug_AbilityConfig("Pistol Closed - Reload NoMag Bullet", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_NOMAGAZINE_BULLET_CLOSED),
		PluginDayzPlayerDebug_AbilityConfig("Pistol Closed Charged - Reload Mag NoBullet", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_MAGAZINE_NOBULLET_CLOSED_COCKED),
		PluginDayzPlayerDebug_AbilityConfig("Pistol Closed Charged - Reload NoMag NoBullet", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_NOMAGAZINE_NOBULLET_CLOSED_COCKED),
		PluginDayzPlayerDebug_AbilityConfig("Pistol Closed Discharged - Reload Mag NoBullet", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_MAGAZINE_NOBULLET_CLOSED_UNCOCKED),
		PluginDayzPlayerDebug_AbilityConfig("Pistol Closed Discharged - Reload NoMag NoBullet", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_NOMAGAZINE_NOBULLET_CLOSED_UNCOCKED),
		PluginDayzPlayerDebug_AbilityConfig("Pistol Opened - Reload NoMag ", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_NOMAGAZINE_NOBULLET_OPENED),
		PluginDayzPlayerDebug_AbilityConfig("Pistol Opened - Reload Mag ", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADPISTOL_MAGAZINE_NOBULLET_OPENED),
		
		PluginDayzPlayerDebug_AbilityConfig("Pistol Detach Magazine", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOAD_MAGAZINE_DETACH),
		PluginDayzPlayerDebug_AbilityConfig("Rifle Detach Magazine", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADRIFLE_MAGAZINE_DETACH),
		
		PluginDayzPlayerDebug_AbilityConfig("Sp Rifle - Reload Mag Bullet", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_MAGAZINE_BULLET),
		PluginDayzPlayerDebug_AbilityConfig("Sp Rifle - Reload NoMag Buller", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_NOMAGAZINE_BULLET),
		PluginDayzPlayerDebug_AbilityConfig("Sp Rifle - Reload Mag NoBullet", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_MAGAZINE_NOBULLET),
		PluginDayzPlayerDebug_AbilityConfig("Sp Rifle - Reload NoMag NoBullet", WeaponActions.RELOAD, WeaponActionReloadTypes.RELOADSRIFLE_NOMAGAZINE_NOBULLET),

		PluginDayzPlayerDebug_AbilityConfig("Mechanism Opened", WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_OPENED),
		PluginDayzPlayerDebug_AbilityConfig("Mechanism Closed", WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED),
		PluginDayzPlayerDebug_AbilityConfig("Mechanism Spec", WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_SPECIAL),
		PluginDayzPlayerDebug_AbilityConfig("Mechanism Closed Uncocked", WeaponActions.MECHANISM, WeaponActionMechanismTypes.MECHANISM_CLOSED_UNCOCKED),

		PluginDayzPlayerDebug_AbilityConfig("Chamb 1 Bullet Opened", WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_OPENED),
		PluginDayzPlayerDebug_AbilityConfig("Chamb 1 Bullet Closed", WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_CLOSED),
		PluginDayzPlayerDebug_AbilityConfig("Chamb 1 Bullet Unique Opened", WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_UNIQUE_OPENED),
		PluginDayzPlayerDebug_AbilityConfig("Chamb 1 Bullet Unique Closed", WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ONEBULLET_UNIQUE_CLOSED),

		PluginDayzPlayerDebug_AbilityConfig("Chamb 2 Bullets Start", WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_TWOBULLETS_START),
		PluginDayzPlayerDebug_AbilityConfig("Chamb 2 Bullets End", WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_TWOBULLETS_END),

		PluginDayzPlayerDebug_AbilityConfig("Chamb Loopable Start Opened", WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_OPENED),
		PluginDayzPlayerDebug_AbilityConfig("Chamb Loopable Start Closed", WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_STARTLOOPABLE_CLOSED),
		PluginDayzPlayerDebug_AbilityConfig("Chamb Loopable End", WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_ENDLOOPABLE),

		PluginDayzPlayerDebug_AbilityConfig("Crossbow Opened", WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_CROSSBOW_OPENED),
		PluginDayzPlayerDebug_AbilityConfig("Crossbow Close", WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_CROSSBOW_CLOSED),
		PluginDayzPlayerDebug_AbilityConfig("Crossbow Full", WeaponActions.CHAMBERING, WeaponActionChamberingTypes.CHAMBERING_CROSSBOW_FULL),


		PluginDayzPlayerDebug_AbilityConfig("Chamb Loader 1 Bullet Opened", WeaponActions.CHAMBERING_LOADER, WeaponActionChamberingLoaderTypes.CHAMBERINGLOADER_OPENED),
		PluginDayzPlayerDebug_AbilityConfig("Chamb Loader 1 Bullet Closed", WeaponActions.CHAMBERING_LOADER, WeaponActionChamberingLoaderTypes.CHAMBERINGLOADER_CLOSED),

		PluginDayzPlayerDebug_AbilityConfig("Unjamming Start", WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_START),
		PluginDayzPlayerDebug_AbilityConfig("Unjamming End", WeaponActions.UNJAMMING, WeaponActionUnjammingTypes.UNJAMMING_END), 

		PluginDayzPlayerDebug_AbilityConfig("Fire Normal", WeaponActions.FIRE, WeaponActionFireTypes.FIRE_NORMAL),
		PluginDayzPlayerDebug_AbilityConfig("Fire Last", WeaponActions.FIRE, WeaponActionFireTypes.FIRE_LAST),
		PluginDayzPlayerDebug_AbilityConfig("Fire Empty Cocked", WeaponActions.FIRE, WeaponActionFireTypes.FIRE_COCKED),
		PluginDayzPlayerDebug_AbilityConfig("Fire Empty Uncocked", WeaponActions.FIRE, WeaponActionFireTypes.FIRE_UNCOCKED),
		PluginDayzPlayerDebug_AbilityConfig("Fire and Jam", WeaponActions.FIRE, WeaponActionFireTypes.FIRE_JAM),
		PluginDayzPlayerDebug_AbilityConfig("Fire Dry", WeaponActions.FIRE, WeaponActionFireTypes.FIRE_DRY),

	};


	int		FindAbilityConfig(int pAction, int pActionT)
	{
		int		l = m_AbilityConfig.Count();
		for (int i = 0; i < l; i++)
		{
			if (m_AbilityConfig[i].m_iAction == pAction && m_AbilityConfig[i].m_iActionT == pActionT)
			{
				return i;
			}
		}
		return -1;
	}


	//---------------------------------------------------
  // gui stuff

	void PluginDayzPlayerDebug_Weapons(Widget pMainWnd)
	{
		m_MainWnd = pMainWnd;

		CreateModuleWidgets();

		//! 
		DayZPlayer			player 	= DayZPlayer.Cast( GetGame().GetPlayer() );
		if (player)
		{
			HumanCommandWeapons hcw 	= player.GetCommandModifier_Weapons();
			hcw.DebugResetEvents();
		}
	}
	
	
	void ~PluginDayzPlayerDebug_Weapons()
	{
		DestroyModuleWidgets();
	}
	
	
	void CreateModuleWidgets()
	{
		m_Selector		= TextListboxWidget.Cast( m_MainWnd.FindAnyWidget("WeapActionSelector") );
		m_ButtonStart	= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("WeapActionStart") );

		m_WidgetActionRunning = TextWidget.Cast( m_MainWnd.FindAnyWidget("WeapActionRunning") );
		m_WidgetActionEvents  = TextWidget.Cast( m_MainWnd.FindAnyWidget("WeapActionEvents") );

		m_SetInitStateI	= EditBoxWidget.Cast( m_MainWnd.FindAnyWidget("WeapInitStateI") );
		m_SetInitStateB	= ButtonWidget.Cast( m_MainWnd.FindAnyWidget("WeapInitStateB") );


		m_WidgetActionEvents.SetText("Events:"); 

	}
	
	void DestroyModuleWidgets()
	{
	}

	

	//---------------------------------------------------
    // main update

	
	void Tick()
	{
		DayZPlayer player = DayZPlayer.Cast( GetGame().GetPlayer() );
		if (!player)
		{
			return;
		}
		
		if (player)
		{
			EntityAI entityInHands = player.GetHumanInventory().GetEntityInHands();
			Weapon_Base w;
			if (Class.CastTo(w, entityInHands))
			{
				if (m_CurrentWeapon != w)
					WeaponsInit(w);
			}
			else
			{
				m_Selector.ClearItems();
			}
		}

		//! update running action 
		HumanCommandWeapons hcw = player.GetCommandModifier_Weapons();
		
		string text = "Action: " + hcw.GetRunningAction().ToString() + " " + hcw.GetRunningActionType().ToString();
		m_WidgetActionRunning.SetText(text); 


		int 	aEvent = hcw.DebugIsEvent();
		if (aEvent != -1)
		{		
			string eventStr = "Event: ";	
			eventStr = eventStr + aEvent.ToString() + " ";
			while(true)
			{
				aEvent = hcw.DebugIsEvent();
				if (aEvent == -1)
				{
					break;
				}

				Print("Event: " + aEvent.ToString());
				
				eventStr += aEvent.ToString();
				eventStr += " ";
			}

			m_WidgetActionEvents.SetText(eventStr); 
		}			
	}
	
	

	//---------------------------------------------------
    // Actions

	void WeaponsInit(Weapon_Base w)
	{
			
		m_Selector.ClearItems();

		//! select weapon abilities
		int abilityCount = w.GetAbilityCount();

		for (int i = 0; i < abilityCount; ++i)
		{
			AbilityRecord rec = w.GetAbility(i);
			int		abConfigIndex = FindAbilityConfig(rec.m_action, rec.m_actionType);
			if (abConfigIndex != -1)
			{
				m_Selector.AddItem(m_AbilityConfig[abConfigIndex].m_Name, new PluginDayzPlayerWeaponsDebugUserData(rec.m_action, rec.m_actionType), 0);
			}
		}

		m_Selector.AddItem("=========== All - not safe =========", NULL, 0);


		// if (abilityCount == 0)
		{
			int		l = m_AbilityConfig.Count();
			for (int a = 0; a < l; a++)
			{
				m_Selector.AddItem(m_AbilityConfig[a].m_Name, new PluginDayzPlayerWeaponsDebugUserData(m_AbilityConfig[a].m_iAction, m_AbilityConfig[a].m_iActionT), 0);
			}
		}

		m_CurrentWeapon = w;
	}


	//!
	void	WeaponsStartAction()
	{
		int		row = m_Selector.GetSelectedRow();
		if (row == -1)
		{
			return;
		}

		PluginDayzPlayerWeaponsDebugUserData	userData;
		m_Selector.GetItemData(row, 0, userData);
		if (userData)
		{
			m_CH_WeapAction		= userData.GetAction();
			m_CH_WeapActionType	= userData.GetActionType();

			Print("DayzPlayerImplement_Weapons::WeaponsStartAction");
		}
	}


	//---------------------------------------------------
    // window ui clicks

	//! buttons clicks
	bool	OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_ButtonStart)
		{
			WeaponsStartAction();
			return true;
		}
	    else if (w == m_SetInitStateB)
		{
			int frameI = m_SetInitStateI.GetText().ToInt();
			
			DayZPlayer player = DayZPlayer.Cast( GetGame().GetPlayer() );

			HumanCommandWeapons 	hcw = player.GetCommandModifier_Weapons();
			hcw.SetInitState(frameI);			
		}
		
	
		return false;
	}


	//---------------------------------------------------
    // Global handler to handle commands from player

	void 	CommandHandler()
	{

		// last action again 
		if (KeyState(KeyCode.KC_MULTIPLY) == 1)
		{
			ClearKey(KeyCode.KC_MULTIPLY);
			m_CH_WeapAction 		= m_CH_LastWeapAction;
			m_CH_WeapActionType 	= m_CH_LastWeapActionType;

			Print("DayzPlayerImplement_Weapons OnKC_MULTIPLY");
		}


		if (m_CH_WeapAction >= 0)
		{
			m_CH_LastWeapAction 	= m_CH_WeapAction;
			m_CH_LastWeapActionType = m_CH_WeapActionType;
				
			//!
			DayZPlayer player = DayZPlayer.Cast( GetGame().GetPlayer() );
			if (player != NULL)
			{
				HumanCommandWeapons 	hcw = player.GetCommandModifier_Weapons();
				hcw.StartAction(m_CH_WeapAction, m_CH_WeapActionType);

				Print("DayzPlayerImplement_Weapons::WeaponsStartAction2");

			}

			m_CH_WeapAction = -1;
		}

	}
}
