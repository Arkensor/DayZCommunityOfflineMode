class ActionMineBushCB : ActionContinuousBaseCB
{
	private const float TIME_BETWEEN_MATERIAL_DROPS = 3;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousMineWood(TIME_BETWEEN_MATERIAL_DROPS);
	}
};

class ActionMineBush: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.03;

	void ActionMineBush()
	{
		m_CallbackClass = ActionMineBushCB;
		m_MessageStartFail = "My tool is too damaged to cut.";
		m_MessageStart = "I started cutting the bush down.";
		m_MessageSuccess = "I have cut the bush down.";
		m_MessageFail = "My tool is ruined.";
		//m_AnimationOneHanded = "DRINK";
		//m_AnimationTwoHanded = "DIGGINGSHOVEL";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BERRIES;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_MINE_BUSH;
	}
		
	override string GetText()
	{
		return "cut the bush down";
	}

		
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		if ( targetObject.IsBush() ) 
		{
			return true;
		}
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};