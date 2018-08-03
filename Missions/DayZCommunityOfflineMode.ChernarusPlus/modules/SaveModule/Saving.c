enum SaveTypes {
    CHARACTER, INVENTORY, HANDS, QUICKBAR
}

class Saving 
{
    static string CargoItemsToJSON(ref EntityAI oEntity)
	{
		string json = "";
		
		ref map<string, ref map<string,string>> tempArray = new map<string, ref map<string,string>>;
	
		auto cargo = oEntity.GetInventory().GetCargo();

		if ( cargo )
		{
            for (int i = 0; i < cargo.GetItemCount(); i++)
            {
                ItemBase itemInCargo = NULL;

                if ( Class.CastTo( itemInCargo, cargo.GetItem(i) ) && itemInCargo.IsItemBase() )
                {
                    if ( itemInCargo )
                    {
                        string itemName;

                        itemName = itemInCargo.GetType();
                        tempArray.Insert( itemName, new ref map<string,string> );

                        string health = itemInCargo.GetHealth("","").ToString();
                        tempArray.Get( itemName ).Insert( "health", health );

                        string qty = Helper.GetItemQuantity( itemInCargo ).ToString();
                        tempArray.Get( itemName ).Insert( "qty", qty );

                        string wet = itemInCargo.GetWet().ToString();
                        tempArray.Get( itemInCargo.GetType() ).Insert( "wet", wet );

                        string attJson = Saving.AttachedItemsToJSON( itemInCargo );

                        if ( attJson != "" )
                        {
                            tempArray.Get( itemName ).Insert( "attachments", attJson );
                        }

                        string cargoJson = Saving.CargoItemsToJSON( itemInCargo );

                        if ( cargoJson != "" )
                        {
                            tempArray.Get( itemName ).Insert( "cargo", cargoJson );
                        }

                        int pos_x, pos_y;

                        oEntity.GetInventory().GetCargo().GetItemRowCol(i, pos_x, pos_y );

                        string x = pos_x.ToString();
                        tempArray.Get( itemName ).Insert( "x", x );

                        string y = pos_y.ToString();
                        tempArray.Get( itemName ).Insert( "y", y );
                    }
                }
            }
		}
		
		if ( tempArray.Count() > 0 ) 
		{
			Helper.JSON.WriteToString(tempArray, false, json);
		}
		
		delete tempArray;
		
		return json;
	}
	
	static string AttachedItemsToJSON(ref EntityAI oEntity)
	{
		ref map<string, ref map<string,string>> tempArray = new map<string, ref map<string,string>>;
		
		string json = "";
		
		for (int att = 0; att < oEntity.GetInventory().AttachmentCount(); att++)
		{
			EntityAI attached = oEntity.GetInventory().GetAttachmentFromIndex(att);
			
			if ( attached  )
			{
				ItemBase item = NULL;
				
				if ( Class.CastTo( item, attached ) && attached.IsItemBase() )
				{
					
					if( item )
					{
						string itemName;
					
						itemName = item.GetType();
						tempArray.Insert( itemName, new ref map<string,string> );

						string health = item.GetHealth("","").ToString();
						tempArray.Get( itemName ).Insert( "health", health );

						string qty = Helper.GetItemQuantity( item ).ToString();
						tempArray.Get( itemName ).Insert( "qty", qty );

						string wet = item.GetWet().ToString();
						tempArray.Get( item.GetType() ).Insert( "wet", wet ); 

						string attJson = Saving.AttachedItemsToJSON( item );
						
						if ( attJson != "" )
						{
							tempArray.Get( itemName ).Insert( "attachments", attJson ); 
						}
						
						string cargoJson = Saving.CargoItemsToJSON( item );
						
						if ( cargoJson != "" )
						{
							tempArray.Get( itemName ).Insert( "cargo", cargoJson );
						}
					}
				}
			}
		}
		
		if ( tempArray.Count() > 0 ) 
		{
			Helper.JSON.WriteToString(tempArray, false, json);
		}

		delete tempArray;
		
		return json;
	}

	static string InventoryToJSON()
	{
		string Data = "";

		string json = "";
		
		ref array<string> DataArray = new array<string>;
		
		ref map<string,ref map<string,string>> PlayerInventory =  new map<string, ref map<string,string>>;
		
		array<EntityAI> playerArray = new array<EntityAI>;
		
		Helper.Player.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, playerArray);
		
		ItemBase playerHands = Helper.Player.GetHumanInventory().GetEntityInHands();
		
		for (int p = 0; p  < playerArray.Count(); p ++)
		{	
			ItemBase itemOnPlayer;
			
			Class.CastTo( itemOnPlayer, playerArray.Get(p) );
			
			if ( itemOnPlayer == NULL || itemOnPlayer == playerHands ) continue;
			
			string typeName = itemOnPlayer.GetType();
			PlayerInventory.Insert( typeName, new map<string, string> );
			
			string health = itemOnPlayer.GetHealth("","").ToString();
			PlayerInventory.Get( typeName ).Insert( "health", health );
			
			string qty = Helper.GetItemQuantity(itemOnPlayer).ToString();
			PlayerInventory.Get( typeName ).Insert( "qty", qty ); 
			
			string wet = itemOnPlayer.GetWet().ToString();
			PlayerInventory.Get( typeName ).Insert( "wet", wet ); 
			
			string attJson = Saving.AttachedItemsToJSON( itemOnPlayer );
			
			if ( attJson != "" )
			{
				PlayerInventory.Get( typeName ).Insert( "attachments", attJson ); 
			}
			
			
			string cargoJson = Saving.CargoItemsToJSON( itemOnPlayer );
			
			if ( cargoJson != "" )
			{
				PlayerInventory.Get( typeName ).Insert( "cargo", cargoJson );
			}
			
			
			Helper.JSON.WriteToString(PlayerInventory, false, json);	
			
			DataArray.Insert(json);
			
			PlayerInventory.Clear();
		}

