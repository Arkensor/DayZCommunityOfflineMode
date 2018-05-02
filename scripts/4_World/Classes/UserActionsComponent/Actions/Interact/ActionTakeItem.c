class ActionTakeItem: ActionInteractBase
{
	string m_ItemName = "";
	ref InventoryLocation il;

	void ActionTakeItem()
	{
		m_MessageSuccess    = "";
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP;
		m_CommandUIDProne	= DayZPlayerConstants.CMD_ACTIONFB_PICKUP;
		m_HUDCursorIcon     = CursorIcons.LootCorpse;
		
		il = new InventoryLocation;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}	
	
	override int GetType()
	{
		return AT_TAKE_ITEM;
	}

	override string GetText()
	{
		return "Take ";
	}
	
	override bool HasProneException()
	{
		return true;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		return true;
	}
	
	override bool ActionConditionStart( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI entity;
	
		if ( Class.CastTo(entity, target.GetObject()) && !target.GetParent() )
		{
			if( entity && entity.IsItemBase() && player.GetInventory().CanAddEntityIntoInventory(entity) && entity.GetHierarchyRootPlayer() != player )
			{
				return true;
			}
		}
		return false;
	}
	
	override void WriteToContext (ParamsWriteContext ctx, ActionTarget target)
	{
		super.WriteToContext(ctx, target);
		il.WriteToContext(ctx);
	}
	
	override bool ReadFromContext(ParamsReadContext ctx, out ActionReceived actionReceived)
	{
		if(super.ReadFromContext(ctx, actionReceived))
			return il.ReadFromContext(ctx);
		return false;
	}
	
	
	
	override bool InventoryReservation(PlayerBase player, ActionTarget target, ItemBase item, out array<ref InventoryLocation> reservedLocations)
	{		
		bool success = true;
		InventoryLocation targetInventoryLocation = NULL;
		InventoryLocation handInventoryLocation = NULL;
		
		ItemBase targetItem;
		if ( ItemBase.CastTo(targetItem,target.GetObject()) )
		{

			
			
			player.GetInventory().FindFreeLocationFor( targetItem , FindInventoryLocationType.ANY, il );
			if ( !player.GetInventory().AddInventoryReservation( targetItem, il, 10000) )
			{
				success = false;
			}				
		}	
		
		if ( success )
		{
			if( il )
				reservedLocations.Insert(il);
		}
		
		return success;
	}
	
	override void OnExecuteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		ItemBase ntarget = ItemBase.Cast(target.GetObject());
		
		if (!GetGame().IsMultiplayer())
		{
			ActionManagerClient am = ActionManagerClient.Cast(player.GetActionManager());
			am.UnlockInventory(this);
		}
		/*InventoryLocation targetInventoryLocation = new InventoryLocation;
		ntarget.GetInventory().GetCurrentInventoryLocation(targetInventoryLocation);
		
		ntarget.GetInventory().TakeToDst(InventoryMode.PREDICTIVE,targetInventoryLocation, il);*/
		
		player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, ntarget);
	}
	
	override void OnExecuteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		ItemBase ntarget = ItemBase.Cast(target.GetObject());

		ActionManagerClient am = ActionManagerClient.Cast(player.GetActionManager());
		am.UnlockInventory(this);
		/*InventoryLocation targetInventoryLocation = new InventoryLocation;
		ntarget.GetInventory().GetCurrentInventoryLocation(targetInventoryLocation);
		
		ntarget.LocalTakeToDst(targetInventoryLocation, il);*/
		player.PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, ntarget);
	}
};