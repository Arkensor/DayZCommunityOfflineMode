enum CookingMethodType
{
	NONE		= 0,		//no cooking method available
	BAKING		= 1,
	BOILING		= 2,
	DRYING		= 3,
	TIME		= 4
}

class Cooking
{
	static const float COOKING_FOOD_TIME_INC_VALUE 		= 2;		//time increase when cooking a food
	static const float COOKING_LARD_DECREASE_COEF 		= 0.05;		//lard quantity decrease when cooking a food (Baking)
	//
	static const float DEFAULT_COOKING_TEMPERATURE 		= 150;		//default temperature for cooking (e.g. cooking on stick)
	static const float FOOD_MAX_COOKING_TEMPERATURE		= 150;		//
	static const float PARAM_BURN_DAMAGE_COEF			= 0.05;		//value for calculating damage on items located in fireplace cargo
	//
	typename COOKING_EQUIPMENT_POT	 					= Pot;
	typename COOKING_INGREDIENT_LARD 					= Lard;
	//
	static const float LIQUID_BOILING_POINT 			= 100;	//boiling point for liquids
	static const float LIQUID_VAPOR_QUANTITY 			= 2;	//vapor quantity
	
	//COOKING PROCESS
	//--- Cooking with equipment (pot)
	//Returns 1 if the item changed its cooking stage, 0 if not
	int CookWithEquipment( ItemBase cooking_equipment, float cooking_time_coef = 1 )
	{
		int cooking_state_update = 0;
		
		//check cooking conditions
		if ( cooking_equipment == NULL )
		{
			return cooking_state_update;
		}
		
		//manage items in cooking equipment
		Cargo cargo = cooking_equipment.GetInventory().GetCargo();
		
		for ( int i = 0; i < cargo.GetItemCount(); i++ )
		{
			ItemBase item = ItemBase.Cast( cargo.GetItem( i ) );
			Edible_Base item_to_cook = Edible_Base.Cast( item );
			
			if ( item_to_cook && item_to_cook.CanBeCooked() )
			{
				//update food
				CookingMethodType cooking_method = GetCookingMethod( cooking_equipment );
				cooking_state_update = UpdateCookingState( item_to_cook, cooking_method, cooking_equipment, cooking_time_coef );
			}
			else   
			{
				//damage item
				item.DecreaseHealth( "", "", PARAM_BURN_DAMAGE_COEF * 100 );
				
				//TODO - non edible items have varTemperatureMax set to 0, so their temperature cannot be set properly
				//add temperature
				//AddTemperatureToItem( item, NULL, 0 );
			}
		}
		
		//boil liquid
		Bottle_Base bottle_base = Bottle_Base.Cast( cooking_equipment );
		if ( bottle_base )
		{
			float cooking_equipment_temp = cooking_equipment.GetTemperature();
			
			if ( cooking_equipment_temp >= LIQUID_BOILING_POINT )
			{
				//remove agents
				//cooking_equipment.RemoveAllAgents();
				
				//vaporize liquid
				if ( cooking_equipment.GetQuantity() > 0 )
				{
					//LIQUID_VAPOR_QUANTITY	
					cooking_equipment.AddQuantity( -LIQUID_VAPOR_QUANTITY );
					
					//start steam particle
					bottle_base.SteamStart();
				}
			}
			else
			{
				//stop steam particle
				bottle_base.SteamStop();
			}			
		}
		
		return cooking_state_update;
	}
	
	//Returns 1 if the item changed its cooking stage, 0 if not
	int CookOnStick( Edible_Base item_to_cook, float cook_time_inc )
	{
		if ( item_to_cook && item_to_cook.CanBeCooked() )
		{
			//update food
			return UpdateCookingStateOnStick( item_to_cook, cook_time_inc );
		}
		
		return 0;
	}
	
