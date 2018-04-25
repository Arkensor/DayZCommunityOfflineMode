class ActionPlaceOnGroundCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_PLACE);
	}
};

class ActionPlaceOnGround: ActionContinuousBase
{
	void ActionPlaceOnGround()
	{
		m_CallbackClass = ActionPlaceOnGroundCB;
		m_MessageStartFail = "There's nothing to place.";
		m_MessageStart = "I have started placing it on ground.";
		m_MessageSuccess = "I have placed it on ground.";
		m_MessageFail = "I have moved and placing was canceled.";
		m_MessageCancel = "I stopped placing it on the ground.";
		//m_Animation = "drop";
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionTarget = new CCTNone;
		m_ConditionItem = new CCIDummy;
	}

	override int GetType()
	{
		return AT_PLACE;
	}
		
	override string GetText()
	{
		return "place on ground";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( player && player.IsAlive() && !item.IsDamageDestroyed() ) //&& IsConscious && IsNotCaptured
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		player.DropItem(item);
	}
};