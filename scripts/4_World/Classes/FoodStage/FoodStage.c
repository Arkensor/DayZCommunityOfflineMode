enum FoodStageType
{
	NONE		= 0,			//food stage not define
	RAW			= 1,			//default
	BAKED		= 2,
	BOILED		= 3,
	DRIED		= 4,
	BURNED		= 5,
	ROTTEN		= 6,
	
	COUNT						//for net sync purposes
}

class FoodStage
{
	protected FoodStageType m_FoodStageType;
	protected Edible_Base m_FoodItem;
	
	protected int m_SelectionIndex;				//visual properties
	protected int m_TextureIndex;
	protected int m_MaterialIndex;
	
	protected ref array<string> m_Modifiers;
	
	protected float m_CookingTime;
	
	//constructor
	void FoodStage( Edible_Base food_item )
	{
		//default
		m_FoodStageType = FoodStageType.RAW;
		m_FoodItem = food_item;
		
		//reset cooking time
		m_CookingTime = 0;
		
		//get config data
		string config_path = "CfgVehicles" + " " + GetFoodItem().GetType() + " " + "Food" + " " + "FoodStages" + " " + GetFoodStageName( m_FoodStageType );
		
		//if not exists in config
		if ( !GetGame().ConfigIsExisting( config_path ) )
		{
			return;
		}
		
		//set params
		//visual properties
		array<int> visual_properties = new array<int>;
		GetGame().ConfigGetIntArray( config_path + " " + "visual_properties", visual_properties );
		if ( visual_properties.Count() > 0 )
		{
			SetSelectionIndex( visual_properties.Get( 0 ) );
			SetTextureIndex( visual_properties.Get( 1 ) );
			SetMaterialIndex( visual_properties.Get( 2 ) );
		}
		
		//modifiers
		m_Modifiers = new array<string>;
		GetGame().ConfigGetTextArray( config_path + " " + "modifiers", m_Modifiers );
	}
	
	//Food Stage Type
	FoodStageType GetFoodStageType()
	{
		return m_FoodStageType;
	}
	void SetFoodStageType( FoodStageType food_stage_type )
	{
		m_FoodStageType = food_stage_type;
	}
	
	//Selection index
	int GetSelectionIndex()
	{
		return m_SelectionIndex;
	}
	void SetSelectionIndex( int index )
	{
		m_SelectionIndex = index;
	}
	
	//Texture index
	int GetTextureIndex()
	{
		return m_TextureIndex;
	}
	void SetTextureIndex( int index )
	{
		m_TextureIndex = index;
	}
	
	//Material index
	int GetMaterialIndex()
	{
		return m_MaterialIndex;
	}
	void SetMaterialIndex( int index )
	{
		m_MaterialIndex = index;
	}
		
	//Food properties
	protected float GetNutritionPropertyFromIndex( int index )
	{
		string config_path;
		string food_stage_name = GetFoodStageName( m_FoodStageType );
		
		config_path = "CfgVehicles" + " " + GetFoodItem().GetType() + " " + "Food" + " " + "FoodStages" + " " + food_stage_name + " " + "nutrition_properties";
		array<float> nutrition_properties = new array<float>;
		GetGame().ConfigGetFloatArray( config_path, nutrition_properties );
		
		if ( nutrition_properties.Count() > 0 )
		{
			return nutrition_properties.Get( index );
		}
		//calculate nutrition properties from base stage and nutrition modifiers
		else
		{
			//get modifiers class for nutrition values
			config_path = "CfgVehicles" + " " + GetFoodItem().GetType() + " " + "Food" + " " + "nutrition_modifiers_class";
			
			if ( GetGame().ConfigIsExisting( config_path ) )
			{
				string nutr_mod_class;
				GetGame().ConfigGetText( config_path, nutr_mod_class );
				
				config_path = "CfgVehicles" + " " + "NutritionModifiers" + " " + nutr_mod_class + " " + "base_stage";
				string nutr_base_stage;
				GetGame().ConfigGetText( config_path, nutr_base_stage );
				
				//get nutrition values for food stage and modifiers 
				config_path = "CfgVehicles" + " " + GetFoodItem().GetType() + " " + "Food" + " " + "FoodStages" + " " + nutr_base_stage + " " + "nutrition_properties";
				array<float> base_nutr_properties = new array<float>;
				GetGame().ConfigGetFloatArray( config_path, base_nutr_properties );
				
				config_path = "CfgVehicles" + " " + "NutritionModifiers" + " " + nutr_mod_class + " " + food_stage_name + " " + "nutrition_properties";
				array<float> nutr_mod_properties = new array<float>;
				GetGame().ConfigGetFloatArray( config_path, nutr_mod_properties );
				
				//base nutrition * food stage nutrition modifier
				if ( base_nutr_properties.Count() > 0 && nutr_mod_properties.Count() > 0 )
				{
					return ( base_nutr_properties.Get( index ) * nutr_mod_properties.Get( index ) );
				}
			}
		}
		
		return 0;
	}
	