		delete playerArray;
		
		delete PlayerInventory;
		for (int i = 0; i < DataArray.Count(); i++) {
			Data = Data + DataArray[i];
		}
		
		delete DataArray;
		return Data;
	}

	static string CharacterToJSON()
	{
		string Data = "";
		
		map<string, string> tempArray = new map<string, string>;
		
		string model = Helper.Player.GetType();
		tempArray.Insert( "model", model );
		
		string pos = Helper.Player.GetPosition().ToString();
		pos.Replace("<", "");
		pos.Replace(">", "");
		pos.Replace(",", "");
		tempArray.Insert( "pos", pos );
		
		string dir = Helper.Player.GetDirection().ToString();
		dir.Replace("<", "");
		dir.Replace(">", "");
		dir.Replace(",", "");
		tempArray.Insert( "dir", dir );
		string ori = Helper.Player.GetOrientation().ToString();
		dir.Replace("<", "");
		dir.Replace(">", "");
		dir.Replace(",", "");
		tempArray.Insert( "ori", dir );
		string health = Helper.Player.GetHealth("","").ToString();
		tempArray.Insert( "health", health ); 
		
		string blood = Helper.Player.GetHealth("GlobalHealth", "Blood").ToString();
		tempArray.Insert( "blood", blood );
		
		string bloodtype = Helper.Player.GetStatBloodType().Get().ToString();
		tempArray.Insert( "bloodtype", bloodtype );
		
		string temperature = Helper.Player.GetStatTemperature().Get().ToString();
		tempArray.Insert( "temperature", temperature );
		
		string energy = Helper.Player.GetStatEnergy().Get().ToString();
		tempArray.Insert( "energy", energy );
		
		string water = Helper.Player.GetStatWater().Get().ToString();
		tempArray.Insert( "water", water );
		
		string heatcomfort = Helper.Player.GetStatHeatComfort().Get().ToString();
		tempArray.Insert( "heatcomfort", heatcomfort );
		
		string stomachenergy = Helper.Player.GetStatStomachEnergy().Get().ToString();
		tempArray.Insert( "stomachenergy", stomachenergy );
		
		string stomachwater = Helper.Player.GetStatStomachWater().Get().ToString();
		tempArray.Insert( "stomachwater", stomachwater );
		
		string stomachsolid = Helper.Player.GetStatStomachSolid().Get().ToString();
		tempArray.Insert( "stomachsolid", stomachsolid );
		
		string wet = Helper.Player.GetStatWet().Get().ToString();
		tempArray.Insert( "wet", wet );

		string tremor = Helper.Player.GetStatTremor().Get().ToString();
		tempArray.Insert( "tremor", tremor );
		string stamina = Helper.Player.GetStatStamina().Get().ToString();
		tempArray.Insert( "stamina", stamina );
		
		string playtime = Helper.Player.StatGet("playtime").ToString();
		tempArray.Insert("playtime", playtime);
		
		string lastshaved = Helper.Player.GetLastShavedSeconds().ToString();
		tempArray.Insert("lastshaved", lastshaved);
		
		string bloodyhands = Helper.Player.HasBloodyHands().ToString();
		tempArray.Insert("bloodyhands", bloodyhands);
		
		Helper.JSON.WriteToString(tempArray, false, Data);
		
		delete tempArray;
		return Data;
	}

	static string QuickbarToJSON()
	{
		string Data = "";
		
		TStringArray tempArray = new TStringArray;
		
		int size = Helper.Player.GetQuickBarSize();
		
		for ( int i = 0; i < size; i++)
		{
			InventoryItem item = InventoryItem.Cast( Helper.Player.GetQuickBarEntity(i) );
			
			if( item != NULL ) 
			{
				tempArray.Insert( item.GetType() );
			}
		}
		
		Helper.JSON.WriteToString(tempArray, false, Data);
		
		delete tempArray;
		return Data;
	}

	static string HandsToJSON()
	{		
		string Data = "{}";
		
		ref map<string,ref map<string,string>> tempArray =  new map<string, ref map<string,string>>;
		
		ItemBase hands = Helper.Player.GetHumanInventory().GetEntityInHands();
		
		if ( hands != NULL )
		{
			string typeName = hands.GetType();
			tempArray.Insert( typeName, new map<string, string> );
			
			string health = hands.GetHealth("","").ToString();			
			tempArray.Get( typeName ).Insert( "health", health );
			
			string qty = Helper.GetItemQuantity(hands).ToString();
			tempArray.Get( typeName ).Insert( "qty", qty );
			
			string wet = hands.GetWet().ToString();
			tempArray.Get( typeName ).Insert( "wet", wet );
			
			
			string attJson = Saving.AttachedItemsToJSON( hands );
			if ( attJson != "" )
			{
				tempArray.Get( typeName ).Insert( "attachments", attJson );
			}
			
			
			string cargoJson = Saving.CargoItemsToJSON( hands );
			if ( cargoJson != "" )
			{
				tempArray.Get( typeName ).Insert( "cargo", cargoJson );
			}
			
			
			Helper.JSON.WriteToString(tempArray, false, Data);
			
			return Data;
		}

		return "{}";
	}
}