class ActionTurnOnWhileOnGround: ActionInteractBase
{
	void ActionTurnOnWhileOnGround()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageSuccess 	= "I've flipped the switch on.";
		m_MessageFail 		= "I've flipped the switch off.";
	}
	
	override int GetType()
	{
		return AT_TURN_ON_WHILE_ON_GROUND;
	}

	override string GetText()
	{
		return "switch on";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI target_entity = EntityAI.Cast( target.GetObject() );
		
		if ( player.IsAlive()  &&  target_entity.HasEnergyManager()  &&  target_entity.GetCompEM().CanSwitchOn() )
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
		EntityAI target_entity = EntityAI.Cast( target.GetObject() );
		
		if ( target_entity.GetCompEM().CanWork() )
		{
			target_entity.GetCompEM().SwitchOn();
			
			InformPlayers( player, target, UA_FINISHED );	//Success
		}
		else
		{
			InformPlayers( player, target, UA_FAILED );		//No power
		}
	}
};