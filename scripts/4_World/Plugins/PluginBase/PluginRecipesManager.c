enum ERecipeSanityCheck
{
	IS_IN_PLAYER_INVENTORY = 1,
	NOT_OWNED_BY_ANOTHER_LIVE_PLAYER = 2,
	CLOSE_ENOUGH = 4,
}

const float ACCEPTABLE_DISTANCE = 5;

const int SANITY_CHECK_ACCEPTABLE_RESULT = ERecipeSanityCheck.NOT_OWNED_BY_ANOTHER_LIVE_PLAYER + ERecipeSanityCheck.CLOSE_ENOUGH;
class PluginRecipesManager extends PluginBase
{
	ref Timer m_TestTimer;
	const string KEYWORD_NEW_ITEM = "Item:";
	const string PATH_CACHE_FILE = "Scripts/Data/cache_recipes.cache";
	const int MAX_NUMBER_OF_RECIPES = 256;
	const int MAX_CONCURENT_RECIPES = 20;
	const int MASK_BOTH_INGREDIENTS = 3;
	
	const int MAX_INGREDIENTS = 5;
	
	int m_ResolvedRecipes[MAX_CONCURENT_RECIPES];
	
	bool m_EnableDebugCrafting = false;
	ItemBase m_Ingredients[MAX_INGREDIENTS];
	int m_IngredientBitMask[MAX_INGREDIENTS];
	int m_IngredientBitMaskSize[MAX_INGREDIENTS];
	
	int m_BitsResults[MAX_INGREDIENTS];
	int m_ResolvePasses;
	
	
	ItemBase m_ingredient1[MAX_CONCURENT_RECIPES];
	ItemBase m_ingredient2[MAX_CONCURENT_RECIPES];
	ItemBase m_ingredient3[MAX_CONCURENT_RECIPES];
	
	ItemBase m_sortedIngredients[MAX_NUMBER_OF_INGREDIENTS];
	
	ref array<int> m_RecipesMatched = new array<int>;
	
	RecipeBase m_Recipe;//member variable for max perf
	 
	CacheObject co_item_a;
	CacheObject co_item_b;
	
	array<int> item_a_recipes;
	array<int> item_b_recipes;
	
	array<int> item_a_masks;
	array<int> item_b_masks;
	
	
	ref map<string,ref CacheObject> m_CacheItemMap;//this is the final result of caching
	ref map<string,ref CacheObject> m_CacheBasesMap;//this is an optimization base map, used as an intermediary to speed up the cache generation by not looking up the already resolved base classes in recipes
	
	ref array<string> m_CachedItems;
	//array<string>	m_Items;
	int m_NumberOfRecipes = 0;

	int m_NumOfConfRecipes;
	
	
	ref RecipeBase m_RecipeList[MAX_NUMBER_OF_RECIPES];//all recipes
	
	ref Timer myTimer1;
	
	void PluginRecipesManager()
	{
		
		m_CacheItemMap = new map<string,ref CacheObject>;
		m_CacheBasesMap = new map<string,ref CacheObject>;
		
		m_NumberOfRecipes = 0;
		
		for(int i = 0; i < MAX_NUMBER_OF_RECIPES; i++)
		{
			m_RecipeList[i] = NULL;
		}
		
		CreateAllRecipes();
		
		myTimer1 = new Timer();
	}
	
	void ~PluginRecipesManager()
	{
	}
	

	bool IsEnableDebugCrafting()
	{
		return m_EnableDebugCrafting;
	}

	void SetEnableDebugCrafting(bool enable)
	{
		m_EnableDebugCrafting = enable;
	}


		string GetRecipeName(int recipe_id)
	{
		if( m_RecipeList[recipe_id] ) return m_RecipeList[recipe_id].GetName();
		return "";
	}


