class ActionUnrestrainTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.UNRESTRAIN);
	}
};

class ActionUnrestrainTarget: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = -0.01;

	void ActionUnrestrainTarget()
	{
		m_CallbackClass = ActionUnrestrainTargetCB;
		m_MessageStartFail = "It's broken.";
		m_MessageStart = "Player started unrestraining you using item.";
		m_MessageSuccess = "Player finished unrestraining you using item.";
		m_MessageFail = "Player moved and unrestraining was canceled.";
		m_MessageCancel = "You stopped unrestraining.";
		//m_Animation = "inject";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CUTTIESTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_UNRESTRAIN_T;
	}
		
	override string GetText()
	{
		return "unrestrain";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = PlayerBase.Cast(target.GetObject());
		//restraintarget
		item.Delete();
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};