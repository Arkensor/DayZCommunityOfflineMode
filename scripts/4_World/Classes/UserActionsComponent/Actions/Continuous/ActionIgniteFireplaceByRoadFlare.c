class ActionIgniteFireplaceByRoadFlare: ActionIgniteFireplace
{
	const float ROUGH_SPECIALTY_WEIGHT = 0.01;
	
	void ActionIgniteFireplaceByRoadFlare()
	{
		m_CallbackClass = ActionIgniteFireplaceCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STARTFIRELIGHTER;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_MessageStartFail = "There's nothing to ignite.";
		m_MessageStart = "I have started igniting the fireplace with a road flare.";
		m_MessageSuccess = "I have ignited the fireplace with a road flare.";
		m_MessageFail = "I have canceled the igniting action.";
		m_MessageCancel = "I have stopped igniting the fireplace.";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}

	override int GetType()
	{
		return AT_IGNITE_FIREPLACE_BY_ROADFLARE;
	}
		
	override string GetText()
	{
		return "ignite by road flare";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object target_object = target.GetObject();
		if ( target_object && target_object.IsFireplace() )
		{
			FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );
			
			if ( !fireplace_target.IsBurning()  &&  item.HasEnergyManager()  &&  item.GetCompEM().IsWorking() ) 
			{
				if ( fireplace_target.IsBarrelWithHoles() ) 		//if barrel with holes
				{
					BarrelHoles_ColorBase barrel = BarrelHoles_ColorBase.Cast( fireplace_target );
					if ( !barrel.IsOpened() )
					{
						return false;
					}
				}
				
				return true;
			}
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		Object target_object = target.GetObject();
		FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );
		
		//remove grass
		Object clutter_cutter = GetGame().CreateObject ( fireplace_target.OBJECT_CLUTTER_CUTTER , target_object.GetPosition(), true, false );
		clutter_cutter.SetOrientation ( target_object.GetOrientation() );
		
		//start fire
		fireplace_target.StartFire();

		//add specialty
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
}