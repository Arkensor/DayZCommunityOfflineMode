class ActionReceived
{
	void ActionReceived()
	{
		ActionID = -1;
		Target = null;
		Parent = null;
		baseItem = null;
		ComponentIndex = -1;
		AcknowledgmentID = -1;
		
	}
	
	int ActionID;
	//Standart Action
	ItemBase baseItem; //normaly set item in hands (exeption for some potencial crafting recipes and etc. )
	Object Target;
	Object Parent;
	int ComponentIndex;
	int AcknowledgmentID;
	
	
	
	//Craft
	int 		recipeID;
	
	//Advanced placement
	vector entity_position;
	vector entity_orientation;
};

class ActionManagerServer: ActionManagerBase
{	
	private ref ActionReceived m_ActionReceived;
	
	void ActionManagerServer(PlayerBase player)
	{
		//ActionManagerBase(player);
	}
	
	//------------------------------------------
	//EVENTS 
	//------------------------------------------
	// Continuous---------------------------------------------
	override void OnContinuousStart()
	{
			StartDeliveredAction();
	}
	
	override void OnContinuousCancel()
	{	
		if( m_PendingAction )
		{
			EntityAI targetObject;
			if( m_PendingAction )
			{
				OnActionEnd( m_PendingAction, m_PendingActionTarget, m_Player.GetItemInHands() );
			}
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
				if ( callback.GetState() == callback.STATE_LOOP_END || callback.GetState() == callback.STATE_LOOP_END2 ) 
				{
					return;
				}
			}
			if ( !callback.IsUserActionCallback() ) 
			{
				return;
			}
			callback.UserEndsAction();
		}
	}
	
	// Single----------------------------------------------------
	override void OnSingleUse()
	{
			StartDeliveredAction();
	}
	
	// Interact----------------------------------------------------
	override void OnInteractAction() //Interact
	{
		if ( m_SelectedActionIndex == 0 ) // tertiary action is always on index 0 now
		{
				StartDeliveredAction();
		}
		else
		{
		//	rozbal debug akci
		}

	}

	override bool OnInputUserDataProcess(int userDataType, ParamsReadContext ctx)
	{		
		switch ( userDataType )
		{
			case INPUT_UDT_STANDARD_ACTION:
			{
				//Advanced placement
				vector entity_position;
				vector entity_orientation;
				
				int actionID = 0;
				if (!ctx.Read(actionID) )
					return false;

				m_ActionReceived = new ActionReceived();
				m_ActionReceived.ActionID = actionID;
				
				ActionBase recvAction = GetAction(actionID);
				
				m_ActionReceived.baseItem = m_Player.GetItemInHands();

				switch (actionID)
				{
					case AT_DEBUG:
					{
						ItemBase targetItem = null;
						if ( !ctx.Read(targetItem) ) //jtomasik - proc ho nenacte?
							return false;

						int debugActionID = 0;
						if ( !ctx.Read(debugActionID) ) //jtomasik - proc ho nenacte?
							return false;
						
						targetItem.OnAction(debugActionID, m_Player, NULL);
						break;
					}
					
					default:
						if (!recvAction.ReadFromContext(ctx, m_ActionReceived))
							return false;

				}
					
				if (recvAction.UseAcknowledgment())
				{
					int AckID;
					if (!ctx.Read(AckID))
						return false;
						
					m_ActionReceived.AcknowledgmentID = AckID;
				}
					
					
				return true;
			}
			default :
				return false;
		}
		
		return true;
	}
	
	override void StartDeliveredAction()
	{
		if( !m_ActionReceived )
		{
			//! error - expected action data
			//Interrupt();
			return;
		}
		
		ActionBase picked_action;
		bool accepted = false;
		bool is_target_free = true;
		ref ActionTarget target;
		ItemBase item;
		
		if (m_ActionReceived.ActionID == AT_WORLD_CRAFT) //TODO - change to set in action onStart() 
		{
			m_Player.SetCraftingRecipeID(m_ActionReceived.recipeID);

		}
		else if (m_ActionReceived.ActionID == AT_PLACE_OBJECT)
		{		
			m_Player.SetLocalProjectionPosition( m_ActionReceived.entity_position );
			m_Player.SetLocalProjectionOrientation( m_ActionReceived.entity_orientation );
		}
		else if (m_ActionReceived.ActionID == AT_DIG_GARDEN_PLOT)
		{		
			m_Player.SetLocalProjectionPosition( m_ActionReceived.entity_position );
			m_Player.SetLocalProjectionOrientation( m_ActionReceived.entity_orientation );
		}
		
		picked_action = GetAction(m_ActionReceived.ActionID);
		target = new ActionTarget(m_ActionReceived.Target, m_ActionReceived.Parent, m_ActionReceived.ComponentIndex, vector.Zero, 0);
		item = m_ActionReceived.baseItem;

		if( is_target_free && !m_Player.GetCommandModifier_Action() && !m_Player.GetCommand_Action() && !m_Player.IsSprinting() && picked_action && picked_action.Can(m_Player,target,item)) 
		{
			accepted = true;
			if( picked_action.HasTarget())
			{
				
				EntityAI targetEntity;
				if ( EntityAI.CastTo(targetEntity,target.GetObject()) )
				{
					if( !Building.Cast(targetEntity))
					{
						//Lock target
						if( !GetGame().AddActionJuncture(m_Player,targetEntity,10000) )
						{
							accepted = false;
						}
					}
				}
			}
		}
		
		if( accepted )
		{
			if(picked_action.UseAcknowledgment())
			{
				if(m_PendingActionTarget && m_PendingActionTarget.GetObject() != target.GetObject())
				{
					InventoryItem oldTrgetItem;
					if( InventoryItem.CastTo(oldTrgetItem,m_PendingActionTarget.GetObject()))
					{
						//Unlock target
						GetGame().ClearJuncture(m_Player, oldTrgetItem);
						m_PendingActionTarget = NULL;
					}
				}
				
				m_RunningAction = picked_action;

				m_PendingAction = picked_action;
				m_PendingActionTarget = target;
				m_PendingActionAcknowledgmentID = m_ActionReceived.AcknowledgmentID;
				m_PendingActionState = UA_AM_PENDING;
				DayZPlayerSyncJunctures.SendActionAcknowledgment(m_Player, m_ActionReceived.AcknowledgmentID, true);
			}
			else
			{
				m_RunningAction = picked_action;
				picked_action.Start(m_Player,target,item);
				if( picked_action.IsInstant() )
				{
					OnActionEnd( picked_action, target, item );
					m_RunningAction = NULL;
				}
			}
		}
		else
		{
			if (picked_action.UseAcknowledgment())
			{
				DayZPlayerSyncJunctures.SendActionAcknowledgment(m_Player, m_ActionReceived.AcknowledgmentID, false);
			}
			else
			{
				Interrupt();
			}
		}
			
		//! discard received action
		m_ActionReceived = NULL;
	}
	
	override void OnActionEnd( ActionBase action, ActionTarget target, ItemBase item )
	{
		if( target )
		{
			ItemBase targetItem;
			if( targetItem.CastTo(targetItem,target.GetObject()))
			{
				GetGame().ClearJuncture(m_Player, targetItem);
			}
		}
	}
	
	
	override void OnSyncJuncture(int pJunctureID, ParamsReadContext pCtx)
	{
		int AcknowledgmentID;
		pCtx.Read(AcknowledgmentID);
		if( AcknowledgmentID == m_PendingActionAcknowledgmentID)
		{
			
			if(pJunctureID == DayZPlayerSyncJunctures.SJ_ACTION_ACK_ACCEPT)
			{
				ref HumanMovementState		state 	= new HumanMovementState();
		
		
				m_Player.GetMovementState(state);
				int movementStateID = state.m_CommandTypeId;

				if( ActionPossibilityCheck(movementStateID) && movementStateID != DayZPlayerConstants.COMMANDID_SWIM && movementStateID != DayZPlayerConstants.COMMANDID_LADDER )
				{
					m_PendingAction.Start(m_Player,m_PendingActionTarget,m_Player.GetItemInHands());
					if( m_PendingAction.IsInstant() )
						OnActionEnd( m_PendingAction, m_PendingActionTarget, m_Player.GetItemInHands());
				}
				
				m_PendingAction = NULL;
				m_PendingActionTarget = NULL;
				m_PendingActionAcknowledgmentID = -1;
				m_PendingActionState = UA_NONE;
			}
				
			if(pJunctureID == DayZPlayerSyncJunctures.SJ_ACTION_ACK_REJECT)
			{
				m_PendingAction = NULL;
				m_PendingActionTarget = NULL;
				m_PendingActionAcknowledgmentID = -1;
				m_PendingActionState = UA_NONE;
			}
		}
	
	}
	
	private void Interrupt()
	{
		DayZPlayerSyncJunctures.SendActionInterrupt(m_Player);
	}
};