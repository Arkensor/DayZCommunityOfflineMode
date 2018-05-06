class PlayerCreate : MissionGameplay
{

	private ref TStringArray itemsArray = new TStringArray;
	private ref map<string, ref TStringArray> itemsAttachArray = new map<string, ref TStringArray>;
	private ref TIntArray itemsQtyArray = new TIntArray;
	private ref PlayerSave playerSave = new PlayerSave;
	private ref JsonSerializer js = new JsonSerializer;
	private vector player_pos = "0 0 0";
	private PlayerBase player;
	private int SAVE_TIMER;
	 
	void PlayerCreate(int t)
	{
		SAVE_TIMER = t;
	}
	
	void ~PlayerCreate() 
	{
	
	}
	

	PlayerBase Spawn() 
	{
		// Print(player_pos);
		
		if ( CreatePlayerFromSave() ) 
		{
			// CreateInventory(); 
			CreateInventoryFromSave(); 
			
		}  else {
			
			player = PlayerBase.Cast( GetGame().CreatePlayer( NULL, GetGame().CreateRandomPlayer(), player_pos, 0, "NONE") );
			
			CreateInventory(); 
		}
		
		
		if ( player ) 
		{
			
			GetGame().SelectPlayer(NULL, player);	
			
			playerSave.Run(SAVE_TIMER);
			
			return player;
		}
		
		return NULL;
	}
	 
	 

	 bool CreatePlayerFromSave() 
	 {
	
		map<string,string>arrayFromJson = new map<string,string>;
		string jsonError;
		string savedPlayer = playerSave.LoadMyPlayer();
		// Print("savedPlayer: " + savedPlayer);
		savedPlayer.Trim();
		
		if ( savedPlayer != "" )
		{
			string json;
			
			bool ok = js.ReadFromString( arrayFromJson, savedPlayer, jsonError );
			
			if(!ok)			
			{
				Print(jsonError);
			}
			
			
			player = PlayerBase.Cast( GetGame().CreatePlayer( NULL, arrayFromJson.Get("model"), "0 0 0", 0, "NONE") );
			
			player.SetPosition( arrayFromJson.Get("pos").ToVector() );
			player.SetDirection( arrayFromJson.Get("dir").ToVector() );
			player.SetHealth("","", arrayFromJson.Get("health").ToFloat() );
			player.SetHealth("GlobalHealth", "Blood", arrayFromJson.Get("blood").ToFloat());
			player.GetStatTemperature().Set(arrayFromJson.Get("temperature").ToFloat());
			player.GetStatEnergy().Set(arrayFromJson.Get("energy").ToFloat());
			player.GetStatWater().Set(arrayFromJson.Get("water").ToFloat());
			player.GetStatStomachWater().Set(arrayFromJson.Get("stomachwater").ToFloat());
			player.GetStatStomachEnergy().Set(arrayFromJson.Get("stomachenergy").ToFloat());
			player.GetStatHeatComfort().Set(arrayFromJson.Get("heatcomfort").ToFloat());
			player.GetStatStomachSolid().Set(arrayFromJson.Get("stomachsolid").ToFloat());
			player.GetStatWet().Set(arrayFromJson.Get("wet").ToFloat());
			player.GetStatShock().Set(arrayFromJson.Get("shock").ToFloat());
			player.GetStatBloodType().Set(arrayFromJson.Get("bloodtype").ToFloat());
			

			CreateInHandsFromSave(); 
			
			return true;
		}
		
		
		return false;
	 }
	 
	
	 
	 void CreateInHandsFromSave() 
	 {
		map<string,string>arrayFromJson = new map<string,string>;
		string jsonError;
		EntityAI itemEnt, itemIn;
		ItemBase itemBs;
		
		string data = playerSave.LoadMyHand();
	
		data.Trim();
		
		if ( data != "" )
		{
	
			bool ok = js.ReadFromString( arrayFromJson, data, jsonError );
			
			if(!ok)			
			{
				Print(jsonError);
			}
			
			TStringArray arrayAttach = new TStringArray;	 
			
			js.ReadFromString( arrayAttach, arrayFromJson.Get("attachment"), jsonError );
						
			itemEnt = player.GetHumanInventory().CreateInHands( arrayFromJson.Get("type") );
			
			CreateQuickBarFromSave(itemEnt);
			
			itemBs = ItemBase.Cast(itemEnt);
			
			if (arrayAttach.Count() > 0 ) 
			{
				for (int i = 0; i < arrayAttach.Count(); i++)
				{	
					itemIn = itemEnt.GetInventory().CreateAttachment( arrayAttach.Get(i) );
						
					SetRandomHealth( itemIn );
				}
			}
			
			
			itemBs.SetHealth("","", arrayFromJson.Get("health").ToInt() );
			
			itemBs.SetWet( arrayFromJson.Get("wet").ToFloat() );
				
			if ( itemEnt.IsInherited(Magazine) )
			{
				Magazine magazine = Magazine.Cast( itemEnt );
				
				magazine.LocalSetAmmoCount( arrayFromJson.Get("qty").ToFloat() );

			} else {
				
				itemBs.SetQuantity( arrayFromJson.Get("qty").ToFloat() );
			}
		
		}
		 

	 }

	 void CreateQuickBarFromSave(EntityAI itemEnt) 
	 {
			TStringArray arrayFromJson = new TStringArray;
			string jsonError;
			string data = playerSave.LoadMyQuickBar();
			
			data.Trim();
			
			if ( data != "" )
			{
				bool ok = js.ReadFromString( arrayFromJson, data, jsonError );
			
				if(!ok)			
				{
					Print(jsonError);
				}
				
				
				if ( arrayFromJson.Count() > 0 ) 
				{
					int index = arrayFromJson.Find( itemEnt.GetType() );
					player.SetQuickBarEntityShortcut( itemEnt, index );
				}
			}
	 }
	 
	 
	 void CreateInventoryFromSave() 
	 {
		 
		TStringArray data = playerSave.LoadMyInventory();
		
		string jsonError;
		
		array<ref map<string,string>> arrayFromJson = new array<ref map<string,string>>;
		
		array<ref map<string,string>> InventoryArray = new array<ref map<string,string>>;
		
		for (int i = 0; i < data.Count(); i++)
		{	
			string json = data.Get(i);
			// Print("JSON: " + json);
			bool ok = js.ReadFromString( arrayFromJson, json, jsonError );
			
			if(!ok)			
			{
				Print(jsonError);
			}
			
			InventoryArray.Insert( arrayFromJson[0] );
			// Print(arrayFromJson[0]);
		}
		
	

		float qty, health, wet;
		string item, attJson;
		EntityAI itemEnt, itemIn;
		ItemBase itemBs;
		
		for (int j = 0; j < InventoryArray.Count(); j++)
		{	
			
			item = InventoryArray.Get(j).Get("type");
			
			qty = InventoryArray.Get(j).Get("qty").ToFloat();
			
			health = InventoryArray.Get(j).Get("health").ToFloat();
			
			wet = InventoryArray.Get(j).Get("wet").ToFloat();
			
			attJson = InventoryArray.Get(j).Get("attachment");
			
			TStringArray attArray;
			
			js.ReadFromString( attArray, attJson, jsonError );
			
			// Print(attArray);
			
			itemEnt = player.GetInventory().CreateInInventory(item);
			
			CreateQuickBarFromSave(itemEnt);
			
			itemBs = ItemBase.Cast(itemEnt);
			
			if ( attArray.Count() > 0 ) 
			{
				ref array<EntityAI> hasAttachment = new array<EntityAI>;
				for (int k = 0; k < attArray.Count(); k++)
				{	
					itemIn = itemEnt.GetInventory().CreateAttachment( attArray.Get(k) );
						
					SetRandomHealth( itemIn );
				}
			}	

		
			itemBs = ItemBase.Cast(itemEnt);
			
			itemBs.SetHealth("","",health);
			
			itemBs.SetWet(wet);
				
			if ( itemEnt.IsInherited(Magazine) )
			{
				Magazine magazine = Magazine.Cast( itemEnt );
				
				magazine.LocalSetAmmoCount(qty);
				// magazine.ServerSetAmmoCount(qty);		
			} else {
				
				itemBs.SetQuantity(qty);
			}
			
			
			
		}
		
	 }	 
	 
	 
	 
	void SetPosition(TVectorArray positions)
	{	

		int index;

		// string savedPos = playerSave.getMyPosition();
		string savedPos = "";

		if ( savedPos != "" )
		{
			player_pos = savedPos.ToVector();
			
		}	else {
			
			index = Math.RandomInt( 0, positions.Count() );
			
			player_pos = positions.Get( index );
			
		}

		// Print(player_pos);
	}
	 
	
	void SetInventory(string item = "", float qty = 0, TStringArray attach = NULL) 
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
			
		// Print(itemsArray);
	} 
	
	 
	void CreateInventory() 
	{
		float qty;
		string item, itemAttach;
		EntityAI itemEnt, itemIn;
		ItemBase itemBs;
		
		if ( itemsArray.Count() > 0 ) 
		{
		
			// Print(itemsArray);
		 
			for ( int i = 0; i < itemsArray.Count(); i++)
			{
				
				item = itemsArray.Get(i);
				
				itemEnt = player.GetInventory().CreateInInventory( item );
				
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
						
							SetRandomHealth( itemIn );
						}	
					}
				}
				 
				SetRandomHealth( itemEnt );
			}
		}
		
	}
	 
	 
	void SetRandomHealth(EntityAI itm)
	{
		// Print(itm);
		int rndHlt = Math.RandomInt(40,100);
		itm.SetHealth("","",rndHlt);
	}
	
	TVectorArray SpawnPositions() 
	{
		return { "15135.1 0 13901.1", "15017.8 0 13892.4", "14887.1 0 14547.9", "14749.7 0 13248.7",
		"14697.6 0 13418.4", "14537.3 0 14755.7", "14415.3 0 14025.2", "14338.0 0 12859.5",
		"14263.8 0 12748.7", "14172.2 0 12304.9", "14071.4 0 12033.3", "14054.9 0 11341.3",
		"14017.8 0 2959.1", "13905.5 0 12489.7", "13852.4 0 11686.0", "13846.6 0 12050.0",
		"13676.0 0 12262.1", "13617.4 0 12759.8", "13610.1 0 11223.6", "13594.3 0 4064.0",
		"13587.8 0 6026.5", "13571.1 0 3056.8", "13552.6 0 4653.7", "13529.9 0 3968.3",
		"13520.8 0 4223.7", "13504.0 0 5004.5", "13476.7 0 6136.3", "13441.6 0 5262.2",
		"13426.6 0 5747.3", "13416.8 0 11840.4", "13400.8 0 4120.7", "13395.8 0 5902.8",
		"13385.0 0 3946.6", "13374.4 0 6454.3", "13367.1 0 10837.1", "13366.3 0 4906.0",
		"13337.1 0 5120.8", "13326.7 0 5489.1", "13312.7 0 6771.1", "13288.7 0 11415.1",
		"13261.6 0 11785.2", "13171.6 0 6534.8", "13159.8 0 5401.7", "13155.2 0 5475.2",
		"13084.9 0 7938.6", "13056.8 0 4848.5", "13048.1 0 8357.6", "13048.1 0 3867.7",
		"12991.7 0 7287.1", "12983.0 0 5539.1", "12978.9 0 9727.8", "12950.2 0 5226.7",
		"12942.1 0 8393.1", "12891.5 0 3673.9", "12628.7 0 10495.2", "12574.3 0 3592.8",
		"12566.3 0 6682.6", "12465.2 0 8009.0", "12354.5 0 3480.0", "13262.8 0 7225.8" };
	};
}