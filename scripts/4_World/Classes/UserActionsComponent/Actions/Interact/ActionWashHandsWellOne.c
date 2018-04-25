class ActionWashHandsWellOneCB : ActionInteractLoopBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAInteractLoop(UATimeSpent.WASH_HANDS);
	}
};

class ActionWashHandsWellOne extends ActionInteractLoopBase
{
	void ActionWashHandsWellOne()
	{
		m_CallbackClass		= ActionWashHandsWellOneCB;
		m_CommandUID		= DayZPlayerConstants.CMD_ACTIONFB_WASHHANDSWELL;
		m_FullBody			= true;
		m_StanceMask		= DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override int GetType()
	{
		return AT_WASH_HANDS_WELL_ONE;
	}

	override string GetText()
	{
		return "Wash hands";
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( player.HasBloodyHands() && !player.GetItemInHands() )
		{
			if ( target.GetObject().GetType() == "Land_Misc_Well_Pump_Yellow" || target.GetObject().GetType() == "Land_Misc_Well_Pump_Blue" )
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		PluginLifespan module_lifespan = PluginLifespan.Cast( GetPlugin( PluginLifespan ) );
		module_lifespan.UpdateBloodyHandsVisibility( player, false );
	}	
};