	//will fill the map 'ids' with valid recipes for the 'item1' and 'item2' items, where the key is the recipe ID, and the value is the recipe name
	int GetValidRecipes(ItemBase item1, ItemBase item2, array<int> ids, PlayerBase player)
	{
		if( (item1 == NULL || item2 == NULL) )
		{
			if(ids) ids.Clear();
			return 0;
		}
		
		m_Ingredients[0] = item1;
		m_Ingredients[1] = item2;
		
		return GetValidRecipesProper(2,m_Ingredients, ids, player);
	}
	
	int GetValidRecipesProper(int num_of_items, ItemBase items[], array<int> ids, PlayerBase player)
	{
		if(ids) ids.Clear();
		GetRecipeIntersection(num_of_items,items, m_RecipesMatched);
		int numOfRecipes = SortIngredients(num_of_items,items,m_ResolvedRecipes);
		//will return number of cached recipes for the 2 items being considered, 
		//and save their indexes in m_ResolvedRecipes, please note the m_ResolvedRecipes is a static array, 
		//and does not clear up with each query, so the number of recipes returned is critical to make sure we don't accidentally
		//mix in some other indexes from previous queries(and obviously loop only as far as we have to)
		//this also saves the ingredients in the correct assignment as ingredient 1/2 into m_ingredient1/m_ingredient2 arrays, these 3 arrays
		//therefore provide you with information per each index with: recipeid,ingredience1,ingredience2
		if( numOfRecipes == 0 ) return 0;
		int found = 0;
		RecipeBase p_recipe = NULL;
		for(int i = 0; i < numOfRecipes; i++)
		{
			p_recipe = m_RecipeList[m_ResolvedRecipes[i]];

			if( p_recipe.CheckRecipe(m_ingredient1[i],m_ingredient2[i], player) == true )
			{
				if(ids) ids.Insert( p_recipe.GetID() );
				found++;
			}
		}
		return found;
	}


	float GetRecipeLengthInSecs(int recipe_id)
	{
		if( m_RecipeList[recipe_id] ) return m_RecipeList[recipe_id].GetLengthInSecs();
		return 0;
	}

	float GetRecipeSpecialty(int recipe_id)
	{
		if( m_RecipeList[recipe_id] ) return m_RecipeList[recipe_id].GetSpecialty();
		return 0;
	}

	bool GetIsInstaRecipe(int recipe_id)
	{
		if( m_RecipeList[recipe_id] ) return m_RecipeList[recipe_id].IsInstaRecipe();
		else return false;
	}

	override void OnInit()
	{
		super.OnInit();
		ReadCacheFromFile(PATH_CACHE_FILE);//read the cache from a file
		//GenerateHumanReadableRecipeList();
	}


	void CallbackGenerateCache()
	{
		Debug.Log("CallbackGenerateCache","recipes");
		SaveCacheToFile(PATH_CACHE_FILE);//generate the cache and save it to a file
		ReadCacheFromFile(PATH_CACHE_FILE);
	}

	protected void GenerateRecipeCache(array<string> cached_items)
	{
		GetGame().ProfilerStart("RECIPE_CACHE");
		
		m_CacheBasesMap.Clear();
		m_CacheItemMap.Clear();
		
		ref TStringArray all_config_paths = new TStringArray;
		
		all_config_paths.Insert(CFG_VEHICLESPATH);
		all_config_paths.Insert(CFG_WEAPONSPATH);
		all_config_paths.Insert(CFG_MAGAZINESPATH);
		//Debug.Log("Got here 0","caching");
		string config_path;
		string child_name;
		int scope;
		bool isInRecipe;
		ref TStringArray full_path = new TStringArray;
		
		for(int i = 0; i < all_config_paths.Count(); i++)
		{
			config_path = all_config_paths.Get(i);
			int children_count = GetGame().ConfigGetChildrenCount(config_path);
			
			for(int x = 0; x < children_count; x++)
			{
				GetGame().ConfigGetChildName(config_path, x, child_name);
				scope = GetGame().ConfigGetInt( config_path + " " + child_name + " scope" );

				if ( scope == 2 )
				{
					GetGame().ConfigGetFullPath(config_path +" "+ child_name,/*out*/ full_path);
					EvaluateFullPathAgainstRecipes(full_path, cached_items);
				}
			}
		}
		SortRecipesOrderInCache();
		GetGame().ProfilerStop("RECIPE_CACHE");
	}

