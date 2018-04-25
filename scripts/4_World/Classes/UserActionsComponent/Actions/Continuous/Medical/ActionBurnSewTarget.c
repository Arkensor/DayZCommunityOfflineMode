class ActionBurnSewTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.SEW_WOUNDS);
	}
};

class ActionBurnSewTarget: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.02;
	
	void ActionBurnSewTarget()
	{
		m_CallbackClass = ActionBurnSewTargetCB;
		m_MessageStartFail = "It's not possible.";
		m_MessageStart = "Player started closing your wounds.";
		m_MessageSuccess = "Player finished closing your wounds.";
		m_MessageFail = "Player moved and closing wounds was canceled.";
		m_MessageCancel = "You stopped closing target's wounds.";
	//	m_Animation = "sew";
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_BURN_SEW_T;
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
		
	override string GetText()
	{
		return "close wounds";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		ntarget.m_ModifiersManager.DeactivateModifier(eModifiers.MDF_BLEEDING);
		ntarget.GetStatShock().Add(1000);
		item.DecreaseHealth ( "", "", 5 );

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};