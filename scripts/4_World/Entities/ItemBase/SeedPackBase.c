class SeedPackBase extends Inventory_Base
{
	private static const float PACK_DAMAGE_TOLERANCE = 0.5;
	
	void SeedPackBase()
	{	
	}

	void EmptySeedPack( PlayerBase player )
	{
		float damage = 100 - this.GetHealth("", "");
		GetGame().ObjectDelete( this );
		
		if ( damage < 1 )
		{
			string pack_type = this.GetType();
			
			string seeds_type = "";
			GetGame().ConfigGetText( "cfgVehicles " + pack_type + " Horticulture ContainsSeedsType", seeds_type );
			int seeds_quantity_max = GetGame().ConfigGetInt( "cfgVehicles " + pack_type + " Horticulture ContainsSeedsQuantity" );
			int seeds_quantity = seeds_quantity_max;
			
			if ( damage >= PACK_DAMAGE_TOLERANCE ) 
			{
				seeds_quantity = Math.Round( seeds_quantity_max * (1 - ((damage - PACK_DAMAGE_TOLERANCE) / PACK_DAMAGE_TOLERANCE)) );
			}
		
			if ( seeds_quantity < 1 )
			{ 
				seeds_quantity = 1;
			}

			ItemBase item = ItemBase.Cast( player.GetHumanInventory().CreateInInventory( seeds_type) );
			item.SetQuantity( seeds_quantity );
			
			string seeds_display_name = "";
			GetGame().ObjectGetDisplayName( item, seeds_display_name );
			
			string message = "";
			if ( seeds_quantity == seeds_quantity_max )
			{
				message = "I've emptied the package. It contained " + seeds_quantity.ToString() + " " + seeds_display_name + ".";
			}
			else if ( seeds_quantity < seeds_quantity_max / 3 && seeds_quantity > 1 )
			{
				message = "I've emptied the ruptured package and it contained only " + seeds_quantity.ToString() + " " + seeds_display_name + ".";
			}
			else if ( seeds_quantity == 1 )
			{
				// Since the player gained only 1 seed, it's needed to remove the plural sense from the message.
				// This is a temporal solution because it works only for English language!
				if ( seeds_display_name.Length() > 1 )
				{
					seeds_display_name = seeds_display_name.Substring( 0, seeds_display_name.Length() - 1 );
				}
				message = "I've emptied the ruptured package and it contained only 1 " + seeds_display_name + ".";
			}
			else
			{
				message = "I've opened the ruptured package and it contained " + seeds_quantity.ToString() + " " + seeds_display_name + ".";
			}
		
			player.MessageAction( message );
		}
		else 
		{
			player.MessageImportant( "I've opened the ruined package, but it was already empty." );
		}
	}
}
