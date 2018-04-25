class ActionConsumeCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.DEFAULT,UATimeSpent.DEFAULT);
	}
};

class ActionConsume: ActionContinuousBase
{
	void ActionConsume()
	{
		m_CallbackClass = ActionConsumeCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EAT;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EAT;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "I have started consuming.";
		m_MessageSuccess = "I have finished consuming.";
		m_MessageFail = "Player moved and consuming was canceled.";
		m_MessageCancel = "I stopped consuming.";
		//m_Animation = "EATTABLETS";		
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINotRuinedAndEmpty;
		m_ConditionTarget = new CCTSelf;
	}
	
	override bool HasProneException()
	{
		return true;
	}

	override int GetType()
	{
		return AT_CONSUME;
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool Consumable()
	{
		return true;
	}

	override string GetText()
	{
		return "consume";
	}

	override void OnCancelServer(PlayerBase player, ActionTarget target, ItemBase item, Param acdata)
	{
		if ( item && item.GetQuantity() <= 0.01 )
		{
			item.SetQuantity(0);
		}
	}
	
	override void OnCompleteLoopServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		if ( item && item.GetQuantity() <= 0.01 )
		{
			item.SetQuantity(0);
		}
	}
};