class ActionCPRCB : ActionContinuousBaseCB
{
	private const float REPEAT_AFTER_SEC = 4;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousRepeat(REPEAT_AFTER_SEC);
	}
};

class ActionCPR: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.03;
	
	void ActionCPR()
	{
		m_CallbackClass = ActionCPRCB;
		m_MessageStartFail = "Target moved and canceled action.";
		m_MessageStart = "Player started reusiscitating you.";
		m_MessageSuccess = "Player revived you.";
		m_MessageFail = "Player moved and CPR was canceled.";
		m_MessageCancel = "You stopped CPR";
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CPR;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_CPR_T;
	}
		
	override string GetText()
	{
		return "CPR";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		return true;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		if (ntarget.GetBleedingManager() ) 
		{
			ntarget.GetBleedingManager().RemoveSingleBleedingSource();
		}
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};
