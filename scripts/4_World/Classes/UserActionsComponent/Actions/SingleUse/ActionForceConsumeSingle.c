class ActionForceConsumeSingleCB : ActionSingleUseBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUseQuantityEdible(UAQuantityConsumed.DEFAULT);
	}
};

class ActionForceConsumeSingle: ActionSingleUseBase
{
	void ActionForceConsumeSingle()
	{
		m_CallbackClass = ActionForceConsumeSingleCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INJECTIONTARGET;
		m_StanceMask = DayZPlayerConstants.STANCEIDX_ERECT | DayZPlayerConstants.STANCEIDX_CROUCH;
		m_FullBody = true;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_FORCE_CONSUME_SINGLE;
	}
		
	override string GetText()
	{
		return "give one";
	}
		
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		if ( item && item.GetQuantity() <= 0 ) 
		{
			item.SetQuantity(0);
		}
	}
};