class ActionDrinkPond: ActionInteractBase
{
	void ActionDrinkPond()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DRINKSIP;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override int GetType()
	{
		return AT_DRINK_POND_ONE;
	}

	override string GetText()
	{
		return "take a sip";
	}

	override string GetTargetDescription()
	{
		return "Pond";
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
		if( g_Game.SurfaceIsPond(pos_cursor[0], pos_cursor[2]) )
		{
			pos_cursor[1] = g_Game.SurfaceY(pos_cursor[0],pos_cursor[2]);
			// TODO: use some automatic way for setting of surface description (when it's possible)
			//GetGame().SurfaceGetType(pos_cursor[0], pos_cursor[2], m_TargetDescription);
			if ( vector.Distance(player.GetPosition(), pos_cursor) < UAMaxDistances.DEFAULT )
			{
				return true;
			}
		}
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		player.Consume(NULL, UAQuantityConsumed.DRINK, EConsumeType.ENVIRO_POND);
	}
};