	//Returns 1 if the item changed its cooking stage, 0 if not
	protected int UpdateCookingState( Edible_Base item_to_cook, CookingMethodType cooking_method, ItemBase cooking_equipment, float cooking_time_coef )
	{
		//food properties
		float food_temperature = item_to_cook.GetTemperature();
		
		//{min_temperature, time_to_cook, max_temperature (optional)}
		//get next stage name - if next stage is not defined, next stage name will be empty "" and no cooking properties (food_min_temp, food_time_to_cook, food_max_temp) will be set
		FoodStageType new_stage_type = item_to_cook.GetNextFoodStageType( cooking_method );
		float food_min_temp = 0;
		float food_time_to_cook = 0;
		float food_max_temp = -1;
		
		//Set next stage cooking properties if next stage possible
		if ( item_to_cook.CanChangeToNewStage( cooking_method ) )
		{
			ref array<float> next_stage_cooking_properties = new array<float>;
			
			string config_path = "CfgVehicles" + " " + item_to_cook.GetType() + " " + "Food" + " " + "FoodStages";
			GetGame().ConfigGetFloatArray( config_path + " " + item_to_cook.GetFoodStageName( new_stage_type ) + " " + "cooking_properties", next_stage_cooking_properties );
			
			food_min_temp = next_stage_cooking_properties.Get( 0 );
			food_time_to_cook = next_stage_cooking_properties.Get( 1 );
			
			if ( next_stage_cooking_properties.Count() > 2)
			{
				food_max_temp = next_stage_cooking_properties.Get ( 2 );
			}	
		}
		
		//add temperature
		AddTemperatureToItem( item_to_cook, cooking_equipment, food_min_temp );
		
		//add cooking time if the food can be cooked by this method
		if ( food_min_temp > 0 && food_temperature >= food_min_temp )
		{
			float new_cooking_time = item_to_cook.GetCookingTime() + COOKING_FOOD_TIME_INC_VALUE * cooking_time_coef;
			item_to_cook.SetCookingTime( new_cooking_time );
			
			//progress to next stage
			if ( item_to_cook.GetCookingTime() >= food_time_to_cook )
			{
				//if max temp is defined check next food stage
				if ( food_max_temp >= 0 )
				{
					if ( food_temperature > food_max_temp && item_to_cook.GetFoodStageType() != FoodStageType.BURNED )
					{
						new_stage_type = FoodStageType.BURNED;
					}
				}
				
				//change food stage
				item_to_cook.ChangeFoodStage( new_stage_type );
				//Temp
				//Remove all modifiers
				item_to_cook.RemoveAllAgents();
				
				//remove lard when baking with cooking equipment
				if ( cooking_equipment && cooking_method == CookingMethodType.BAKING )
				{
					if ( item_to_cook.Type() != COOKING_INGREDIENT_LARD )
					{
						//get lard item
						ItemBase item_lard = GetItemTypeFromCargo( COOKING_INGREDIENT_LARD, cooking_equipment );
						
						//decrease lard quantity
						float lard_quantity = item_lard.GetQuantity() - COOKING_LARD_DECREASE_COEF;
						lard_quantity = Math.Clamp( lard_quantity, 0, item_lard.GetQuantityMax() );
						item_lard.SetQuantity( lard_quantity );
					}
				}
				
				//reset cooking time
				item_to_cook.SetCookingTime( 0 );
				
				return 1;
			}
		}
		
		return 0;
	}
	
	//Returns 1 if the item changed its cooking stage, 0 if not
	protected int UpdateCookingStateOnStick( Edible_Base item_to_cook, float cook_time_inc )
	{
		//food properties
		float food_temperature = item_to_cook.GetTemperature();
		
		//{min_temperature, time_to_cook, max_temperature (optional)}
		//get next stage name - if next stage is not defined, next stage name will be empty "" and no cooking properties (food_min_temp, food_time_to_cook, food_max_temp) will be set
		FoodStageType new_stage_type = item_to_cook.GetNextFoodStageType( CookingMethodType.BAKING );
		float food_min_temp = 0;
		float food_time_to_cook = 0;
		float food_max_temp = -1;
		
		//Set next stage cooking properties if next stage possible
		if ( item_to_cook.CanChangeToNewStage ( CookingMethodType.BAKING ) )
		{
			ref array<float> next_stage_cooking_properties = new array<float>;
			
			string config_path = "CfgVehicles" + " " + item_to_cook.GetType() + " " + "Food" + " " + "FoodStages";
			GetGame().ConfigGetFloatArray ( config_path + " " + item_to_cook.GetFoodStageName( new_stage_type ) + " " + "cooking_properties", next_stage_cooking_properties );
			
			food_min_temp = next_stage_cooking_properties.Get( 0 );
			food_time_to_cook = next_stage_cooking_properties.Get( 1 );
			
			if ( next_stage_cooking_properties.Count() > 2)
			{
				food_max_temp = next_stage_cooking_properties.Get( 2 );
			}	
		}
		
		//add temperature
		AddTemperatureToItem( item_to_cook, NULL, food_min_temp );
		
		//add cooking time if the food can be cooked by this method
		if ( food_min_temp > 0 && food_temperature >= food_min_temp )
		{
			float new_cooking_time = item_to_cook.GetCookingTime() + cook_time_inc;
			item_to_cook.SetCookingTime( new_cooking_time );
			
			//progress to next stage
			if ( item_to_cook.GetCookingTime() >= food_time_to_cook )
			{
				//if max temp is defined check next food stage
				if ( food_max_temp >= 0 )
				{
					if ( food_temperature > food_max_temp && item_to_cook.GetFoodStageType() != FoodStageType.BURNED )
					{
						new_stage_type = FoodStageType.BURNED;
					}
				}
				
				//change food stage
				item_to_cook.ChangeFoodStage( new_stage_type );
				//Temp
				//Remove all modifiers
				item_to_cook.RemoveAllAgents();

				//reset cooking time
				item_to_cook.SetCookingTime( 0 );
				
				return 1;
			}
		}
		
		return 0;
	}
	
