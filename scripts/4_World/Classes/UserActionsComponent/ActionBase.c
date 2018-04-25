class ActionBaseCB : HumanCommandActionCallback
{	
	protected ActionBase				m_ActionData;//ref to static part
	protected int						m_State; //action module returns here its current state
	protected ref ActionTarget			m_Target; //target of action
	protected ItemBase					m_Item; //item in players hands being used for action
	protected PlayerBase				m_Player;//performer of actions
	protected ref CABase				m_ActionComponent;//action component
	protected SoundOnVehicle 			m_SoundObject;//object of sound playing on entity
	protected bool 						m_Canceled;//helps prevent doubled calling of actionbase End method
	protected bool						m_Interrupted;//force callback to wait till action syncs its interruption
	protected int 						m_PossibleStanceMask;//in which stances is action possible
		
		
	void ActionBaseCB()
	{
		m_State = UA_START;
	}
	
	int GetPossileStanceMask()
	{
		return m_PossibleStanceMask;
	}
	
	//Command events
	override void OnFinish(bool pCanceled)	
	{
		PrintString("ActionBase.c | ActionBaseCB | OnFinish called : " + pCanceled.ToString() );	
		if ( m_SoundObject ) 
		{
			GetGame().ObjectDelete(m_SoundObject);
		}
		if ( m_Player )
		{
			if ( pCanceled ) 
			{
				m_State = m_ActionComponent.Interrupt(m_Player, m_Target, m_Item);
			}
			
			if(m_ActionData)
				m_ActionData.End(this,m_State,m_ActionComponent,m_Player,m_Target,m_Item);
			
			m_Player.GetActionManager().OnActionEnd(m_ActionData, m_Target, m_Item);
		}

		
	}
	
	void SetCommand( int command_uid )
	{		
		InternalCommand(command_uid);
	}

	//---------------------------------------------------------------
	//Action Component handlers
	void CreateActionComponent() 
	{
		m_ActionComponent = new CADummy;
	}
	
	void InitActionComponent()
	{
	}
	
	void ProgressActionComponent()
	{
		if ( m_ActionComponent )
		{ 
			m_State = m_ActionComponent.Execute(m_Player, m_Target, m_Item);
		}
	}	
	
	void EndActionComponent()
	{
	}	
	
	//data
	void Interrupt()
	{
		if ( GetGame().IsServer() )
		{
			PrintString("Interupt in actionbase called - server");
			if( GetGame().IsMultiplayer() )
			{
				DayZPlayerSyncJunctures.SendActionInterrupt(m_Player);
			}
		}
		m_Interrupted = true;
	}
	
	void SetActionData(ActionBase actionBase, PlayerBase player, ActionTarget target, ItemBase item, int possibleStanceMask )
	{
		m_ActionData = actionBase;
		m_Player = player;
		m_Target = target;
		m_Item = item;
		m_PossibleStanceMask = possibleStanceMask;
	}
	
	override bool IsUserActionCallback()
	{
		return true;
	}
	
	float GetActionComponentProgress()
	{
		if ( m_ActionComponent )
		{
			return m_ActionComponent.GetProgress();
		}
		return 0;
	}
	
	int GetActionState()
	{
		return m_State;
	}
	
	ActionTarget GetTarget()
	{
		return m_Target;
	}
};



class ActionBase
{	
	//STATIC DATA
	// Configurable action parameters
	protected string				m_MessageStartFail; //called from ontick method of quantity and time baseclasses, usually informing player that item is empty
	protected string				m_MessageStart; //called from onstart method, usually informing player that he started the action
	protected string				m_MessageSuccess; //called from ontick method, ussualy informing player that he succesfully finished the action
	protected string 				m_MessageFail; //called from cancel or ontick methods,  ussualy informing player that he or target moved and thus was action canceled
	protected string				m_MessageCancel; //called from cancel method, ussualy informing player that he stoped holding RMB and canceled action
	protected string				m_MessageSuccessTarget; //called from ontick method, ussualy informing other player that he succesfully finished the action
	protected string 				m_MessageStartTarget; //called from cancel or ontick methods,  ussualy informing other player that he or target moved and thus was action canceled
	protected string				m_MessageCancelTarget; //called from cancel method, ussualy informing other player that he stoped holding RMB and canceled action
	protected string				m_Sound; //sound played at the beggining of action

