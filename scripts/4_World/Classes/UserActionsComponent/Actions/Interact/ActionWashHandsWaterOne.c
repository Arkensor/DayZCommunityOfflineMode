class ActionWashHandsWaterOneCB : ActionInteractLoopBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAInteractLoop(UATimeSpent.WASH_HANDS);
	}
};

class ActionWashHandsWaterOne extends ActionInteractLoopBase
{
	void ActionWashHandsWaterOne()
	{
		m_CallbackClass		= ActionWashHandsWaterOneCB;
		m_CommandUID		= DayZPlayerConstants.CMD_ACTIONFB_WASHHANDSPOND;
		m_FullBody			= true;
		m_StanceMask		= DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override int GetType()
	{
		return AT_WASH_HANDS_WATER_ONE;
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

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		PluginLifespan module_lifespan = PluginLifespan.Cast( GetPlugin( PluginLifespan ) );
		module_lifespan.UpdateBloodyHandsVisibility( player, false );
	}
};