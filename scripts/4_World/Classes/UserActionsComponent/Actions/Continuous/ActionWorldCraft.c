class ActionWorldCraftCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousCraft(UATimeSpent.DEFAULT_CRAFT); //default value can be set in recipes
	}

	/*override void OnFinish(bool pCanceled)	
	{
		super.OnFinish(pCanceled);
		if( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			PlayerBase player;
			if( Class.CastTo(player, GetGame().GetPlayer()) )
			{
				if( player.GetCraftingManager().IsInventoryCraft() )
					player.GetCraftingManager().CancelInventoryCraft();
			}
		}
	}*/
};

class ActionWorldCraft: ActionContinuousBase
{
	private string m_ActionPrompt;

	void ActionWorldCraft()
	{
		m_CallbackClass = ActionWorldCraftCB;		
		m_MessageStartFail = "Ooops";
		m_MessageStart = "I have started crafting.";
		m_MessageSuccess = "I crafted something!";
		m_MessageFail = "Oops again..";
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_ActionPrompt = "Craft";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_WORLD_CRAFT;
	}
		
	override string GetText()
	{
		PlayerBase player;
		if( Class.CastTo(player, GetGame().GetPlayer()) )
		{
			PluginRecipesManager module_recipes_manager;
			Class.CastTo(module_recipes_manager,  GetPlugin(PluginRecipesManager) );
			return module_recipes_manager.GetRecipeName( player.GetCraftingManager().GetRecipeID() );
		}

		return "default worldcraft text";
	}
	
	override bool Can( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//Client
		if( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{					
			if ( /*player.GetCraftingManager().IsWorldCraft() || player.GetCraftingManager().IsInventoryCraft()*/ player.GetCraftingManager().GetRecipesCount() > 0 )
			{
				return true;
			}
			return false;
		}
		//Server
		return true;		
	}
			
	/*override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{	
			//Client	
			if ( player.GetCraftingManager().IsWorldCraft() || player.GetCraftingManager().IsInventoryCraft() )
			{
				PluginRecipesManager module_recipes_manager = GetPlugin(PluginRecipesManager);
				m_ActionPrompt = module_recipes_manager.GetRecipeName( player.GetCraftingManager().GetRecipeID() );
				return true;
			}
			return false;
		}
		else
		{
			//Server
			return true;
		}
				
		return false;
	}*/

	
	override void Start( PlayerBase player, ActionTarget target, ItemBase item ) //Setup on start of action
	{
		super.Start(player, target, item);	
		if( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			ref ActionTarget newTarget;
			newTarget = new ActionTarget(player.GetCraftingManager().m_item2, null, -1, vector.Zero, 0);		
			m_Callback.SetActionData( this, player, newTarget, player.GetCraftingManager().m_item1, GetStanceMask(player) ); 
		}
		if ( item ) item.SetInvisible(true);
	}
	
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		if ( item ) item.SetInvisible(false);
	}
	
	override void OnCancelServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		if ( item ) item.SetInvisible(false);
	}
	
	override void OnCompleteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		if ( item ) item.SetInvisible(false);
	}
	
	override void OnCancelClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		if ( item ) item.SetInvisible(false);
	}	
	
	override void OnCompleteLoopServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		PluginRecipesManager module_recipes_manager;
		Class.CastTo(module_recipes_manager,  GetPlugin(PluginRecipesManager) );
		ItemBase item2;
		Class.CastTo(item2,  target.GetObject() );
		module_recipes_manager.PerformRecipeServer( player.GetCraftingRecipeID(), item, item2, player );
	}
	
	override void WriteToContext (ParamsWriteContext ctx, ActionTarget target)
	{
		PlayerBase player;
		Class.CastTo(player, GetGame().GetPlayer());

		ctx.Write(INPUT_UDT_STANDARD_ACTION);
		ctx.Write(GetType());
		ctx.Write(player.GetCraftingManager().m_item1);
		ctx.Write(player.GetCraftingManager().m_item2);
		ctx.Write(player.GetCraftingRecipeID());
	}
};

