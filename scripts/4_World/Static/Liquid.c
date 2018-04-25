class Liquid
{
	//---------------------------------------------------------------------------------------------------------
	static void Transfer(ItemBase source_ent, ItemBase target_ent, float quantity = -1)
	{
		if( !Liquid.CanTransfer(source_ent, target_ent) ) return;
		
		ItemBase source = source_ent;
		ItemBase target = target_ent;
		
		Debug.Log("Transfer, source:"+source.ToString()+" target: "+target.ToString(), "LiquidTransfer");
		
		float source_quantity 	= source.GetQuantity();
		float target_quantity 	= target.GetQuantity();
		int source_liquid_type 	= source.GetLiquidType();
		int target_liquid_type 	= target.GetLiquidType();
		int target_mask 		= target_ent.ConfigGetFloat("liquidContainerType");
		int source_mask 		= source_ent.ConfigGetFloat("liquidContainerType");
		
		float available_capacity = target.GetQuantityMax() - target_quantity;
		
		Debug.Log("target_mask ="+target_mask.ToString(), "LiquidTransfer");
		Debug.Log("source_mask ="+source_mask.ToString(), "LiquidTransfer");
		Debug.Log("source_liquid_type ="+source_liquid_type.ToString(), "LiquidTransfer");
		Debug.Log("target_liquid_type ="+target_liquid_type.ToString(), "LiquidTransfer");
		
		//target.GetBloodType(source_liquid_type);//override target liquidType
		float quantity_to_transfer;
		//transfers all
		if (quantity == -1)
		{
			quantity_to_transfer = Math.Clamp(source_quantity,0,available_capacity);
		}
		//transfers exact ammount
		else
		{
			quantity_to_transfer = Math.Clamp(quantity,0,available_capacity);
		}
		
		//target.AddQuantity(quantity_to_transfer);
		PluginTransmissionAgents m_mta = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));
		m_mta.TransmitAgents(source_ent, target_ent, AGT_TRANSFER_COPY);
		
		source.AddQuantity(-quantity_to_transfer);
		
		Liquid.FillContainer(target_ent,source_liquid_type,quantity_to_transfer);
	}
	
	static bool CanTransfer(ItemBase source_ent, ItemBase target_ent)
	{
	
		if( !source_ent.IsItemBase() || !target_ent.IsItemBase() )
		{
			//Debug.Log("One of the Items is not of ItemBase type", "LiquidTransfer");
			return false;
		}
		
		ItemBase source = source_ent;
		ItemBase target = target_ent;

		
		float source_quantity = source.GetQuantity();
		if( source_quantity <= 0 )
		{
			//Debug.Log("source has no quantity", "LiquidTransfer");
			return false;//if there is nothing to transfer
		}
		
		int source_liquid_type 	= source.GetLiquidType();	
		if( source_liquid_type < 1 ) 
		{
			//Debug.Log("source has some quantity, but does not have a valid liquidType set, liquidType = "+ToString(source_liquid_type), "LiquidTransfer");
			return false;//if source is not a container
		}
		
		if(!CanFillContainer(target_ent,source_liquid_type) )
		{
			return false;
		}
		
		
		return true;
	}
	static void FillContainer(ItemBase container, int liquid_type, float amount)
	{
		if(!CanFillContainer(container,liquid_type) )
		{
			return;
		}
		//filling
		container.SetLiquidType(liquid_type);
		container.AddQuantity(amount); 
		
	}

	static void FillContainerEnviro(ItemBase container, int liquid_type, float amount)
	{
		FillContainer(container,liquid_type,amount);
		PluginTransmissionAgents plugin = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));
		plugin.TransmitAgents(NULL, container, AGT_WATER_POND, amount);
	}
	
	static bool CanFillContainer(ItemBase container, int liquid_type)
	{
		if(!container) return false;
		bool is_container_full = container.IsFullQuantity();
		
		if( is_container_full )
		{
			//Debug.Log("container is full", "LiquidTransfer");
			return false;
			
		}
		int container_mask 	= container.ConfigGetFloat("liquidContainerType");
		
		if( container_mask == 0 )
		{
			//Debug.Log("target is not a container", "LiquidTransfer");
			return false;//if the target liquidContainerType is set to 0
		}
		
		if( (liquid_type & container_mask) == 0 )
		{
			//Debug.Log("target liquidContainerType does not support this liquid type", "LiquidTransfer");
			return false;
		}
		
		float container_quantity 	= container.GetQuantity();
		
		int container_liquid_type 	= container.GetLiquidType();
		
		if( container_quantity > 0 && container_liquid_type != liquid_type) 
		{
			//Debug.Log("target is not empty AND is of different liquid type than liquid_type added in", "LiquidTransfer");
			return false;
		}
		return true;
	}
	
	private static string GetLiquidConfigProperty(int liquid_type, string property_name, bool is_nutrition_property = false)
	{
		string cfg_classname = "cfgLiquidDefinitions";
		string property_value = "NULL_PROPERTY";
		int cfg_item_count = g_Game.ConfigGetChildrenCount(cfg_classname);

		for ( int i = 0; i < cfg_item_count; i++ )
		{
			string liquid_class_name;
			GetGame().ConfigGetChildName(cfg_classname, i, liquid_class_name);
			string liquid_full_path = cfg_classname + " " + liquid_class_name;
			int config_liquid_type = GetGame().ConfigGetInt(liquid_full_path + " " + "type");
			
			if( config_liquid_type == liquid_type )// found the specific class, now lets extract the values
			{
				if(!is_nutrition_property) 
				{
					GetGame().ConfigGetText(liquid_full_path + " " + property_name,property_value);
					return property_value;
				}
				else
				{
					GetGame().ConfigGetText(liquid_full_path + " Nutrition " + property_name,property_value);
					return property_value;
				}
			}
		}
		return property_value;
	}

	static NutritionalProfile GetNutritionalProfile(int liquid_type)
	{
		float energy = GetEnergy(liquid_type);
		float nutritional_index = GetNutritionalIndex(liquid_type);
		float volume = GetVolume(liquid_type);
		float water_content = GetWaterContent(liquid_type);
		float toxicity = GetToxicity(liquid_type);
		NutritionalProfile profile = new NutritionalProfile(energy, water_content, nutritional_index, volume, toxicity );
		return profile;
	}
	
	static float GetToxicity(int liquid_type)
	{
		return Liquid.GetLiquidConfigProperty(liquid_type, "toxicity", true).ToFloat();
	}
	
	static float GetWaterContent(int liquid_type)
	{
		return Liquid.GetLiquidConfigProperty(liquid_type, "water", true).ToFloat();
	}
	
	static float GetEnergy(int liquid_type)
	{
		return Liquid.GetLiquidConfigProperty(liquid_type, "energy", true).ToFloat();
	}
	
	static float GetNutritionalIndex(int liquid_type)
	{
		return Liquid.GetLiquidConfigProperty(liquid_type, "nutritionalIndex", true).ToFloat();
	}
	
	static string GetName(int liquid_type)
	{
		return Liquid.GetLiquidConfigProperty(liquid_type, "name");
	}
	
	static float GetFlammability(int liquid_type)
	{
		return Liquid.GetLiquidConfigProperty(liquid_type, "flammability").ToFloat();
	}
	
	static float GetVolume(int liquid_type)
	{
		return Liquid.GetLiquidConfigProperty(liquid_type, "totalVolume", true).ToFloat();
	}
	
};