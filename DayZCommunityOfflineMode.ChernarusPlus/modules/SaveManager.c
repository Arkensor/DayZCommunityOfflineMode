class SaveManager 
{
		
	
	private string BASE_DIR = "$saves:CommunityOfflineMode//";
	private string BASE_PLAYERDIR = BASE_DIR + "//PlayerSaves";
	private string CHAR_FILE = "//character.json";
	private string INV_FILE = "//inventory.json";
	private string HND_FILE = "//hands.json";
	private string QB_FILE = "//quickbar.json";
	
	private PlayerBase player;
	private ref JsonSerializer js = new JsonSerializer;	
	private string SaveFile;

	private string Data = "";
	private vector position = "0 0 0";
	private int timer;
	
	private ref TStringArray itemsArray = new TStringArray;
	private ref TIntArray itemsQtyArray = new TIntArray;
	private ref map<string, ref TStringArray> itemsAttachArray = new map<string, ref TStringArray>;
	
	void SaveManager(int t = 30)
	{
		timer = t;
		
		player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		CreateDir(BASE_PLAYERDIR);
		
	}
	
	void ~SaveManager()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.ProcessPlayerSaves);
	}
	
	
	
	void ProcessPlayerSaves()
	{
		if ( !player ) return;
		
	
		if ( player.GetPlayerState() == EPlayerStates.ALIVE && !player.IsRestrained() )
		{
			SavePlayerCharacter();
			
			SavePlayerQuickBar();
			
			SavePlayerInventory();
			
		} else {
			
			DeletePlayer();
		}
	}
	
	
	PlayerBase SpawnPlayer() 
	{
				
		if ( CreatePlayerCharacterFromSave() ) 
		{
			CreatePlayerInventoryFromSave(); 
			
		}  else {
			
			player = PlayerBase.Cast( GetGame().CreatePlayer( NULL, GetGame().CreateRandomPlayer(), position, 0, "NONE") );
			
			CreatePlayerInventory(); 
		}
		
		
		if ( player ) 
		{
			
			GetGame().SelectPlayer(NULL, player);
			
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.ProcessPlayerSaves, timer * 1000, true);
			
			return player;
		}
		
		return NULL;
	}

	
	void SavePlayerCharacter()
	{	
		Data = "";
		SaveFile = BASE_PLAYERDIR + CHAR_FILE;
		
		map<string,string>PlayerArray = new map<string,string>;
		
		PlayerArray.Insert( "model", player.GetType() );
		PlayerArray.Insert( "health", player.GetHealth("", "").ToString() ); 
		PlayerArray.Insert( "blood", player.GetHealth("GlobalHealth", "Blood").ToString() ); 
		PlayerArray.Insert( "temperature", player.GetStatTemperature().Get().ToString() );
		PlayerArray.Insert( "energy", player.GetStatEnergy().Get().ToString() );
		PlayerArray.Insert( "water", player.GetStatWater().Get().ToString() );
		PlayerArray.Insert( "heatcomfort", player.GetStatHeatComfort().Get().ToString() );
		PlayerArray.Insert( "stomachenergy", player.GetStatStomachEnergy().Get().ToString() );
		PlayerArray.Insert( "stomachwater", player.GetStatStomachWater().Get().ToString() );
		PlayerArray.Insert( "stomachsolid", player.GetStatStomachSolid().Get().ToString() );
		PlayerArray.Insert( "wet", player.GetStatWet().Get().ToString() );
		PlayerArray.Insert( "shock", player.GetStatShock().Get().ToString() );
		PlayerArray.Insert( "bloodtype", player.GetStatBloodType().Get().ToString() );

		string pos = player.GetPosition().ToString();
		
		pos.Replace("<", "");
		pos.Replace(">", "");
		pos.Replace(",", "");
		
		PlayerArray.Insert( "pos", pos );
	
		string dir = player.GetDirection().ToString();
		
		dir.Replace("<", "");
		dir.Replace(">", "");
		dir.Replace(",", "");
		
		PlayerArray.Insert( "dir", dir );
	
		js.WriteToString(PlayerArray, false, Data);
	
		WriteFile(Data);
	}
	
	
	void SavePlayerQuickBar()
	{
		Data = "";
		SaveFile = BASE_PLAYERDIR + QB_FILE;
		
		TStringArray qb_array = new TStringArray;
		
		int size = player.GetQuickBarSize();
		
		for ( int i = 0; i < size; i++)
		{
			InventoryItem item = InventoryItem.Cast( player.GetQuickBarEntity(i) );
		
			// Print( item );
			if( item != NULL ) 
			{
				qb_array.Insert( item.GetType() );
			}
		}
		
		js.WriteToString(qb_array, false, Data);
	
		WriteFile(Data);
	}
	
	
	
	void SavePlayerHand() 
	{
		
		SaveFile = BASE_PLAYERDIR + HND_FILE;
		string Data = "";		
		ItemBase hands = player.GetHumanInventory().GetEntityInHands();
	
		if ( hands != NULL ) 
		{
			TStringArray attachmentArray = new TStringArray;
			map<string,string>itemArray = new map<string,string>;
			
			// Print(hands.GetType());
			itemArray.Insert( "type", hands.GetType() ); 
			itemArray.Insert( "health", hands.GetHealth("","").ToString() ); 
			itemArray.Insert( "qty", GetItemQuantity(hands).ToString() ); 
			itemArray.Insert( "wet", hands.GetWet().ToString() ); 
			
			string b = "";	
			for (int j = 0; j < hands.GetInventory().AttachmentCount(); j++)
			{		ItemBase attachment;
					attachment = hands.GetInventory().GetAttachmentFromIndex(j);
					attachmentArray.Insert(attachment.GetType());
			}
			
			js.WriteToString(attachmentArray, false, b);
				
			itemArray.Insert( "attachment", b ); 
		
			js.WriteToString(itemArray, false, Data);

			WriteFile(Data);
		} else {
			
			DelFile(BASE_PLAYERDIR + HND_FILE);
		}
	}
	
	
	
	void SavePlayerInventory() 
	{
		
		SaveFile = BASE_PLAYERDIR + INV_FILE;
		
		FileHandle file = OpenFile( SaveFile, FileMode.WRITE );
				
		array<ref map<string,string>> InventoryArray = new array<ref map<string,string>>;
		
		ItemBase inHands = player.GetHumanInventory().GetEntityInHands();
		
		array<EntityAI> inventory = new array<EntityAI>;
		
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, inventory);

		for (int i = 0; i < inventory.Count(); i++)
		{	
			ItemBase item;
			string a = "";
			string b = "";		
			Class.CastTo(item, inventory.Get(i));
			
			if ( item && !item.IsInherited(SurvivorBase) && item != inHands) 
			{	
				
				if( item.GetHierarchyParent().IsWeapon() ) continue;
					
				map<string,string>itemArray = new map<string,string>;
				TStringArray attachmentArray = new TStringArray;
			
				itemArray.Insert( "type", item.GetType() ); 
				itemArray.Insert( "health", item.GetHealth("","").ToString() ); 
				itemArray.Insert( "qty", GetItemQuantity(item).ToString() ); 
				itemArray.Insert( "wet", item.GetWet().ToString() ); 
				
				for (int j = 0; j < item.GetInventory().AttachmentCount(); j++)
				{		ItemBase attachment;
						attachment = item.GetInventory().GetAttachmentFromIndex(j);
						attachmentArray.Insert(attachment.GetType());
				}
					
				js.WriteToString(attachmentArray, false, b);
				
				itemArray.Insert( "attachment", b ); 
				InventoryArray.Insert( itemArray );
				js.WriteToString(InventoryArray, false, Data);
				
				FPrintln(file,Data);
				
				InventoryArray.Clear();	
			}
		}
			
			CloseFile(file);
		
			SavePlayerHand(); 
	}
	



	 bool CreatePlayerCharacterFromSave() 
	 {
	
		map<string,string>arrayFromJson = new map<string,string>;
		string jsonError;
		string data = LoadPlayerCharacter();
		// Print("LoadPlayerCharacter: " + data);
		
		if ( data != "" )
		{
			string json;
			
			bool ok = js.ReadFromString( arrayFromJson, data, jsonError );
			
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
			

			CreatePlayerHandsFromSave(); 
			
			return true;
		}
		
		
		return false;
	 }
	 
	
	 
	 void CreatePlayerHandsFromSave() 
	 {
		map<string,string>arrayFromJson = new map<string,string>;
		string jsonError;
		EntityAI itemEnt, itemIn;
		ItemBase itemBs;
		
		string data = LoadPlayerHands();
	

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
			
			CreatePlayerQuickBarFromSave(itemEnt);
			
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

	 void CreatePlayerQuickBarFromSave(EntityAI itemEnt) 
	 {
			TStringArray arrayFromJson = new TStringArray;
			string jsonError;
			string data = LoadPlayerQuickBar();
			
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
	 
	 
	 void CreatePlayerInventoryFromSave() 
	 {
		 
		TStringArray data = LoadPlayerInventory();
		
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
			
			CreatePlayerQuickBarFromSave(itemEnt);
			
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
	

	void CreatePlayerInventory() 
	{
		
		float qty;
		string item, itemAttach;
		EntityAI itemEnt, itemIn;
		ItemBase itemBs;

		 
		if ( itemsArray.Count() > 0 ) 
		{
				 
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
	
	void SetPlayerInventory(string item = "", float qty = 0, TStringArray attach = NULL) 
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
	
	void SetPosition(TVectorArray positions)
	{	
		int index = Math.RandomInt( 0, positions.Count() );
		position = positions.Get( index );
	}
	
	void SetRandomHealth(EntityAI itm)
	{
		int rndHlt = Math.RandomInt(40,100);
		itm.SetHealth("","",rndHlt);
	}
	
	
	float GetItemQuantity( InventoryItem item )
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
	
	
	string LoadPlayerQuickBar()
	{	
		return ReadFile(BASE_PLAYERDIR + QB_FILE);
	}
	
	
	string LoadPlayerHands()
	{	
		return ReadFile(BASE_PLAYERDIR + HND_FILE);
	}
	
	
	TStringArray LoadPlayerInventory()
	{	
		return ReadFileLn(BASE_PLAYERDIR + INV_FILE);
	}
	
	
	string LoadPlayerCharacter()
	{	
		return ReadFile(BASE_PLAYERDIR + CHAR_FILE);
	}

	
	void CreateDir(string directory = "")
	{
		if ( !FileExist( directory ) ) 
		{
			FileHandle dir = OpenFile( directory, FileMode.READ );
			MakeDirectory( directory );
			CloseFile( dir );
		}
	}
	
	
	void WriteFile(string data) 
	{	
		FileHandle file = OpenFile( SaveFile, FileMode.WRITE );
		FPrint(file, data);
		CloseFile(file);
	}

	string ReadFile(string filename = "" ) 
	{
		string FileContent = "";
		
		if ( FileExist(filename) )
		{
			FileHandle file = OpenFile( filename, FileMode.READ );
			FGets(file, FileContent);
			CloseFile(file);
			FileContent.Trim();
		}	
	
		return FileContent;
	}
	
	
	TStringArray ReadFileLn(string filename = "") 
	{
		 TStringArray FileContentArray = new TStringArray;
		 
		string FileContent = "";
		
		if ( FileExist(filename) )
		{
			FileHandle file = OpenFile( filename, FileMode.READ );
			
			int count = FGets( file, FileContent );
			
			while ( count != -1 )
			{
				FileContent.Trim();
				
				FileContentArray.Insert(FileContent);
				
				count = FGets( file,  FileContent );
				// Print(FileContent);
			}
		
			// Print(FileContentArray.Debug());
			
			CloseFile(file);
		}	
	
		return FileContentArray;
	}
	
	
	void DeletePlayer()
	{

		DelFile(BASE_DIR + HND_FILE);
		DelFile(BASE_DIR + CHAR_FILE);
		DelFile(BASE_DIR + INV_FILE);
	}
	
	
	void DelFile(string filename)
	{
		if ( FileExist(filename) )
		{
			DeleteFile(filename);
		}
	}
	

}