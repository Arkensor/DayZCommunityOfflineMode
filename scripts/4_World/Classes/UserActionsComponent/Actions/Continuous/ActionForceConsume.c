class ActionForceConsumeCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.DEFAULT,UATimeSpent.DEFAULT);
	}
};

class ActionForceConsume: ActionContinuousBase
{
	void ActionForceConsume()
	{
		m_CallbackClass = ActionForceConsumeCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_FORCEFEED;
		m_StanceMask = DayZPlayerConstants.STANCEIDX_ERECT | DayZPlayerConstants.STANCEIDX_CROUCH;
		m_FullBody = true;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_FORCE_CONSUME;
	}

	override string GetText()
	{
		return "force feed";
	}

	override void OnCancelServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		if ( item && item.GetQuantity() <= 0.01 ) 
		{
			item.SetQuantity(0);
		}
	}
	
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		if ( item && item.GetQuantity() <= 0.01 ) 
		{
			item.SetQuantity(0);
		}
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) //condition for action
	{
		if( item.GetQuantity() > item.GetQuantityMin() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};