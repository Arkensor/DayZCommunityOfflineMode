class ActionDisinfectSelfCB : ActionSingleUseBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUseQuantity(UAQuantityConsumed.DISINFECT);
	}
};


class ActionDisinfectSelf: ActionSingleUseBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.01;
	float m_GramsConsumedPerUse;
	
	void ActionDisinfectSelf()
	{
		m_CallbackClass = ActionDisinfectSelfCB;
		m_MessageSuccess = "I disinfected myself";
		m_MessageStartFail = "Its' empty";
		//m_Animation = "lick";
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_DISINFECT_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "disinfect";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		//RemoveModifiers(target, item); ?

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		if ( item && item.GetQuantity() <= 0 ) 
		{
			item.SetQuantity(0);
		}
	}
};