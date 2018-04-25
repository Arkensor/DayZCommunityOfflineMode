class CacheObject
{
	ref array<int> m_Recipes;
	ref array<int> m_RecipeMasks;
	ref array<int> m_RecipeBitCount;

	void CacheObject()
	{
		m_Recipes = new array<int>;
		m_RecipeMasks = new array<int>;
		m_RecipeBitCount = new array<int>;
	}

	void AddRecipe(int recipe, int mask)
	{
		m_Recipes.Insert(recipe);
		m_RecipeMasks.Insert(mask);
		m_RecipeBitCount.Insert(GetNumberOfSetBits(mask));
	}

	void UpdateMask(int recipe_id, int new_mask)
	{
		for(int i = 0; i < m_Recipes.Count(); i++)
		{
			if( m_Recipes.Get(i) == recipe_id )
			{
				m_RecipeMasks.Set(i,new_mask);
				m_RecipeBitCount.Set(i,GetNumberOfSetBits(new_mask));

			}
		}
	}

	
	int GetNumberOfRecipes()
	{
		return m_Recipes.Count();
	}
	
	array<int> GetRecipes()
	{
		return m_Recipes;
	}
	
	array<int> GetMasks()
	{
		return m_RecipeMasks;
	}
	
	bool IsContainRecipe(int id)
	{
		for(int i = 0; i < m_Recipes.Count(); i++)
		{
			if( m_Recipes.Get(i) == id ) return true;
		}
		return false;
	}
	
	array<int> GetBitCount()
	{
		return m_RecipeBitCount;
	}

	int GetMaskByRecipeID(int id)//REWORK.V speed it up by using a map
	{
		for(int i = 0; i < m_Recipes.Count(); i++)
		{
			if( m_Recipes.Get(i) == id ) return m_RecipeMasks.Get(i);
		}
		return 0;
	}
	
	int GetBitCountByRecipeID(int id)//REWORK.V speed it up by using a map
	{
		for(int i = 0; i < m_Recipes.Count(); i++)
		{
			if( m_Recipes.Get(i) == id ) return m_RecipeBitCount.Get(i);
		}
		return 0;
	}
}