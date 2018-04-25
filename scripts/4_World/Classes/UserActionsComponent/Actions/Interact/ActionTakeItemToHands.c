class ActionTakeItemToHands: ActionInteractBase
{
	string m_ItemName = "";

	void ActionTakeItemToHands()
	{
		m_MessageSuccess    = "";
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_TAKEITEM;
		m_CommandUIDProne	= DayZPlayerConstants.CMD_ACTIONFB_TAKEITEM;
		m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}	
	
	override bool HasProneException()
	{
		return true;
	}
	
	override int GetType()
	{
		return AT_TAKE_ITEM_TO_HANDS;
	}

	override string GetText()
	{
		return "Take To Hands";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI entity;

		if ( Class.CastTo(entity, target.GetObject()) && !target.GetParent() )
		{
			if ( entity && entity.IsItemBase() && !player.GetInventory().CanAddEntityIntoInventory(entity) && player.GetInventory().CanAddEntityIntoHands(entity) && entity.GetHierarchyRootPlayer() != player )
			{
				return true;
			}
		}
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		EntityAI ntarget = EntityAI.Cast(target.GetObject());
		player.PredictiveTakeEntityToHands(ntarget);
	}
	
	override void OnCompleteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		EntityAI ntarget = EntityAI.Cast(target.GetObject());
		player.PredictiveTakeEntityToHands(ntarget);
	}
};