class ActionDrinkPondContinuousCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousRepeat(UATimeSpent.DRINK_POND);
	}
};

class ActionDrinkPondContinuous: ActionContinuousBase
{
	private const float WATER_DRANK_PER_SEC = 35;
	
	void ActionDrinkPondContinuous()
	{
		m_CallbackClass = ActionDrinkPondContinuousCB;
		m_MessageStartFail = "...";
		m_MessageStart = "I started drinking from pond.";
		m_MessageSuccess = "I finished drinking.";
		m_MessageFail = "I moved and stoped drinking.";
		m_MessageCancel = "I moved and stoped drinking.";	
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DRINKPOND;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override int GetType()
	{
		return AT_DRINK_POND;
	}

	override string GetText()
	{
		return "drink";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if( GetGame().IsMultiplayer() && GetGame().IsServer() )
			return true;

		if ( !player.HasBloodyHands() )
		{
			vector pos_cursor = target.GetCursorHitPos();
			if( !player.GetItemInHands() && (vector.Distance(player.GetPosition(), pos_cursor) < UAMaxDistances.DEFAULT) && g_Game.SurfaceIsPond(pos_cursor[0], pos_cursor[2]))
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
		player.Consume(NULL,amount, EConsumeType.ENVIRO_POND);
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		OnRepeatServer(player,target,item, acdata);
	}
};