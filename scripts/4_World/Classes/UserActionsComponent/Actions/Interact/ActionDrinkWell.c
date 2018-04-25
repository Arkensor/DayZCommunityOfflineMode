class ActionDrinkWell extends ActionInteractBase
{
	void ActionDrinkWell()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DRINKSIP;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override int GetType()
	{
		return AT_DRINK_WELL_ONE;
	}

	override string GetText()
	{
		return "take a sip";
	}

	override string GetTargetDescription()
	{
		return "Well";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
			return true;
		
		vector pos_cursor = target.GetCursorHitPos();
		if ( target.GetObject() && target.GetObject().IsWell() )
		{
			if ( vector.Distance(player.GetPosition(), pos_cursor) < UAMaxDistances.DEFAULT )
			{
				return true;
			}
		}
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		player.Consume(NULL, UAQuantityConsumed.DRINK, EConsumeType.ENVIRO_WELL);
		if(player.HasBloodyHands())
		{
			player.InsertAgent(AGT_CHOLERA, UAQuantityConsumed.DRINK);
		}
	}
}