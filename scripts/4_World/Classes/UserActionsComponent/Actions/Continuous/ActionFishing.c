class ActionFishingCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousFish;
	}
};

class ActionFishing: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = -0.02;

	void ActionFishing()
	{
		m_CallbackClass = ActionFishingCB;
		m_Sounds.Insert("FishStruggling_0");
		m_Sounds.Insert("FishStruggling_1");
		m_Sounds.Insert("FishStruggling_2");
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_FISHING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override int GetType()
	{
		return AT_FISHING;
	}
		
	override string GetText()
	{
		return "Pull";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		FishingRod_Base nitem = FishingRod_Base.Cast( item );
		if ( nitem.IsFishingActive() )
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
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
};

	