class PaintItem
{
	//! WIll open the 'item_target' by spawning a new entity and transferring item variables to the new one
	static void Paint(ItemBase item_tool, ItemBase item_target, string base_name, PlayerBase player, float specialty_weight = 0)
	{
		string spray_color = item_tool.ConfigGetString("color");
		string item_color = item_target.ConfigGetString("color");

		string new_class_name = base_name + "_" + spray_color;
		PaintItem.SwitchItems(item_target, new_class_name, player);
	}

	static bool CanPaintItem(ItemBase item_tool, ItemBase item_target)
	{
		string spray_color = item_tool.ConfigGetString("color");
		string item_color = item_target.ConfigGetString("color");
		
		if( spray_color != item_color )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	//! Will switch the 'item' for a new game entity, the new entity's classname will be formed by adding the 'suffix' to the classname of the old 'item'
	static void SwitchItems (EntityAI old_item, string new_item, PlayerBase player)
	{
		MiscGameplayFunctions.TurnItemIntoItemEx(player, new PaintItemLambda(old_item, new_item, player));
	}
};


class PaintItemLambda : TurnItemIntoItemLambda
{
	void PaintItemLambda (EntityAI old_item, string new_item_type, PlayerBase player) {}

	override void CopyOldPropertiesToNew (notnull EntityAI old_item, notnull EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);
		Print("PaintItemLambda::OnCreated object=" + new_item);
	}
};
