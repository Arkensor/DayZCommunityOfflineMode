class ActionTurnOnWhileInHands: ActionSingleUseBase
{
	void ActionTurnOnWhileInHands()
	{
		m_MessageSuccess = "It's turned on.";
		m_MessageFail = "It's out of energy";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_TURN_ON_WHILE_IN_HANDS;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "switch on";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( item.IsInherited(Roadflare) )
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LIGHTFLARE;
		}
		else if (item.IsInherited(Chemlight_ColorBase))
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LITCHEMLIGHT;
		}
		else
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP;
		}
		
		
		if ( item.HasEnergyManager()  &&  item.GetCompEM().CanSwitchOn()  &&  item.GetCompEM().CanWork() )
		{
			return true;
		}
		
		return false;
	}

	override void OnExecuteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )  //Takhle to nedelat, vyrob si action componentu a tehle check patri do jeji Execute metody. Message o selhani pak napis jako messageStartFail
	{
		if ( item.GetCompEM().CanWork() )
		{
			item.GetCompEM().SwitchOn();
		} 
		else
		{
			InformPlayers(player,target,UA_FAILED);
		}
	}
};
