class ActionBandageSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.BANDAGE);
	}
};

class ActionBandageSelf: ActionContinuousBase
{	
	private const float PRECISE_SPECIALTY_WEIGHT = -0.01;

	void ActionBandageSelf()
	{
		m_CallbackClass = ActionBandageSelfCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		
		m_MessageStartFail = "There's nothing to bandage.";
		m_MessageStart = "I have started bandaging myself";
		m_MessageSuccess = "I have bandaged myself.";
		m_MessageFail = "I have moved and bandaging was canceled.";
		m_MessageCancel = "I stopped bandaging.";
		m_Sounds.Insert("bandage_0");
		m_Sounds.Insert("bandage_1");
		m_Sounds.Insert("bandage_2");	
		m_SpecialtyWeight = PRECISE_SPECIALTY_WEIGHT;
	}

	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}
		
	override int GetType()
	{
		return AT_BANDAGE_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "bandage";
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		if (player.GetBleedingManager() )
		{
			player.GetBleedingManager().RemoveSingleBleedingSource();	
		}
		
		if (item.GetQuantity() > 0)
		{
			item.AddQuantity(-1,true);
		}

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};
