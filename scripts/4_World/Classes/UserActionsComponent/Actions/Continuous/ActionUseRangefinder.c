class ActionUseRangefinderCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime( 1.0 );
	}
};

class ActionUseRangefinder : ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = -0.01;
	
	void ActionUseRangefinder()
	{
		m_CallbackClass = ActionUseRangefinderCB;
		m_MessageStartFail = "ActionUseRangefinder - m_MessageStartFail";
		m_MessageStart = "ActionUseRangefinder - m_MessageStart";
		m_MessageSuccess = "ActionUseRangefinder - m_MessageSuccess";
		m_MessageFail = "ActionUseRangefinder - m_MessageFail";
		m_MessageCancel = "ActionUseRangefinder - m_MessageCancel";
		////m_TimeToCompleteAction = 999; // TO DO: Must be infinite!
		//m_Animation = "close";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;	
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override int GetType()
	{
		return AT_USE_RANGE_FINDER;
	}
		
	override string GetText()
	{
		return "use range finder";
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		return item.GetCompEM().CanWork() && Rangefinder.Cast( item ).IsInOptics();
	}
	
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		if (player)
		{
			string message = Rangefinder.Cast( item ).DoMeasurement(player);
			SendMessageToClient(player,message);
		}
		
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
}