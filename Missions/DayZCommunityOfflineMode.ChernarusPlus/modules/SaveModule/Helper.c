class Helper
{
    private static ref TStringArray itemsArray = new TStringArray;
	private static ref TIntArray itemsQtyArray = new TIntArray;
	private static ref map<string, ref TStringArray> itemsAttachArray = new map<string, ref TStringArray>;

	private static vector position = "0 0 0";

	static ref JsonSerializer JSON = new JsonSerializer;

    static PlayerBase Player;

    static void CreatePlayerInventory(ref EntityAI entity)
	{
		float qty;
		
		string item, itemAttach;
		
		EntityAI itemEnt, itemIn;
		
		ItemBase itemBs;
		
		for ( int i = 0; i < itemsArray.Count(); i++)
		{
			
			item = itemsArray.Get(i);
			
			itemEnt = entity.GetInventory().CreateInInventory( item );
			
			qty = itemsQtyArray.Get(i);
			
			if ( qty > 0 ) 
			{
				itemBs = ItemBase.Cast( itemEnt );
				
				itemBs.SetQuantity( qty );
			} 
			
			if( itemsAttachArray != NULL && itemsAttachArray.Count() > 0 )
			{
				
				TStringArray attArr = itemsAttachArray.Get(item);
			
				if ( attArr != NULL && attArr.Count() > 0 )
				{
					for ( int j = 0; j < attArr.Count(); j++)
					{	
						itemIn = itemEnt.GetInventory().CreateAttachment( attArr.Get(j) );
					
						itemIn.SetHealth( "","",100 );
					}
				}
			}

			itemEnt.SetHealth( "","",100 );
		}
	}

	static void SetPlayerInventory(string item = "", float qty = 0, TStringArray attach = NULL)
	{
		if ( item != "" ) 
		{
			itemsArray.Insert( item );
			
			itemsQtyArray.Insert( qty );
		}
		
		if ( attach != NULL && attach.Count() > 0 )
		{
			itemsAttachArray.Insert( item, attach );
		}
	} 

	static void SetPosition(TVectorArray positions)
	{	
		int index = Math.RandomInt( 0, positions.Count() );
		
		position = positions.Get( index );
	}

	static void SetRandomHealth(EntityAI itm)
	{
		int rndHlt = Math.RandomInt(40,100);
		
		itm.SetHealth("","",rndHlt);
	}

	static float GetItemQuantity( InventoryItem item )
	{
		float quantity = 0;
		
		if ( item.IsInherited( InventoryItem ) )
		{
			ItemBase item_base = ItemBase.Cast( item );
			
			if ( item.IsInherited( Magazine) )
			{
				Magazine magazine_item = Magazine.Cast( item );
				
				quantity = magazine_item.GetAmmoCount();
			}
			else
			{
				quantity = item_base.GetQuantity();
			}
		}
		return quantity;
	}

    static bool StringToBool(string str = "")
	{
		str.ToLower();
		
		if ( str == "false"  ||  str == "0" ) 
		{
			return false;
		}
		
		if( str == "true"  ||  str == "1" ) 
		{
			return true;
		}
		
		return false;
	}
}