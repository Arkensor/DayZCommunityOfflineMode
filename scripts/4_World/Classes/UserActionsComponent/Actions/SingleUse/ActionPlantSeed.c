class ActionPlantSeed: ActionSingleUseBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.02;

	void ActionPlantSeed()
	{
		m_MessageSuccess = "";
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";
		//m_Animation = "open";
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTDummy;
	}

	override int GetType()
	{
		return AT_PLANT_SEED;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "Plant seed";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		GardenBase targetObject = GardenBase.Cast( target.GetObject() );
		
		if( targetObject )
		{
			string selection = targetObject.GetActionComponentName(target.GetComponentIndex());
			
			if ( item != NULL && item.GetQuantity() > 0 && targetObject.CanPlantSeed( selection ) )
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Process(player, target, item, acdata);
	}
	
	override void OnCompleteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Process(player, target, item, acdata);
	}

	void Process( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object targetObject = target.GetObject();
		
		if ( targetObject != NULL && targetObject.IsInherited(GardenBase) )
		{
			GardenBase garden_base = GardenBase.Cast( targetObject );
			string selection = targetObject.GetActionComponentName(target.GetComponentIndex());
			Slot slot = garden_base.GetSlotBySelection( selection );
			int slot_ID = slot.GetSlotId();
			player.PredictiveTakeEntityToTargetAttachmentEx(garden_base, item, slot_ID );
		}

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};