	protected void EvaluateFullPathAgainstRecipes(TStringArray full_path, array<string> cached_items)
	{
		ResolveBaseClasses(full_path);
		ResolveItemClasses(full_path);
		LeechRecipesFromBases(full_path);
	}

	
	protected void SortRecipesOrderInCache()
	{
		for(int i = 0; i < m_CacheItemMap.Count(); i++)
		{
			string key = m_CacheItemMap.GetKey(i);
			CacheObject value = m_CacheItemMap.GetElement(i);
			
			//Sort( value.GetRecipes(), value.GetRecipes().Count() ); //use the sort here
			
		}
	
	}

	
	protected void ResolveBaseClasses(TStringArray full_path)
	{
		int mask;
		string item;
		RecipeBase p_recipe;
		CacheObject co_base = NULL;
		for(int i = 1; i < full_path.Count(); i++)
		{
			item = full_path.Get(i);
			
			if( !m_CacheBasesMap.Contains( item ) )//resolve new base classes
			{
				for(int x = 0; x < MAX_NUMBER_OF_RECIPES; x++)
				{
					if (m_RecipeList[x] != NULL)
					{
						p_recipe = m_RecipeList[x];
						mask = p_recipe.GetIngredientMaskForItem( item );
						
						if( mask > 0 )
						{
							if(!co_base)
							{
								m_CacheBasesMap.Insert(item,new CacheObject);
								co_base = m_CacheBasesMap.Get(item);
							}
							co_base.AddRecipe(x, mask);
						}
					}
				}
			}
		}
		
	}
	
	protected void LeechRecipesFromBases(TStringArray full_path)
	{
		string item_name = full_path.Get(0);
		
		string base_name;
		
		RecipeBase p_recipe;
		
		array<int> temp_base_rcps_array = NULL;
		array<int> temp_base_msks_array = NULL;

		CacheObject co_item;
		for(int i = 1/*skip the item classname*/; i < full_path.Count(); i++)
		{
			base_name = full_path.Get(i);
			
			if( m_CacheBasesMap.Contains( base_name ) )
			{
				temp_base_rcps_array = m_CacheBasesMap.Get(base_name).GetRecipes();
				temp_base_msks_array = m_CacheBasesMap.Get(base_name).GetMasks();
				if( !m_CacheItemMap.Get(item_name) )
				{
					CreateNewCacheItem(item_name);
				}
				co_item = m_CacheItemMap.Get(item_name);
				for( int x = 0; x < temp_base_rcps_array.Count(); x++ )//base recipes
				{
					int recipe_id = temp_base_rcps_array.Get(x);
					int base_mask = temp_base_msks_array.Get(x);
					
					int item_mask = co_item.GetMaskByRecipeID(recipe_id);
						
					if( item_mask > 0 )
					{
						co_item.UpdateMask(recipe_id, base_mask | item_mask );
					}
					else
					{
						co_item.AddRecipe(recipe_id,base_mask);
					}
				}
				
			}
		}
	}
	
	//this will take the item class name and resolve it against all recipes
	protected void ResolveItemClasses(TStringArray full_path)
	{
		string item_name = full_path.Get(0);
		RecipeBase p_recipe;
		
		for(int i = 0; i < MAX_NUMBER_OF_RECIPES; i++)
		{
			if (m_RecipeList[i] != NULL)
			{
				p_recipe = m_RecipeList[i];
				int mask = p_recipe.GetIngredientMaskForItem( item_name );
				
	
				if( mask > 0)
				{
					if( m_CacheItemMap.Contains( item_name ) )//this should not happen as every item should be unique
					{
						m_CacheItemMap.Get(item_name).AddRecipe(i,mask);
					}
					else
					{
						CreateNewCacheItem(item_name);
						m_CacheItemMap.Get(item_name).AddRecipe(i,mask);
					}
				}
			}
		}
	}

