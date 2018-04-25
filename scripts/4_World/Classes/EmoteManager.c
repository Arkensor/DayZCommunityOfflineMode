class EmoteCB : HumanCommandActionCallback
{
	bool m_IsFullbody;
	
	bool CancelCondition()
	{
		return false;
	}
	
	bool IsEmoteCallback()
	{
		return true;
	}
	
	/*override void OnFinish(bool pCanceled)
	{
		Print("Cancelled? " + pCanceled);
	}*/
};

/*
class EmoteSittingAndDoingSmthingCB : HumanCommandActionCallback //jtomasik - sem se daj asi dopsat nejaky dalsi callback extra burty pro sezeni se zbrani atp
{	
	bool IsEmoteCallback()
	{
		return true;
	}
};
*/

class EmoteManager
{		
	PlayerBase 				m_Player;
	ItemBase				m_item;
	EmoteCB					m_Callback;
	HumanInputController 	m_HIC;
	protected int  			m_GestureID;
	protected bool			m_BelayedEmote;
	protected bool			m_ItemToHands;
	protected bool			m_ItemIsOn;
	protected bool			m_bEmoteIsPlaying;
	protected int			m_BelayedEmoteID;
	protected int			m_PreviousGestureID;
	protected int			m_CurrentGestureID;
	protected bool			m_MouseButtonPressed;
	protected bool 			m_PlayerDies;
	
	void EmoteManager( PlayerBase player ) 
	{
		m_Player = player;
		m_HIC = m_Player.GetInputController();
		m_ItemIsOn = false;
	}
	
	void SetGesture(int id)
	{
		m_GestureID = id;
	}
	
	int GetGesture()
	{
		return m_GestureID;
	}
	