	//COOKING DATA
	//
	protected ItemBase GetItemTypeFromCargo( typename item_type, ItemBase cooking_equipment )
	{
		Cargo cargo = cooking_equipment.GetInventory().GetCargo();
		
		for ( int i = 0; i < cargo.GetItemCount(); i++ )
		{
			EntityAI entity = cargo.GetItem( i );
			if ( entity.Type() == item_type )
			{
				ItemBase item = ItemBase.Cast( entity );

				return item;
			}
		}
		
		return NULL;
	}

	protected CookingMethodType GetCookingMethod( ItemBase cooking_equipment )
	{
		if ( cooking_equipment.Type() == COOKING_EQUIPMENT_POT )
		{
			//has water
			if ( cooking_equipment.GetQuantity() > 0 )
			{
				return CookingMethodType.BOILING;
			}
			
			//has lard in cargo
			if ( GetItemTypeFromCargo( COOKING_INGREDIENT_LARD, cooking_equipment ) )
			{
				return CookingMethodType.BAKING;
			}
			
			return CookingMethodType.DRYING;
		}
			
		return CookingMethodType.NONE;
	}
	
	Edible_Base GetFoodOnStick( ItemBase stick_item )
	{
		Edible_Base food_on_stick = Edible_Base.Cast( stick_item.GetAttachmentByType( Edible_Base ) );

		return food_on_stick;
	}
	
	float GetTimeToCook( Edible_Base item_to_cook, CookingMethodType cooking_method )
	{
		ref array<float> next_stage_cooking_properties = new array<float>;
		string config_path = "CfgVehicles" + " " + item_to_cook.GetType() + " " + "Food" + " " + "FoodStages";
		FoodStageType food_stage_type = item_to_cook.GetNextFoodStageType( cooking_method );
		GetGame().ConfigGetFloatArray( config_path + " " + item_to_cook.GetFoodStageName( food_stage_type ) + " " + "cooking_properties", next_stage_cooking_properties );
		
		return next_stage_cooking_properties.Get( 1 );
	}

	float GetMinTempToCook( Edible_Base item_to_cook, CookingMethodType cooking_method )
	{
		ref array<float> next_stage_cooking_properties = new array<float>;
		string config_path = "CfgVehicles" + " " + item_to_cook.GetType() + " " + "Food" + " " + "FoodStages";
		FoodStageType food_stage_type = item_to_cook.GetNextFoodStageType( cooking_method );
		GetGame().ConfigGetFloatArray( config_path + " " + item_to_cook.GetFoodStageName( food_stage_type ) + " " + "cooking_properties", next_stage_cooking_properties );
		
		return next_stage_cooking_properties.Get( 0 );
	}
	
	//add temperature to item
	protected void AddTemperatureToItem( ItemBase cooked_item, ItemBase cooking_equipment, float min_temperature )
	{
		float item_temperature = cooked_item.GetTemperature();
		
		//set actual cooking temperature
		float actual_cooking_temp = DEFAULT_COOKING_TEMPERATURE;	//default
		if ( cooking_equipment )
		{
			actual_cooking_temp = cooking_equipment.GetTemperature();
		}
		
		//add temperature
		if ( actual_cooking_temp > item_temperature )
		{
			
			item_temperature = actual_cooking_temp * 0.5;
			item_temperature = Math.Clamp( item_temperature, min_temperature, FOOD_MAX_COOKING_TEMPERATURE );
			
			//set new temperature
			cooked_item.SetTemperature( item_temperature );
		}
	}
	
}
