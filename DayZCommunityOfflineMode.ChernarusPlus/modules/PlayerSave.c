class PlayerSave 
{
	
	private string BASE_DIR = "$saves:PlayerSave";
	private string CHAR_FILE = "//character.json";
	private string INV_FILE = "//inventory.json";
	private string HND_FILE = "//hand.json";
	private string QB_FILE = "//quickbar.json";
	
	private ref JsonSerializer js = new JsonSerializer;
	private string SaveFile;
	private string Data = "";
	private PlayerBase player;


	void PlayerSave()
	{
		player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		CreateDir();
	}
	
	void ~PlayerSave()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.ProcessSaves);
	}
	
	void Run(int sec = 60)
	{
		int seconds = sec * 1000;
		
		player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		ProcessSaves();
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.ProcessSaves, seconds, true);
		
	}

	
	void ProcessSaves()
	{
		if ( !player ) return;
		
	
		if ( player.GetPlayerState() == EPlayerStates.ALIVE && !player.IsRestrained() )
		{
			SaveMyPlayer();
			SaveMyQuickBar();
			SaveMyInventory();
			
		} else {
			
			DeletePlayer();
		}
	}
	

	
	void SaveMyPlayer()
	{	
		Data = "";
		SaveFile = BASE_DIR + CHAR_FILE;
		
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
	
	
	void SaveMyQuickBar()
	{
		Data = "";
		SaveFile = BASE_DIR + QB_FILE;
		
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
	
	
	
	void SaveMyHand() 
	{
		
		SaveFile = BASE_DIR + HND_FILE;
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
			
			DelFile(BASE_DIR + HND_FILE);
		}
	}
	
	
	
	void SaveMyInventory() 
	{
		
		SaveFile = BASE_DIR + INV_FILE;
		
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
		
			SaveMyHand(); 
	}
	

	string LoadMyQuickBar()
	{	
		SaveFile = BASE_DIR + QB_FILE;
		
		return ReadFile();
	}
	
	string LoadMyHand()
	{	
		SaveFile = BASE_DIR + HND_FILE;
		
		return ReadFile();
	}
	
	TStringArray LoadMyInventory()
	{	
		SaveFile = BASE_DIR + INV_FILE;
		
		return ReadFileLn();
	}
	
	string LoadMyPlayer()
	{	
		SaveFile = BASE_DIR + CHAR_FILE;
		
		return ReadFile();
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
	
	
	void CreateDir()
	{
		if ( !FileExist(BASE_DIR) ) 
		{
			FileHandle dir = OpenFile( BASE_DIR, FileMode.READ );
			if (dir == 0) {
				MakeDirectory(BASE_DIR);
			}
		}
	}
	
	
	void WriteFile(string data) 
	{	
		FileHandle file = OpenFile( SaveFile, FileMode.WRITE );
		FPrint(file, data);
		CloseFile(file);
	}

	string ReadFile() 
	{
		string FileContent = "";
		
		if ( FileExist(SaveFile) )
		{
			FileHandle file = OpenFile( SaveFile, FileMode.READ );
			FGets(file, FileContent);
			CloseFile(file);
		}	
	
		return FileContent;
	}
	
	
	TStringArray ReadFileLn() 
	{
		 TStringArray FileContentArray = new TStringArray;
		 
		string FileContent = "";
		
		if ( FileExist(SaveFile) )
		{
			FileHandle file = OpenFile( SaveFile, FileMode.READ );
			
			int count = FGets( file, FileContent );
			
			while ( count != -1 )
			{
				
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
	
	void DelFile(string file)
	{

		if ( FileExist(file) )
		{
			DeleteFile(file);
		}
	}

}