	protected bool					m_LockTargetOnUse;	//this parameter sets wheter player can perform actions on target while other player is already performing action on it. defaulted as true
	protected bool 					m_FullBody; //tells whether action is full body or additive
	protected int					m_StanceMask;
	protected int 					m_CommandUID;	//tells which command should be used for callback
	protected int					m_CommandUIDProne;
	protected typename 				m_CallbackClass; //tells which callback should be instantiated
	protected ActionBaseCB			m_Callback;
	protected ref TStringArray		m_Sounds;			//User action sound is picked from this array randomly
	ref CCIBase 					m_ConditionItem;	//Condition Component
	ref CCTBase						m_ConditionTarget; 	//Condition Component

	//RUNTIME DATA
	protected ref Param1<string> 	m_MessageParam; //used for passing messages from server to client
	protected ref Param2<int,int>	m_MessagesParam; 	
	
	//SOFT SKILLS
	protected float					m_SpecialtyWeight;
	
	//-----------------------------------------------------
	// 	Action events and methods
	//-----------------------------------------------------
	void ActionBase() 
	{
		// definable
		m_CallbackClass = ActionBaseCB;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE;
		m_FullBody = false;
		m_Sound = "";
		m_LockTargetOnUse = HasTarget();
		m_MessageStartFail = "The action failed";
		m_MessageStart = "I have started an action.";
		m_MessageSuccess = "The action succesfully finished.";
		m_MessageFail = "The action failed.";
		m_MessageCancel = "I have canceled the action.";
		m_MessageStartTarget = "Other player started performing action on you.";
		m_MessageSuccessTarget = "Other player succesfuly performed action on you.";
		m_MessageCancelTarget = "Other player canceled the action.";
		
		// dont override
		m_MessageParam = new Param1<string>("");
		m_MessagesParam = new Param2<int,int>(0,0);
		m_Sounds = new TStringArray;
	}
			
	void CreateConditionComponents() //Instantiates components, called once from ActionManager on actions construction
	{
		m_ConditionItem = new CCIDummy;
		m_ConditionTarget = new CCTDummy;
	}

	bool HasTarget()
	{
		return true;
	}
	
	//Action isn't synchronize to server 
	bool IsLocal()
	{
		return false;
	}
	
	//Action not using animation/callback action are executed instantly (OnStart)
	bool IsInstant()
	{
		return false;
	}
	
	bool RemoveForceTargetAfterUse()
	{
		return true;
	}

	bool Consumable()
	{
		return false;
	}
	
	int GetActionCategory()
	{
		return AC_UNCATEGORIZED;
	}
	
	int GetType()  //returns action uid
	{
		return 0;
	}

	string GetText() //text game displays in HUD hint 
	{
		return "default action text";
	}
	
	string GetTargetDescription()
	{
		return "default target description";
	}
	
	// Server version call on single too
	protected void OnStartServer( PlayerBase player, ActionTarget target, ItemBase item ) //setup of action parameters, called before performing action
	{
	}
	protected void OnStartClient( PlayerBase player, ActionTarget target, ItemBase item ) //setup of action parameters, called before performing action
	{
	}

