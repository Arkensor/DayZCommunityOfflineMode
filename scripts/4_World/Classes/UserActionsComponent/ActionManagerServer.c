class ActionReceived
{
	void ActionReceived()
	{
		ActionID = -1;
		Target = null;
		Parent = null;
		ComponentIndex = -1;
		AcknowledgmentID = -1;
	}
	
	int ActionID;
	//Standart Action
	Object Target;
	Object Parent;
	int ComponentIndex;
	int AcknowledgmentID;
	
	//Craft
	ItemBase 	item1;
	ItemBase 	item2;
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
						//return true;
						break;
					}
					case AT_WORLD_CRAFT:
					{
						ItemBase item1 = null;
						ItemBase item2 = null;
						int recipeID = -1;
						if (!ctx.Read(item1))
							return false;
						if (!ctx.Read(item2))
							return false;
						if (!ctx.Read(recipeID))
							return false;
					
						m_ActionReceived.item1 = item1;
						m_ActionReceived.item2 = item2;
						m_ActionReceived.recipeID = recipeID;
						//return true;
						break;
					}
					case AT_PLACE_OBJECT:
					{
						entity_position = "0 0 0";
						entity_orientation = "0 0 0";
						if (!ctx.Read(entity_position))
							return false;
						if (!ctx.Read(entity_orientation))
							return false;
					
						m_ActionReceived.entity_position = entity_position;
						m_ActionReceived.entity_orientation = entity_orientation;
						//return true;
						break;
					}	
					case AT_DIG_GARDEN_PLOT:
					{
						entity_position = "0 0 0";
						entity_orientation = "0 0 0";
						if (!ctx.Read(entity_position))
							return false;
						if (!ctx.Read(entity_orientation))
							return false;
					
						m_ActionReceived.entity_position = entity_position;
						m_ActionReceived.entity_orientation = entity_orientation;
						//return true;
						break;
					}
					default:
					{
						Object actionTargetObject = null;
						Object actionTargetParent = null;
						int componentIndex = -1;
						
						if( recvAction.HasTarget() )
						{
							if ( !ctx.Read(actionTargetObject) )
								return false;
							
							if ( !ctx.Read(actionTargetParent))
								return false;
							
							if ( !ctx.Read(componentIndex) )
								return false;
						}
						m_ActionReceived.Target = actionTargetObject;
						m_ActionReceived.Parent = actionTargetParent;
						m_ActionReceived.ComponentIndex = componentIndex;
						break;
						//return true;
					}
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
		
		if (m_ActionReceived.ActionID == AT_WORLD_CRAFT)
		{
			is_target_free = m_Player.m_ModuleObjectsInteractionManager.IsFree(m_ActionReceived.item1);
			is_target_free = is_target_free && m_Player.m_ModuleObjectsInteractionManager.IsFree(m_ActionReceived.item2);
			
			m_Player.SetCraftingRecipeID(m_ActionReceived.recipeID);
			picked_action = GetAction(AT_WORLD_CRAFT);
			target = new ActionTarget(m_ActionReceived.item2, m_ActionReceived.Parent, -1, vector.Zero, 0);
			item = m_ActionReceived.item1;
		}
		else if (m_ActionReceived.ActionID == AT_PLACE_OBJECT)
		{		
			picked_action = GetAction(AT_PLACE_OBJECT);
			target = new ActionTarget(m_ActionReceived.Target, m_ActionReceived.Parent, m_ActionReceived.ComponentIndex, vector.Zero, 0);
			item = m_Player.GetItemInHands();	
			
			m_Player.SetLocalProjectionPosition( m_ActionReceived.entity_position );
			m_Player.SetLocalProjectionOrientation( m_ActionReceived.entity_orientation );
		}
		else if (m_ActionReceived.ActionID == AT_DIG_GARDEN_PLOT)
		{		
			picked_action = GetAction(AT_PLACE_OBJECT);
			target = new ActionTarget(m_ActionReceived.Target, m_ActionReceived.Parent, m_ActionReceived.ComponentIndex, vector.Zero, 0);
			item = m_Player.GetItemInHands();	
			
			m_Player.SetLocalProjectionPosition( m_ActionReceived.entity_position );
			m_Player.SetLocalProjectionOrientation( m_ActionReceived.entity_orientation );
		}
		else
		{
			if ( m_ActionReceived.Target && m_Player.m_ModuleObjectsInteractionManager ) 
			{
				is_target_free = m_Player.m_ModuleObjectsInteractionManager.IsFree(m_ActionReceived.Target);
			}
			
			picked_action = GetAction(m_ActionReceived.ActionID);
			target = new ActionTarget(m_ActionReceived.Target, m_ActionReceived.Parent, m_ActionReceived.ComponentIndex, vector.Zero, 0);
			item = m_Player.GetItemInHands();			
		}
		
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