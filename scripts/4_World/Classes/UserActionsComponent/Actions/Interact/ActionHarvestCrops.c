class ActionHarvestCrops: ActionInteractBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.02;

	void ActionHarvestCrops()
	{
		m_MessageSuccess = "";
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";

		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}

	override int GetType()
	{
		return AT_HARVEST_CROPS;
	}

	override string GetText()
	{
		return "harvest crops";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		if ( targetObject.IsInherited(PlantBase) )
		{
			PlantBase plant = PlantBase.Cast( targetObject );
			
			if (  ( plant.IsMature()  ||  plant.IsSpoiled() )  &&  plant.HasCrops()  )
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object targetObject = target.GetObject();
		PlantBase plant = PlantBase.Cast( targetObject );
		m_MessageSuccess = plant.Harvest( player );

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};