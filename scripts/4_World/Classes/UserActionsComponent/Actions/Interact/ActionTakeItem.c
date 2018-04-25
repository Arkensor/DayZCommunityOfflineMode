class ActionTakeItem: ActionInteractBase
{
	string m_ItemName = "";

	void ActionTakeItem()
	{
		m_MessageSuccess    = "";
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP;
		m_CommandUIDProne	= DayZPlayerConstants.CMD_ACTIONFB_PICKUP;
		m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}	
	
	override int GetType()
	{
		return AT_TAKE_ITEM;
	}

	override string GetText()
	{
		return "Take ";
	}
	
	override bool HasProneException()
	{
		return true;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI entity;
	
		if ( Class.CastTo(entity, target.GetObject()) && !target.GetParent() )
		{
			if( entity && entity.IsItemBase() && player.GetInventory().CanAddEntityIntoInventory(entity) && entity.GetHierarchyRootPlayer() != player )
			{
				return true;
			}
		}
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		ItemBase ntarget = ItemBase.Cast(target.GetObject());
		
		player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, ntarget);
	}
	
	override void OnCompleteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		ItemBase ntarget = ItemBase.Cast(target.GetObject());
		player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, ntarget);
	}
};