	protected void CreateNewCacheItem(string item_name)
	{
		m_CacheItemMap.Insert(item_name, new CacheObject);
		m_CachedItems.Insert(item_name);
	}



	void PerformRecipeServer(int id, ItemBase item_a,ItemBase item_b ,PlayerBase player)
	{
		m_Ingredients[0] = item_a;
		m_Ingredients[1] = item_b;
		if( !item_a || !item_b ) return;
		SortIngredientsInRecipe(id, 2,m_Ingredients, m_sortedIngredients);

		bool is_recipe_valid = CheckRecipe(id,m_sortedIngredients[0],m_sortedIngredients[1],player);
		bool passed_sanity_check = RecipeSanityCheck(2,m_sortedIngredients,player);
		
		if(!is_recipe_valid || !passed_sanity_check) return;
		RecipeBase ptrRecipe = m_RecipeList[id];
		ptrRecipe.PerformRecipe(m_sortedIngredients[0],m_sortedIngredients[1],player);
		//player.RequestCraftingDisable();
		
	}
	
	void GenerateHumanReadableRecipeList()
	{
		FileHandle file = OpenFile("$profile:RecipeDump.txt", FileMode.WRITE);
		if( file == 0 )
		{
			//error message
			PrintString("failed to open file RecipeDump");
			return;
		}
		array<int> recipes = new array<int>;
		for(int i = 0; i < m_CacheItemMap.Count(); i++)
		{
			string key = m_CacheItemMap.GetKey(i);
			CacheObject value = m_CacheItemMap.GetElement(i);
			
			string line = key;
			recipes.Clear();
			recipes.InsertAll( value.GetRecipes() );
			
			//PrintString("Item: " + key);
			
			for(int x = 0; x < recipes.Count(); x++)
			{
				int recipe_id = recipes.Get(x);
				string recipe_name = GetRecipeName(recipe_id);
				line += "," +recipe_name;
			}
			FPrintln(file, line);
		}
		CloseFile(file);
	}
	
	bool CheckRecipeUnsorted(int id, ItemBase item_a,ItemBase item_b,PlayerBase player)
	{
		m_Ingredients[0] = item_a;
		m_Ingredients[1] = item_b;
		if( !item_a || !item_b ) return false;
		SortIngredientsInRecipe(id, 2,m_Ingredients, m_sortedIngredients);

		return CheckRecipe(id,m_sortedIngredients[0],m_sortedIngredients[1],player);
	}

	protected bool RecipeSanityCheck(int num_of_ingredients, InventoryItemBase items[], PlayerBase player)
	{
		int check_results[MAX_INGREDIENTS];

		for(int i = 0; i < num_of_ingredients;i++)
		{
			ItemBase item = items[i];
			Man item_owner_player = item.GetHierarchyRootPlayer();
			vector item_pos = item.GetPosition();
			vector player_pos = player.GetPosition();
			
			if(item_owner_player == player)
			{
				check_results[i] =  check_results[i] | ERecipeSanityCheck.IS_IN_PLAYER_INVENTORY;
			}
			
			if( item_owner_player == NULL || item_owner_player == player || !item_owner_player.IsAlive() )
			{
				check_results[i] = check_results[i] | ERecipeSanityCheck.NOT_OWNED_BY_ANOTHER_LIVE_PLAYER;			
			}
			
			if( vector.Distance(item_pos, player_pos ) < ACCEPTABLE_DISTANCE )
			{
				check_results[i] = check_results[i] | ERecipeSanityCheck.CLOSE_ENOUGH;			
			}
		}
		for(i = 0; i < num_of_ingredients;i++)
		{
			if( !((check_results[i] & SANITY_CHECK_ACCEPTABLE_RESULT) == SANITY_CHECK_ACCEPTABLE_RESULT))
			{
				return false;
			}
		}
		return true;
	}

