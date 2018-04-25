class ActionShaveTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.SHAVE);
	}
};

class ActionShaveTarget: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.01;
	
	void ActionShaveTarget()
	{
		m_CallbackClass = ActionShaveTargetCB;
		m_MessageStartFail = "There's no beard to be shaved.";
		m_MessageStart = "I have started shaving target.";
		m_MessageSuccess = "I have shaved target.";
		m_MessageFail = "I have failed to shave target.";
		m_MessageCancel = "I stopped shaving target.";
		//m_Animation = "ignite";
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		PlayerBase man;
		Class.CastTo(man,  target.GetObject() );
		
		if ( man.CanShave() )
		{		
			return true;
		}

		return false;
	}

	override int GetType()
	{
		return AT_SHAVE_T;
	}
		
	override string GetText()
	{
		return "Shave target";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		PlayerBase man;
		if ( target && target.GetObject() && Class.CastTo(man, target.GetObject()) )
		{
			man.ShavePlayer();
			player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		}
	}
};