	protected void OnCancelServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata  ) //method called when player stops inputing
	{
	}
	protected void OnCancelClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata  ) //method called when player stops inputing
	{
	}
	
	protected void OnInterruptServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata  ) //method called when player can no longer continue action
	{
		OnCancelServer( player, target, item, acdata );
	}
	protected void OnInterruptClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata  ) //method called when player can no longer continue action
	{
		OnCancelClient( player, target, item, acdata );
	}
	
	protected void OnAlternativeEndServer( PlayerBase player ) //method called when action has not met conditions in action component
	{
	}
	protected void OnAlternativeEndClient( PlayerBase player ) //method called when action has not met conditions in action component
	{
	}

	protected void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata ) //method called on succesful finishing of action (after out animation)
	{
	}
	protected void OnCompleteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata ) //method called on succesful finishing of action (after out animation)
	{
	}
	
	protected void OnCompleteLoopServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata ) //method called on succesful finishing of action (before out animation - continuous actions only )
	{
	}
	protected void OnCompleteLoopClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata ) //method called on succesful finishing of action (before out animation - continuous actions only )
	{
	}	
	
	protected void OnRepeatServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata ) //method called on succesful finishing of action
	{
	}
	protected void OnRepeatClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata ) //method called on succesful finishing of action
	{
	}
	
	protected void OnExecuteServer(PlayerBase player, ActionTarget target, ItemBase item, Param acdata)
	{
	}
	
	protected void OnExecuteClient(PlayerBase player, ActionTarget target, ItemBase item, Param acdata)
	{
	}
	
	protected bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) //condition for action
	{
		return true;
	}
	
	void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item ) // method that is planned to be called after every succesful completion of action to transfer diseases and other modifiers, now is called before oncompletes
	{
		PlayerBase targetPlayer;
		if ( Class.CastTo(targetPlayer, target.GetObject()) && item )
		{
			item.TransferModifiers(targetPlayer);
		}
	}

	void WriteToContext (ParamsWriteContext ctx, ActionTarget target)
	{
		ctx.Write(INPUT_UDT_STANDARD_ACTION);
		ctx.Write(GetType());
		
		
		if( HasTarget() )
		{
			// callback data
			Object targetObject = target.GetObject();
			ctx.Write(targetObject);
			Object targetParent = target.GetParent();
			ctx.Write(targetParent);
			int componentIndex = target.GetComponentIndex();
			ctx.Write(componentIndex);
		}
	}
	
	//----------------------------------------------------------------------------------------------
	// Core methods don't override unless you know what you are doing
	//----------------------------------------------------------------------------------------------		
	// COMMANDS -----------------------------------------------------------------------		
	protected int GetStanceMask(PlayerBase player)
	{
		if ( HasProneException() )
		{
			if ( player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT))
				return DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
			else
				return DayZPlayerConstants.STANCEMASK_PRONE;
		}
		return m_StanceMask;
	}
	
	protected bool IsFullBody(PlayerBase player)
	{
		if ( HasProneException() )
		{
			if ( player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT))
				return false;
			else
				return true;
		}
		return m_FullBody;
	}
	
	protected int GetActionCommand(PlayerBase player)
	{
		if ( HasProneException() )
		{
			if ( player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT))
				return m_CommandUID;
			else
				return m_CommandUIDProne;
		}
		return m_CommandUID;
	}
	
	protected typename GetCallbackClassTypename()
	{
		return m_CallbackClass; 
	}
	
	// if it is set to true if action have special fullbody animation for prone and additive for crouch and erect
	protected bool HasProneException()
	{
		return false;
	}
	
	protected ActionBaseCB CreateAndSetupActionCallback( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//Print("ActionBase.c | CreateAndSetupActionCallback | DBG ACTION CALLBACK CREATION CALLED");
		ActionBaseCB callback;
		if (  IsFullBody(player) )
		{
			Class.CastTo(callback, player.StartCommand_Action(GetActionCommand(player),GetCallbackClassTypename(),GetStanceMask(player)));	
			//Print("ActionBase.c | CreateAndSetupActionCallback |  DBG command starter");		
		}
		else
		{
			Class.CastTo(callback, player.AddCommandModifier_Action(GetActionCommand(player),GetCallbackClassTypename()));
			//Print("ActionBase.c | CreateAndSetupActionCallback |  DBG command modif starter: "+callback.ToString()+"   id:"+GetActionCommand().ToString());
			
		}
		callback.SetActionData(this, player, target, item, GetStanceMask(player)); 
		callback.InitActionComponent(); //jtomasik - tohle mozna patri do constructoru callbacku?
		return callback;
	}
		
		
		
	//	ACTION LOGIC -------------------------------------------------------------------
	// called from actionmanager.c
	void Start( PlayerBase player, ActionTarget target, ItemBase item ) //Setup on start of action
	{
		if( GetGame().IsServer() )
		{
			OnStartServer(player, target, item);
		}
		else
		{
			OnStartClient(player, target, item);
		}
		
		InformPlayers(player,target,UA_START);	
		if( !IsInstant() )
		{
		//player.GetActionManager().DisableActions();
			m_Callback = CreateAndSetupActionCallback(player, target, item);
		}
		/*Debug
		SendMessageToClient( player, "ActionBase.c : Start");
		Print("ActionBase.c : Start");
		*/
	}

	bool Can( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !m_FullBody && (!player || !player.IsPlayerInStance(GetStanceMask(player))) )
		{
			return false;
		}
		if ( m_ConditionItem && m_ConditionItem.Can(player, item) && m_ConditionTarget && m_ConditionTarget.Can(player, target) && ActionCondition(player, target, item) ) 
		{	
			return true;
		}
		return false;
	}
	
	protected bool CanContinue( PlayerBase player, ActionTarget target, ItemBase item, int possibleStanceMask )
	{
		if ( player.IsPlayerInStance(possibleStanceMask) && m_ConditionItem && m_ConditionItem.CanContinue(player,item) && m_ConditionTarget && m_ConditionTarget.CanContinue(player,target) && ActionCondition(player, target, item) )
		{
			return true;
		}
		return false;
	}
		
	// called from ActionBaseCB.c
	void Do( ActionBaseCB callback, int state, CABase action_component, PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( state == UA_ERROR || !callback || !player || !action_component ) //jtomasik - tohle mozna muze byt v CancelCondtion metodu callbacku?
		{
#ifdef DEVELOPER
			Print("ActionBase.c | Do | ABRUPT CANCEL, CONDITIONS NOT MET");
#endif
			if ( callback && player )
			{			
				callback.Interrupt();
			}
			else
			{
				Print("ActionBase.c | Do | PLAYER LOST");
			}	
		}
		else
		{
			switch ( state )
			{				
				//case UA_SETEND_2:
					//OnAlternativeEnd();
					//InformPlayers(player,target,UA_ERROR);
					//callback.Interrupt();
					//callback.SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END2);//jtomasik - tohle pak zapoj do toho endovani az ti vse ostatni pobezi,  ted prehraje END2 (a tim to i ukonci), pozdeji by mozna piti prazdne lahve atp. mela byt samostatna akce
					//break;
				
				case UA_PROCESSING:	
					if ( CanContinue(player,target,item,callback.GetPossileStanceMask()) )
					{	
						callback.ProgressActionComponent();
						InformPlayers(player,target,UA_PROCESSING);
					}
					else
					{
						callback.Interrupt();
						InformPlayers(player,target,UA_FAILED);
						Do(callback,UA_CANCEL,action_component,player,target,item);
					}
					break;
					
				case UA_REPEAT:
					if ( CanContinue(player,target,item,callback.GetPossileStanceMask()) )
					{
						if ( GetGame().IsServer() )
						{
							OnRepeatServer(player,target,item,action_component.GetACData());
							ApplyModifiers(player,target,item);
						}
						else
						{
							OnRepeatClient(player,target,item,action_component.GetACData());
						}
						InformPlayers(player,target,UA_REPEAT);
						Do(callback,UA_PROCESSING,action_component,player,target,item);
					}
					else
					{
						callback.Interrupt();
						InformPlayers(player,target,UA_FAILED);
						Do(callback,UA_FINISHED,action_component,player,target,item);
					}				
					break;
				
				case UA_ANIM_EVENT:
					if( CanContinue(player,target,item,callback.GetPossileStanceMask()))
					{
						if ( GetGame().IsServer() )
						{
							OnExecuteServer(player,target,item,action_component.GetACData());
							ApplyModifiers(player,target,item);
						}
						else
						{
							OnExecuteClient(player,target,item,action_component.GetACData());
						}
						InformPlayers(player,target,UA_REPEAT);
						Do(callback,UA_PROCESSING,action_component,player,target,item);
					
				
					}
					else
					{
						callback.Interrupt();
						InformPlayers(player,target,UA_FAILED);
						Do(callback,UA_CANCEL,action_component,player,target,item);
					}
					break;
				case UA_FINISHED:
					if ( GetGame().IsServer() )
					{
						OnCompleteLoopServer(player,target,item,action_component.GetACData());
						ApplyModifiers(player,target,item);
					}
					else
					{
						OnCompleteLoopClient(player,target,item,action_component.GetACData());
					}
					InformPlayers(player,target,UA_FINISHED);
					callback.EndActionComponent();
					break;

				case UA_CANCEL:
					InformPlayers(player,target,UA_CANCEL);
					callback.EndActionComponent();
					break;								
					
				default:
					PrintString("ActionBase.c | Do | ACTION COMPONENT RETURNED WRONG VALUE: "+state.ToString());
					callback.Interrupt();
					break;
			}
		}
	}
	
	// called from ActionBaseCB.c 
	void End( ActionBaseCB callback, int state, CABase action_component, PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( player && action_component ) 
		{
			switch ( state )
			{
				case UA_FINISHED:		
					if ( GetGame().IsServer() )
					{
						OnCompleteServer(player,target,item,action_component.GetACData());
						ApplyModifiers(player,target,item);
					}
					else
					{
						OnCompleteClient(player,target,item,action_component.GetACData());					
					}
					InformPlayers(player,target,UA_FINISHED);
					break;
					
				case UA_CANCEL:
					if ( GetGame().IsServer() )
					{
						OnCancelServer(player, target, item,action_component.GetACData());
					}
					else
					{
						OnCancelClient(player, target, item,action_component.GetACData());					
					}
					InformPlayers(player,target,UA_CANCEL);
					break;
				case UA_INTERRUPT:
					if ( GetGame().IsServer() )
					{
						OnInterruptServer(player, target, item,action_component.GetACData());
					}
					else
					{
						OnInterruptClient(player, target, item,action_component.GetACData());					
					}
					InformPlayers(player,target,UA_INTERRUPT);
					break;
				case UA_PROCESSING:
				case UA_REPEAT:
				case UA_START:
				case UA_STARTT:
					if ( GetGame().IsServer() )
					{
						OnInterruptServer(player, target, item,action_component.GetACData());
					}
					else
					{
						OnInterruptClient(player, target, item,action_component.GetACData());					
					}
					InformPlayers(player,target,UA_INTERRUPT);
					break;
				
					break;
				default:
					Print("ActionBase.c | End | ACTION COMPONENT RETURNED WRONG VALUE");
					callback.Interrupt();
				break;
			}
		}
		else
		{
			Print("ActionBase.c | End | ACTION COULD NOT BE FINISHED RIGHT AT THE END");
			callback.Interrupt();
		}
		//player.GetActionManager().EnableActions();
	}
	

	/*
	void LockTarget( PlayerBase player, ActionTarget target )
	{
		if ( m_LockTargetOnUse && player.m_ModuleObjectsInteractionManager ) 
		{
			player.m_ModuleObjectsInteractionManager.Lock(target.GetObject());
		}
	}
	
	void UnlockTarget( PlayerBase player, ActionTarget target )
	{
		if ( m_LockTargetOnUse && player.m_ModuleObjectsInteractionManager )
		{
			player.m_ModuleObjectsInteractionManager.Release(target.GetObject()); 
		}
	}*/
	
	// call only on client side for lock inventory before action
	// return if has successfuly reserved inventory
	bool InventoryReservation(PlayerBase player, ActionTarget target, ItemBase item, out array<ref InventoryLocation> reservedLocations)
	{
		if( (IsLocal() || !UseAcknowledgment()) && IsInstant() )
			return true;
		
		bool success = true;
		InventoryLocation targetInventoryLocation = NULL;
		InventoryLocation handInventoryLocation = NULL;
		
		// lock target if it has target
		if( HasTarget() )
		{
			ItemBase targetItem;
			if ( ItemBase.CastTo(targetItem,target.GetObject()) )
			{
				targetInventoryLocation = new InventoryLocation;
				targetItem.GetInventory().GetCurrentInventoryLocation(targetInventoryLocation);
				if ( !player.GetInventory().AddInventoryReservation( targetItem, targetInventoryLocation, 10000) )
				{
					success = false;
				}				
			}
		}	
		
		handInventoryLocation = new InventoryLocation;
		handInventoryLocation.SetHands(player,player.GetItemInHands());

		if ( !player.GetInventory().AddInventoryReservation( player.GetItemInHands(), handInventoryLocation, 10000) )
		{
			success = false;
		}
		
		
		if ( success )
		{
			if( targetInventoryLocation )
				reservedLocations.Insert(targetInventoryLocation);
			
			if( handInventoryLocation )
				reservedLocations.Insert(handInventoryLocation);
		}
		// lock Hands
		// On Fail unlock targetEntity
		//if ( targetEntity ) player.GetInventory().ClearReservation(player,targetEntity);
		// find way how to lock only hand slot
		
		return success;
	}

	void ClearInventoryReservation(PlayerBase player, array<ref InventoryLocation> reservedLocations)
	{
		InventoryLocation il;
		for ( int i = 0; i < reservedLocations.Count(); i++)
		{
			il = reservedLocations.Get(i);
			player.GetInventory().ClearInventoryReservation( il.GetItem() , il );
		}
		reservedLocations.Clear();
	}
		
	// MESSAGES --------------------------------------------------------------------
	string GetMessageText( int state ) //returns text of action based on given id
	{
		string message = "";
		switch ( state )
		{
			case UA_ERROR:
				message = GetMessageStartFail();
				break;
				
			case UA_FAILED:
				message = GetMessageFail();
				break;
			
			case UA_INITIALIZE:
				message = "Initialize";
				break;
				
			case UA_PROCESSING:
				message = "Processing";
				break;
				
			case UA_REPEAT:
				message = "loop end";
				break;
				
			case UA_START:
				message = GetMessageStart();
				break;
				
			case UA_FINISHED:
				message = GetMessageSuccess();
				break;
				
			case UA_CANCEL:
				message = GetMessageCancel();
				break;
			
			case UA_INTERRUPT:
				message = GetMessageInterrupt();
				break;
				
			case UA_STARTT:
				message = GetMessageStartTarget();
				break;
				
			case UA_FINISHEDT:
				message = GetMessageSuccessTarget();
				break;
				
			case UA_CANCELT:
				message = GetMessageCancelTarget();
				break;
				
			default:
				Print("ActionBase.c | Informplayers | CALLED WITH WRONG STATE PARAMETER");
				break;
		}

		return message;
	}
	
	// action need first have permission from server before can start
	bool UseAcknowledgment()
	{
		return true;
	}
	
	protected void InformPlayers( PlayerBase player, ActionTarget target, int state ) //delivers message ids to clients based on given context
	{	
		/*player.MessageAction(GetMessageText(state)); //jtomasik - tohle se smaze az to pujde ven, ted je to jako debug pro lokalni testovani
		
		if( GetGame().IsServer() && player && m_MessagesParam && state > 0 && state < 12 )
		{
			int message_target = 0;
			switch ( state )
			{
				case UA_START:
					message_target = UA_STARTT;
					break;
					
				case UA_FINISHED:
					message_target = UA_FINISHEDT;
					break;
					
				case UA_CANCEL:
					message_target = UA_CANCELT;
					break;
			}
			if ( player.IsDamageDestroyed() )
			{
				m_MessagesParam.param1 = GetType();
				m_MessagesParam.param2 = state;
				GetGame().RPCSingleParam(player, RPC_USER_ACTION_MESSAGES, m_MessagesParam, player.GetIdentity());
			}
			if ( message_target > 0 && m_ConditionTarget && m_ConditionTarget.IsActionOnTarget() && target && target.IsMan() && target.IsAlive() )
			{
				m_MessagesParam.param1 = GetType();
				m_MessagesParam.param2 = message_target;
				GetGame().RPCSingleParam(target, RPC_USER_ACTION_MESSAGES, m_MessagesParam, target.GetIdentity());
			}
		}*/
	}
		
	void SendMessageToClient( Object reciever, string message ) //sends given string to client, don't use if not nescessary
	{
		PlayerBase man;
		if( GetGame().IsServer() && Class.CastTo(man, reciever) && m_MessageParam && reciever.IsAlive() && message != "" )
		{
			m_MessageParam.param1 = message;
			GetGame().RPCSingleParam(man, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, man.GetIdentity());
		}
	}
	
	protected string GetMessageStartFail()
	{
		return m_MessageStartFail;
	}
	
	protected string GetMessageStart()
	{
		return m_MessageStart;
	}
	
	protected string GetMessageSuccess() 
	{
		return m_MessageSuccess;
	}
	
	protected string GetMessageFail()
	{
		return m_MessageFail;
	}
	
	protected string GetMessageCancel() 
	{
		return m_MessageCancel;
	}
	
	protected string GetMessageInterrupt() 
	{
		return "";
		//return m_MessageInterrupt;
	}
	
	protected string GetMessageSuccessTarget() 
	{
		return m_MessageSuccessTarget;
	}
	
	protected string GetMessageStartTarget()
	{
		return m_MessageStartTarget;
	}
	
	protected string GetMessageCancelTarget()
	{
		return m_MessageCancelTarget;
	}

	// ActionCondition Rules
	// ------------------------------------------------------
	protected bool IsDamageDestroyed(ActionTarget target)
	{
		Object obj;
		if(Class.CastTo(obj, target.GetObject()) )
		{
			if( obj.IsDamageDestroyed() )
				return true;
		}

		return false;
	}

	protected bool IsBuilding(ActionTarget target)
	{
		Object obj;
		
		if( Class.CastTo(obj, target.GetObject()) )
			return obj.IsBuilding();

		return false;
	}
	
	protected bool IsTransport(ActionTarget target)
	{
		Object obj;
		if( Class.CastTo(obj, target.GetObject()) )
			return obj.IsTransport();

		return false;		
	}

	protected bool IsInReach(PlayerBase player, ActionTarget target, float maxDistance = 1.0 )
	{
		Object obj;
		if( Class.CastTo(obj, target.GetObject()) )
		{
			string compName;
			float distanceRoot, distanceHead;
			vector modelPos, worldPos, playerRootPos, playerHeadPos;
			
			// we're using sq distance in comparison
			maxDistance = maxDistance * maxDistance;
			playerRootPos = player.GetPosition();
			
			// get position of Head bone
			MiscGameplayFunctions.GetHeadBonePos(player, playerHeadPos);

			compName = target.GetObject().GetActionComponentName(target.GetComponentIndex());
			modelPos = target.GetObject().GetSelectionPosition(compName);
			worldPos = target.GetObject().ModelToWorld(modelPos);

			distanceRoot = Math.AbsFloat(vector.DistanceSq(worldPos, playerRootPos));
			distanceHead = Math.AbsFloat(vector.DistanceSq(worldPos, playerHeadPos));
			if ( distanceRoot <= maxDistance || distanceHead <= maxDistance )
			{	
				return true;			
			}
		}

		return false;
	}	
	// ------------------------------------------------------
	
	
	
	// SOUNDS ------------------------------------------------------
	SoundOnVehicle PlayActionSound( PlayerBase player )
	{
		if( GetGame().IsServer() )
		{
			if ( player ) 
			{
				if ( m_Sound != "" )
				{
					return GetGame().CreateSoundOnObject(player, m_Sound, 6, false);
				}
				if ( m_Sounds && m_Sounds.Count() > 0 )
				{
					int rand_num =  Math.RandomInt(0, m_Sounds.Count());
					return GetGame().CreateSoundOnObject(player, m_Sounds.Get(rand_num), 6, false);
				}
			}
		}

		return NULL;
	}

	// SOFT SKILLS ------------------------------------------------
	float GetSpecialtyWeight()
	{
		if(m_SpecialtyWeight == 0)
		{
#ifdef DEVELOPER
			Print("m_SpecialtyWeight should never be zero");
#endif
		}	

		return m_SpecialtyWeight;	
	}
	
	int GetActionState()
	{
		if ( m_Callback )
		{
			return m_Callback.GetActionState();
		}
		return UA_NONE;
	}
	
	float GetActionComponentProgress()
	{
		if ( m_Callback )
		{	 
			return m_Callback.GetActionComponentProgress();
		}
		return 0;
	}
};