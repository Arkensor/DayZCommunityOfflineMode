class TurnItemIntoItemLambda : ReplaceItemWithNewLambda
{
	bool m_TransferAgents;
	bool m_TransferVariables;
	bool m_TransferHealth;
	bool m_ExcludeQuantity;
	
	void TurnItemIntoItemLambda (EntityAI old_item, string new_item_type, PlayerBase player)
	{
		SetTransferParams();
	}
	
	void SetTransferParams (bool transfer_agents = true, bool transfer_variables = true, bool transfer_health = true, bool exclude_quantity = false)
	{
		m_TransferAgents = transfer_agents;
		m_TransferVariables = transfer_variables; 
		m_TransferHealth = transfer_health;
		m_ExcludeQuantity = exclude_quantity;
	}

	override void CopyOldPropertiesToNew (notnull EntityAI old_item, notnull EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);

		if (new_item) 
		{
			MiscGameplayFunctions.TransferItemProperties(old_item, new_item, m_TransferAgents, m_TransferVariables, m_TransferHealth, m_ExcludeQuantity);
			MiscGameplayFunctions.TransferAttachments(old_item, new_item, m_Player);
		}
		else
		{
			Debug.LogError("TurnItemIntoItemLambda: failed to create new item","static");
		}
	}
};

class MiscGameplayFunctions
{	
	//! will transform item' variables, agents and other local scripted properties as well as any relevant non-scripted properties like health
	static void TransferItemProperties(EntityAI source, notnull EntityAI target, bool transfer_agents = true, bool transfer_variables = true, bool transfer_health = true, bool exclude_quantity = false)
	{
		ItemBase target_ib = ItemBase.Cast(target);
		ItemBase source_ib = ItemBase.Cast(source);
		
		if( transfer_agents ) target_ib.TransferAgents( source.GetAgents() );
		
		if( transfer_variables )
		{
			MiscGameplayFunctions.TransferItemVariables(source_ib, target_ib, exclude_quantity);
		}

		if( transfer_health ) target_ib.SetHealth("", "", source.GetHealth("",""));
	}

	static void TransferItemVariables(ItemBase source, ItemBase target, bool exclude_quantity = false)
	{
		target.TransferVariablesFloat( source.GetVariablesFloat() );
		//target.TransferVariablesString( source.GetVariablesString() );
		if( exclude_quantity )
		{
			target.RemoveItemVariable(VARIABLE_QUANTITY);
		}
		else if( source.IsMagazine() && target.IsMagazine() )
		{
			Magazine source_mag = Magazine.Cast(source);
			Magazine target_mag = Magazine.Cast(target);
			
			target_mag.ServerSetAmmoCount( source_mag.GetAmmoCount() );
		}
	}

	static void TransferAttachments( EntityAI source, EntityAI target, PlayerBase player)
	{
		for(int i = source.GetInventory().AttachmentCount() - 1; i > -1; i--)
		{
			EntityAI attachment = source.GetInventory().GetAttachmentFromIndex(i);
			if( target.LocalTakeEntityAsAttachment(attachment) )
			{
				//pass
			}
			else
			{
				player.LocalDropEntity(attachment);
			}
		}
	}
	
	static void UnlimitedAmmoDebugCheck(Weapon_Base weapon)
	{
		if ((ItemBase.GetDebugActionsMask() & DebugActionType.UNLIMITED_AMMO) && GetGame().IsServer())
		{
			float ammoDamage;
			string ammoTypeName;
			int currentMuzzle = weapon.GetCurrentMuzzle();
			weapon.GetCartridgeInfo(currentMuzzle, ammoDamage, ammoTypeName);
			Magazine magazine = weapon.GetMagazine(currentMuzzle);
			if(magazine)
			{
				if (magazine.GetAmmoCount() <= 1)
				{
					for(int i = 0; i < magazine.GetAmmoMax();i++)
					{
						magazine.ServerStoreCartridge(0, ammoTypeName);
					}
				}
			}
		}
	}
	

		
	static void TurnItemIntoItem (notnull ItemBase old_item, string new_item_type, notnull PlayerBase player)
	{
		TurnItemIntoItemEx(player, new TurnItemIntoItemLambda(old_item, new_item_type, player));
	}
		
	static void TurnItemIntoItemEx (notnull PlayerBase player, ReplaceItemWithNewLambdaBase lambda)
	{
		player.LocalReplaceItemWithNew(lambda);
	}

	static array<ItemBase> CreateItemBasePiles(string item_name, vector ground_position, float quantity,  float health )
	{
		ref array<ItemBase>	items;
		float stack_size;
		ItemBase pile;
		
		items = new array<ItemBase>;
		stack_size = g_Game.ConfigGetInt("cfgVehicles " + item_name + " varQuantityMax");
		
		int piles_count = Math.Floor(quantity/stack_size);
		int rest = quantity - (piles_count*stack_size);
		
		for ( int i = 0; i < piles_count; i++ )
		{
			pile = ItemBase.Cast(GetGame().CreateObject(item_name, ground_position, false));
			pile.SetQuantity(stack_size);
			items.Insert(pile);
		}
		if ( rest > 0)
		{
			pile = ItemBase.Cast(GetGame().CreateObject(item_name, ground_position, false));
			pile.SetQuantity(rest);
			items.Insert(pile);
		}
		return items;
	}
	
