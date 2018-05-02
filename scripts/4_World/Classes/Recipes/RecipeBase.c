const int MAX_NUMBER_OF_INGREDIENTS = 2;
const int MAXIMUM_RESULTS = 10;

class RecipeBase
{
	string m_ItemsToCreate[MAXIMUM_RESULTS];
	ref array<string> m_Ingredients[MAX_NUMBER_OF_INGREDIENTS];
	
	ItemBase m_Items[MAX_NUMBER_OF_INGREDIENTS];
	
	ItemBase m_IngredientsSorted[MAX_NUMBER_OF_INGREDIENTS]; //if the recipe is valid, this array will contain all ingredients sorted against the recipe ingredients
	
	//ref array<string> m_ItemsToCreate;
	
	//string m_Result;
	ref array<ItemBase> m_IngredientsToBeDeleted = new array<ItemBase>;
	string m_Name;
	
	int	m_ID;
	int m_NumberOfResults;
	float m_AnimationLength = 1;//animation length in relative time units
	float m_Specialty = 0;// value > 0 for roughness, value < 0 for precision
	bool m_IsInstaRecipe;//should this recipe be performed instantly without animation
	//bool m_IsActionType;//will this recipe be also a user action(only set true if you know what you are doing)
	bool m_AnywhereInInventory;//is this recipe valid even when neither of the items is in hands
	//array<string> ptr;

	float m_MinQuantityIngredient[MAX_NUMBER_OF_INGREDIENTS];
	float m_MaxQuantityIngredient[MAX_NUMBER_OF_INGREDIENTS];
	float m_MinDamageIngredient[MAX_NUMBER_OF_INGREDIENTS];
	float m_MaxDamageIngredient[MAX_NUMBER_OF_INGREDIENTS];

	bool m_IngredientUseSoftSkills[MAX_NUMBER_OF_INGREDIENTS];
	float m_IngredientAddHealth[MAX_NUMBER_OF_INGREDIENTS];
	float m_IngredientAddQuantity[MAX_NUMBER_OF_INGREDIENTS];
	float m_IngredientSetHealth[MAX_NUMBER_OF_INGREDIENTS];
	bool m_IngredientDestroy[MAX_NUMBER_OF_INGREDIENTS];
	
	
	bool m_ResultSetFullQuantity[MAXIMUM_RESULTS];
	float m_ResultSetQuantity[MAXIMUM_RESULTS];
	float m_ResultSetHealth[MAXIMUM_RESULTS];
	int m_ResultToInventory[MAXIMUM_RESULTS];
	int m_ResultInheritsHealth[MAXIMUM_RESULTS];
	int m_ResultInheritsColor[MAXIMUM_RESULTS];
	int	m_ResultReplacesIngredient[MAXIMUM_RESULTS];
	bool m_ResultUseSoftSkills[MAXIMUM_RESULTS];
	


	void RecipeBase()
	{

		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			m_Ingredients[i] = new array<string>;
			m_IngredientsSorted[i] = NULL;
		}

		m_NumberOfResults = 0;
		//m_IsActionType = false;
		
