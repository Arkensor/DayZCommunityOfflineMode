class ActionDefibrilateSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFIBRILATE);
	}
};

class ActionDefibrilateSelf: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.03;
	
	void ActionDefibrilateSelf()
	{
		m_CallbackClass = ActionDefibrilateSelfCB;
		m_MessageStartFail = "It's out of juice.";
		m_MessageStart = "I have started defibrilating myself";
		m_MessageSuccess = "I have defibrilated myself.";
		m_MessageFail = "I have moved and defibrilating was canceled.";
		m_MessageCancel = "I stopped defibrilating.";
		//m_Animation = "defibrilate";
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_DEFIBRILATE_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "defibrilate yourself";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Defibrillator defib;
		Class.CastTo(defib, item);
		
		if ( defib.IsCharged() ) 
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
		Defibrillator defib;
		Class.CastTo(defib, item);
		
		if ( defib.IsCharged() ) 
		{
			defib.Discharge(player);
			
			float regain_energy = player.GetSoftSkillManager().SubtractSpecialtyBonus( defib.GetEnergyNeededToCharge(), this.GetSpecialtyWeight() );
			regain_energy = defib.GetEnergyNeededToCharge() - regain_energy;
			
			ItemBase battery;
			
			if (Class.CastTo(battery, item.GetCompEM().GetEnergySource()))
			{
				battery.GetCompEM().AddEnergy( regain_energy );
			}
			else
			{
				DPrint("ERROR! Defibrillator has no battery! Defibrillator softskill bonus can't be applied!");
			}
		}
		else
		{
			player.MessageImportant ( m_MessageStartFail );		
		}
		
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};