	static array<Magazine> CreateMagazinePiles(string item_name, vector ground_position, float quantity,  float health )
	{
		ref array<Magazine>	items;
		float stack_size;
		Magazine pile;
		
		items = new array<Magazine>;
		stack_size = g_Game.ConfigGetInt("cfgMagazines " + item_name + " count");
		
		int piles_count = Math.Floor(quantity/stack_size);
		int rest = quantity - (piles_count*stack_size);
		
		for ( int i = 0; i < piles_count; i++ )
		{
			pile = Magazine.Cast(GetGame().CreateObject(item_name, ground_position, false));
			pile.ServerSetAmmoCount(stack_size);
			items.Insert(pile);
		}
		if ( rest > 0)
		{
			pile = Magazine.Cast(GetGame().CreateObject(item_name, ground_position, false));
			pile.ServerSetAmmoCount(rest);
			items.Insert(pile);
		}
		return items;
	}
	
	static array<Magazine> CreateMagazinePilesFromBullet(string bullet_type, vector ground_position, float quantity,  float health )
	{
		ref array<Magazine>	items;
		items = new array<Magazine>;
		float stack_size;
		Magazine pile;
		string item_name;
		if( !g_Game.ConfigGetText("cfgAmmo " + bullet_type + " spawnPileType", item_name) )
			return items;
		
		stack_size = g_Game.ConfigGetInt("cfgMagazines " + item_name + " count");
		
		if(stack_size > 0)
		{
			int piles_count = Math.Floor(quantity/stack_size);
			int rest = quantity - (piles_count*stack_size);
		
			for ( int i = 0; i < piles_count; i++ )
			{
				pile = Magazine.Cast(GetGame().CreateObject(item_name, ground_position, false));
				pile.ServerSetAmmoCount(stack_size);
				items.Insert(pile);
			}
			if ( rest > 0)
			{
				pile = Magazine.Cast(GetGame().CreateObject(item_name, ground_position, false));
				pile.ServerSetAmmoCount(rest);
				items.Insert(pile);
			}
		}
		return items;
	}
	
	static int GetHealthLabelForAmmo(string class_name, float health)
	{
		float health_normalized = health / 100;
		string config_path = CFG_WEAPONSPATH + " " + class_name + " DamageSystem" + " GlobalHealth" + " healthLabels";
		CashedObjectsArrays.ARRAY_FLOAT.Clear();
		GetGame().ConfigGetFloatArray(config_path, CashedObjectsArrays.ARRAY_FLOAT);
		for(int i = 0; i < CashedObjectsArrays.ARRAY_FLOAT.Count(); i++)
		{
			if( health_normalized >= CashedObjectsArrays.ARRAY_FLOAT.Get(i) )
			{
				return i;
			}
		}
		return -1;
	}
	
	static bool GetProjectedCursorPos3d (out vector position, Weapon_Base weapon)
	{
		vector usti_hlavne_position = weapon.GetSelectionPosition( "usti hlavne" );
		vector konec_hlavne_position = weapon.GetSelectionPosition( "konec hlavne" );
		vector end_point = weapon.ModelToWorld(usti_hlavne_position);
		vector begin_point = weapon.ModelToWorld(konec_hlavne_position);
		vector contact_dir;
		int contact_component;
		
		vector aim_point = end_point - begin_point;
		aim_point = aim_point.Normalized() * PROJECTED_CURSOR_DISTANCE;
		aim_point = aim_point + end_point;
		
		if (DayZPhysics.RaycastRV(begin_point, aim_point, position, contact_dir, contact_component, null, null, null, false, false, ObjIntersectFire))
			return true;
		return false;
	}
	
	static void GetHeadBonePos(notnull PlayerBase player, out vector pos)
	{
		// Get position of head for starting trace pos, otherwise use sane default
		int headBoneIdx = player.GetBoneIndexByName("Head");
		if ( headBoneIdx == -1 )
			{ pos = player.GetPosition()[1] + 1.6; }
		else
			{ pos = player.GetBonePositionWS(headBoneIdx); }	
	}

	static vector GetHeadingVector(notnull PlayerBase player)
	{
		vector dir = vector.Zero;
		float headingAngle = GetHeadingAngle(player);
		dir[0] = Math.Cos(headingAngle + Math.PI_HALF);
		dir[2] = Math.Sin(headingAngle + Math.PI_HALF);
		
		return dir.Normalized();
	}

	static float GetHeadingAngle(notnull PlayerBase player)
	{
		HumanInputController hic = player.GetInputController();
		float headingAngle = hic.GetHeadingAngle();
		
		return headingAngle;
	}
	
	static float GetEnergyMetabolicSpeed(int movement_speed)
	{
		float speed;
		switch (movement_speed)
		{
			case 1:
			{
				speed = ModifierConstants.METABOLIC_SPEED_ENERGY_WALK;
				break;
			}
			case 2:
			{
				speed = ModifierConstants.METABOLIC_SPEED_ENERGY_JOG;
				break;
			}
			case 3:
			{
				speed = ModifierConstants.METABOLIC_SPEED_ENERGY_SPRINT;
				break;
			}
			default:
			{
				speed = 0;
				break;
			}
		}
		speed += ModifierConstants.METABOLIC_SPEED_ENERGY_BASAL;
		return speed;
	}
	
	static float GetWaterMetabolicSpeed(int movement_speed)
	{
		float speed;
		switch (movement_speed)
		{
			case 1:
			{
				speed = ModifierConstants.METABOLIC_SPEED_WATER_WALK;
				break;
			}
			case 2:
			{
				speed = ModifierConstants.METABOLIC_SPEED_WATER_JOG;
				break;
			}
			case 3:
			{
				speed = ModifierConstants.METABOLIC_SPEED_WATER_SPRINT;
				break;
			}
			default:
			{
				speed = 0;
				break;
			}
		}
		speed += ModifierConstants.METABOLIC_SPEED_WATER_BASAL;
		return speed;
	}
	
	
	
};
