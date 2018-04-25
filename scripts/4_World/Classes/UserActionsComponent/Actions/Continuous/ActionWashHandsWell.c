class ActionWashHandsWellCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousRepeat(UATimeSpent.WASH_HANDS);
	}
};

class ActionWashHandsWell: ActionContinuousBase
{
	void ActionWashHandsWell()
	{
		m_CallbackClass 	= ActionWashHandsWellCB;
		m_CommandUID		= DayZPlayerConstants.CMD_ACTIONFB_WASHHANDSWELL;
		m_FullBody			= true;
		m_StanceMask		= DayZPlayerConstants.STANCEMASK_CROUCH;
		m_MessageSuccess = "I have washed my hands.";
		//m_Animation = "open";
	}

	override int GetType()
	{
		return AT_WASH_HANDS_WELL;
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

	override void OnRepeatServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		PluginLifespan module_lifespan = PluginLifespan.Cast( GetPlugin( PluginLifespan ) );
		module_lifespan.UpdateBloodyHandsVisibility( player, false );
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		OnRepeatServer(player,target,item, acdata);
	}
};