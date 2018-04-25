class CAContinuousCraft : CAContinuousTime
{		
	override void Setup( PlayerBase player, ActionTarget target, ItemBase item  )
	{
		m_TimeElpased = 0;
		m_TimeToComplete = 1000; //indication of arror if somting will be craft 1000 sec
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>(0);
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}
		int recipeID = player.GetCraftingRecipeID();		
		
		PluginRecipesManager module_recipes_manager;
		Class.CastTo(module_recipes_manager, GetPlugin(PluginRecipesManager));
		if( module_recipes_manager )
		{
			m_TimeToComplete = module_recipes_manager.GetRecipeLengthInSecs( recipeID );
			if( module_recipes_manager.GetIsInstaRecipe(recipeID) || module_recipes_manager.IsEnableDebugCrafting() )
			{
				m_TimeToComplete = 0;
			}
			float specialty_weight = module_recipes_manager.GetRecipeSpecialty( recipeID );
			m_TimeToComplete = player.GetSoftSkillManager().AdjustCraftingTime( m_TimeToComplete, specialty_weight );
			
			//PrintString("ttc:" + m_TimeToComplete.ToString());
		}
	}
	
	override int Execute( PlayerBase player, ActionTarget target, ItemBase item  )
	{
		if ( !player )
		{
			return UA_ERROR;
		}
		
		if ( m_TimeElpased < m_TimeToComplete )
		{
			m_TimeElpased += player.GetDeltaT();
		}
		else
		{
			if ( m_SpentUnits )
			{
				m_SpentUnits.param1 = m_TimeElpased;
				SetACData(m_SpentUnits);
			}
			return UA_FINISHED;
		}
		return UA_PROCESSING;
	}
	
	override float GetProgress()
	{	
		if( m_TimeToComplete > 0 )
		{
			//float progress = m_TimeElpased/m_TimeToComplete;
			return m_TimeElpased/m_TimeToComplete;
		}
		return 1;
	}
};