class ActionHandcuffTarget: ActionSingleUseBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = 0.02;
	
	void ActionHandcuffTarget()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HANDCUFFTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Sound = "action_handcuff_0";
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_HANDCUFF_T;
	}
		
	override string GetText()
	{
		return "Handcuff";
	}
	
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = PlayerBase.Cast(target.GetObject());
		//ntarget.SetCaptive();
		item.Delete();

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};