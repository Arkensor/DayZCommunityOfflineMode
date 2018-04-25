class ActionSewTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.SEW_CUTS);
	}
};

class ActionSewTarget: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.02;
	
	void ActionSewTarget()
	{
		m_CallbackClass = ActionSewTargetCB;
		m_MessageStartFail = "There's no sewing left.";
		m_MessageStart = "Player started sewing you.";
		m_MessageSuccess = "Player finished sewing you.";
		m_MessageFail = "Player moved and sewing was canceled.";
		m_MessageCancel = "You stopped sewing.";
		//m_Animation = "sew";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACTITEM;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_SEW_T;
	}
		
	override string GetText()
	{
		return "sew target's cuts";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{			
		const float ITEM_DAMAGE = 10;
		float delta = player.GetSoftSkillManager().SubtractSpecialtyBonus( ITEM_DAMAGE, this.GetSpecialtyWeight() );
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		
		ntarget.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_BLEEDING);
		item.AddHealth("GlobalHealth","Health",-delta);

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};