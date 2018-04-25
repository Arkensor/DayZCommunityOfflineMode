/*
	User action for emptying packs of seeds (horticulture)
*/


class ActionEmptySeedsPack: ActionSingleUseBase
{
	void ActionEmptySeedsPack()
	{
		m_MessageSuccess = "I've emptied the pack.";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_EMPTY_SEEDS_PACK;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "Empty the pack";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		return true;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		SeedPackBase item_SPB = SeedPackBase.Cast( item );
		item_SPB.EmptySeedPack( player );
	}
};