	protected void RegisterRecipe(RecipeBase recipe, int id)
	{
		recipe.SetID(id);
		m_RecipeList[id] = recipe;
	}

	/*void PerformRecipeClientRequest(int id, ItemBase item_a, ItemBase item_b, PlayerBase player, int craft_type = AT_CRAFT)
	{
		player.RequestCraftingSetup(id,item_a,item_b, craft_type);			
	}*/

	protected bool CheckRecipe(int id, ItemBase item1, ItemBase item2, PlayerBase player)//requires ordered items
	{
		RecipeBase p_recipe = m_RecipeList[id];
		return p_recipe.CheckRecipe(item1,item2, player);
	}
	
	protected void RecipeSelected(int id, ItemBase item1, ItemBase item2, PlayerBase player)//requires ordered items
	{
		RecipeBase p_recipe = m_RecipeList[id];
		p_recipe.OnSelectedRecipe(item1,item2, player);
	}

	
	protected bool CheckMaskOverlay(int mask_a, int mask_b )
	{
		if( mask_a | mask_b == MASK_BOTH_INGREDIENTS ) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	/*void OnCraftingSetUpClient(CraftingMeta meta, PlayerBase player)
	{
		int id = meta.GetRecipeID();
		ItemBase item_a = meta.GetIngredient1();
		ItemBase item_b = meta.GetIngredient2();
		
		if(!item_a || !item_b) return;
		bool is_insta_recipe = GetIsInstaRecipe(id);
		
		m_Ingredients[0] = item_a;
		m_Ingredients[1] = item_b;

		SortIngredientsInRecipe(id, 2,m_Ingredients,m_sortedIngredients);
		
		RecipeSelected( id, m_sortedIngredients[0], m_sortedIngredients[1], player );

		if( (IsEnableDebugCrafting() && !GetGame().IsMultiplayer()) || (is_insta_recipe && !GetGame().IsMultiplayer()) )
		{
			PerformRecipeServer(id, item_a, item_b, player);
		}

		GetGame().GetUIManager().HideScriptedMenu(GetGame().GetUIManager().FindMenu(MENU_INVENTORY));			
	}*/

	
	/*void OnCraftingSetUpServer(CraftingMeta meta, PlayerBase player)
	{
		int id = meta.GetRecipeID();
		ItemBase item_a = meta.GetIngredient1();
		ItemBase item_b = meta.GetIngredient2();
		
		if(!item_a || !item_b) return;
		bool is_insta_recipe = GetIsInstaRecipe(id);
		
		m_Ingredients[0] = item_a;
		m_Ingredients[1] = item_b;

		SortIngredientsInRecipe(id, 2,m_Ingredients,m_sortedIngredients);
		
		if( IsEnableDebugCrafting() || is_insta_recipe )
		{
			PerformRecipeServer(id, item_a, item_b, player);
		}
	}*/
	
		
	protected void PrintCache()
	{
		for(int i = 0; i < m_CacheItemMap.Count(); i++)
		{
			string key = m_CacheItemMap.GetKey(i);
			CacheObject value = m_CacheItemMap.GetElement(i);
			ref array<int> recipes = new array<int>;

			recipes.InsertAll( value.GetRecipes() );
			PrintString("Item: " + key);
			
			for(int x = 0; x < recipes.Count(); x++)
			{
				PrintString("Recipe: " + recipes.Get(x).ToString());
			}
		}
	}
	//!sorts ingredients correctly as either first or second ingredient based on their masks
	protected bool SortIngredientsInRecipe(int id, int num_of_ingredients, ItemBase ingredients_unsorted[], ItemBase ingredients_sorted[] )
	{
		ClearResults();
		
		for(int i = 0; i < num_of_ingredients; i++)
		{
			CacheObject co_item = m_CacheItemMap.Get( ingredients_unsorted[i].GetType() );
			m_IngredientBitMask[i] = co_item.GetMaskByRecipeID(id);
			m_IngredientBitMaskSize[i] = co_item.GetBitCountByRecipeID(id);
		}
		
		bool result = ResolveIngredients(num_of_ingredients);
		
		if(result)
		{
			for(i = 0; i < num_of_ingredients; i++)
			{
				int index = Math.Log2( m_BitsResults[i]);
				ingredients_sorted[index] = ingredients_unsorted[ i ];
			}
		}
		//PrintResultMasks(num_of_ingredients);
		return result;
	}
	
	protected void ClearResults()
	{
		for(int i = 0; i < MAX_INGREDIENTS; i++)
		{
			m_BitsResults[i] = 0;			
		}	
			
	}
	
	protected bool ResolveIngredients(int num_of_ingredients, int passes = 0)
	{
		int rightmost_bit;
		int smallest = 99999;
		int smallest_index = 0;

		for(int i = 0; i < num_of_ingredients; i++)
		{
			int count = m_IngredientBitMaskSize[i];
			if( count != 0 && count < smallest)
			{
				smallest = m_IngredientBitMaskSize[i];
				smallest_index = i;
			}
		}
		
		rightmost_bit = m_IngredientBitMask[smallest_index] & (-m_IngredientBitMask[smallest_index]);
		m_BitsResults[smallest_index] = m_BitsResults[smallest_index] | rightmost_bit;
		
		for(int x = 0; x < num_of_ingredients; x++)
		{
			m_IngredientBitMask[x] = ~rightmost_bit & m_IngredientBitMask[x];
			m_IngredientBitMask[smallest_index] = 0;
			m_IngredientBitMaskSize[smallest_index] = 0;
		}
		
		// check validity
		int check_sum_vectical = 0;
		
		for(int z = 0; z < num_of_ingredients; z++)
		{
			check_sum_vectical = check_sum_vectical | m_IngredientBitMask[z];//vertical sum
			check_sum_vectical = check_sum_vectical | m_BitsResults[z];//vertical sum
			if((m_IngredientBitMask[z] | m_BitsResults[z]) == 0) return false;//horizontal check
		}
		
		if( check_sum_vectical != (Math.Pow(2, num_of_ingredients) - 1)) return false;//vertical check
		
		passes++;
		
		if(passes < num_of_ingredients) 
		{
			if( !ResolveIngredients(num_of_ingredients, passes) ) return false;
		}
		return true;
	}
	
	
	protected void PrintResultMasks(int num)
	{
		for(int i = 0; i < num; i++)
		{
			Debug.Log("results mask("+i.ToString()+") = " +m_BitsResults[i].ToString() );
		}
	}
	
	//!fills an array given as a parameter with recipe IDs which 'item_a' and 'item_b' share and where they can function as different ingredients, will return number of recipes in this array
	protected int GetRecipeIntersection(int num_of_ingredients, ItemBase items[], array<int> matches)
	{
		int count = 0;
		int smallest = 9999;
		int smallest_index = 0;
		matches.Clear();
		
		/*
		m_Ingredients[0] = item_a;
		m_Ingredients[1] = item_b;
		*/
		//find the item with smallest number of recipes
		CacheObject co_least_recipes;
		
		for(int i = 0; i < num_of_ingredients; i++)
		{
			CacheObject cobject = m_CacheItemMap.Get( items[i].GetType() );
			if(!cobject) 
			{
				return 0;
			}
			if(cobject.GetNumberOfRecipes() < smallest)
			{
				smallest = cobject.GetNumberOfRecipes();
				smallest_index = i;
				co_least_recipes = cobject;
			}
		}
		
		//look for matches
		array<int> recipes = co_least_recipes.GetRecipes();
		bool fail;
		for(int x = 0; x < recipes.Count(); x++)
		{
			int id = recipes.Get(x);
			fail = false;
			for(int z = 0; z < num_of_ingredients; z++)
			{
				if( z!= smallest_index)
				{
					CacheObject cobject2 = m_CacheItemMap.Get( items[z].GetType() );
					if( cobject2.IsContainRecipe(id) )
					{
						matches.Insert(id);
						count++;
					}
				}
			}
		}
		return count;
	}
	
	protected int SortIngredients(int num_of_ingredients, ItemBase items_unsorted[], int resolved_recipes[])
	{
		int count = 0;
		for(int i = 0; i < m_RecipesMatched.Count(); i++)
		{
			int recipe_id = m_RecipesMatched.Get(i);
			
				if(SortIngredientsInRecipe(recipe_id, num_of_ingredients, items_unsorted, m_sortedIngredients))//...and now we need to determine which item will be which ingredient number
				{
					resolved_recipes[count] = recipe_id;
					m_ingredient1[count] = m_sortedIngredients[0];
					m_ingredient2[count] = m_sortedIngredients[1];
					//m_ingredient3[count] = m_sortedIngredients[2];
					count++;
				}
		}
		return count;
	}
	
	
	protected void CreateAllRecipes()
	{
		#include "Scripts\4_World\Classes\Recipes\Recipes\_RecipeList.inc"
	}
	
	protected void ReadTest()
	{
		ReadCacheFromFile("PATH_CACHE_FILE");//read the cache from a file
	}

	
	protected void SaveCacheToFile(string filename)
	{
		FileHandle file = OpenFile(filename, FileMode.WRITE);
		if( file!=0 )
		{
			m_CachedItems = new array<string>;
			GenerateRecipeCache(m_CachedItems);
		
			for(int i = 0; i < m_CachedItems.Count(); i++)
			{
				string cache_key = m_CachedItems.Get(i);
				array<int> recipe_array = m_CacheItemMap.Get(cache_key).GetRecipes();
				array<int> mask_array = m_CacheItemMap.Get(cache_key).GetMasks();
				
				
				FPrintln(file, KEYWORD_NEW_ITEM);
				FPrintln(file,cache_key);
				
				for(int x = 0; x < recipe_array.Count(); x++)
				{
					int recipe_id = recipe_array.Get(x);
					int mask = mask_array.Get(x);
					FPrintln(file,"recipeID:");
					FPrintln(file,recipe_id);
					FPrintln(file,"mask:");
					FPrintln(file,mask);
				}
			}
			CloseFile(file);
			m_CachedItems.Clear();
		}
		else
		{
			Debug.Log("failed to open the cache file");
		}
	}
	
	protected void ReadCacheFromFile(string filename)
	{
		GetGame().ProfilerStart("RECIPE_CACHE_READ");
		Debug.Log("Reading cache from file","recipes");
		FileHandle file = OpenFile(filename, FileMode.READ);
//		Print(file);
		int line_index = 0;
		int num_of_items = 0;
		string cache_key;
		string curr_line;
		m_CacheItemMap.Clear();
		
		while(true)
		{
			int fg = FGets(file,curr_line);
			//PrintString("fg: "+ToString(fg));
			if( fg == -1) break;
			//Debug.Log("Got here","recipes");
			if( curr_line == KEYWORD_NEW_ITEM)
			{
				FGets(file,curr_line);//skip the keyword to get to the classname
				cache_key = curr_line;
				m_CacheItemMap.Insert(cache_key, new CacheObject);
			}
			else
			{
				FGets(file,curr_line);//skip keyword for recipeID
				int item = curr_line.ToInt();
				FGets(file,curr_line);//skip keyword for mask
				FGets(file,curr_line);//get the mask
				int mask = curr_line.ToInt();
				m_CacheItemMap.Get(cache_key).AddRecipe(item, mask);
			}
		}
		//PrintCache();
		CloseFile(file);
		GetGame().ProfilerStop("RECIPE_CACHE_READ");
	}
}