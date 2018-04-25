class ActionShaveCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.SHAVE);
	}
};

class ActionShave: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.01;

	void ActionShave()
	{
		m_CallbackClass = ActionShaveCB;
		m_MessageStartFail = "I have no beard to be shaved.";
		m_MessageStart = "I have started shaving myself.";
		m_MessageSuccess = "I have shaved myself.";
		m_MessageFail = "I have failed to shave myself.";
		m_MessageCancel = "I stopped shaving myself.";
		//m_Animation = "ignite";
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_SHAVE;
		m_FullBody = false;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( player.CanShave() )
		{		
			return true;
		}
			
		return false;
	}
	
	override bool HasTarget()
	{
		return false;
	}

	override int GetType()
	{
		return AT_SHAVE;
	}
		
	override string GetText()
	{
		return "Shave myself";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		player.ShavePlayer();
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};