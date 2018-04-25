//can be eventually extended to allow switching for multiple action types?
class ActionWorldLiquidActionSwitch: ActionSingleUseBase
{
	string 	m_switch_to;
	void ActionWorldLiquidActionSwitch()
	{
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}
	
	override int GetType()
	{
		return AT_WORLD_LIQUID_ACTION_SWITCH;
	}
	
	override string GetText()
	{
		return "Switch to " + m_switch_to;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ItemBase target_item = ItemBase.Cast(target.GetObject());
		if ( target_item && item && target_item.IsKindOf("Bottle_Base") && item.IsKindOf("Bottle_Base") && !GetGame().IsInventoryOpen() ) //TODO find better condition than IsKindOf
		{
			if ( target_item.GetQuantity() > target_item.GetQuantityMin() && item.GetQuantity() < item.GetQuantityMax() && !player.GetLiquidTendencyDrain() && Liquid.CanFillContainer( item, target_item.GetLiquidType() ) )
			{
				if ( Liquid.CanFillContainer( target_item, item.GetLiquidType() ) )
				{
					m_switch_to = "Liquid Drain";
					return true;
				}
				else
				{
					player.SetLiquidTendencyDrain(true);
					return false;
				}
			}
			if ( item.GetQuantity() > item.GetQuantityMin() && target_item.GetQuantity() < target_item.GetQuantityMax() && player.GetLiquidTendencyDrain() && Liquid.CanFillContainer( target_item, item.GetLiquidType() ) )
			{
				if ( Liquid.CanFillContainer( item, target_item.GetLiquidType() ) )
				{
					m_switch_to = "Liquid Pour";
					return true;
				}
				else
				{
					player.SetLiquidTendencyDrain(false);
					return false;
				}
			}
		}
		return false;
	}
	
	override void Start( PlayerBase player, ActionTarget target, ItemBase item ) //Setup on start of action
	{
		bool state;
		state = player.GetLiquidTendencyDrain();
		player.SetLiquidTendencyDrain(!state);
		
		//Print(player.GetLiquidTendencyDrain());
	}
	
	override bool IsLocal()
	{
		return true;
	}
	
	override bool IsInstant()
	{
		return true;
	}
	
	override bool RemoveForceTargetAfterUse()
	{
		return false;
	}
};