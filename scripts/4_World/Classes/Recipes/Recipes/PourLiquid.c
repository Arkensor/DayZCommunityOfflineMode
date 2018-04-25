class PourLiquid extends RecipeBase
{	
	override void Init()
	{
		m_Name = "#STR_PourLiquid0";
		m_IsInstaRecipe = false;//should this recipe be performed instantly without animation
		m_AnimationLength = 1.5;//animation length in relative time units
		m_Specialty = -0.01;// value > 0 for roughness, value < 0 for precision
		
		
		//conditions
		m_MinDamageIngredient[0] = -1;//-1 = disable check
		m_MaxDamageIngredient[0] = 3;//-1 = disable check
		
		m_MinQuantityIngredient[0] = 0;//-1 = disable check
		m_MaxQuantityIngredient[0] = -1;//-1 = disable check
		
		m_MinDamageIngredient[1] = -1;//-1 = disable check
		m_MaxDamageIngredient[1] = 3;//-1 = disable check
		
		m_MinQuantityIngredient[1] = 0;//-1 = disable check
		m_MaxQuantityIngredient[1] = -1;//-1 = disable check
		//----------------------------------------------------------------------------------------------------------------------
		
		//INGREDIENTS
		//ingredient 1
		InsertIngredient(0,"Pot");//you can insert multiple ingredients this way
		InsertIngredient(0,"CanisterGasoline");//you can insert multiple ingredients this way
		InsertIngredient(0,"Canteen");//you can insert multiple ingredients this way
		InsertIngredient(0,"WaterBottle");//you can insert multiple ingredients this way
		InsertIngredient(0,"Vodka");//you can insert multiple ingredients this way
		InsertIngredient(0,"WaterPouch_ColorBase");//you can insert multiple ingredients this way
		InsertIngredient(0,"Barrel_ColorBase");//you can insert multiple ingredients this way
		
		m_IngredientAddHealth[0] = 0;// 0 = do nothing
		m_IngredientSetHealth[0] = -1; // -1 = do nothing
		m_IngredientAddQuantity[0] = 0;// 0 = do nothing
		m_IngredientDestroy[0] = false;//true = destroy, false = do nothing
		m_IngredientUseSoftSkills[0] = false;// set 'true' to allow modification of the values by softskills on this ingredient
		
		//ingredient 2
		InsertIngredient(1,"Pot");//you can insert multiple ingredients this way
		InsertIngredient(1,"CanisterGasoline");//you can insert multiple ingredients this way
		InsertIngredient(1,"Canteen");//you can insert multiple ingredients this way
		InsertIngredient(1,"WaterBottle");//you can insert multiple ingredients this way
		InsertIngredient(1,"Vodka");//you can insert multiple ingredients this way
		InsertIngredient(1,"WaterPouch_ColorBase");//you can insert multiple ingredients this way
		InsertIngredient(1,"Barrel_ColorBase");//you can insert multiple ingredients this way
		
		m_IngredientAddHealth[1] = 0;// 0 = do nothing
		m_IngredientSetHealth[1] = -1; // -1 = do nothing
		m_IngredientAddQuantity[1] = 0;// 0 = do nothing
		m_IngredientDestroy[1] = false;// false = do nothing
		m_IngredientUseSoftSkills[1] = false;// set 'true' to allow modification of the values by softskills on this ingredient
		//----------------------------------------------------------------------------------------------------------------------
		
		//result1
		//AddResult("");//add results here

		m_ResultSetFullQuantity[0] = false;//true = set full quantity, false = do nothing
		m_ResultSetQuantity[0] = -1;//-1 = do nothing
		m_ResultSetHealth[0] = -1;//-1 = do nothing
		m_ResultInheritsHealth[0] = -1;// (value) == -1 means do nothing; a (value) >= 0 means this result will inherit health from ingredient number (value);(value) == -2 means this result will inherit health from all ingredients averaged(result_health = combined_health_of_ingredients / number_of_ingredients)
		m_ResultInheritsColor[0] = -1;// (value) == -1 means do nothing; a (value) >= 0 means this result classname will be a composite of the name provided in AddResult method and config value "color" of ingredient (value)
		m_ResultToInventory[0] = -2;//(value) == -2 spawn result on the ground;(value) == -1 place anywhere in the players inventory, (value) >= 0 means switch position with ingredient number(value)
		m_ResultUseSoftSkills[0] = false;// set 'true' to allow modification of the values by softskills on this result
		m_ResultReplacesIngredient[0] = -1;// value == -1 means do nothing; a value >= 0 means this result will transfer item propertiesvariables, attachments etc.. from an ingredient value
	}

	override bool CanDo(ItemBase ingredients[], PlayerBase player)//final check for recipe's validity
	{
		ItemBase ingredient1;
		Class.CastTo(ingredient1, ingredients[0]);
		ItemBase ingredient2;
		Class.CastTo(ingredient2, ingredients[1]);

		ItemBase source = NULL;
		ItemBase target = NULL;
		
		ItemBase entity_in_hands = ItemBase.Cast(player.GetHumanInventory().GetEntityInHands());
		
		//condition always true for any bottle, another recipe needed for pouring TO handheld bottle
		if( entity_in_hands == ingredient1 )
		{
			return Liquid.CanTransfer(ingredient1,ingredient2);
		}
		else
		{
			return Liquid.CanTransfer(ingredient2,ingredient1);
		}
	}

	override void Do(ItemBase ingredients[], PlayerBase player,array<ItemBase> results, float specialty_weight)//gets called upon recipe's completion
	{
		ItemBase ingredient1 = ingredients[0];
		ItemBase ingredient2 = ingredients[1];
		
		ItemBase entity_in_hands = ItemBase.Cast(player.GetHumanInventory().GetEntityInHands());
		
		if( entity_in_hands == ingredient1 )
		{
			Liquid.Transfer(ingredient1,ingredient2);
		}
		else
		{
			Liquid.Transfer(ingredient2,ingredient1);
		}
	}
};
