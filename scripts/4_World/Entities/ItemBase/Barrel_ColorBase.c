class Barrel_ColorBase : Container_Base
{
	private bool m_IsLocked = false;
	private ref Timer m_BarrelOpener;
	ref RainProcurementManager m_RainProcurement;
	
	void Barrel_ColorBase()
	{
		m_Opened = false;
		m_BarrelOpener = new Timer();	
	}
	
	bool IsLocked()
	{
		return m_IsLocked;
	}

	bool IsOpened()
	{
		if ( GetAnimationPhase("Lid") == 1 )
		{
			return true;
		}
		
		return false;
	}
	
	override void Open()
	{
		super.Open();
		SetAnimationPhase("Lid",1);
		SetAnimationPhase("Lid2",0);	
		
		m_RainProcurement = new RainProcurementManager( this );
		m_RainProcurement.InitRainProcurement();
	}
	
	void Lock(float actiontime)
	{
		m_IsLocked = true;
		m_BarrelOpener.Run(actiontime, this, "Unlock", NULL,false);
	}
	
	void Unlock()
	{
		m_IsLocked = false;
		Open();
	}
	
	override void Close()
	{
		super.Close();
		SetAnimationPhase("Lid",0);
		SetAnimationPhase("Lid2",1);
		
		m_RainProcurement.StopRainProcurement();
	}

	void DetermineAction ( PlayerBase player )
	{
		int slot_id;
		ItemBase ingredient; 
		slot_id = InventorySlots.GetSlotIdFromString("Lime");
		ingredient = ItemBase.Cast( GetInventory().FindAttachment(slot_id) ); 
		if ( ingredient && GetQuantity() > 10000 ) 
		{
			TanPelts(ingredient, player);
		}
		else
		{
			slot_id = InventorySlots.GetSlotIdFromString("Nails");
			ingredient = ItemBase.Cast( GetInventory().FindAttachment(slot_id) ); 
			if ( ingredient && GetQuantity() > 10000 ) 
			{
				ColourLeatherClothes(ingredient, player, "Black");
			}
			else
			{
				slot_id = InventorySlots.GetSlotIdFromString("BirchBark");
				ingredient = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );
				if ( ingredient && GetQuantity() > 10000 ) 
				{
					ColourLeatherClothes(ingredient, player, "Beige");
				}
				else
				{
					slot_id = InventorySlots.GetSlotIdFromString("OakBark");
					ingredient = ItemBase.Cast( GetInventory().FindAttachment(slot_id) ); 
					if ( ingredient && GetQuantity() > 10000 ) 
					{
						ColourLeatherClothes(ingredient, player, "Brown");	
					}
					else
					{
						
						slot_id = InventorySlots.GetSlotIdFromString("Disinfectant");
						ingredient = ItemBase.Cast( GetInventory().FindAttachment(slot_id) ); 
						if ( ingredient && GetQuantity() > 10000 )
						{
							BleachClothes(ingredient, player);
						}
						else
						{
							slot_id = InventorySlots.GetSlotIdFromString("Plant");
							ingredient = ItemBase.Cast( GetInventory().FindAttachment(slot_id) ); 
							slot_id = InventorySlots.GetSlotIdFromString("Guts");
							ItemBase reactant = ItemBase.Cast( GetInventory().FindAttachment(slot_id) ); 
							if ( ingredient && reactant && GetQuantity() > 5000 )
							{
								ProduceFertilizer(ingredient,reactant,player);
							}
							else
							{
								slot_id = InventorySlots.GetSlotIdFromString("BerryR");
								ItemBase ingredientR = ItemBase.Cast( GetInventory().FindAttachment(slot_id) ); 
								slot_id = InventorySlots.GetSlotIdFromString("Plant");
								ItemBase ingredientG = ItemBase.Cast( GetInventory().FindAttachment(slot_id) );
								slot_id = InventorySlots.GetSlotIdFromString("BerryB");
								ItemBase ingredientB = ItemBase.Cast( GetInventory().FindAttachment(slot_id) ); 
								if ( (ingredientR || ingredientG || ingredientB) && GetQuantity() > 10000) 
								{
									ColourClothes(ingredientR,ingredientG,ingredientB,player);	
								}
							}
						}
					}
				}
			}
		}
		Close();
	}
	
	void ProduceFertilizer( ItemBase guts, ItemBase plant, PlayerBase player )
	{	
		float lime_quantity = plant.GetQuantity()*BAREL_LIME_PER_PLANT;
		
		guts.Delete();
		plant.Delete();
			
		ItemBase lime = ItemBase.Cast( GetInventory().CreateInInventory("GardenLime") );

		lime.SetQuantity(lime_quantity);
		Lock(50);
	}
	
	void TanPelts( ItemBase lime, PlayerBase player )
	{
		EntityAI item;		
		int item_count = GetInventory().GetCargo().GetItemCount();
		int pelt_count = 0;
		int lime_amount = Math.Floor(lime.GetQuantity()/BAREL_LIME_PER_PELT);
		
			
		for (int i = 0; i < item_count; i++)
		{
			item = GetInventory().GetCargo().GetItem(i);
			if ( item.IsPeltBase() )
			{
				pelt_count = g_Game.ConfigGetInt("cfgVehicles " + item.GetType() + " peltGain");
				if ( pelt_count <= lime_amount )
				{
					TanLeatherLambda lambda = new TanLeatherLambda(item, "TannedLeather", player, pelt_count);
					lambda.SetTransferParams(true, false, true);
					player.LocalReplaceItemWithNew(lambda);
										
					lime_amount -= pelt_count;
					if ( lime_amount <= 0 )
					{
						lime.Delete();
						break;
					}
				}
			}
		}	
		if ( lime ) 
		{
			lime.SetQuantity(lime_amount*BAREL_LIME_PER_PELT);		
		}
		if ( pelt_count > 0 ) 
		{
			Lock(30);
		}
	}
	
	void ColourLeatherClothes( ItemBase color_source, PlayerBase player, string color )
	{
		EntityAI item;		
		int item_count = GetInventory().GetCargo().GetItemCount();
		string item_name = "";
		
		
		for (int i = 0; i < item_count; i++)
		{
			item = GetInventory().GetCargo().GetItem(i);
			if ( item.IsClothing() )
			{	
				if ( GetGame().ObjectIsKindOf (item, "LeatherSack_Natural") ) 
				{
					item_name = "Bag_LeatherSack_"+color;
				}
				else if ( GetGame().ObjectIsKindOf (item, "LeatherStorageVest_Natural") )
				{
					item_name = "Vest_LeatherStorage_"+color;
				}
				else if ( GetGame().ObjectIsKindOf (item, "LeatherJacket_Natural") ) 
				{
					item_name = "Top_LeatherJacket_"+color;
				}
				else if ( GetGame().ObjectIsKindOf (item, "LeatherPants_Natural") ) 
				{
					item_name = "Pants_LeatherPants_"+color;
				}
				else if ( GetGame().ObjectIsKindOf (item, "LeatherMoccasinsShoes_Natural") ) 
				{
					item_name = "Shoes_LeatherMoccasins_"+color;
				}
				else if ( GetGame().ObjectIsKindOf (item, "LeatherHat_Natural") ) 
				{
					item_name = "Hat_Leather_"+color;
				}
				if ( item_name != "" )
				{
					TurnItemIntoItemLambda lambda = new TurnItemIntoItemLambda(item, item_name, player);
					lambda.SetTransferParams(true, false, true);
					player.LocalReplaceItemWithNew(lambda);
				}				
			}	
		}	
		if ( item_name != "" ) 
		{
			Lock(20);	
		}
	}
	
	
	void BleachClothes( ItemBase bleach, PlayerBase player )
	{
		EntityAI item;	
		int bleach_amount = bleach.GetQuantity();
		int item_count = GetInventory().GetCargo().GetItemCount();
		string item_name = "";
		
		for (int i = 0; i < item_count; i++)
		{
			item = GetInventory().GetCargo().GetItem(i);
			if ( item.IsClothing() )
			{	
				if ( bleach_amount >= BAREL_BLEACH_PER_CLOTH )
				{
					if ( ( GetGame().ObjectIsKindOf (item, "TShirt_ColorBase") && !GetGame().ObjectIsKindOf (item, "TShirt_White") ) || ( GetGame().ObjectIsKindOf (item, "Armband_ColorBase") && !GetGame().ObjectIsKindOf (item, "Armband_White") ) ) //cannot bleach white items...?
					{	
						int index = item.GetType().IndexOf("_");
						string itemtype = item.GetType().Substring( 0, index + 1 );
						ItemBase itemIB = ItemBase.Cast(item);
						MiscGameplayFunctions.TurnItemIntoItem(itemIB,itemtype+"White",player);
						
						bleach_amount -= BAREL_BLEACH_PER_CLOTH;
					}
					if ( GetGame().IsKindOf (item.GetDisplayName(), "LeatherSack_ColorBase") ) 
					{
						item_name = "LeatherSack_Natural";
					}
					else if ( GetGame().IsKindOf (item.GetDisplayName(), "LeatherStorageVest_ColorBase") ) 
					{
						item_name = "LeatherStorageVest_Natural";
					}
					else if ( GetGame().IsKindOf (item.GetDisplayName(), "LeatherJacket_ColorBase") )
					{
						item_name = "LeatherJacket_Natural";
					}
					else if ( GetGame().IsKindOf (item.GetDisplayName(), "LeatherPants_ColorBase") ) 
					{
						item_name = "LeatherPants_Natural";
					}
					else if ( GetGame().IsKindOf (item.GetDisplayName(), "LeatherMoccasinsShoes_ColorBase") ) 
					{
						item_name = "LeatherMoccasinsShoes_Natural";
					}
					else if ( GetGame().IsKindOf (item.GetDisplayName(), "LeatherHat_ColorBase") ) 
					{
						item_name = "LeatherHat_Natural";
					}
					if ( item_name != "" )
					{
						TurnItemIntoItemLambda lambda = new TurnItemIntoItemLambda(item, item_name, player);
						lambda.SetTransferParams(true, false, true);
						player.LocalReplaceItemWithNew(lambda);
						bleach_amount -= BAREL_BLEACH_PER_CLOTH;
					}
				}
				else
				{
					bleach.Delete();
					break;
				}
			}	
		}	
		if ( bleach )
		{
			bleach.SetQuantity(bleach_amount);		
		}
		if ( item_name != "" ) 
		{
			Lock(10);
		}
	}		
	
	void ColourClothes( ItemBase rci, ItemBase gci, ItemBase bci, PlayerBase player )
	{
		EntityAI item;		
		float r = 0;
		float g = 0;
		float b = 0;
		float a = 1; //alpha
		int item_count = GetInventory().GetCargo().GetItemCount();
		bool was_colored = false;
		if ( rci ) 
		{
			r = rci.GetQuantity()/10; 
			r = Math.Clamp(r, 0, 0.78); //TODO adjust clamp as needed, full RGB would result in a white item
		}
		if ( gci )
		{ 
			g = gci.GetQuantity()/10;
			g = Math.Clamp(g, 0, 0.78);
		}
		if ( bci ) 
		{
			b = bci.GetQuantity()/10; 
			b = Math.Clamp(b, 0, 0.78);
		}
		for (int i = 0; i < item_count; i++)
		{
			item = GetInventory().GetCargo().GetItem(i);
			if ( item.IsClothing() )
			{
				if ( GetGame().ObjectIsKindOf (item, "TShirt_White") || GetGame().ObjectIsKindOf (item, "Armband_White") )
				{
					ItemBase itemIB = ItemBase.Cast(item);
					int index = itemIB.GetType().IndexOf("_");
					string itemtype = itemIB.GetType().Substring( 0, index + 1 );
					
					ColourClothesLambda clambda = new ColourClothesLambda(itemIB, itemtype + "Dyed", player, r, g, b, a);
					clambda.SetTransferParams(true, false, true);
					MiscGameplayFunctions.TurnItemIntoItemEx(player, clambda);
					was_colored = true;
				}
			}	
		}	
		if ( was_colored ) 
		{
			if ( rci ) 
			{
				//rci.AddQuantity(-r*10,true,false);
				rci.Delete();
			}
			if ( gci ) 
			{
				//gci.AddQuantity(-g*10,true,false);
				gci.Delete();
			}
			if ( bci ) 
			{
				//bci.AddQuantity(-b*10,true,false);
				bci.Delete();
			}
			Lock(30);
		}
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}		
		if ( GetNumberOfItems() == 0 && !IsOpened() )
		{
			return true;
		}
		return false;
	}
	
	override bool CanPutIntoHands(EntityAI parent)
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		if ( GetNumberOfItems() == 0 && !IsOpened() )
		{
			return true;
		}
		return false;

	}
	
	override bool CanReceiveItemIntoCargo(EntityAI cargo)
	{
		if ( IsOpened() )
		{
			return true;
		}
		return false;
	}

    override bool CanRemoveFromCargo(EntityAI cargo)
	{
		if ( IsOpened() )
		{
			return true;
		}
		return false;
	}
};

