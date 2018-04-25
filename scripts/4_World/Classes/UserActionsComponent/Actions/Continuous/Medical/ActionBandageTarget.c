class ActionBandageTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.BANDAGE);
	}
};

class ActionBandageTarget: ActionContinuousBase
{
	private const float PRECISE_SPECIALTY_WEIGHT = -0.01;
	
	void ActionBandageTarget()
	{
		m_CallbackClass = ActionBandageTargetCB;
		m_MessageStartFail = "There's no bandage left.";
		m_MessageStart = "Player started bandaging you.";
		m_MessageSuccess = "Player finished bandaging you.";
		m_MessageFail = "Player moved and bandaging was canceled.";
		m_MessageCancel = "You stopped bandaging.";
		//m_Animation = "BANDAGET";
		m_Sounds.Insert("bandage_0");
		m_Sounds.Insert("bandage_1");
		m_Sounds.Insert("bandage_2");
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGETARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_BANDAGE_T;
	}
		
	override string GetText()
	{
		return "bandage target";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = PlayerBase.Cast(target.GetObject());
		if (ntarget.GetBleedingManager() )
		{
			ntarget.GetBleedingManager().RemoveSingleBleedingSource();
		}
		
		if (item.GetQuantity() > 0)
		{
			item.AddQuantity(-1,true);
		}

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};
