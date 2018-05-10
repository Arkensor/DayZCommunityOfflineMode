class SaveManager 
{
	PlayerBase player;
	protected CommunityOfflineMode m_Mission;
	
	protected const string BASE_DIR = "$saves:CommunityOfflineMode//"; // Default[windows]: /Documents/DayZ/CommunityOfflineMode
	protected const string BASE_PLAYERDIR = SaveManager.BASE_DIR + "//PlayerSaves";
	protected const string CHAR_FILE = "//character.json";
	protected const string INV_FILE = "//inventory.json";
	protected const string HND_FILE = "//hands.json";
	protected const string QB_FILE = "//quickbar.json";
	protected const string TEST_FILE1 = "//test1.json";
	protected const string TEST_FILE2 = "//test2.json";

	protected ref JsonSerializer js = new JsonSerializer;	
	protected string SaveFile;
	protected string Data = "";
	protected vector position = "0 0 0";
	protected int timer;
	protected ref TStringArray itemsArray = new TStringArray;
	protected ref TIntArray itemsQtyArray = new TIntArray;
	protected ref map<string, ref TStringArray> itemsAttachArray = new map<string, ref TStringArray>;
	

	
	
	void SaveManager(CommunityOfflineMode m_Mission , int t = 30)
	{
		timer = t;
		
		player = m_Mission.m_oPlayer;
		
		this.CreateDir(BASE_PLAYERDIR);
		
	}
	
	
	
	void ~SaveManager()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.ProcessPlayerSaves);
		
		Data = "";
		
		delete js;
		
		delete itemsArray;
		
		delete itemsQtyArray;
		
		delete itemsAttachArray;
		
	}
	
	
	
	void ProcessPlayerSaves()
	{
		if ( !player ) return;
		
		// Print(player.GetPlayerState());
	
		if ( player.GetPlayerState() == 0  )
		{
			this.SavePlayerCharacter();
			
			this.SavePlayerInventory();
			
			this.SavePlayerHands();
			
			this.SavePlayerQuickBar();
			
		} else {
			
			this.DeletePlayer();
		}
	}
	
	
	
	PlayerBase SpawnPlayer() 
	{
	
		if ( CreatePlayerCharacterFromSave() ) 
		{
		 // this.CreatePlayerInventory(); 
		 this.CreatePlayerInventoryFromSave();
		
		} else {
			
			player = PlayerBase.Cast( GetGame().CreatePlayer( NULL, GetGame().CreateRandomPlayer(), position, 0, "NONE") );
			
			this.CreatePlayerInventory(); 
		}
		
		
		if ( player ) 
		{
			
			GetGame().SelectPlayer(NULL, player);
			
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.ProcessPlayerSaves, timer * 1000, true);
			
			return player;
		}
		
		return NULL;
	}

	
	
	string GetCargoItemsAsJson(EntityAI entity)
	{
		
		string json = "";
		
		ref map<string, ref map<string,string>> tempArray = new map<string, ref map<string,string>>;
	
		Cargo cargo = entity.GetInventory().GetCargo();
		
			if ( cargo  )
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
							
							string qty = this.GetItemQuantity( itemInCargo ).ToString();
							tempArray.Get( itemName ).Insert( "qty", qty );
							
							string wet = itemInCargo.GetWet().ToString();
							tempArray.Get( itemInCargo.GetType() ).Insert( "wet", wet ); 
							
							string attJson = this.GetAttachedItemsAsJson( itemInCargo );
							tempArray.Get( itemName ).Insert( "attachments", attJson ); 
							
							string cargoJson = this.GetCargoItemsAsJson( itemInCargo );
							tempArray.Get( itemName ).Insert( "cargo", cargoJson );
							
							int pos_x, pos_y;
					
							entity.GetInventory().GetCargo().GetItemPos(i, pos_x, pos_y );
							
							string x = pos_x.ToString();
							tempArray.Get( itemName ).Insert( "x", x );
							
							string y = pos_y.ToString();
							tempArray.Get( itemName ).Insert( "y", y );	
							
						}
					}
				}
				
			}

		js.WriteToString(tempArray, false, json);
		
		delete tempArray;
		
		return json;
	}
	
	
	
	string GetAttachedItemsAsJson(EntityAI entity)
	{
		ref map<string, ref map<string,string>> tempArray = new map<string, ref map<string,string>>;
		
		string json = "";
		
		for (int att = 0; att < entity.GetInventory().AttachmentCount(); att++)
		{
			
			EntityAI attached = entity.GetInventory().GetAttachmentFromIndex(att);
			
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

						string qty = this.GetItemQuantity( item ).ToString();
						tempArray.Get( itemName ).Insert( "qty", qty );

						string wet = item.GetWet().ToString();
						tempArray.Get( item.GetType() ).Insert( "wet", wet ); 

						string attJson = this.GetAttachedItemsAsJson( item );
						tempArray.Get( itemName ).Insert( "attachments", attJson ); 

						string cargoJson = this.GetCargoItemsAsJson( item );
						tempArray.Get( itemName ).Insert( "cargo", cargoJson );
					
					}
				}
			}
		}
		
		
		
		js.WriteToString(tempArray, false, json);
		
		delete tempArray;
		
		return json;
	}
	
	
	
	void SavePlayerInventory()
	{
		
		string json = "";
		
		ref array<string> DataArray = new array<string>;
		
		ref map<string,ref map<string,string>> PlayerInventory =  new map<string, ref map<string,string>>;
		
		array<EntityAI> playerArray = new array<EntityAI>;
		
		player.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, playerArray);
		
		ItemBase playerHands = player.GetHumanInventory().GetEntityInHands();
		
		for (int p = 0; p  < playerArray.Count(); p ++)
		{	
			ItemBase itemOnPlayer;
			
			Class.CastTo( itemOnPlayer, playerArray.Get(p) );
			
			if ( itemOnPlayer == NULL || itemOnPlayer == playerHands ) continue;
			
			string typeName = itemOnPlayer.GetType();
			PlayerInventory.Insert( typeName, new map<string, string> );
			
			string health = itemOnPlayer.GetHealth("","").ToString();
			PlayerInventory.Get( typeName ).Insert( "health", health );
			
			string qty = this.GetItemQuantity(itemOnPlayer).ToString();
 			PlayerInventory.Get( typeName ).Insert( "qty", qty ); 
			
			string wet = itemOnPlayer.GetWet().ToString();
			PlayerInventory.Get( typeName ).Insert( "wet", wet ); 
			
			string attJson = this.GetAttachedItemsAsJson( itemOnPlayer );
			PlayerInventory.Get( typeName ).Insert( "attachments", attJson ); 
			
			string cargoJson = this.GetCargoItemsAsJson( itemOnPlayer );
			PlayerInventory.Get( typeName ).Insert( "cargo", cargoJson );
			
			js.WriteToString(PlayerInventory, false, json);	
			
			DataArray.Insert(json);
			
			PlayerInventory.Clear();
		}
		
		
		delete playerArray;
		
		delete PlayerInventory;
		
		delete PlayerInventory;
		
		SaveFile = BASE_PLAYERDIR + INV_FILE;
		
		WriteFileLn(DataArray);
		
		delete DataArray;
	}
	

	
	void SavePlayerCharacter()
	{
		Data = "";
		
		map<string,string>tempArray = new map<string,string>;
		
		string model = player.GetType();
		tempArray.Insert( "model", model );
		
		string health = player.GetHealth("","").ToString();
		tempArray.Insert( "health", health ); 
		
		string blood = player.GetHealth("GlobalHealth", "Blood").ToString();
		tempArray.Insert( "blood", blood );
		
		string bloodtype = player.GetStatBloodType().Get().ToString();
		tempArray.Insert( "bloodtype", bloodtype );
		
		string temperature = player.GetStatTemperature().Get().ToString();
		tempArray.Insert( "temperature", temperature );
		
		string energy = player.GetStatEnergy().Get().ToString();
		tempArray.Insert( "energy", energy );
		
		string water = player.GetStatWater().Get().ToString();
		tempArray.Insert( "water", water );
		
		string heatcomfort = player.GetStatHeatComfort().Get().ToString();
		tempArray.Insert( "heatcomfort", heatcomfort );
		
		string stomachenergy = player.GetStatStomachEnergy().Get().ToString();
		tempArray.Insert( "stomachenergy", stomachenergy );
		
		string stomachwater = player.GetStatStomachWater().Get().ToString();
		tempArray.Insert( "stomachwater", stomachwater );
		
		string stomachsolid = player.GetStatStomachSolid().Get().ToString();
		tempArray.Insert( "stomachsolid", stomachsolid );
		
		string wet = player.GetStatWet().Get().ToString();
		tempArray.Insert( "wet", wet );
		
		string shock = player.GetStatShock().Get().ToString();
		tempArray.Insert( "shock", shock );
		
		string pos = player.GetPosition().ToString();
		
		pos.Replace("<", "");
		pos.Replace(">", "");
		pos.Replace(",", "");
		
		tempArray.Insert( "pos", pos );
		
		string dir = player.GetDirection().ToString();
		
		dir.Replace("<", "");
		dir.Replace(">", "");
		dir.Replace(",", "");
		
		tempArray.Insert( "dir", dir );
		
		js.WriteToString(tempArray, false, Data);
		
		delete tempArray;
		
		SaveFile = BASE_PLAYERDIR + CHAR_FILE;
		
		WriteFile(Data);
	}
	
	
	
	void SavePlayerQuickBar()
	{
		Data = "";
		
		TStringArray tempArray = new TStringArray;
		
		int size = player.GetQuickBarSize();
		
		for ( int i = 0; i < size; i++)
		{
			InventoryItem item = InventoryItem.Cast( player.GetQuickBarEntity(i) );
			
			if( item != NULL ) 
			{
				tempArray.Insert( item.GetType() );
			}
		}
		
		js.WriteToString(tempArray, false, Data);
		
		delete tempArray;
		
		SaveFile = BASE_PLAYERDIR + QB_FILE;
		
		WriteFile(Data);
	}
	
	
	
	void SavePlayerHands()
	{
		
		SaveFile = BASE_PLAYERDIR + HND_FILE;
		
		string Data = "";
		
		ItemBase hands = player.GetHumanInventory().GetEntityInHands();
	
		if ( hands != NULL )
		{
			TStringArray attachmentArray = new TStringArray;
			
			map<string,string>itemArray = new map<string,string>;
			
			string typName = hands.GetType();
			itemArray.Insert( "type", typName );
			
			string health = hands.GetHealth("","").ToString();
			itemArray.Insert( "health", health );
			
			string qty = GetItemQuantity(hands).ToString();
			itemArray.Insert( "qty", qty );
			
			string wet = hands.GetWet().ToString();
			itemArray.Insert( "wet", wet ); 
			
			string b = "";	
			
			for (int j = 0; j < hands.GetInventory().AttachmentCount(); j++)
			{		
				ItemBase attachment;
				
				attachment = hands.GetInventory().GetAttachmentFromIndex(j);
				
				attachmentArray.Insert(attachment.GetType());
			}
			
			js.WriteToString(attachmentArray, false, b);
			
			itemArray.Insert( "attachments", b );
			
			js.WriteToString(itemArray, false, Data);
			
			WriteFile(Data);
			
			delete attachmentArray;
			
			delete itemArray;
		} else {
			
			DelFile(SaveFile);
			
		}
	
	}
	
	
	
	void CreatePlayerInventoryFromSave(ref TStringArray data = NULL, ref EntityAI entity = NULL, string createAs = "" )
	{
		
		if ( data == NULL )
		{	
			data = this.LoadPlayerInventory();
		}
		
		if ( entity == NULL ) 
		{
			entity = player;
		}
		
		
		for (int i = 0; i < data.Count(); i++)
		{
			map<string,map<string,string>> arrayFromJson = new map<string,map<string,string>>;
			
			string jsonError;
			
			string jsonArr = data.Get(i);
			
			// Print("JSON: " + jsonArr );
			
			bool ok = js.ReadFromString( arrayFromJson, jsonArr, jsonError );
			
			if(!ok)
			{
				Print(jsonError);
				return;
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
				
					if( qty != 0) 
					{
						if ( itemEnt.IsInherited(Magazine) )
						{
							Magazine magazine = Magazine.Cast( itemEnt );
							
							magazine.LocalSetAmmoCount(qty);
							
						} else {
							itemBs.SetQuantity(qty);
						}
					}
			 
			 
					TStringArray temp;
					
					string cargo = itemArray.Get("cargo");
				
					if ( cargo != "{}" && cargo != "" )
					{
						temp = new TStringArray;
						
						temp.Insert(cargo);
						
						this.CreatePlayerInventoryFromSave(temp, itemBs, "cargo");
						
					}
				
					string attachments = itemArray.Get("attachments");
				
					if ( attachments != "{}" && attachments != "" )
					{	
						temp = new TStringArray;
						
						temp.Insert(attachments);
						
						this.CreatePlayerInventoryFromSave(temp, itemBs, "att");
	
					}
				
					this.CreatePlayerQuickBarFromSave( itemEnt );
				}
			}
		}
		
		delete arrayFromJson;
		delete data;
	}
	
	

	bool CreatePlayerCharacterFromSave()
	{
		
		map<string,string>arrayFromJson = new map<string,string>;
		
		string data = this.LoadPlayerCharacter();
		
		if ( data != "" )
		{
			string json;
			string jsonError;
			bool ok = js.ReadFromString( arrayFromJson, data, jsonError );
			
			if(!ok)
			{
				Print(jsonError);
			}
		
		
			player = PlayerBase.Cast( GetGame().CreatePlayer( NULL, arrayFromJson.Get("model"), "0 0 0", 0, "NONE") );
			
			vector pos = arrayFromJson.Get("pos").ToVector();
			player.SetPosition( pos );
			
			vector dir = arrayFromJson.Get("dir").ToVector();
			player.SetDirection( dir );
			
			float health = arrayFromJson.Get("health").ToFloat();
			player.SetHealth("","", health );
			
			float blood = arrayFromJson.Get("blood").ToFloat();
			player.SetHealth("GlobalHealth", "Blood", blood );
			
			float bloodtype = arrayFromJson.Get("bloodtype").ToFloat();
			player.GetStatBloodType().Set( bloodtype );
			
			float temperature = arrayFromJson.Get("temperature").ToFloat();
			player.GetStatTemperature().Set( temperature );
			
			float energy = arrayFromJson.Get("energy").ToFloat();
			player.GetStatEnergy().Set( energy );
			
			float water = arrayFromJson.Get("water").ToFloat();
			player.GetStatWater().Set( water );
			
			float stomachwater = arrayFromJson.Get("stomachwater").ToFloat();
			player.GetStatStomachWater().Set( stomachwater );
			
			float stomachenergy = arrayFromJson.Get("stomachenergy").ToFloat();
			player.GetStatStomachEnergy().Set( stomachenergy );
			
			float heatcomfort = arrayFromJson.Get("heatcomfort").ToFloat();
			player.GetStatHeatComfort().Set( heatcomfort );
			
			float stomachsolid = arrayFromJson.Get("stomachsolid").ToFloat();
			player.GetStatStomachSolid().Set( stomachsolid );
			
			float wet = arrayFromJson.Get("wet").ToFloat();
			player.GetStatWet().Set( wet );
			
			float shock = arrayFromJson.Get("shock").ToFloat();
			player.GetStatShock().Set( shock );
			
	
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
		
		string data = this.LoadPlayerHands();
		
		if ( data != "" )
		{
	
			bool ok = js.ReadFromString( arrayFromJson, data, jsonError );
			
			if(!ok)
			{
				Print(jsonError);
			}
			
			TStringArray arrayAttach = new TStringArray;	 
			
			string attachments = arrayFromJson.Get("attachments");
			js.ReadFromString( arrayAttach, attachments, jsonError );
			
			string type = arrayFromJson.Get("type");
			itemEnt = player.GetHumanInventory().CreateInHands( type );
			
			this.CreatePlayerQuickBarFromSave( itemEnt );
			
			itemBs = ItemBase.Cast( itemEnt );
			
			if (arrayAttach.Count() > 0 ) 
			{
				for (int i = 0; i < arrayAttach.Count(); i++)
				{	
					itemIn = itemEnt.GetInventory().CreateAttachment( arrayAttach.Get(i) );
				
					SetRandomHealth( itemIn );
				}
			}
			
			float health = arrayFromJson.Get("health").ToFloat();
			itemBs.SetHealth("","", health );
			
			float wet = arrayFromJson.Get("wet").ToFloat();
			itemBs.SetWet( wet );
			
			float qty = arrayFromJson.Get("qty").ToFloat();
			if ( itemEnt.IsInherited(Magazine) )
			{
				Magazine magazine = Magazine.Cast( itemEnt );
				
				magazine.LocalSetAmmoCount( qty );
				
			} else {
				
				itemBs.SetQuantity( qty );
			}
		
		}
		
	}
	
	
	
	void CreatePlayerQuickBarFromSave(EntityAI itemEnt)
	{
			TStringArray arrayFromJson = new TStringArray;
			
			string jsonError;
			
			string data = this.LoadPlayerQuickBar();
			
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

	 
	 
	void CreatePlayerInventory()
	{
		
		float qty;
		
		string item, itemAttach;
		
		EntityAI itemEnt, itemIn;
		
		ItemBase itemBs;
		
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
		return this.ReadFile(BASE_PLAYERDIR + QB_FILE);
	}
	
	
	
	string LoadPlayerHands()
	{	
		return this.ReadFile(BASE_PLAYERDIR + HND_FILE);
	}
	

	
	TStringArray LoadPlayerInventory()
	{	
		return this.ReadFileLn(BASE_PLAYERDIR + INV_FILE);
	}
	
	
	
	string LoadPlayerCharacter()
	{	
		return this.ReadFile(BASE_PLAYERDIR + CHAR_FILE);
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
	

	
	void WriteFileLn(array<string> data) 
	{	
		FileHandle file = OpenFile(SaveFile, FileMode.WRITE);  
		
		for (int i = 0; i  < data.Count(); i ++)
		{	 	
			FPrintln(file, data.Get(i) );   
		}
		
		CloseFile(file);
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
			}
			
			CloseFile(file);
		}	
		
		return FileContentArray;
	}
	
	
	
	void DeletePlayer()
	{

		this.DelFile(BASE_PLAYERDIR + CHAR_FILE);
		
		this.DelFile(BASE_PLAYERDIR + INV_FILE);
		
		this.DelFile(BASE_PLAYERDIR + HND_FILE);
		
		this.DelFile(BASE_PLAYERDIR + QB_FILE);
	}
	
	
	
	void DelFile(string filename)
	{
		if ( FileExist(filename) )
		{
			DeleteFile(filename);
		}
	}
}