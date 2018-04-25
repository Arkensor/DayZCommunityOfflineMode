class ActionSplintSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.APPLY_SPLINT);
	}
};

class ActionSplintSelf: ActionContinuousBase	
{
	private const float ROUGH_SPECIALTY_WEIGHT = -0.01;

	void ActionSplintSelf()
	{
		m_CallbackClass = ActionSplintSelfCB;
		m_MessageStartFail = "There's no splint left";
		m_MessageStart = "I have started fixing myself";
		m_MessageSuccess = "I have fixed myself.";
		m_MessageFail = "I have moved and fixing was canceled.";
		m_MessageCancel = "I stopped fixing.";
		//m_Animation = "bandage";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_SPLINT_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "apply splint";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		item.TransferModifiers(player);
		player.ApplySplint();
		player.m_NotifiersManager.DetachByType(NTF_FRACTURE);
		item.Delete();

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};