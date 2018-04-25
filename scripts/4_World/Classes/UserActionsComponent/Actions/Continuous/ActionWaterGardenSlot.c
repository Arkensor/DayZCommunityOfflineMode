class ActionWaterGardenSlotCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 150;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousWaterSlot(QUANTITY_USED_PER_SEC);
	}
};

class ActionWaterGardenSlot: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.01;
	
	void ActionWaterGardenSlot()
	{
		m_CallbackClass = ActionWaterGardenSlotCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTYBOTTLE;
		m_MessageSuccess = "I've watered slot.";
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
		return AT_WATER_GARDEN_SLOT;
	}
	
	override string GetText()
	{
		return "Water slot";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		
		if (item.GetQuantity() == 0)
			return false;
		
		if ( targetObject.IsInherited(GardenBase) )
		{
			GardenBase garden_base = GardenBase.Cast( targetObject );
			
			string selection = targetObject.GetActionComponentName(target.GetComponentIndex());
			
			Slot slot = garden_base.GetSlotBySelection( selection );
		
			if ( slot  &&  !slot.GetPlant()  &&  (slot.IsDigged() || slot.IsPlanted())  &&  slot.NeedsWater() ) // !slot.GetPlant() is here because we have separate user action for watering plants
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		
		return false;
	}
	
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		// The functionality is in the Execute event of this user action's component.
	}
};