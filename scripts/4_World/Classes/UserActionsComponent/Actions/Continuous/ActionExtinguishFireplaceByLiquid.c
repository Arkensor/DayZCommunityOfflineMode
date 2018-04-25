class ActionExtinguishFireplaceByLiquidCB : ActionContinuousBaseCB
{
	private const float TIME_TO_REPEAT 				= 0.5;
	private const float WETNESS_GAIN_MULTIPLIER 	= 1.0;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityExtinguish( UAQuantityConsumed.FIREPLACE_LIQUID, TIME_TO_REPEAT, WETNESS_GAIN_MULTIPLIER );
	}
};

class ActionExtinguishFireplaceByLiquid: ActionContinuousBase
{
	void ActionExtinguishFireplaceByLiquid()
	{
		m_CallbackClass = ActionExtinguishFireplaceByLiquidCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_POURBOTTLE;
		m_FullBody = true;
		m_MessageStartFail = "There's nothing to extinguish.";
		m_MessageStart = "I have started extinguishing the fireplace.";
		m_MessageSuccess = "I have extinguished the fireplace.";
		m_MessageFail = "I have canceled the extinguish action.";
		m_MessageCancel = "I have stopped extinguishing the fireplace.";
	}
	
	override void CreateConditionComponents()  
	{
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTDummy;
	}

	override int GetType()
	{
		return AT_EXTINGUISH_FIREPLACE_BY_LIQUID;
	}
		
	override string GetText()
	{
		return "extinguish";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		if ( target_object.IsFireplace() )
		{
			FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );
			
			if ( fireplace_target.IsBurning() && !item.IsDamageDestroyed() && (item.GetLiquidType() & (GROUP_LIQUID_BLOOD | LIQUID_WATER | LIQUID_RIVERWATER | LIQUID_BEER)) ) 
			{
				return true;
			}		
		}
		
		return false;
	}
}