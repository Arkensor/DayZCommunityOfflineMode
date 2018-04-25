class ActionIgniteFireplaceHandDrillCB : ActionContinuousBaseCB
{
	private const float TIME_TO_REPEAT_CHECK = 2;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTimeIgnite( UATimeSpent.FIREPLACE_HANDDRILL, TIME_TO_REPEAT_CHECK );
	}
}

class ActionIgniteFireplaceByHandDrill: ActionIgniteFireplace
{
	const float PRECISE_SPECIALTY_WEIGHT = -0.02;
	
	void ActionIgniteFireplaceByHandDrill()
	{
		m_CallbackClass = ActionIgniteFireplaceHandDrillCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STARTFIREDRILL;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;		
		m_MessageStartFail = "There's nothing to ignite.";
		m_MessageStart = "I have started igniting with hand drill.";
		m_MessageSuccess = "I have ignited the fireplace with a hand drill.";
		m_MessageFail = "I have canceled the igniting action.";
		m_MessageCancel = "I have stopped igniting the fireplace.";
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}

	override int GetType()
	{
		return AT_IGNITE_FIREPLACE_BY_HANDDRILL;
	}
		
	override string GetText()
	{
		return "ignite by hand drill";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		Object target_object = target.GetObject();
		FireplaceBase fireplace_target = FireplaceBase.Cast( target_object );
				
		//add damage to hand drill
		item.DecreaseHealth( "", "", m_HandDrillDamagePerUse, true );
		
		//remove grass
		Object clutter_cutter = GetGame().CreateObject ( fireplace_target.OBJECT_CLUTTER_CUTTER , target_object.GetPosition(), true, false );
		clutter_cutter.SetOrientation ( target_object.GetOrientation() );
		
		//start fire
		fireplace_target.StartFire();

		//add specialty
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
}