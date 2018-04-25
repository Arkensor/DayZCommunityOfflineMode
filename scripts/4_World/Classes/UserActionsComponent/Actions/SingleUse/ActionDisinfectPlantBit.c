// TO DO: Remove this script!

class ActionDisinfectPlantBitCB : ActionSingleUseBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUseQuantity(UAQuantityConsumed.GARDEN_DISINFECT_PLANT);
	}
};


class ActionDisinfectPlantBit: ActionSingleUseBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = -0.01;
	
	void ActionDisinfectPlantBit()
	{
		m_CallbackClass = ActionDisinfectPlantBitCB;
		m_MessageSuccess = "";
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";
		
		//m_Animation = "open";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_DISINFECT_PLANT_BIT;
	}

	override string GetText()
	{
		return "Disinfect plant a bit";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		
		if ( targetObject != NULL && targetObject.IsInherited(PlantBase) && !item.IsDamageDestroyed() )
		{
			PlantBase plant = PlantBase.Cast( targetObject );
			
			if ( plant.IsGrowing() && plant.NeedsSpraying() )
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
		Object targetObject = target.GetObject();
		
		if ( targetObject != NULL && targetObject.IsInherited(PlantBase) )
		{
			
			PlantBase plant = PlantBase.Cast( targetObject );
			Param1<float> nacdata = Param1<float>.Cast( acdata );
			SendMessageToClient(player, plant.StopInfestation( nacdata.param1 ));
		}
		
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};