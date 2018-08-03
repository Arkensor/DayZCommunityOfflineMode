class Reading
{
	static bool LoadInventoryFromJSON(ref TStringArray data, ref EntityAI entity = NULL, string createAs = "")
	{
		if (entity == NULL) {
			entity = Helper.Player;
		}

		for (int i = 0; i < data.Count(); i++)
		{
			map<string,map<string,string>> arrayFromJson = new map<string,map<string,string>>;
			
			string jsonError;
			string jsonArr = data.Get(i);

			bool ok = Helper.JSON.ReadFromString( arrayFromJson, jsonArr, jsonError );
			
			if(!ok)
			{
				Print(jsonError);

                delete arrayFromJson;
		        delete data;
				return false;
			}
			
			for (int j = 0; j < arrayFromJson.Count(); j++)
			{
				map<string,string> itemArray = new map<string,string>;
				
				string itemName = arrayFromJson.GetKey(j);
			
				itemArray = arrayFromJson.GetElement(j);
				
				EntityAI itemEnt;
				
				switch (createAs)
				{
					case "cargo":
						int x = itemArray.Get("x").ToInt();
						int y = itemArray.Get("y").ToInt();
						itemEnt = entity.GetInventory().CreateEntityInCargoEx( itemName, 0, y, x );
					break;
					
					case "att":
						itemEnt = entity.GetInventory().CreateAttachment( itemName );
					break;
					
					default: 
						itemEnt = entity.GetInventory().CreateInInventory( itemName );
					break;
				}
				
				if (itemEnt != NULL ) 
				{
					ItemBase itemBs = ItemBase.Cast(itemEnt);
				
					itemBs.SetHealth("","", itemArray.Get("health").ToFloat() );
			
					itemBs.SetWet( itemArray.Get("wet").ToFloat() );
			 
					float qty = itemArray.Get("qty").ToFloat();
				
					if ( itemEnt.IsInherited(Magazine) )
					{
						Magazine magazine = Magazine.Cast( itemEnt );
						magazine.LocalSetAmmoCount(qty);
					} else {
						itemBs.SetQuantity(qty);
					}
				
			 
					TStringArray temp;
					
					string cargo = itemArray.Get("cargo");
				
					if ( cargo != "{}"  && cargo != "[]" && cargo != "" )
					{
						temp = new TStringArray;
						
						temp.Insert(cargo);
						
						LoadInventoryFromJSON(temp, itemBs, "cargo");
					}
				
					string attachments = itemArray.Get("attachments");

					if ( attachments != "{}" && attachments != "[]" && attachments != "" )
					{	
						temp = new TStringArray;
						
						temp.Insert(attachments);
						
						LoadInventoryFromJSON(temp, itemBs, "att");
					}
				
					LoadQuickbarFromJSON( itemEnt );
				}
			}
		}
		
		delete arrayFromJson;
		delete data;

        return true;
	}

	static bool LoadCharacterFromJSON(ref TStringArray file, PluginLifespanPatched cpl = NULL)
	{
        string data = file[0];

		map<string, string>arrayFromJson = new map<string, string>;

		if ( data != "" )
		{
			string json;
			string jsonError;
			bool ok = Helper.JSON.ReadFromString( arrayFromJson, data, jsonError );
			
			if(!ok)
			{
				Print(jsonError);

                delete arrayFromJson;
                return false;
			}
		
			Helper.Player = PlayerBase.Cast( GetGame().CreatePlayer( NULL, arrayFromJson.Get("model"), "0 0 0", 0, "NONE") );
			
			vector pos = arrayFromJson.Get("pos").ToVector();
			Helper.Player.SetPosition( pos );
			
			vector dir = arrayFromJson.Get("dir").ToVector();
			Helper.Player.SetDirection( dir );
			
			vector ori = arrayFromJson.Get("ori").ToVector();
			Helper.Player.SetOrientation( ori );
			
			float health = arrayFromJson.Get("health").ToFloat();
			Helper.Player.SetHealth("","", health );
			
			float blood = arrayFromJson.Get("blood").ToFloat();
			Helper.Player.SetHealth("GlobalHealth", "Blood", blood );
			
			int bloodtype = arrayFromJson.Get("bloodtype").ToInt();
			Helper.Player.GetStatBloodType().Set( bloodtype );
			
			float temperature = arrayFromJson.Get("temperature").ToFloat();
			Helper.Player.GetStatTemperature().Set( temperature );
			
			float energy = arrayFromJson.Get("energy").ToFloat();
			Helper.Player.GetStatEnergy().Set( energy );
			
			float water = arrayFromJson.Get("water").ToFloat();
			Helper.Player.GetStatWater().Set( water );
			
			float stomachwater = arrayFromJson.Get("stomachwater").ToFloat();
			Helper.Player.GetStatStomachWater().Set( stomachwater );
			
			float stomachenergy = arrayFromJson.Get("stomachenergy").ToFloat();
			Helper.Player.GetStatStomachEnergy().Set( stomachenergy );
			
			float heatcomfort = arrayFromJson.Get("heatcomfort").ToFloat();
			Helper.Player.GetStatHeatComfort().Set( heatcomfort );
			
			float stomachsolid = arrayFromJson.Get("stomachsolid").ToFloat();
			Helper.Player.GetStatStomachSolid().Set( stomachsolid );
			
			float wet = arrayFromJson.Get("wet").ToFloat();
			Helper.Player.GetStatWet().Set( wet );

			float tremor = arrayFromJson.Get("tremor").ToFloat();
			Helper.Player.GetStatTremor().Set( tremor );
			
			float stamina = arrayFromJson.Get("stamina").ToFloat();
			Helper.Player.GetStatStamina().Set( stamina );
		  
			float playtime = arrayFromJson.Get("playtime").ToFloat();
			Helper.Player.StatRegister("playtime");
			Helper.Player.StatUpdate("playtime", playtime);
			
			float lastshaved = arrayFromJson.Get("lastshaved").ToFloat();
			Helper.Player.SetLastShavedSeconds(lastshaved);
			
			bool bloodyhands = Helper.StringToBool( arrayFromJson.Get("bloodyhands") );
			Helper.Player.SetBloodyHands(bloodyhands);

			if(cpl != NULL)
			{
				cpl.UpdateLifespan(Helper.Player, true);
			}

			Helper.Player.SetSynchDirty();
	
			// LoadHandsFromJSON(); 
			
			return true;
		}
		return false;
	}

	static bool LoadHandsFromJSON(ref TStringArray rawData, ref EntityAI entity = NULL, string createAs = "") 
	{
        string data = rawData[0];

		return LoadHandsFromJSON(data, entity, createAs);
	}

	static bool LoadHandsFromJSON(string data, ref EntityAI entity = NULL, string createAs = "") 
	{
		if (entity == NULL) {
			entity = Helper.Player;
		}


		map<string, map<string, string>> arrayFromJson = new map<string, map<string, string>>;
		
		if ( data != "" )
		{
			string jsonError;
			
			bool ok = Helper.JSON.ReadFromString( arrayFromJson, data, jsonError );
			
			if(!ok)
			{
				Print(jsonError);

                delete arrayFromJson;
                return false;
			}
			
			for (int i = 0; i < arrayFromJson.Count(); i++)
			{
				EntityAI itemEnt;
				
				map<string,string> itemArray = new map<string,string>;
				
				string itemName = arrayFromJson.GetKey(i);
			
				itemArray = arrayFromJson.GetElement(i);
				
				switch (createAs)
				{
					case "cargo":
						int x = itemArray.Get("x").ToInt();
						int y = itemArray.Get("y").ToInt();
						itemEnt = entity.GetInventory().CreateEntityInCargoEx( itemName, 0, y, x );
					break;
					
					case "att":
						itemEnt = entity.GetInventory().CreateAttachment( itemName );
					break;
					
					default: 
						itemEnt = Helper.Player.GetHumanInventory().CreateInHands( itemName );
					break;
				}
				

				if (itemEnt != NULL ) 
				{
					
					ItemBase itemBs = ItemBase.Cast( itemEnt );
					
					float health = itemArray.Get("health").ToFloat();
					itemBs.SetHealth("","", health );
					
					float wet = itemArray.Get("wet").ToFloat();
					itemBs.SetWet( wet );
					
					float qty = itemArray.Get("qty").ToFloat();
					
					if ( itemEnt.IsInherited(Magazine) )
					{
						Magazine magazine = Magazine.Cast( itemEnt );
						magazine.LocalSetAmmoCount( qty );
					} else {
						itemBs.SetQuantity( qty );
					}
					
					string cargo = itemArray.Get("cargo");
				
					if ( cargo != "{}"  && cargo != "[]" && cargo != "" )
					{
						LoadHandsFromJSON(cargo, itemBs, "cargo");
					}
				
					string attachments = itemArray.Get("attachments");
				
					if ( attachments != "{}" && attachments != "[]" && attachments != "" )
					{	
						LoadHandsFromJSON(attachments, itemBs, "att");
					}
				
					LoadQuickbarFromJSON( itemEnt );
				}
			}

		    delete arrayFromJson;
            return true;
		}

		delete arrayFromJson;
        return false;
	}

	static bool LoadQuickbarFromJSON(EntityAI itemEnt)
	{
        string data = "";

		TStringArray arrayFromJson = new TStringArray;
			
		string jsonError;
			
		if ( data != "" )
		{
			bool ok = Helper.JSON.ReadFromString( arrayFromJson, data, jsonError );
		
			if(!ok)
			{
				Print(jsonError);
			}
			
			if ( arrayFromJson.Count() > 0 )
			{
				int index = arrayFromJson.Find( itemEnt.GetType() );
				
				Helper.Player.SetQuickBarEntityShortcut( itemEnt, index );
			}

		    delete arrayFromJson;
            return true;
		}

		delete arrayFromJson;
        return false;
	}
}