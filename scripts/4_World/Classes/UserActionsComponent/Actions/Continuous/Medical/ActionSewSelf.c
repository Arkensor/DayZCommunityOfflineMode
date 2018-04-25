class ActionSewSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.SEW_CUTS);
	}
};

class ActionSewSelf: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.02;

	void ActionSewSelf()
	{
		m_CallbackClass = ActionSewSelfCB;
		m_MessageStartFail = "There's no sewing left.";
		m_MessageStart = "I have started sewing myself";
		m_MessageSuccess = "I have sewed myself.";
		m_MessageFail = "I have moved and sewing was canceled.";
		m_MessageCancel = "I stopped sewing.";
		//m_Animation = "bandage";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STITCHUPSELF;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_SEW_S;
	}

	override bool HasTarget()
	{
		return false;
	}
			
	override string GetText()
	{
		return "sew cuts";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		const float ITEM_DAMAGE = 10;
		float delta = player.GetSoftSkillManager().SubtractSpecialtyBonus( ITEM_DAMAGE, this.GetSpecialtyWeight() );

		item.AddHealth("GlobalHealth","Health",-delta);
		player.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_BLEEDING);
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};