		m_Name = "RecipeBase default name";
		Init();
	}

	void Init();

	float GetLengthInSecs()
	{
		return m_AnimationLength * CRAFTING_TIME_UNIT_SIZE;
	}
	
	float GetSpecialty()
	{
		return m_Specialty;
	}
	
	bool IsRecipeAnywhere()
	{
		return m_AnywhereInInventory;		
	}
	
	bool CheckIngredientMatch(ItemBase item1, ItemBase item2)
	{
		if( item1 == NULL || item2 == NULL ) return false;
		
		m_Items[0] = item1;
		m_Items[1] = item2;
		
		bool found = false;
		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)//all ingredients
		{
			found = false;
			array<string> tempArray = m_Ingredients[i];
			for(int x = 0; x < tempArray.Count(); x++)//particular ingredient array
			{
				for(int z = 0; z < MAX_NUMBER_OF_INGREDIENTS; z++)
				{
					if( m_Items[z] != NULL )
					{
						ItemBase item = m_Items[z];
						if( tempArray.Get(x) == item.GetType() || GetGame().IsKindOf(item.GetType(),tempArray.Get(x)))
						{
							found = true;//we found a match
							//m_IngredientsSorted.Insert(item);
							m_IngredientsSorted[i] = item;
							m_Items[z] = NULL;
						}
					}
					if(found) break;//we found a match, no need to check the remaining ingredients
				}
				if(found) break;//we found a match, no need to check this m_Ingredient array
			}
			if(!found) return false;// no match within an m_Ingredient array, no reason to continue the search, recipe is invalid
		}
		if(found)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void InsertIngredient(int index, string ingredient)
	{
		array<string> ptr = m_Ingredients[index];
		ptr.Insert(ingredient);
	}


	void AddResult(string item)
	{
		m_ItemsToCreate[m_NumberOfResults] = item;
		m_NumberOfResults++;
	}
	/*
	void AddResultColored(string item)
	{
		class_name = 
		color = 
		m_ItemsToCreate[m_NumberOfResults] = item;
		m_NumberOfResults++;
	}
	*/
	string GetName()
	{
		return m_Name;
	}
