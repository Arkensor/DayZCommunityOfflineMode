class OpenItem
{
	//! WIll open the 'item_target' by spawning a new entity and transferring item variables to the new one
	static void OpenAndSwitch(ItemBase item_tool, ItemBase item_target, PlayerBase player, float specialty_weight = 0)
	{
		ref array<int> spill_range = new array<int>;
		
		if( item_tool.ConfigIsExisting("OpenItemSpillRange") )
		{
			item_tool.ConfigGetIntArray("OpenItemSpillRange", spill_range );
		}
		else
		{
			Debug.LogError("OpenItemSpillRange config parameter missing, default values used ! ");
			Error("OpenItemSpillRange config parameter missing, default values used !");
			spill_range.Insert(0);
			spill_range.Insert(100);
		}
		float spill_modificator = Math.RandomIntInclusive( spill_range.Get(0),spill_range.Get(1) ) / 100;
		
		OpenItem.SwitchItems(item_target, player, spill_modificator, specialty_weight);
	}

	//! Will switch the 'item' for a new game entity, the new entity's classname will be formed by adding the 'suffix' to the classname of the old 'item'
	static void SwitchItems (EntityAI old_item, PlayerBase player, float spill_modificator, float specialty_weight)
	{
		string old_name = old_item.GetType();
		string new_name = old_name + "_Opened";
		OpenAndSwitchLambda l = new OpenAndSwitchLambda(old_item, new_name, player, spill_modificator, specialty_weight);
		l.SetTransferParams(true, false);
		MiscGameplayFunctions.TurnItemIntoItemEx(player, l);
	}
};


class OpenAndSwitchLambda : TurnItemIntoItemLambda
{
	float m_SpillModifier;
	float m_SpecialtyWeight;
	void OpenAndSwitchLambda (EntityAI old_item, string new_item_type, PlayerBase player, float spill_modificator, float specialty_weight) { m_SpillModifier = spill_modificator; m_SpecialtyWeight = specialty_weight; }

	override void CopyOldPropertiesToNew (notnull EntityAI old_item, notnull EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);

		ItemBase ib = ItemBase.Cast(new_item);
		if ( new_item ) 
		{	
			float quantity_old = ib.GetQuantity();
			float spill_amount = quantity_old * m_SpillModifier;
			spill_amount = m_Player.m_SoftSkillsManager.SubtractSpecialtyBonus(spill_amount, m_SpecialtyWeight);
			float quantity_new = quantity_old - spill_amount;
	
			Debug.Log("quantity before spill: "+quantity_old.ToString());
			Debug.Log("spill_amount: "+spill_amount.ToString());
			Debug.Log("quantity_new: "+quantity_new.ToString());
			
			ib.SetQuantity(quantity_new);
		}
	}
};