class ColourClothesLambda : TurnItemIntoItemLambda
{
	int r, g, b, a;
	void ColourClothesLambda (EntityAI old_item, string new_item_type, PlayerBase player, int rr, int gg, int bb, int aa) { r = rr; g = gg; b = bb; a = aa; }

	override void CopyOldPropertiesToNew (notnull EntityAI old_item, notnull EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);

		Print("ColourClothesLambda::OnCreated object=" + new_item);
		ItemBase ib = ItemBase.Cast(new_item);
		if ( new_item ) 
		{	
			string save_color = r.ToString()+","+g.ToString()+","+b.ToString()+","+a.ToString();
			string color = "#(argb,8,8,3)color("+save_color+",CO)";
			
			array<string> config_selections	= new array<string>; //TODO correct use of *new*?
			string cfg_hidden_selections = "CfgVehicles" + " " + new_item.GetType() + " " + "hiddenSelections";
			GetGame().ConfigGetTextArray ( cfg_hidden_selections, config_selections );
			for (int j = 0; j < config_selections.Count(); j++)
			{
				if (config_selections.Get(j) != "personality") 	new_item.SetObjectTexture(j, color);
			}
			//nitem.SetItemVariableString("varColor", color); //SYNCFAIL
			ib.SetColor(r*255,g*255,b*255,a*255);
		}
	}
};

class TanLeatherLambda : TurnItemIntoItemLambda
{
	int m_PeltCount;

	void TanLeatherLambda (EntityAI old_item, string new_item_type, PlayerBase player, int pelt_count) { m_PeltCount = pelt_count; }

	override void CopyOldPropertiesToNew (notnull EntityAI old_item, notnull EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);

		if (new_item) 
		{							
			ItemBase tanned_pelt = ItemBase.Cast( new_item );
			tanned_pelt.SetQuantity(m_PeltCount);
		}
		else
		{
			Debug.LogError("TanLeatherLambda: failed to create new item","static");
		}
	}
};