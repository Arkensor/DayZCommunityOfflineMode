class SaveManager 
{
	
	private string BASE_DIR = "$saves:CommunityOfflineMode//"; // Default[windows]: /Documents/DayZ/CommunityOfflineMode
	private string BASE_PLAYERDIR = BASE_DIR + "//PlayerSaves";
	private string CHAR_FILE = "//character.json";
	private string INV_FILE = "//inventory.json";
	private string HND_FILE = "//hands.json";
	private string QB_FILE = "//quickbar.json";
	private string TEST_FILE1 = "//test1.json";
	private string TEST_FILE2 = "//test2.json";

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
		
		Data = "";
		
		delete js;
		
		delete itemsArray;
		
		delete itemsQtyArray;
		
		delete itemsAttachArray;
		
	}
	
	
	
	
	void ProcessPlayerSaves()
	{
		if ( !player ) return;
		
	
		if ( player.GetPlayerState() == EPlayerStates.ALIVE && !player.IsRestrained() )
		{
			SavePlayerCharacter();
			
			SavePlayerQuickBar();
			
			// OldSavePlayerInventory(); 
			
			NewSavePlayerInventory();
			 
		} else {
			
			DeletePlayer();
		}
	}
	
	
	
	PlayerBase SpawnPlayer() 
	{
				
		if ( CreatePlayerCharacterFromSave() ) 
		{
			//CreatePlayerInventory(); 
			
			 CreatePlayerInventoryFromSave();
			
		} else {
			
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

	
	
	void CreatePlayerInventoryFromSave(string data = "")
	{
		
		if ( data == "" )
		{
			data = LoadPlayerInventory();
		}
		
		//Print("JSON: " + data);
	
		map<string,map<string,string>> arrayFromJson = new map<string,map<string,string>>;
		
		string jsonError;
		
		bool ok = js.ReadFromString( arrayFromJson, data, jsonError );
		
		if(!ok)
		{
			Print(jsonError);
		}
		
		map<string,string> itemArray = new map<string,string>;
		
		string itemName, attachments, cargo;
		
		EntityAI itemEnt;
		
		ItemBase itemBs;
		
		
		for (int i = 0; i < arrayFromJson.Count(); i++)
		{
			
			itemName = arrayFromJson.GetKey(i);
			
			itemArray = arrayFromJson.GetElement(i);
			
			itemEnt = player.GetInventory().CreateInInventory(itemName);
			
			if (itemEnt != NULL ) 
			{
			
				itemBs = ItemBase.Cast(itemEnt);
				
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
			 
				cargo = itemArray.Get("cargo");
				
				if ( cargo != "{}" && cargo != "" )
				{
					this.CreatePlayerInventoryFromSave(cargo);
				}
				
				attachments = itemArray.Get("attachments");
				
				if ( attachments != "{}" && attachments != "" )
				{
					this.CreatePlayerInventoryFromSave(attachments);
				}
			
			}
		}
		
	}
	
	
	
	void NewSavePlayerInventory()
	{
		
		string Data = "";
		
		ref map<string,ref map<string,string>> PlayerInventory =  new map<string, ref map<string,string>>;
		
		array<EntityAI> playerArray = new array<EntityAI>;
		
		player.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, playerArray);
		
		ItemBase playerHands = player.GetHumanInventory().GetEntityInHands();
		
		for (int p = 0; p  < playerArray.Count(); p ++)
		{	
			ItemBase itemOnPlayer;
			
			Class.CastTo(itemOnPlayer, playerArray.Get(p));
			
			if ( itemOnPlayer == NULL || itemOnPlayer == playerHands ) continue;
			
			PlayerInventory.Insert( itemOnPlayer.GetType(), new map<string, string> );
			
			PlayerInventory.Get( itemOnPlayer.GetType() ).Insert( "health", itemOnPlayer.GetHealth("","").ToString() );
			
			PlayerInventory.Get( itemOnPlayer.GetType() ).Insert( "qty", this.GetItemQuantity(itemOnPlayer).ToString() ); 
			
			PlayerInventory.Get( itemOnPlayer.GetType() ).Insert( "wet", itemOnPlayer.GetWet().ToString() ); 
			
			PlayerInventory.Get( itemOnPlayer.GetType() ).Insert( "attachments", this.GetAttachedItemsAsJson( itemOnPlayer ) ); 
			
			PlayerInventory.Get( itemOnPlayer.GetType() ).Insert( "cargo", this.GetCargoItemsAsJson( itemOnPlayer ) ); 
		
		}
		
		js.WriteToString(PlayerInventory, false, Data);
		
		delete playerArray;
		
		delete PlayerInventory;
		
		SaveFile = BASE_PLAYERDIR + INV_FILE;
		
		WriteFile(Data);
	}
	
	
	
	string GetCargoItemsAsJson(EntityAI entity)
	{
		
		string json = "";
	
		map<string,map<string,string>> tempArray =  new map<string,map<string,string>>;
		
		for (int att = 0; att < player.GetInventory().AttachmentCount(); att++)
		{
			
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
							string itemName = itemInCargo.GetType();
							
							Print(itemName);
							
							tempArray.Insert(itemName, new map<string,string> );
							
							
							// ref map<string,string> tempArray2 = tempArray.Get( itemInCargo.GetType() );
							
							// int h = ;
							
							 Print("->" + itemInCargo + "->" + itemInCargo.GetHealth("",""));
							 Print("->" + itemInCargo + "->" + itemInCargo.GetWet());
							 Print("->" + itemInCargo + "->" + this.GetItemQuantity( itemInCargo ));
							
							 Print("CARGO->" + itemInCargo + "->" + this.GetCargoItemsAsJson( itemInCargo ));
							 
							 Print("ATTACH->" + itemInCargo + "->" + this.GetAttachedItemsAsJson( itemInCargo ));
							 							 
							 tempArray.Get( itemName ).Insert( "cargo", this.GetCargoItemsAsJson( itemInCargo ) );
							  
							// tempArray2.Insert( "health",  h.ToString());
							
							// tempArray.Get( itemInCargo.GetType() ).Insert( "qty", this.GetItemQuantity( itemInCargo ).ToString() );
							
							// tempArray.Get( itemInCargo.GetType() ).Insert( "wet", itemInCargo.GetWet().ToString() ); 
							
							// tempArray.Get( itemInCargo.GetType() ).Insert( "attachments", this.GetAttachedItemsAsJson( itemInCargo ) );
							
							// tempArray.Get( itemInCargo.GetType() ).Insert( "cargo", this.GetCargoItemsAsJson( itemInCargo ) );
							
						}
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
		ref map<string, ref map<string, ref map<string,string>>> tempArray =  new map<string, ref map<string, ref map<string, string>>>;
		
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
						
						tempArray.Insert( item.GetType(), new map<string, ref map<string, string>>);
						
					}
				}
			}
		}
		
		js.WriteToString(tempArray, false, json);
		
		delete tempArray;
		
		return json;
	}
	
	
	
	void SavePlayerCharacter()
	{
		Data = "";
		
		map<string,string>tempArray = new map<string,string>;
		
		tempArray.Insert( "model", player.GetType() );
		
		tempArray.Insert( "health", player.GetHealth("", "").ToString() ); 
		
		tempArray.Insert( "blood", player.GetHealth("GlobalHealth", "Blood").ToString() );
		
		tempArray.Insert( "temperature", player.GetStatTemperature().Get().ToString() );
		
		tempArray.Insert( "energy", player.GetStatEnergy().Get().ToString() );
		
		tempArray.Insert( "water", player.GetStatWater().Get().ToString() );
		
		tempArray.Insert( "heatcomfort", player.GetStatHeatComfort().Get().ToString() );
		
		tempArray.Insert( "stomachenergy", player.GetStatStomachEnergy().Get().ToString() );
		
		tempArray.Insert( "stomachwater", player.GetStatStomachWater().Get().ToString() );
		
		tempArray.Insert( "stomachsolid", player.GetStatStomachSolid().Get().ToString() );
		
		tempArray.Insert( "wet", player.GetStatWet().Get().ToString() );
		
		tempArray.Insert( "shock", player.GetStatShock().Get().ToString() );
		
		tempArray.Insert( "bloodtype", player.GetStatBloodType().Get().ToString() );
		
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
	
	
	
	void SavePlayerHand()
	{
		
		SaveFile = BASE_PLAYERDIR + HND_FILE;
		
		string Data = "";
		
		ItemBase hands = player.GetHumanInventory().GetEntityInHands();
	
		if ( hands != NULL )
		{
			TStringArray attachmentArray = new TStringArray;
			map<string,string>itemArray = new map<string,string>;
			
			itemArray.Insert( "type", hands.GetType() );
			
			itemArray.Insert( "health", hands.GetHealth("","").ToString() );
			
			itemArray.Insert( "qty", GetItemQuantity(hands).ToString() );
			
			itemArray.Insert( "wet", hands.GetWet().ToString() ); 
			
			string b = "";	
			
			for (int j = 0; j < hands.GetInventory().AttachmentCount(); j++)
			{		
				ItemBase attachment;
				attachment = hands.GetInventory().GetAttachmentFromIndex(j);
				attachmentArray.Insert(attachment.GetType());
			}
			
			js.WriteToString(attachmentArray, false, b);
			
			itemArray.Insert( "attachment", b );
			
			js.WriteToString(itemArray, false, Data);
			
			WriteFile(Data);
			
			delete attachmentArray;
			
			delete itemArray;
		} else {
			
			DelFile(SaveFile);
			
		}
	
	}
	
	
	
	void OldSavePlayerInventory()
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
				
				if ( item.GetHierarchyParent().IsWeapon() ) continue;
					
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
		
		string data = LoadPlayerCharacter();
		
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
/*

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
	*/ 
	 
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
		return ReadFile(BASE_PLAYERDIR + QB_FILE);
	}
	
	
	
	string LoadPlayerHands()
	{	
		return ReadFile(BASE_PLAYERDIR + HND_FILE);
	}
	

	
	string LoadPlayerInventory()
	{	
		return ReadFile(BASE_PLAYERDIR + INV_FILE);
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
			}
			
			CloseFile(file);
		}	
		
		delete FileContentArray;
		
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