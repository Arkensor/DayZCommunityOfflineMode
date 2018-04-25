class ActionWashHandsItem: ActionSingleUseBase
{
	void ActionWashHandsItem()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_WALKIETALKIEON;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageSuccess = "I have washed my hands.";
		//m_Animation = "open";
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINotRuinedAndEmpty;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_WASH_HANDS_ITEM;
	}

	override string GetText()
	{
		return "Wash hands";
	}
	
	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( player.HasBloodyHands() )
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
		PluginLifespan module_lifespan = PluginLifespan.Cast( GetPlugin( PluginLifespan ) );
		module_lifespan.UpdateBloodyHandsVisibility( player, false );
	}
};