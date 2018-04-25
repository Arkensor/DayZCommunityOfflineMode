class ActionDisinfectTargetCB : ActionSingleUseBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUseQuantity(UAQuantityConsumed.DISINFECT);
	}

	bool CancelCondition()
	{
		if ( !m_Interrupted && (GetState() == STATE_LOOP_LOOP || GetState() == STATE_LOOP_LOOP2) )
		{	
			m_ActionData.Do(this,m_State,m_ActionComponent,m_Player,m_Target,m_Item);
		}
		return DefaultCancelCondition(); 
	}
	override void InitActionComponent()
	{
		super.InitActionComponent();
		EnableCancelCondition(true);
	}
};


class ActionDisinfectTarget: ActionSingleUseBase
{	
	private const float PRECISE_SPECIALTY_WEIGHT = -0.01;
		
	void ActionDisinfectTarget()
	{
		m_CallbackClass = ActionDisinfectTargetCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CLEANWOUNDSTARGET;
		m_StanceMask = DayZPlayerConstants.STANCEIDX_ERECT | DayZPlayerConstants.STANCEIDX_CROUCH;
		m_FullBody = true;
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_DISINFECT_T;
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