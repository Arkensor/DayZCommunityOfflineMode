class ActionFertilizeSlotCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 10;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousFertilizeGardenSlot(QUANTITY_USED_PER_SEC);
	}
};

class ActionFertilizeSlot: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.02;
	
	void ActionFertilizeSlot()
	{
		m_CallbackClass = ActionFertilizeSlotCB;
		m_MessageSuccess = "I've fertilized slot.";
		m_MessageStartFail = "There's not enough fertilizer.";
		m_MessageStart = "I've started fertilizing.";
		m_MessageFail = "There's not enough fertilizer.";
		m_MessageCancel = "I stoped fertilizing.";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTDummy;
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_FERTILIZE_SLOT;
	}
		
	override string GetText()
	{
		return "Fertilize slot";
	}


	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		GardenBase garden_base;
		if ( Class.CastTo(garden_base, target.GetObject()))
		{
			string selection = garden_base.GetActionComponentName(target.GetComponentIndex());
			
			if ( garden_base.IsCorrectFertilizer( item, selection ) && garden_base.NeedsFertilization( selection ) )
			{
				if ( item.GetQuantity() > 0 )
				{
					return true;
				}
			}
		}
		return false;
	}
	
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		// The functionality is in the Execute event of this user action's component.
	}
};