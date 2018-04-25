class ActionZoomIn: ActionSingleUseBase
{
	void ActionZoomIn() {}
	
	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_ZOOM_IN;
	}

	override string GetText()
	{
		return "Zoom in";
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ItemOptics optic;
		if( Class.CastTo(optic, item) && !optic.IsInOptics() )
		{ 
			return true;
		}
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		ItemOptics optic;
		if( Class.CastTo(optic, item) )
		{
			optic.EnterOptics(); 
		}
	}
};