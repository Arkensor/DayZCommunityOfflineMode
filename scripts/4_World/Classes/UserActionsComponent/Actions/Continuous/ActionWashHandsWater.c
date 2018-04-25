class ActionWashHandsWaterCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousRepeat(UATimeSpent.WASH_HANDS);
	}
};

class ActionWashHandsWater: ActionContinuousBase
{
	void ActionWashHandsWater()
	{
		m_CallbackClass 	= ActionWashHandsWaterCB;
		m_CommandUID		= DayZPlayerConstants.CMD_ACTIONFB_WASHHANDSPOND;
		m_FullBody			= true;
		m_StanceMask		= DayZPlayerConstants.STANCEMASK_CROUCH;
		m_MessageSuccess = "I have washed my hands.";
		//m_Animation = "open";
	}
	
	override int GetType()
	{
		return AT_WASH_HANDS_WATER;
	}

	override string GetText()
	{
		return "Wash hands";
	}

	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			return true;
		}
		
		if ( player.HasBloodyHands() && !player.GetItemInHands() )
		{	
			vector pos_cursor = target.GetCursorHitPos();
			if( (vector.Distance(player.GetPosition(), pos_cursor) < UAMaxDistances.DEFAULT) && (GetGame().SurfaceIsPond(pos_cursor[0], pos_cursor[2]) || GetGame().SurfaceIsSea(pos_cursor[0], pos_cursor[2])))
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