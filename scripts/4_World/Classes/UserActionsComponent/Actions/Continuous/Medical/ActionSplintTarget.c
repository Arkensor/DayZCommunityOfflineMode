class ActionSplintTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.APPLY_SPLINT);
	}
};

class ActionSplintTarget: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = -0.01;
	
	void ActionSplintTarget()
	{
		m_CallbackClass = ActionSplintTargetCB;
		m_MessageStartFail = "There's no splint left.";
		m_MessageStart = "Player started fixing you.";
		m_MessageSuccess = "Player finished fixing you.";
		m_MessageFail = "Player moved and fixing was canceled.";
		m_MessageCancel = "You stopped fixing.";
		//m_Animation = "splint";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGETARGET;
		m_FullBody = true;
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_SPLINT_T;
	}
		
	override string GetText()
	{
		return "apply splint";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		item.TransferModifiers(ntarget);
		ntarget.ApplySplint();
		ntarget.m_NotifiersManager.DetachByType(NTF_FRACTURE);
		item.Delete();

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};