class ActionCraftCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousCraft(0); //default value can be set in recipes
	}
};

class ActionCraft: ActionContinuousBase
{
	bool m_HasStarted;
		
	ItemBase m_Item1;
	ItemBase m_Item2;
		
	int m_RecipeID;
	
	void ActionCraft()
	{
		m_MessageStartFail = "Ooops";
		m_MessageStart = "I have started crafting.";
		m_MessageSuccess = "I crafted something!";
		m_MessageFail = "Oops again..";
		
		m_Sound = "craft_universal_0";
		
		m_CallbackClass = ActionCraftCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_CRAFT;
	}
		
	override string GetText()
	{
		return "craft";
	}	
		
	//override void OnStart( PlayerBase player, ActionTarget target, ItemBase item )
	//{
		/*
		if( player.GetCraftingMeta() ) 	
		{
			m_Item1 = player.GetCraftingMeta().GetIngredient1();
		 	m_Item2 = player.GetCraftingMeta().GetIngredient2();
			m_RecipeID = player.GetCraftingMeta().GetRecipeID();
			if(m_Item1 == NULL || m_Item2 == NULL || m_RecipeID < 0) return;
		}
		*/
		//m_HasStarted = true;
	//}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		/*if( !m_HasStarted )
		{
			if ( player.IsCraftingSetUp() )
			{
				return true;
			}
			return false;
		}
		else
		{
			return true;
		}*/
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		//player.PerformRecipe();
		//m_HasStarted = false;
	}
	
	override void ApplyModifiers( PlayerBase player, ActionTarget target, ItemBase item )
	{
	}

	override void OnCancelServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata  )
	{
		//m_HasStarted = false;
	}

	void CancelCraft(PlayerBase player)
	{
		//player.SetCraftingReady(false);
		//player.DisableCrafting();
	}
};