	//Called from players commandhandler each frame, checks input
	void Update( float deltaT )
	{
		if ( m_Callback ) 
		{
			bool uiGesture = false;
			if( !GetGame().IsMultiplayer() || GetGame().IsClient() )
			{
				uiGesture = GetGame().GetUIManager().IsMenuOpen(MENU_GESTURES);
			}
			
			//jtomasik - asi bych nemel checkovat jestli hrac klika v menu nebo ve scene tady, ale mel by to vedet input manager?
			if( (m_HIC.IsGestureSlot() > 0) || uiGesture || (m_HIC.IsSingleUse() && !uiGesture) || (m_HIC.IsContinuousUseStart() && !uiGesture) || (m_Callback.m_IsFullbody && !uiGesture && m_HIC.IsWeaponRaised()) ) 
			{
				if (m_CurrentGestureID == ID_EMOTE_SUICIDE  && m_HIC.IsSingleUse() /*m_MouseButtonPressed*/)
				{
					if (m_Callback.GetState() == m_Callback.STATE_LOOP_LOOP)
					{
						CommitSuicide();
					}
					else
					{
						return;
					}
				}
				else if (m_CurrentGestureID == ID_EMOTE_THUMB && m_HIC.IsSingleUse())
				{
					m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_ACTION);
				}
				else
				{
					m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
				}
				
				//m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END);

				if ( m_HIC.IsGestureSlot() && m_PreviousGestureID != m_HIC.IsGestureSlot() )
				{
					m_BelayedEmote = true;
					m_BelayedEmoteID = m_HIC.IsGestureSlot();
				}
			}
		}
		else
		{
			if( m_bEmoteIsPlaying)
			{
				m_bEmoteIsPlaying = false;
				OnEmoteEnd();
			}
			else if( !m_HIC.IsWeaponRaised() && !m_Player.IsSwimming() && !m_Player.IsClimbingLadder() ) 	//TODO rework conditions into something better?
			{
				PickEmote();
			}
		}
	}
	
	void OnEmoteEnd()
	{
		if ( m_ItemToHands )
		{
			ShowItemInHands();
		}
		
		//TODO : fix once animations are in place
		//if suicide has been completed, kill player
		if ( m_PlayerDies )
		{
			m_Player.SetHealth(0.0);
			return;
		}
		
		if ( m_BelayedEmote )
		{
			if ( m_PreviousGestureID != m_BelayedEmoteID )
			{
				BelayedPickEmote(m_BelayedEmoteID);
			}
			m_BelayedEmoteID = -1;
			m_BelayedEmote = false;
		}
		else
		{
			/*int gesture = GetGesture();
			if ( gesture > 0 )
			{
				if( !GetGame().IsMultiplayer() || GetGame().IsClient() ) 
				{
					PlayEmote(gesture);
					SetGesture(-1);
				}
				if( GetGame().IsClient() )
				{
					ScriptInputUserData ctx = new ScriptInputUserData;
					ctx.Write(INPUT_UDT_GESTURE);
					ctx.Write(gesture);
					ctx.Send();
				}
			}
			else
			{*/
				if( m_Player.GetActionManager() ) m_Player.GetActionManager().EnableActions();					
			//}
		}

		//! back to the default - shoot from camera - if not set already
		if (!m_Player.IsShootingFromCamera()) m_Player.OverrideShootFromCamera(true);
	}
	
	/*bool OnInputUserDataProcess(int userDataType, ParamsReadContext ctx)
	{
		if( userDataType == INPUT_UDT_GESTURE)
		{
			int gestureID = -1;
			if(ctx.Read(gestureID))
			{
				if( gestureID >= 0 )
				{
					PlayEmote(gestureID);
				}
				else
				{
					m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END);		
				}								
			}
			return true;
		}
		return false;
	}*/
	
	//Configure Emote parameters for callback here
	bool PlayEmote( int id )
	{
		if ( !m_Player || m_Player.GetCommand_Action() || m_Player.GetCommandModifier_Action() )
		{	
			return false;
		}
		m_CurrentGestureID = id;
		if( id > 0)
		{
			m_bEmoteIsPlaying = true;
			
			switch ( id )
			{
				case ID_EMOTE_GREETING :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_GREETING,DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
				break;
			
				case ID_EMOTE_SOS :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_SOS,DayZPlayerConstants.STANCEMASK_ERECT,true);
					HideItemInHands();
				break;
			
				case ID_EMOTE_HEART :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_HEART,DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					HideItemInHands();
				break;
			
				case ID_EMOTE_TAUNT :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_TAUNT,DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
				break;
			
				case ID_EMOTE_LYINGDOWN :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_LYINGDOWN,DayZPlayerConstants.STANCEMASK_CROUCH,true);
					HideItemInHands();
				break;
			
				case ID_EMOTE_TAUNTKISS :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_TAUNTKISS,DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
				break;
			
				case ID_EMOTE_POINT :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_POINT,DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
					//m_bEmoteIsPlaying = false;
				break;
				
				case ID_EMOTE_TAUNTELBOW :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_TAUNTELBOW,DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
				break;
			
				case ID_EMOTE_THUMB :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_THUMB,DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
				break;
			
				case ID_EMOTE_THROAT :
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREMOD_THROAT,DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT,false);
				break;
			
				case ID_EMOTE_SUICIDE :
					int suicideID = -1;
					ItemBase weapon;
					weapon = m_Player.GetItemInHands();
					//suicideID = DayZPlayerConstants.CMD_SUICIDEFB_UNARMED; //unarmed suicide...optional?
					if (weapon)
					{
						if (weapon.IsKindOf("Pistol_Base"))
						{
							suicideID = DayZPlayerConstants.CMD_SUICIDEFB_PISTOL;
							//! switch to shoot from weapons instead of camera
							m_Player.OverrideShootFromCamera(false);
						}

						else if (weapon.IsKindOf("Rifle_Base"))
						{
							suicideID = DayZPlayerConstants.CMD_SUICIDEFB_RIFLE;
							//! switch to shoot from weapons instead of camera
							m_Player.OverrideShootFromCamera(false);
						}
				
						else if (weapon.GetType() == "FirefighterAxe" || weapon.IsKindOf("FirefighterAxe")) suicideID = DayZPlayerConstants.CMD_SUICIDEFB_FIREAXE; //should work for colored variants
				
						else if (weapon.GetType() == "Pitchfork") suicideID = DayZPlayerConstants.CMD_SUICIDEFB_PITCHFORK;
				
						else if (weapon.GetType() == "Sword") suicideID = DayZPlayerConstants.CMD_SUICIDEFB_SWORD;
				
						else if (weapon.GetType() == "WoodAxe") suicideID = DayZPlayerConstants.CMD_SUICIDEFB_WOODAXE;
				
						else if (weapon.GetType() == "Spear") suicideID = DayZPlayerConstants.CMD_SUICIDEFB_SPEAR;
				
						else if (weapon.ConfigGetBool("isMeleeWeapon")) suicideID = DayZPlayerConstants.CMD_SUICIDEFB_1HD; //one-handed suicide checks "isMeleeWeapon" bool of items
					}
			
					if (suicideID > -1)
					{
						CreateEmoteCallback(EmoteCB,suicideID,DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH,true);
					}
				break;
			
				case ID_EMOTE_DANCE:
					CreateEmoteCallback(EmoteCB,DayZPlayerConstants.CMD_GESTUREFB_DANCE,DayZPlayerConstants.STANCEMASK_ERECT,true);
					HideItemInHands();
				break;
			
				default :
					Print("EmoteManager.c | PlayEmote | WRONG ID");
					m_bEmoteIsPlaying = false;
					return false;
				break;
			}

		}
		if( m_bEmoteIsPlaying )
			if( m_Player.GetActionManager() )m_Player.GetActionManager().DisableActions();	
		
		return true;
	}
	
	protected void PickEmote()
	{
		if ( m_Player )
		{
			m_PreviousGestureID = m_HIC.IsGestureSlot();
			
			switch ( m_HIC.IsGestureSlot() )
			{
				case 1 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot1() );
				break;
				
				case 2 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot2() );
				break;
				
				case 3 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot3() );
				break;
				
				case 4 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot4() );
				break;
				
				case 5 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot5() );
				break;
				
				case 6 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot6() );
				break;
				
				case 7 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot7() );
				break;
				
				case 8 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot8() );
				break;
				
				case 9 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot9() );
				break;
				
				case 10 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot10() );
				break;
				
				case 11 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot11() );
				break;
				
				case 12 :
					//PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot12() );
				break;
			}
		}
	}
	
	protected void BelayedPickEmote(int id)
	{
		if ( m_Player )
		{
			m_PreviousGestureID = id;
			
			switch ( id )
			{
				case 1 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot1() );
				break;
				
				case 2 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot2() );
				break;
				
				case 3 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot3() );
				break;
				
				case 4 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot4() );
				break;
				
				case 5 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot5() );
				break;
				
				case 6 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot6() );
				break;
				
				case 7 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot7() );
				break;
				
				case 8 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot8() );
				break;
				
				case 9 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot9() );
				break;
				
				case 10 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot10() );
				break;
				
				case 11 :
					PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot11() );
				break;
				
				case 12 :
					//PlayEmote( m_Player.m_ConfigEmotesProfile.GetEmoteSlot12() );
				break;
			}
		}
	}
	
	//creates Emote callback
	protected void CreateEmoteCallback(typename callbacktype, int id, int mask, bool fullbody )
	{
		if ( m_Player )
		{
			if (  fullbody )
			{
				Class.CastTo(m_Callback, m_Player.StartCommand_Action(id,callbacktype,mask));
				m_Callback.m_IsFullbody = true;
				m_Callback.EnableCancelCondition(true);
			}
			else if (m_Player.IsPlayerInStance(mask))
			{
				Class.CastTo(m_Callback, m_Player.AddCommandModifier_Action(id,callbacktype));
			}
		}
	}
	
	protected void HideItemInHands()
	{
		m_item = m_Player.GetItemInHands();
		if(m_Callback && m_item)
		{
			m_ItemIsOn = m_item.IsPilotLight();
			if (m_ItemIsOn)
				m_item.SetPilotLight(false);
			m_item.SetInvisible(true);
			m_ItemToHands = true;
		}
	}
	
	protected void ShowItemInHands()
	{
		if (m_item)
		{
			if (m_ItemIsOn)
			{
				m_item.SetPilotLight(m_ItemIsOn);
				m_ItemIsOn = false;
			}
			m_item.SetInvisible(false);
			m_Player.GetItemAccessor().OnItemInHandsChanged();
		}
		m_ItemToHands = false;
	}
	
	protected void CommitSuicide()
	{
		Weapon_Base weapon;
		WeaponEventBase weapon_event = new WeaponEventTrigger;
		//firearm suicide
		if (Weapon_Base.CastTo(weapon,m_Player.GetItemInHands()))
		{
			/*
			if (weapon.GetMagazine(weapon.GetCurrentMuzzle()))
			{
				Magazine mag = weapon.GetMagazine(weapon.GetCurrentMuzzle());
				float damage;
				string ammotype;
				mag.GetCartridgeAtIndex(0,damage,ammotype);
				Print(damage + " " + ammotype);
			}
			*/
			
			//TODO : check multiple muzzles for shotguns, eventually
			if (weapon.CanFire())
			{
				weapon.ProcessWeaponEvent(weapon_event);
				m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END2);
				if (m_Player.IsAlive()) 	(GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.KillPlayer, 3800, false); //kills player after certain amount of time, hotfix
			}
			else
			{
				weapon.ProcessWeaponEvent(weapon_event);
				m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
			}
		}
		//melee weapon suicide
		else
		{
			m_Callback.InternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END2);
			//m_PlayerDies = true;
			
			//temporary workaround, fix in animation
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.KillPlayer, 4000, false);
			//
		}
	}

	protected void KillPlayer()
	{
		if (GetGame().IsServer())
		{
			m_Player.SetHealth(0);
		}
	}
/*
	bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		m_MouseButtonPressed = true;
	}
	
	bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		m_MouseButtonPressed = false;
	}
*/
};