	float GetFullnessIndex()
	{
		return GetNutritionPropertyFromIndex( 0 );
	}
	
	float GetEnergy()
	{
		return GetNutritionPropertyFromIndex( 1 );
	}
	
	float GetWater()
	{
		return GetNutritionPropertyFromIndex( 2 );
	}
	
	float GetNutritionalIndex()
	{
		return GetNutritionPropertyFromIndex( 3 );
	}
	
	float GetToxicity()
	{
		return GetNutritionPropertyFromIndex( 4 );
	}
	
	//Modifiers
	array<string> GetModifiers()
	{
		if ( !m_Modifiers )
		{
			m_Modifiers = new array<string>;
		}
		
		return m_Modifiers;
	}
	
	//Food item
	protected Edible_Base GetFoodItem()
	{
		return m_FoodItem;
	}
	
	//Cooking time
	float GetCookingTime()
	{
		return m_CookingTime;
	}
	void SetCookingTime( float time )
	{
		m_CookingTime = time;
	}
	
	//********************************************/
	//	FOOD STAGE CHANGE
	//********************************************/
	//Checks if food stage can be changed to another stage
	bool CanChangeToNewStage( CookingMethodType cooking_method )
	{
		if ( GetNextFoodStageType( cooking_method ) == FoodStageType.NONE )
		{
			return false;
		}
		
		return true;
	}
	
	//returns possible food stage type according to given cooking method
	FoodStageType GetNextFoodStageType( CookingMethodType cooking_method )
	{
		ref array<string> string_output = new array<string>;
		ref array<string> stage_transitions = new array<string>;
		
		//get stage transitions from config
		string config_path = "CfgVehicles" + " " + GetFoodItem().GetType() + " " + "Food" + " " + "FoodStageTransitions" + " " + GetFoodStageName( GetFoodStageType() );;
		
		if ( GetGame().ConfigIsExisting( config_path ) )
		{
			int	child_count = GetGame().ConfigGetChildrenCount( config_path );
			
			for ( int i = 0; i < child_count; i++ )
			{
				string child_name;
				GetGame().ConfigGetChildName( config_path, i, child_name );
				string transition_config_path = config_path + " " + child_name + " " + "cooking_method";
				
				if ( GetGame().ConfigIsExisting( transition_config_path ) )
				{
					if ( GetGame().ConfigGetInt( transition_config_path ) == cooking_method )
					{
						string cooking_method_config_path = config_path + " " + child_name + " " + "transition_to";
						return GetGame().ConfigGetInt( cooking_method_config_path );
					}
				}
			}			
		}
		
		return FoodStageType.NONE;
	}
	
