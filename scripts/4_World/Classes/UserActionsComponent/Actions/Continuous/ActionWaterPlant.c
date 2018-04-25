class ActionWaterPlantCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 150;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousWaterPlant(QUANTITY_USED_PER_SEC);
	}
};

class ActionWaterPlant: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.01;

	void ActionWaterPlant()
	{
		m_CallbackClass = ActionWaterPlantCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTYBOTTLE;
		m_MessageSuccess = "I've watered plant.";
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINotRuinedAndEmpty;
	}

	override int GetType()
	{
		return AT_WATER_PLANT;
	}
		
	override string GetText()
	{
		return "Water plant";
	}

	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		
		if ( targetObject != NULL && targetObject.IsInherited(PlantBase) && item != NULL && !item.IsDamageDestroyed() )
		{
			PlantBase plant = PlantBase.Cast( targetObject );
			
			if ( plant.NeedsWater() && item.GetQuantity() > 0 )
			{
				return true;
			}
		}
		
		return false;
	}
	
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object targetObject = target.GetObject();
		
		if ( targetObject != NULL && targetObject.IsInherited(PlantBase) )
		{
			PlantBase plant = PlantBase.Cast( targetObject );
			Param1<float> nacdata = Param1<float>.Cast( acdata );
			float water = nacdata.param1;
			Slot slot = plant.GetSlot();
			water = player.GetSoftSkillManager().AddSpecialtyBonus( water, this.GetSpecialtyWeight() );
			SendMessageToClient(player, slot.GiveWater( item, water ) );
			player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		}
	}
};