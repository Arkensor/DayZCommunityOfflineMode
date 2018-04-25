//! Client only - manage set up crafting on client 
class CraftingManager
{
	
	PlayerBase m_player;
	PluginRecipesManager m_recipesManager; 
	int m_recipeID;
	int m_contextualRecipeID;
	int m_recipeCount;
	int m_craftingMode;
	ItemBase m_item1;
	ItemBase m_item2;


const int CM_MODE_NONE = 0;
const int CM_MODE_WORLD = 1;
const int CM_MODE_INVENTORY = 2;


	void CraftingManager(PlayerBase player, PluginRecipesManager recipesManager)
	{
		m_recipesManager = recipesManager;
		m_player = player;
		m_craftingMode = CM_MODE_NONE;
	}

	void SetRecipeID(int recipeID)
	{
		m_recipeID = recipeID;
	}
	
	int GetRecipeID()
	{
		return m_recipeID;
	}
	
	bool IsInventoryCraft()
	{
		return m_craftingMode == CM_MODE_INVENTORY;
	}
	
	bool IsWorldCraft()
	{
		return m_craftingMode == CM_MODE_WORLD;
	}
	
	int GetRecipesCount()
	{
		return 	m_recipeCount;
	}
	 
	void SetNextRecipe()
	{
		if(m_craftingMode == CM_MODE_WORLD || m_craftingMode == CM_MODE_INVENTORY )
		{
			ref array<int> recipes = new array<int>;
			if( m_recipesManager.GetValidRecipes(m_item1,m_item2,recipes, m_player) > m_contextualRecipeID + 1 )
			{
				m_contextualRecipeID++;	
			}
			else
			{
				m_contextualRecipeID = 0;				
			}
			m_recipeID = recipes.Get(m_contextualRecipeID);
		}
	}
	
	void OnUpdate(ItemBase item1, ItemBase item2)
	{
		if( m_player.GetActionManager().GetRunningAction() )
			return;
		
		ref array<int> recipes = new array<int>;
		int recipeCount = m_recipesManager.GetValidRecipes(item1,item2,recipes, m_player);

		if(recipeCount == 0)
		{
			m_recipeCount = 0;
			m_craftingMode = CM_MODE_NONE;					
		}
		else
		{
			if( m_craftingMode == CM_MODE_NONE || m_recipeCount != recipeCount  || m_item1 != item1  || m_item2 != item2 )
			{
				m_craftingMode = CM_MODE_WORLD;
				m_recipeCount = recipeCount;
				m_contextualRecipeID = 0;
				m_item1 = item1;
				m_item2 = item2;
			}
			m_recipeID = recipes.Get(m_contextualRecipeID);			
		}
		
	}
	
	bool SetInventoryCraft(int recipeID, ItemBase item1, ItemBase item2)
	{
		ref array<int> recipes = new array<int>;
		int recipeCount = m_recipesManager.GetValidRecipes(item1,item2,recipes, m_player);
		
		for( int i = 0; i < recipeCount; i++ )
		{
			if(recipeID == -1 || recipes.Get(i) == recipeID)
			{
				if( m_recipesManager.GetIsInstaRecipe(recipes.Get(i)) || m_recipesManager.IsEnableDebugCrafting() )
				{
					ref Param craftParam = new Param3<int, ItemBase, ItemBase>(recipes.Get(i), item1, item2);
					m_player.RPCSingleParam(ERPCs.RPC_CRAFTING_INVENTORY_INSTANT, craftParam, true, m_player.GetIdentity());
					return true;
				}
				else
				{
					m_craftingMode = CM_MODE_INVENTORY;
					m_recipeCount = recipeCount;
					m_contextualRecipeID = i;
					m_item1 = item1;
					m_item2 = item2;
					m_recipeID = recipes.Get(i);

					if( m_player.GetItemInHands() == item1) m_player.GetInventoryActionHandler().SetAction(InventoryActionHandler.IAH_CONTINUOUS, AT_WORLD_CRAFT, item2);
					else m_player.GetInventoryActionHandler().SetAction(InventoryActionHandler.IAH_CONTINUOUS, AT_WORLD_CRAFT, item1);
					
					return true;
				}		
			}			
			
		}	
		return false;
	}
	
	bool IsEnableDebugCrafting()
	{
		return true;
	}

}
