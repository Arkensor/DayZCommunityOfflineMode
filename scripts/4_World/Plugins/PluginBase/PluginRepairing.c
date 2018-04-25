class PluginRepairing extends PluginBase
{	
	static protected const float KIT_TO_ITEM_HEALTH = 25;
	static protected const int PRISTINE = 0;
	static protected const int WORN = 1;
	static protected const int DAMAGED = 2;
	static protected const int BADLY_DAMAGED = 3;
	static protected const int RUINED = 4;

	bool Repair(PlayerBase player, ItemBase item_a, ItemBase item_b, float specialty_weight)
	{
		if ( !CanRepair(item_a, item_b) )
		{
			return false;
		}

		if ( item_a.IsRuined() || item_b.IsRuined() )
		{
			Print("Some item is ruined !");
			return false;
		}
		
		ItemBase item = item_a;
		ItemBase item_kit = item_b;
		
		if ( IsRepairValid(item_a.ConfigGetInt("repairKitType"), item_b.ConfigGetInt("repairableWith")) )
		{
			item = item_b;
			item_kit = item_a;
		}
	
		// 0 = pristine, 1 = worn, 2 = damaged, 3 = badly damaged, 4 = ruined)
		switch (item.GetHealthLabel()) 
		{
			case PRISTINE:
				break;
				
			case WORN:
				if( CanRepairToPristine( player ) )
				{
					CalculateHealth( player, item_kit, item, specialty_weight );
				}
				break;
				
			case DAMAGED:
				CalculateHealth( player, item_kit, item, specialty_weight );
				break;
				
			case BADLY_DAMAGED:
				CalculateHealth( player, item_kit, item, specialty_weight );
				break;
			
			case RUINED:
				break;
		}
		
		return true;
	}

	void CalculateHealth( PlayerBase player, ItemBase item_kit, ItemBase item, float specialty_weight )
	{
		float cur_kit_health = item_kit.GetHealth("","");
		float cur_item_health = item.GetHealth("","");

		int w, h;
		GetGame().GetInventoryItemSize(item, w, h);
		
		float item_size			= w * h;
		float item_size_sqrt 	= Math.Sqrt( item_size );
		
		//calculate new kit health
		float new_kit_health;

		new_kit_health = cur_kit_health - KIT_TO_ITEM_HEALTH;
		new_kit_health = player.GetSoftSkillManager().AddSpecialtyBonus( new_kit_health, specialty_weight );
		new_kit_health = Math.Clamp( new_kit_health, 0, 100 );

		//calculate new item health
		float new_item_health;
		float kit_to_item_health_adjusted;

		if( cur_kit_health >= KIT_TO_ITEM_HEALTH )
		{
			kit_to_item_health_adjusted = ( KIT_TO_ITEM_HEALTH / 2 ) + ( ( KIT_TO_ITEM_HEALTH / 2) / item_size_sqrt );
			new_item_health = cur_item_health + kit_to_item_health_adjusted;
			new_item_health = player.GetSoftSkillManager().AddSpecialtyBonus( new_item_health, specialty_weight );
			new_item_health = Math.Clamp( new_item_health, 0, 100 );
		}
		else
		{
			kit_to_item_health_adjusted = ( cur_kit_health / 2 ) + ( ( cur_kit_health / 2) / item_size_sqrt );
			new_item_health = cur_item_health + kit_to_item_health_adjusted;
			new_item_health = player.GetSoftSkillManager().AddSpecialtyBonus( new_item_health, specialty_weight );
			new_item_health = Math.Clamp( new_item_health, 0, 100 );
		}
		
		//set health values to item and kit
		item_kit.SetHealth( "", "", new_kit_health );
		item.SetHealth( "", "", new_item_health );
		
		return;
	}

	bool CanRepair(ItemBase item_a, ItemBase item_b)
	{
		if ( !item_a || !item_b )
		{
			return false;
		}
				
		// ItemA can repair ItemB
		if ( IsRepairValid(item_a.ConfigGetInt("repairKitType"), item_b.ConfigGetInt("repairableWith")) )
		{
			return true;
		}
		
		// ItemB can repair ItemA
		if ( IsRepairValid(item_b.ConfigGetInt("repairKitType"), item_a.ConfigGetInt("repairableWith")) )
		{
			return true;
		}

		// Check if either of the items is a ductTape, but not both
		if ( item_a.IsKindOf( "DuctTape" ) || item_b.IsKindOf( "DuctTape" ) )
		{	
			if ( item_a.IsKindOf( "DuctTape" ) && item_b.IsKindOf( "DuctTape" ) )
			{
				return false;
			}
		
			return true;
		}

		return false;
	}
	
	private bool IsRepairValid(int repair_kit_type, int repair_with_type)
	{
		if ( repair_kit_type > 0 && repair_with_type > 0 )
		{
			if ( repair_kit_type == repair_with_type )
			{
				return true;
			}
		}
		
		return false;
	}

	private bool CanRepairToPristine( PlayerBase player )
	{
		float specialty = player.GetStatSpecialty().Get();
		
		if ( specialty <= -0.5 || specialty >= 0.5 )
		{
			return true;
		}
		else
		{
			return false; 
		}
	}
}
