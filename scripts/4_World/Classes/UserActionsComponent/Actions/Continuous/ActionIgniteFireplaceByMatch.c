class ActionIgniteFireplaceByMatch: ActionIgniteFireplace
{
	const float ROUGH_SPECIALTY_WEIGHT = 0.01;
		
	void ActionIgniteFireplaceByMatch()
	{
		m_CallbackClass = ActionIgniteFireplaceCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STARTFIREMATCH;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
 		m_MessageStartFail = "There's nothing to ignite.";
		m_MessageStart = "I have started igniting the fireplace with a match.";
		m_MessageSuccess = "I have ignited the fireplace with a match.";
		m_MessageFail = "I have canceled the igniting action.";
		m_MessageCancel = "I have stopped igniting the fireplace.";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
		m_ConditionItem = new CCINotEmpty;
	}

	override int GetType()
	{
		return AT_IGNITE_FIREPLACE_BY_MATCH;
	}
		
	override string GetText()
	{
		return "ignite by match";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		Object target_object = target.GetObject();
		FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );
		
		//remove match
		item.AddQuantity ( -m_MatchSpentPerUse );
		
		//remove grass
		Object clutter_cutter = GetGame().CreateObject ( fireplace_target.OBJECT_CLUTTER_CUTTER , target_object.GetPosition(), true, false );
		clutter_cutter.SetOrientation ( target_object.GetOrientation() );
		
		//start fire
		fireplace_target.StartFire();

		//add specialty
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
}