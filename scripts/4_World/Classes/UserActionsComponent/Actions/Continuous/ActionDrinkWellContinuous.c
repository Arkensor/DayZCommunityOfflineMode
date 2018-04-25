class ActionDrinkWellContinuousCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousRepeat(UATimeSpent.DRINK_WELL);
	}
};

class ActionDrinkWellContinuous: ActionContinuousBase
{
	private const float WATER_DRANK_PER_SEC = 35;
	
	void ActionDrinkWellContinuous()
	{
		m_CallbackClass = ActionDrinkWellContinuousCB;
		m_MessageStartFail = "...";
		m_MessageStart = "I started drinking from pond.";
		m_MessageSuccess = "I finished drinking.";
		m_MessageFail = "I moved and stoped drinking.";
		m_MessageCancel = "I moved and stoped drinking.";	
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DRINKWELL;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override int GetType()
	{
		return AT_DRINK_WELL;
	}

	override string GetText()
	{
		return "drink";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( !player.HasBloodyHands() )
		{
			if( player.GetItemInHands())
			{
				return false;			
			}

			if( target.GetObject() && target.GetObject().IsWell() )
			{
				return true;
			}	
		}
			
		return false;
	}
	
	override void OnRepeatServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Param1<float> nacdata = Param1<float>.Cast( acdata );
		float amount = nacdata.param1 * WATER_DRANK_PER_SEC;
		player.Consume(NULL,amount, EConsumeType.ENVIRO_WELL);
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		OnRepeatServer(player,target,item, acdata);
	}
};