	void ChangeFoodStage( FoodStageType new_stage_type )
	{
		string config_path = "CfgVehicles" + " " + GetFoodItem().GetType() + " " + "Food" + " " + "FoodStages" + " " + GetFoodStageName( new_stage_type );
		
		//merge stages
		//food stage type
		SetFoodStageType( new_stage_type );
		
		array<int> visual_properties = new array<int>;
		GetGame().ConfigGetIntArray( config_path + " " + "visual_properties", visual_properties );
		if ( visual_properties.Count() > 0 )
		{
			//selection index
			int index = visual_properties.Get( 0 );
			if ( index >= 0 )
			{
				SetSelectionIndex( index );
			}
			//texture index
			index = visual_properties.Get( 1 );
			if ( index >= 0 )
			{
				SetTextureIndex( index );
			}
			//material index
			index = visual_properties.Get( 2 );
			if ( index >= 0 )
			{
				SetMaterialIndex( index );
			}	
		}
		
		//refresh visual
		RefreshVisuals();
	}	

	void RefreshVisuals()
	{
		if ( GetGame() && GetGame().IsServer() )
		{

			Edible_Base food_item = GetFoodItem();
			
			//if item has no food stages
			if ( !GetFoodItem().CanBeCooked() ) 
			{
				return;
			}
			
			//Selections
			string config_path;
			ref array<string> config_selections	= new array<string>;
			ref array<string> config_textures	= new array<string>;
			ref array<string> config_materials	= new array<string>;
				
			//selections
			config_path = "CfgVehicles" + " " + food_item.GetType() + " " + "hiddenSelections";
			GetGame().ConfigGetTextArray( config_path, config_selections );
			//textures
			config_path = "CfgVehicles" + " " + food_item.GetType() + " " + "hiddenSelectionsTextures";
			GetGame().ConfigGetTextArray( config_path, config_textures );
			//materials
			config_path = "CfgVehicles" + " " + food_item.GetType() + " " + "hiddenSelectionsMaterials";
			GetGame().ConfigGetTextArray( config_path, config_materials );
			
			//hide all selection except the configured one
			for ( int i = 0; i < config_selections.Count(); i++ )
			{
				if ( config_selections.Get( i ) != config_selections.Get( GetSelectionIndex() ) )
				{
					food_item.SetAnimationPhase( config_selections.Get( i ), 1 );
				}
			}
			
			//show selection
			food_item.SetAnimationPhase( config_selections.Get( GetSelectionIndex() ), 0 );
			//set texture
			food_item.SetObjectTexture( GetSelectionIndex(), config_textures.Get( GetTextureIndex() ) );
			//set materials
			food_item.SetObjectMaterial( GetSelectionIndex(), config_materials.Get( GetMaterialIndex() ) );
		}
	}

	//Food States
	//check food stages
	bool IsFoodInStage( FoodStageType food_stage_type )
	{
		if ( GetFoodStageType() == food_stage_type )
		{
			return true;
		}
		
		return false;
	}
	
	bool IsFoodRaw()
	{
		return IsFoodInStage( FoodStageType.RAW );
	}
	
	bool IsFoodBaked()
	{
		return IsFoodInStage( FoodStageType.BAKED );
	}
	
	bool IsFoodBoiled()
	{
		return IsFoodInStage( FoodStageType.BOILED );
	}
	
	bool IsFoodDried()
	{
		return IsFoodInStage( FoodStageType.DRIED );
	}
	
	bool IsFoodBurned()
	{
		return IsFoodInStage( FoodStageType.BURNED );
	}

	bool IsFoodRotten()
	{
		return IsFoodInStage( FoodStageType.ROTTEN );
	}
			
	//get name of food stage type
	string GetFoodStageName( FoodStageType food_stage_type )
	{
		switch( food_stage_type )
		{
			case FoodStageType.RAW: 	return "Raw";
			case FoodStageType.BAKED: 	return "Baked";
			case FoodStageType.BOILED: 	return "Boiled";
			case FoodStageType.DRIED: 	return "Dried";
			case FoodStageType.BURNED: 	return "Burned";
			case FoodStageType.ROTTEN: 	return "Rotten";
		}
		
		return "Raw";
	}
}