/*
	bool IsAction()
	{
		return m_IsActionType;
	}
*/
	bool IsInstaRecipe()
	{
		return m_IsInstaRecipe;
	}


	//spawns results in the world
	void SpawnItems(ItemBase ingredients[], PlayerBase player, array<ItemBase> spawned_objects/*out*/)
	{
		//vector item_position = player.GetPosition() + ( player.GetDirection() * 1 );
		Debug.Log("SpawnItems called","recipes");
		spawned_objects.Clear();//just to make sure
		EntityAI object = NULL;
		
		for(int i = 0; i < m_NumberOfResults; i++)
		{
			string item_to_spawn = m_ItemsToCreate[i];
			
			if( m_ResultInheritsColor[i] != -1 )
			{
				ItemBase item = ingredients[m_ResultInheritsColor[i]];
				string class_name = item.GetType();
				string color = item.ConfigGetString("color");
				string new_class_name = m_ItemsToCreate[i] + color;
				item_to_spawn = new_class_name;
			}
			
			if( m_ResultToInventory[i] == -1 ) 
			{
				Debug.Log("  = "+m_ResultToInventory[i].ToString(),"recipes");
				/*
				InventoryLocation inv_loc = new InventoryLocation;
				if (player.GetInventory().FindFirstFreeLocationForNewEntity(item_to_spawn, FindInventoryLocationType.ANY, inv_loc))
				{
					object = SpawnItemOnLocation(item_to_spawn, inv_loc, false);
				}
				*/
				object = player.GetInventory().CreateInInventory(item_to_spawn);
			}
			else if (m_ResultToInventory[i] >= 0)
			{
				/*
				object = player.SpawnEntityOnGroundOnCursorDir(item_to_spawn, 0.5);

				ItemBase item_swap_with = ingredients[m_ResultToInventory[i]];
				player.SwapEntities(true, item_swap_with, EntityAI.Cast(object));
				*/
			}
			else if ( m_ResultToInventory[i] == -2 )
			{
				object = player.SpawnEntityOnGroundOnCursorDir(item_to_spawn, 0.5);
			}
			if( !object )
			{
				//spawning in inventory failed, spawning on the ground instead.....
				object = player.SpawnEntityOnGroundOnCursorDir(item_to_spawn, 0.5);
			}
			spawned_objects.Insert(ItemBase.Cast(object));
			Debug.Log("spawning item "+item_to_spawn,"recipes");
		}

	}

	//applies final modifications to results
	void ApplyModificationsResults(ItemBase sorted[], array<ItemBase> results, ItemBase result, PlayerBase player)
	{
		float all_ingredinets_health = 0;//this is used later in results
		int value_delta;
		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			ItemBase ingrd = ItemBase.Cast(sorted[i]);
			all_ingredinets_health += ingrd.GetHealth("", "");//accumulate health of all ingredients, used in results	
		}
		//------------------- results ----------------------
		for(i = 0; i < m_NumberOfResults; i++)
		{
			ItemBase res = results.Get(i);
			
			bool use_soft_skills = m_ResultUseSoftSkills[i];
			
			if( res.IsItemBase() )
			{
				value_delta = m_ResultSetQuantity[i];

				ItemBase resIb = ItemBase.Cast(res);

				if( use_soft_skills ) 
				{
					value_delta = player.m_SoftSkillsManager.AddSpecialtyBonus(value_delta, m_Specialty);
				}
				
				if( !resIb.IsMagazine() )//is not a magazine
				{
					if( m_ResultSetFullQuantity[i] == 1 )//<------m_ResultSetFullQuantity
					{
						resIb.SetQuantityMax();
					}
					else if( value_delta != -1 )//<------m_ResultSetQuantity
					{
						resIb.SetQuantity( value_delta );
					}
				}
				else//is magazine
				{
					Magazine mgzn = Magazine.Cast(resIb);
					if( m_ResultSetFullQuantity[i] == 1 )//<------m_ResultSetFullQuantity
					{
						mgzn.ServerSetAmmoMax();
					}
					else if( value_delta != -1 )//<------m_ResultSetQuantity
					{
						mgzn.ServerSetAmmoCount( value_delta );
					}
				}
			}
			if( m_ResultSetHealth[i] != -1 )//<------m_ResultSetHealth
			{
				value_delta = m_ResultSetHealth[i];
				if( use_soft_skills ) 
				{
					value_delta = player.m_SoftSkillsManager.AddSpecialtyBonus(value_delta, m_Specialty);
				}
				res.SetHealth("","",value_delta);
			}
			if( m_ResultInheritsHealth[i] != -1 )//<------m_ResultInheritsHealth
			{
				if( m_ResultInheritsHealth[i] >= 0 )
				{
					int ing_number = m_ResultInheritsHealth[i];
					ItemBase ing = sorted[ing_number];
					
					if( ing )
					{
						float ing_health = ing.GetHealth("","");
						res.SetHealth("", "", ing_health);
						Debug.Log("Inheriting health from ingredient:"+m_ResultInheritsHealth[i].ToString(),"recipes");
					}
				}
				else if( m_ResultInheritsHealth[i] == -2 )
				{
					float average_health = all_ingredinets_health / MAX_NUMBER_OF_INGREDIENTS;
					res.SetHealth("", "", average_health);
				}
			}

			if( m_ResultReplacesIngredient[i] != -1 )//<------ResultReplacesIngredient
			{
				if( m_ResultReplacesIngredient[i] > -1 )
				{
					int ing_num = m_ResultReplacesIngredient[i];
					ItemBase ingr = sorted[ing_num];
					
					if( ingr )
					{
						MiscGameplayFunctions.TransferItemProperties(ingr, res);
						MiscGameplayFunctions.TransferAttachments(ingr,res,player);
					}
				}
			}
		}
	}
	
	
	void DeleleIngredientsPass()
	{
		for(int i = 0; i < m_IngredientsToBeDeleted.Count(); i++)
		{
			ItemBase ingredient = m_IngredientsToBeDeleted.Get(i);
			ingredient.Delete();
		}
		m_IngredientsToBeDeleted.Clear();
	}
	
	//applies final modifications to ingredients
	void ApplyModificationsIngredients(ItemBase sorted[], PlayerBase player)
	{
		//---------------------- ingredients ----------------------
		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			ItemBase ingredient = sorted[i];
			
			if( m_IngredientDestroy[i] == 1 )//<------m_IngredientDestroy
			{
				if( ingredient ) m_IngredientsToBeDeleted.Insert(ingredient);
				//ingredient.Delete();
				//sorted[i] = NULL;
			}
			else
			{
				bool use_soft_skills = m_IngredientUseSoftSkills[i];
				
				if( m_IngredientAddHealth[i] != 0 )//<------m_IngredientAddHealth
				{
					float health_delta = m_IngredientAddHealth[i];
					if( use_soft_skills ) 
					{
						if(health_delta <0) health_delta = player.m_SoftSkillsManager.SubtractSpecialtyBonus(health_delta, m_Specialty);
						else 				health_delta = player.m_SoftSkillsManager.AddSpecialtyBonus(health_delta, m_Specialty);
					}
					ingredient.AddHealth("","",health_delta);
				}
				else if(m_IngredientSetHealth[i] != -1)//<------m_IngredientSetHealth
				{
					float new_health = m_IngredientSetHealth[i];
					ingredient.SetHealth("","",new_health);
				}
				if( m_IngredientAddQuantity[i] != 0 )//<------m_IngredientAddQuantity
				{
					float quantity_delta = m_IngredientAddQuantity[i];
					
					if(	use_soft_skills )
					{
						if(quantity_delta <0) quantity_delta = player.m_SoftSkillsManager.SubtractSpecialtyBonus(quantity_delta, m_Specialty);
						else 				quantity_delta = player.m_SoftSkillsManager.AddSpecialtyBonus(quantity_delta, m_Specialty);
					}
						
					if( !ingredient.IsMagazine() )
					{
						ItemBase obj = ingredient;
						bool isDestroyed = obj.AddQuantity(quantity_delta, true);
						if( isDestroyed ) 
						{
							//if (obj) m_IngredientsToBeDeleted.Insert(obj);
							continue;
						}
					}
					else
					{
						Magazine mag = Magazine.Cast(ingredient);
						int newQuantity = mag.GetAmmoCount() + quantity_delta;
						if( newQuantity <= 0 )
						{
							if(mag) m_IngredientsToBeDeleted.Insert(mag);
							continue;
						}
						else
						{
							mag.ServerSetAmmoCount( newQuantity );
						}
					}
				}
			}
		}
	}

	//checks the recipe conditions
	bool CheckConditions(ItemBase sorted[])
	{
		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			ItemBase ingredient = sorted[i];
			if( !ingredient.IsMagazine() )
			{
				if( ingredient.GetQuantityMax() !=0 && m_MinQuantityIngredient[i] >= 0 && ingredient.GetQuantity() < m_MinQuantityIngredient[i] )
				{
					//Debug.Log("Recipe condition check failing1: m_MinQuantityIngredient","recipes");
					return false;
				}
				if( m_MaxQuantityIngredient[i] >= 0 && ingredient.GetQuantity() > m_MaxQuantityIngredient[i] )
				{
					//Debug.Log("Recipe condition check failing1: m_MaxQuantityIngredient","recipes");
					return false;
				}
			}
			else
			{
				Magazine mag1 = Magazine.Cast(ingredient);
				if( m_MinQuantityIngredient[i] >= 0 && mag1.GetAmmoCount() < m_MinQuantityIngredient[i] )
				{
					//Debug.Log("Recipe condition check failing1: m_MinQuantityIngredient[0]","recipes");
					return false;
				}
				if( m_MaxQuantityIngredient[i] >= 0 && mag1.GetAmmoCount() > m_MaxQuantityIngredient[i] )
				{
					//Debug.Log("Recipe condition check failing1: m_MaxQuantityIngredient[0]","recipes");
					return false;
				}
			}
			int dmg3 = ingredient.GetHealthLabel();
			if( m_MinDamageIngredient[i] >= 0 && ingredient.GetHealthLabel() < m_MinDamageIngredient[i] )
			{
				int dmg = ingredient.GetHealthLabel();
				//Debug.Log("Recipe condition check failing1: m_MinDamageIngredient[0]","recipes");
				return false;
			}
			if( m_MaxDamageIngredient[i] >= 0 && ingredient.GetHealthLabel() > m_MaxDamageIngredient[i] )
			{
				int dmg2 = ingredient.GetHealthLabel();
				//Debug.Log("Recipe condition check failing1: m_MaxDamageIngredient[0]","recipes");
				return false;
			}
		}
		return true;
	}

	//checks overall validity of this recipe
	bool CheckRecipe(ItemBase item1, ItemBase item2, PlayerBase player)
	{
		if( item1 == NULL || item2 == NULL )
		{
			Debug.Log("recipe invalid, at least one of the ingredients is NULL","recipes");
			return false;
		}
		
		ItemBase item_in_hand = player.GetItemInHands();
		
		if( !IsRecipeAnywhere() && (item1 != item_in_hand && item2 != item_in_hand) )
		{
			Debug.Log("recipe invalid, not 'IsRecipeAnywhere' and neither ingredience is in hands","recipes");
			return false;			
		}
		
		m_IngredientsSorted[0] = item1;
		m_IngredientsSorted[1] = item2;
	
		if( CanDo( m_IngredientsSorted, player ) && CheckConditions( m_IngredientsSorted ) )
		{
			return true;
		}
		return false;
	}
	
	void OnSelectedRecipe(ItemBase item1, ItemBase item2, PlayerBase player)
	{
		if( item1 == NULL || item2 == NULL )
		{
			Error("CheckRecipe: recipe invalid, at least one of the ingredients is NULL");
			Debug.Log("recipe invalid, at least one of the ingredients is NULL","recipes");
			return;
		}
		OnSelected(item1,item2,player);
	}
	
	void OnSelected(ItemBase item1, ItemBase item2, PlayerBase player)
	{
		
	}

	//performs this recipe
	void PerformRecipe(ItemBase item1, ItemBase item2, PlayerBase player)
	{
		if( item1 == NULL || item2 == NULL )
		{
			Error("PerformRecipe: recipe invalid, at least one of the ingredients is NULL");
			Debug.Log("PerformRecipe: at least one of the ingredients is NULL","recipes");
		}
		/*
		m_IngredientsSorted[0] = item1;
		m_IngredientsSorted[1] = item2;
		*/
		
		/*
		Debug.Log("PerformRecipe Ingredient 1: "+ToString(item1.Ptr().GetType()),"recipes");
		Debug.Log("PerformRecipe Ingredient 2: "+ToString(item2.Ptr().GetType()),"recipes");
		*/
		if( CheckRecipe(item1,item2,player) )
		{
			ref array<ItemBase> spawned_objects = new array<ItemBase>;
			SpawnItems(m_IngredientsSorted, player,spawned_objects );
			
			ApplyModificationsResults(m_IngredientsSorted, spawned_objects, NULL, player);
			ApplyModificationsIngredients(m_IngredientsSorted, player);
			
			ApplySoftSkillsSpecialty(player);
			
			Do( m_IngredientsSorted, player, spawned_objects, m_Specialty );
			
			
			DeleleIngredientsPass();
		}
		else
		{
			Debug.Log("CheckRecipe failed on server","recipes");
		}
	}

	void ApplySoftSkillsSpecialty(PlayerBase player)
	{
		player.GetSoftSkillManager().AddSpecialty(m_Specialty);
	}	
	
	bool CanDo(ItemBase ingredients[], PlayerBase player)
	{
		return true;
	}

	void Do(ItemBase ingredients[], PlayerBase player, array<ItemBase> results, float specialty_weight)
	{
		Debug.Log("Called Do on a recipe id:" + m_ID.ToString(),"recipes");
	}

	int GetID()
	{
		return m_ID;
	}


	void SetID(int id)
	{
		m_ID = id;
	}

	void GetAllItems(array<string> items)
	{
		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			array<string> ptr = m_Ingredients[i];
			
			for(int x = 0; x < ptr.Count(); x++)
			{
				items.Insert( ptr.Get(x) );
			}
		}
	}

	bool IsItemInRecipe(string item)
	{
		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			array<string> ptr = m_Ingredients[i];
			
			for(int x = 0; x < ptr.Count(); x++)
			{
				if( ptr.Get(x) == item ) return true;
			}
		}
		return false;
	}
	//! returns a mask which marks ingredient positions for a given item in this recipe(for example mask of value 3 [....000011] means this item is both ingredient 1 and 2 in this recipe[from right to left])
	int GetIngredientMaskForItem(string item)
	{
		int mask = 0;
		
		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			array<string> ptr = m_Ingredients[i];
			
			for(int x = 0; x < ptr.Count(); x++)
			{
				if( ptr.Get(x) == item )
				{
					mask = ((int)Math.Pow(2, i)) | mask;
				}
			}
		}
		return mask;
	}
}
