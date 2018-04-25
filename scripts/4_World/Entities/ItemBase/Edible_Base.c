class Edible_Base extends ItemBase
{
	ref FoodStage m_FoodStage;
	
	void Edible_Base()
	{
		if ( !m_FoodStage )
		{
			m_FoodStage = new FoodStage ( this );
		}
		
		//synchronized variables
		RegisterNetSyncVariableInt( "m_FoodStage.m_FoodStageType", FoodStageType.NONE, FoodStageType.COUNT );
		RegisterNetSyncVariableFloat( "m_FoodStage.m_CookingTime", 0, 600, 0 );						//min = 0; max = 0; precision = 0;
	}
	
	override void EEInit()
	{
		super.EEInit();
		
		//refresh visual after init
		RefreshVisuals();
	}
	
	void RefreshVisuals()
	{
		GetFoodStage().RefreshVisuals();
	}
	
	//================================================================
	// SERIALIZATION
	//================================================================	
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );
		
		//Food stage type 
		ctx.Write( GetFoodStage().GetFoodStageType() );
		
		//Selection index
		ctx.Write( GetFoodStage().GetSelectionIndex() );
		
		//Texture index
		ctx.Write( GetFoodStage().GetTextureIndex() );
		
		//Material index
		ctx.Write( GetFoodStage().GetMaterialIndex() );
				
		//Modifiers
		//size
		int modifiers_size = 0;
		if ( GetFoodStage().GetModifiers() )	//if not null
		{
			modifiers_size = GetFoodStage().GetModifiers().Count();
		}
		ctx.Write( modifiers_size );
		//data
		for ( int j = 0; j < modifiers_size; j++ )
		{
			ctx.Write( GetFoodStage().GetModifiers().Get( j ) );
		}
	}
	
	override void OnStoreLoad( ParamsReadContext ctx )
	{
		super.OnStoreLoad(ctx);
		
		//Food stage name 
		FoodStageType stage_type = FoodStageType.RAW;
		ctx.Read( stage_type );
		GetFoodStage().SetFoodStageType( stage_type );
		
		//Selection index
		int selection_idx = 0;
		ctx.Read( selection_idx );
		GetFoodStage().SetSelectionIndex( selection_idx );
		
		//Texture index
		int texture_idx = 0;
		ctx.Read( texture_idx );
		GetFoodStage().SetTextureIndex( texture_idx );
		
		//Material index
		int material_idx = 0;
		ctx.Read( material_idx );
		GetFoodStage().SetMaterialIndex( material_idx );
		
		//Modifiers
		//count
		int modifiers_size = 0;
		ctx.Read( modifiers_size );		
		//data
		for ( int j = 0; j < modifiers_size; j++ )
		{
			string modifier;
			ctx.Read( modifier );
			GetFoodStage().GetModifiers().Insert( modifier );
		}
		
		//refresh visual after load
		RefreshVisuals();
	}
	
	//================================================================
	// SYNCHRONIZATION
	//================================================================	
	void Synchronize()
	{
		SetSynchDirty();
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
	}
	
	//get food stage
	FoodStage GetFoodStage()
	{
		return m_FoodStage;
	}
	
	//food types
	bool IsMeat()
	{
		return false;
	}
	
	//================================================================
	// NUTRITIONAL VALUES
	//================================================================	
	//food properties
	float GetFoodTotalVolume()
	{
		if ( !CanBeCooked() )
		{
			string class_path = "cfgVehicles " + GetType() + " Nutrition";
			return GetGame().ConfigGetFloat( class_path + " fullnessIndex" );			
		}
		
		return GetFoodStage().GetFullnessIndex();
	}
	
	float GetFoodEnergy()
	{
		if ( !CanBeCooked() )
		{
			string class_path = "cfgVehicles " + GetType() + " Nutrition";
			return GetGame().ConfigGetFloat( class_path + " energy" );			
		}

		return GetFoodStage().GetEnergy();
	}
	
	float GetFoodWater()
	{
		if ( !CanBeCooked() )
		{
			string class_path = "cfgVehicles " + GetType() + " Nutrition";
			return GetGame().ConfigGetFloat( class_path + " water" );			
		}

		return GetFoodStage().GetWater();
	}
	
	float GetFoodNutritionalIndex()
	{
		if ( !CanBeCooked() )
		{
			string class_path = "cfgVehicles " + GetType() + " Nutrition";
			return GetGame().ConfigGetFloat( class_path + " nutritionalIndex" );			
		}

		return GetFoodStage().GetNutritionalIndex();
	}
	
	float GetFoodToxicity()
	{
		if ( !CanBeCooked() )
		{
			string class_path = "cfgVehicles " + GetType() + " Nutrition";
			return GetGame().ConfigGetFloat( class_path + " toxicity" );			
		}

		return GetFoodStage().GetToxicity();
	}
	
	
	//================================================================
	// FOOD STAGING
	//================================================================
	FoodStageType GetFoodStageType()
	{
		return GetFoodStage().GetFoodStageType();
	}
	
	//food stage states
	bool IsFoodRaw()
	{
		return GetFoodStage().IsFoodRaw();
	}

	bool IsFoodBaked()
	{
		return GetFoodStage().IsFoodBaked();
	}
	
	bool IsFoodBoiled()
	{
		return GetFoodStage().IsFoodBoiled();
	}
	
	bool IsFoodDried()
	{
		return GetFoodStage().IsFoodDried();
	}
	
	bool IsFoodBurned()
	{
		return GetFoodStage().IsFoodBurned();
	}
	
	bool IsFoodRotten()
	{
		return GetFoodStage().IsFoodRotten();
	}				
	
	//food stage change
	void ChangeFoodStage( FoodStageType new_food_stage_type )
	{
		GetFoodStage().ChangeFoodStage( new_food_stage_type );
	}
	
	FoodStageType GetNextFoodStageType( CookingMethodType cooking_method )
	{
		return GetFoodStage().GetNextFoodStageType( cooking_method );
	}
	
	string GetFoodStageName( FoodStageType food_stage_type )
	{
		return GetFoodStage().GetFoodStageName( food_stage_type );
	}
	
	bool CanChangeToNewStage( CookingMethodType cooking_method )
	{
		return GetFoodStage().CanChangeToNewStage( cooking_method );
	}
	
	//================================================================
	// COOKING
	//================================================================
	//cooking time
	float GetCookingTime()
	{
		return GetFoodStage().GetCookingTime();
	}
	
	void SetCookingTime( float time )
	{
		GetFoodStage().SetCookingTime( time );
		
		//synchronize when calling on server
		Synchronize();
	}
	
	void ReplaceEdibleWithNew (string typeName)
	{
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		ReplaceEdibleWithNewLambda lambda = new ReplaceEdibleWithNewLambda(this, typeName, player);
		player.LocalReplaceItemInHandsWithNew(lambda);
	}
}

class ReplaceEdibleWithNewLambda : ReplaceItemWithNewLambdaBase
{
	void ReplaceEdibleWithNewLambda (EntityAI old_item, string new_item_type, PlayerBase player) { }

	override void CopyOldPropertiesToNew (notnull EntityAI old_item, notnull EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);

		float health = old_item.GetHealth("", "");
		Print("ReplaceEdibleWithNewLambda::OnCreated old_item=" + old_item + " health=" + health);
		new_item.SetHealth("", "", health);
	}
};
