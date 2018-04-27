class ActionManagerClient: ActionManagerBase 
{
	
	//Last send AcknowledgmentID (client can send more requests before recive ackfor first action)
	protected int 								m_LastAcknowledgmentID;
	protected bool								m_ActionPossible;
	protected ref array<ref InventoryLocation>	m_ReservedInventoryLocations;

	void ActionManagerClient(PlayerBase player)
	{
		//ActionManagerBase(player);
		m_LastAcknowledgmentID = 1;
		m_Targets	= new ActionTargets(player);
		m_ReservedInventoryLocations = new array<ref InventoryLocation>;
		EnableActions();
	}
	
	override void Update(int pCurrentCommandID)
	{
		super.Update(pCurrentCommandID);
		
		m_ActionPossible = ActionPossibilityCheck(pCurrentCommandID);
		
		switch (m_PendingActionState)
		{
			case UA_AM_PENDING:
				return;
			
			case UA_AM_ACCEPTED:
				// check pCurrentCommandID before start or reject 
				if( m_ActionPossible && pCurrentCommandID != DayZPlayerConstants.COMMANDID_SWIM && pCurrentCommandID != DayZPlayerConstants.COMMANDID_LADDER )
				{
					m_PendingAction.Start(m_Player,m_PendingActionTarget, m_Player.GetItemInHands());
					if( m_PendingAction.IsInstant() )
						OnActionEnd( m_PendingAction, m_PendingActionTarget, m_Player.GetItemInHands());
				}
				else
				{
					OnActionEnd(m_PendingAction,m_PendingActionTarget,m_Player.GetItemInHands());
				}
				m_PendingAction = NULL;
				m_PendingActionTarget = NULL;
				m_PendingActionAcknowledgmentID = -1;
				m_PendingActionState = UA_NONE;
				return;
			
			case UA_AM_REJECTED:
				OnActionEnd(m_PendingAction,m_PendingActionTarget,m_Player.GetItemInHands());
				m_PendingAction = NULL;
				m_PendingActionTarget = NULL;
				m_PendingActionAcknowledgmentID = -1;
				m_PendingActionState = UA_NONE;
			
				//m_Player.GetDayZPlayerInventory().UnlockHands();
				break;
			
			default:
				break;
		}
		
#ifdef DEVELOPER
			if( DeveloperFreeCamera.IsFreeCameraEnabled() )
			{
				m_ActionPossible = false;
				RemoveSingleUseAction();
				RemoveContinuousAction();
				m_SelectableActions.Clear();
				m_SelectedActionIndex = 0;
			}
			else
			{
#endif
			m_Targets.Update();
			//Looking for suitable contextual actions
			FindContextualUserActions( pCurrentCommandID );
#ifdef DEVELOPER
			}
#endif
	}
	
	override void OnSyncJuncture(int pJunctureID, ParamsReadContext pCtx)
	{
		int AcknowledgmentID;
		pCtx.Read(AcknowledgmentID);
		if ( AcknowledgmentID == m_PendingActionAcknowledgmentID)
		{
			if (pJunctureID == DayZPlayerSyncJunctures.SJ_ACTION_ACK_ACCEPT)
				m_PendingActionState = UA_AM_ACCEPTED;
			else if (pJunctureID == DayZPlayerSyncJunctures.SJ_ACTION_ACK_REJECT)
				m_PendingActionState = UA_AM_REJECTED;
		}
	}
	
	//--------------------------------------------------------
	// Controls avaliability of contextual actions
	//--------------------------------------------------------
/*	void EnableContinuousAction()
	{
		m_PrimaryActionEnabled = true;	
	}
*/	
	override void DisableContinuousAction()
	{
		RemoveContinuousAction();
		m_PrimaryActionEnabled = false;
	}
	
/*	void EnableSingleUseAction()
	{
		m_SecondaryActionEnabled = true;
	}
*/	
	override void DisableSingleUseAction()
	{
		RemoveSingleUseAction();
		m_SecondaryActionEnabled = false;
	}
/*	
	void EnableInteractAction()
	{
		m_TertiaryActionEnabled = true;
	}
*/
	/*override void DisableInteractAction()
	{
		m_TertiaryActionEnabled = false;
	}*/
	
/*	void EnableActions()
	{
		EnableContinuousAction();
		EnableSingleUseAction();
		EnableInteractAction();
	}
	
	void DisableActions()
	{
		DisableContinuousAction();
		DisableSingleUsesAction();
		DisableInteractAction();
	}
*/	
	//--------------------------------------------------------
	// Alows to set different action to current contextual one //jtomasik - pri injectu budu muset serveru poslat ID injectnute akce
	//--------------------------------------------------------
	void InjectContinuousAction( int actionid, ActionTarget target, ItemBase item )
	{
		//DisableContinuousAction();
		
		ActionBase action = GetAction(actionid);
		SetContinuousAction(action,target,item);
		//DeliverInjectedAction(1,action_id,action_target,action_item);
	}
	
	void EjectContinuousAction()
	{
		RemoveContinuousAction();
		EnableContinuousAction();
		//DeliverEjectAction(1);
	}
	
	void InjectSingleUseAction( int actionid, ActionTarget target, ItemBase item )
	{
		//DisableSingleUseAction();
		SetSingleUseAction(GetAction(actionid),target,item);
		
		
		//DeliverInjectedAction(2,action_id,action_target,action_item);
	}
	
	void EjectSingleUseAction()
	{
		RemoveSingleUseAction();
		EnableSingleUseAction();
		//DeliverEjectAction(2);
	}
			
	void EjectActions()
	{
		EjectContinuousAction();
		EjectSingleUseAction();
		//DeliverEjectAction(4);
	}
	
	void ForceTarget(Object targetObject)
	{
		m_ForceTarget = new ActionTarget(targetObject, null, -1, vector.Zero, -1);
	}
	
	void ClearForceTarget()
	{
		m_ForceTarget = NULL;
	}
	
			
	//-------------------------------------------------------------------------
	override ActionTarget FindActionTarget()
	{
		if(m_ForceTarget)
			return m_ForceTarget;
		
		ActionTarget action_target;
		action_target = NULL;
		int targetsCount = m_Targets.GetTargetsCount();
		if( targetsCount )
		{
			for( int i = 0; i < targetsCount; ++i )
			{
				action_target = m_Targets.GetTarget(i);
				Object target = action_target.GetObject();					
				if( target && (target.IsEntityAI() || target.IsWoodBase() || target.IsRock()) )
				{
					break;
				}
			}
		}
		return action_target;
	}
	
	ItemBase FindActionItem()
	{
		ItemBase item;
		if ( m_Player && m_Player.GetItemInHands() && m_Player.GetItemInHands().IsItemBase() ) 
		{
			item = m_Player.GetItemInHands();
		}
		return item;
	}
	
	protected void FindContextualUserActions( int pCurrentCommandID )
	{
		if ( (m_PrimaryActionEnabled || m_SecondaryActionEnabled || m_TertiaryActionEnabled) )
		{
			ActionBase action;
			ActionTarget 	target;
			ItemBase 		item;

			// Gathering current inputs
			RemoveActions();
			item = FindActionItem();
			target = FindActionTarget();
			
			//When player swim there is only enter ladder action
			if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_SWIM )
			{
				m_SelectableActions.Clear();
				m_SelectedActionIndex = 0;
				
				action = GetAction(AT_ENTER_LADDER);
				if ( action && action.Can(m_Player, target, NULL) )
				{
					m_SelectableActions.Insert(new TSelectableActionInfo(SAT_INTERACT_ACTION, AT_ENTER_LADDER, action.GetText()));
				}
				return;
			}
			
			if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_LADDER )
			{
				m_SelectableActions.Clear();
				m_SelectedActionIndex = 0;
				
				action = GetAction(AT_EXIT_LADDER);
				if ( action && action.Can(m_Player, target, NULL) )
				{
					m_SelectableActions.Insert(new TSelectableActionInfo(SAT_INTERACT_ACTION, AT_EXIT_LADDER, action.GetText()));
				}
				return;
			}
			
			
			
			ItemBase target_item;
			Class.CastTo(target_item,  target.GetObject() );
			
			m_Player.GetCraftingManager().OnUpdate(item,target_item);
					
			//Looking for suitable user actions with given inputs
			if ( m_PrimaryActionEnabled ) 
			{
				SetContinuousAction(FindContextualContinuousAction(target,item),target,item);
			}
			if ( m_SecondaryActionEnabled ) 
			{
				SetSingleUseAction(FindContextualSingleUseAction(target,item),target,item);
			}
			if ( m_TertiaryActionEnabled ) 
			{
				FindSelectableActions(target);
			}
		}
	}
	
	
	protected ActionBase FindContextualContinuousAction(ActionTarget target, ItemBase item)
	{
		ref TIntArray primary_action_ids = new TIntArray;
		ActionBase picked_action;
		
		// Adding default continuous actions of player
		if ( m_Player ) 
		{
			m_Player.GetContinuousActions(primary_action_ids);
		}
		
		if ( item )
		{
			item.GetContinuousActions(primary_action_ids);
		}

		// Adding actions of item

		// Looking for first possible continuous action 
		if ( primary_action_ids && primary_action_ids.Count() > 0  )
		{
			for ( int i = 0; i < primary_action_ids.Count(); i++ )
			{
				picked_action = GetAction(primary_action_ids.Get(i));
				if ( picked_action && picked_action.Can(m_Player,target, item) )
				{
					return picked_action;
				}
			}
		}
		return NULL;
	}
		
	protected ActionBase FindContextualSingleUseAction(ActionTarget target, ItemBase item)
	{
		ref TIntArray secondary_action_ids = new TIntArray;
		ActionBase picked_action;
		// Adding default single use actions of player
		if ( m_Player ) 
		{
			m_Player.GetSingleUseActions(secondary_action_ids);
		}
		// Adding actions of item
		if ( item )
		{
			item.GetSingleUseActions(secondary_action_ids);
		}
		// Looking for first possible single use action 
		if ( secondary_action_ids  && secondary_action_ids.Count() > 0 )
		{
			for ( int i = 0; i < secondary_action_ids.Count(); i++ )
			{
				picked_action = GetAction(secondary_action_ids.Get(i));		
				if ( picked_action && picked_action.Can(m_Player, target, item) ) 
				{
					return picked_action;
				}
			}
		}
		return NULL;
	}
	
	protected void GetContextualInteractActions(out TSelectableActionInfoArray outarray, ActionTarget target)
	{
		Object targetObject;
		if( target )
		{
			targetObject = target.GetObject();
		}
		ref TIntArray tertiary_action_ids = new TIntArray;
		ActionBase picked_action; 
		// Adding default interact actions of player
		if ( m_Player ) 
		{
			m_Player.GetInteractActions(tertiary_action_ids);	
		}
		// Adding interact actions of items in the world
		if ( targetObject )
		{ 
			targetObject.GetInteractActions(tertiary_action_ids);
		}
		// Looking for first possible interact action 
		if ( tertiary_action_ids && tertiary_action_ids.Count() > 0 )
		{
			for ( int i = 0; i < tertiary_action_ids.Count(); i++ )
			{
				picked_action = GetAction(tertiary_action_ids.Get(i));
				if ( picked_action && picked_action.Can(m_Player, target, NULL) )
				{
					outarray.Insert(new TSelectableActionInfo(SAT_INTERACT_ACTION, tertiary_action_ids.Get(i), picked_action.GetText()));
				}
			}
		}
	}
	
	void FindSelectableActions(ActionTarget target)
	{
		ref TSelectableActionInfoArray newSelectableActions = new TSelectableActionInfoArray;

			// add contextual user actions
			GetContextualInteractActions(newSelectableActions, target);
			// add rest
			ItemBase targetItem;
			if( Class.CastTo(targetItem, target.GetObject()) )
			{
				//targetItem.GetRecipesActions(m_Player, newSelectableActions); //jtomasik - now are those actions peformed through user action actionworldcraft.c
				if ( ItemBase.GetDebugActionsMask() & DebugActionType.GENERIC_ACTIONS )
				{	
					targetItem.GetDebugActions(newSelectableActions);
				}		
				if( ItemBase.GetDebugActionsMask() & DebugActionType.PLAYER_AGENTS )
				{
					m_Player.GetDebugActions(newSelectableActions);
				}
			}
		// was there any change?
		m_SelectableActionsHasChanged = !newSelectableActions.IsSameAs(m_SelectableActions);
		if( m_SelectableActionsHasChanged )
		{
			m_SelectableActions = newSelectableActions;
			m_SelectedActionIndex = 0;
		}
	}
	
	int GetContinuousActionForTargetItem(ItemBase item)
	{
		ItemBase itemInHand = m_Player.GetItemInHands();
		ActionTarget target;
		target = new ActionTarget(item, null, -1, vector.Zero, -1);
		
		if( itemInHand )
		{
			ref TIntArray primary_action_ids = new TIntArray;
			ActionBase picked_action;
			itemInHand.GetContinuousActions(primary_action_ids);			
			
			if ( primary_action_ids && primary_action_ids.Count() > 0  )
			{
				for ( int i = 0; i < primary_action_ids.Count(); i++ )
				{
					picked_action = GetAction(primary_action_ids.Get(i));
					if ( picked_action && picked_action.HasTarget() && picked_action.Can(m_Player,target, itemInHand) )
					{
						return picked_action.GetType();
					}
				}
			}
		}		
		
		return -1;
	}
	//TOOD MW In progress
	protected bool LockInventory(ActionBase action, ActionTarget target, ItemBase item)
	{
		if ( action.IsInstant() )
		{
			Print("[AM][INVL](-) Inventory lock - Not Used");
				return true;
		}
		else
		{
			Print("[AM][INVL](X) Inventory lock");
			if (action)
				return action.InventoryReservation(m_Player, target, item, m_ReservedInventoryLocations);
		}
		return false;
	}
	void UnlockInventory(ActionBase action)
	{
		if ( action.IsInstant() )
		{
			Print("[AM][INVL](-) Inventory unlock - Not Used");
		}
		else
		{
			Print("[AM][INVL](O) Inventory unlock");
			if (action)
				action.ClearInventoryReservation(m_Player,m_ReservedInventoryLocations);
		}
	}
	
	protected void ActionStart(ActionBase action, ActionTarget target, ItemBase item )
	{
		if ( action ) 
		{	
			if ( !LockInventory(action, target, item) )
				return;
			
			m_RunningAction = action;
			
			if ( GetGame().IsMultiplayer() && !action.IsLocal() )
			{
				if (!ScriptInputUserData.CanStoreInputUserData())
				{
					UnlockInventory(action);
					m_RunningAction = NULL;
					Error("ScriptInputUserData already posted - ActionManagerClient");
					return;
				}
				ScriptInputUserData ctx = new ScriptInputUserData;
				ctx.Write(INPUT_UDT_STANDARD_ACTION);
				ctx.Write(action.GetType());
				action.WriteToContext(ctx, target);
					
				if (action.UseAcknowledgment())
				{
					m_PendingAction = action;
					m_PendingActionTarget = target;
					m_PendingActionState = UA_AM_PENDING;
					m_PendingActionAcknowledgmentID = m_LastAcknowledgmentID++;
						
					ctx.Write(m_PendingActionAcknowledgmentID);
				}
					
				ctx.Send();
					
				if (!action.UseAcknowledgment())
				{
					action.Start(m_Player, target, item);
					if( action.IsInstant() )
						OnActionEnd( action, target, item );
				}
			}
			else
			{
				action.Start(m_Player, target, item);
				if( action.IsInstant() )
					OnActionEnd( action, target, item );
			}
		}
	}
	
	
	//---------------------------------
	// EVENTS
	//---------------------------------
	override void OnContinuousStart()
	{
		if (!m_PrimaryActionEnabled || !m_ActionPossible )
			return;
								
		ActionStart(m_PrimaryAction, m_PrimaryActionTarget, m_PrimaryActionItem);
	}
	
	override void OnContinuousCancel()
	{
		if( m_PendingAction )
		{
			OnActionEnd( m_PendingAction, m_PendingActionTarget, m_Player.GetItemInHands() );
			m_PendingAction = NULL;
			m_PendingActionTarget = NULL;
			m_PendingActionAcknowledgmentID = -1;
			m_PendingActionState = UA_NONE;
		}
		else	
		{
			ActionContinuousBaseCB callback;
			if ( !Class.CastTo(callback, m_Player.GetCommandModifier_Action()))
			{
				if ( !Class.CastTo(callback, m_Player.GetCommand_Action()) ) 
				{
					return;
				}
			}
			if ( callback.GetState() == callback.STATE_LOOP_END || callback.GetState() == callback.STATE_LOOP_END2 )
			{
				return;
			}
			if ( !callback.IsUserActionCallback() ) 
			{
				return;
			}
			callback.UserEndsAction();
		}
		//TODO MW different solution for lock hands
		//m_Player.GetDayZPlayerInventory().UnlockHands();
	}
		
	// Single----------------------------------------------------
	override void OnSingleUse()
	{
		if (!m_SecondaryActionEnabled || !m_ActionPossible)
			return;
		
		ActionStart(m_SecondaryAction, m_SecondaryActionTarget, m_SecondaryActionItem);		
	}
	
	// Interact----------------------------------------------------
	override void OnInteractAction() //Interact
	{
		if(!m_TertiaryActionEnabled || !m_ActionPossible)
			return;
		
		if ( m_SelectableActions.Count() > 0 )
		{
			int action_type = m_SelectableActions.Get(m_SelectedActionIndex).param1;
			switch ( action_type )
			{
				case SAT_INTERACT_ACTION :
					ActionBase action = GetAction(m_SelectableActions.Get(m_SelectedActionIndex).param2);
					ActionTarget target = NULL;
					if (action.HasTarget())
						target = FindActionTarget();
				
					ActionStart(action, target, NULL);
					break;
					
				case SAT_DEBUG_ACTION :
					int action_id = m_SelectableActions.Get(m_SelectedActionIndex).param2;
					Object item = FindActionTarget().GetObject();
					if ( item.IsItemBase() )
					{
						ItemBase action_item;
						Class.CastTo(action_item, item);
						OnInstantAction(AT_DEBUG,new Param2<ItemBase,int>(action_item,action_id)); 
					}
					break;
				default :
					Print("WRONG SELECTABLE ACTION TYPE!");
					break;
			}
		}
	}
	
	
	//Instant Action (Debug Actions) ---------------------------------
	override void OnInstantAction(int user_action_id, Param data)
	{
		ActionInstantBase i_action;
		Class.CastTo(i_action, GetAction(user_action_id));
		if( GetGame().IsMultiplayer() && !i_action.IsLocal() )
		{
			if (!ScriptInputUserData.CanStoreInputUserData())
			{
				Error("ActionManagerClient - ScriptInputUserData already posted");
				return;
			}
			ScriptInputUserData ctx = new ScriptInputUserData;
			ctx.Write(INPUT_UDT_STANDARD_ACTION);
			ctx.Write(i_action.GetType());
			i_action.WriteToContext(ctx, NULL);
			ctx.Send();
		}
		i_action.PerformAction(m_Player, data);
	}
	
	override void OnActionEnd( ActionBase action, ActionTarget target, ItemBase item )
	{
		super.OnActionEnd( action, target, item );
		UnlockInventory(action);
		if( action.RemoveForceTargetAfterUse() )
			m_Player.GetInventoryActionHandler().DeactiveAction();
	}
	
	private ref ActionTarget m_ForceTarget;
	private ref ActionTargets m_Targets;
};