class ActionWringClothesCB : ActionContinuousBaseCB
{
	private const float QUANTITY_WRINGLED_PER_SECOND = 0.03;
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousEmpty(QUANTITY_WRINGLED_PER_SECOND);
	}
};

class ActionWringClothes: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.01;

	void ActionWringClothes()
	{
		m_CallbackClass = ActionWringClothesCB;
		//m_CommandUID = DayZPlayerConstants.CMD_ACTION_WRING;
		m_MessageStartFail = "It's ruined.";
		m_MessageStart = "I have started wringing clothes.";
		m_MessageSuccess = "I have finished wringing clothes.";
		m_MessageFail = "Player moved and stopped wringing clothes.";
		m_MessageCancel = "I stopped wringing clothes.";
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_WRING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_WRING_CLOTHES;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "wring the clothes";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if( item && item.GetWet() >= 0.1 && item.GetWet() <= item.GetWetMax())
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
		//item.TransferModifiers(player);
		Param1<float> nacdata = Param1<float>.Cast( acdata );		
		float delta = nacdata.param1;
		item.AddWet( -delta );

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}	
};