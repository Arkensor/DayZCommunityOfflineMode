class ActionBurnSewSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.SEW_WOUNDS);
	}
};

class ActionBurnSewSelf: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.02;
	
	void ActionBurnSewSelf()
	{
		m_CallbackClass = ActionBurnSewSelfCB;
		m_MessageStartFail = "It's not possible.";
		m_MessageStart = "I have started closing my wounds";
		m_MessageSuccess = "I have closed my wounds.";
		m_MessageFail = "I have moved and healing was canceled.";
		m_MessageCancel = "I stopped closing my wounds.";
		//m_Animation = "bandage";
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_BURN_SEW_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "close wounds";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( item.GetTemperature() > 80 ) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		const float ITEM_DAMAGE = 0.05;
		const float SHOCK_AMOUNT = 1000;
		item.DecreaseHealth ( "", "", player.GetSoftSkillManager().SubtractSpecialtyBonus( ITEM_DAMAGE, this.GetSpecialtyWeight() )*100 );
		player.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_BLEEDING);		
		player.GetStatShock().Add( player.GetSoftSkillManager().SubtractSpecialtyBonus( SHOCK_AMOUNT, this.GetSpecialtyWeight() ) );
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};