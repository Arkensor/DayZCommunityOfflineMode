/*
class CraftingMeta
{
	const int NUMBER_OF_ITEMS = 6;
	int 					m_RecipeID = -1;
	EntityAI 				m_Item1;
	EntityAI 				m_Item2;
	float 					m_RecipeAnimLength;
	vector 					m_CraftingInitialPos;
	float					m_SpecialtyWeight;
	
	void CraftingMeta( int id, EntityAI item1, EntityAI item2, vector init_pos, float time, float specialty_weight )
	{
		m_RecipeID 	= id;
		m_Item1 	= item1;
		m_Item2 	= item2;
		m_RecipeAnimLength = time;
		m_CraftingInitialPos = init_pos;
		m_SpecialtyWeight = specialty_weight;
	}
	
	int GetRecipeID()
	{
		return m_RecipeID;
	}
	
	EntityAI GetIngredient1()
	{
		return m_Item1;
	}
	
	EntityAI GetIngredient2()
	{
		return m_Item2;
	}
	
	vector GetInitPos()
	{
		return m_CraftingInitialPos;
	}
	
	float GetRecipeAnimLength()
	{
		return m_RecipeAnimLength;
	}
	
	float GetSpecialtyWeight()
	{
		return m_SpecialtyWeight;
	}

	void SerializeValues(array<ref Param> ser_obj)
	{
		ser_obj.Insert( new Param1<int>( GetRecipeID()) );
		ser_obj.Insert( new Param1<ItemBase>( GetIngredient1()) );
		ser_obj.Insert( new Param1<ItemBase>( GetIngredient2()) );
		ser_obj.Insert( new Param1<vector>( GetInitPos()) );
		ser_obj.Insert( new Param1<float>( GetRecipeAnimLength()) );
		ser_obj.Insert( new Param1<float>( GetSpecialtyWeight()) );
	}
	void DeserializeValuesCtx(ParamsReadContext ctx)
	{
		Param1<int> id = new Param1<int>(0);
		Param1<ItemBase> item1 = new Param1<ItemBase>(NULL);
		Param1<ItemBase> item2 = new Param1<ItemBase>(NULL);
		Param1<vector> pos = new Param1<vector>("0 0 0");
		Param1<float> length = new Param1<float>(0);
		Param1<float> weight = new Param1<float>(0);
		
		ctx.Read(id);
		ctx.Read(item1);
		ctx.Read(item2);
		ctx.Read(pos);
		ctx.Read(length);
		ctx.Read(weight);

		m_RecipeID = id.param1;
		m_Item1 = item1.param1;
		m_Item2 = item2.param1;
		m_CraftingInitialPos = pos.param1;
		m_RecipeAnimLength = length.param1;
		m_SpecialtyWeight = weight.param1;
	}
	void DeserializeValues(array<ref Param> ser_obj)
	{
		Param1<int> id 			= ser_obj.Get(0);
		m_RecipeID 				= id.param1;
		Param1<ItemBase> item1 	= ser_obj.Get(1);
		m_Item1 				= item1.param1;
		Param1<ItemBase> item2 	= ser_obj.Get(2);
		m_Item2 				= item2.param1;
		Param1<vector> pos 		= ser_obj.Get(3);
		m_CraftingInitialPos 	= pos.param1;
		Param1<float> length 	= ser_obj.Get(4);
		m_RecipeAnimLength 		= length.param1;
		Param1<float> weight 	= ser_obj.Get(5);
		m_SpecialtyWeight 		= weight.param1;
	}
}
*/