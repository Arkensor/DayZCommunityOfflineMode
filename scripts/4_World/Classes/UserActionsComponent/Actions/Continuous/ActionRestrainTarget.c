class ActionRestrainTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.RESTRAIN);
	}
};

class ActionRestrainTarget: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = 0.02;
	
	void ActionRestrainTarget()
	{
		m_CallbackClass = ActionRestrainTargetCB;
		m_MessageStartFail = "Restraints are damage.";
		m_MessageStart = "Player started restraining you.";
		m_MessageSuccess = "Player finished restraining you.";
		m_MessageFail = "Player moved and restraining was canceled.";
		m_MessageCancel = "You stopped restraining.";
		//m_Animation = "restrain";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_TIEUPTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Sound = "action_handcuff_0";
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_RESTRAIN_T;
	}
		
	override string GetText()
	{
		return "restrain";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = PlayerBase.Cast(target.GetObject());
		//ntarget.SetCaptive();
		item.Delete();

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};