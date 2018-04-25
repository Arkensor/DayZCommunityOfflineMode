class ActionCoverHeadSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.COVER_HEAD);
	}
};

class ActionCoverHeadSelf: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.01;
	
	void ActionCoverHeadSelf()
	{
		m_CallbackClass = ActionCoverHeadSelfCB;
		m_MessageStartFail = "It's too torn.";
		m_MessageStart = "I have started putting sack on";
		m_MessageSuccess = "I have put sack on.";
		m_MessageFail = "I have moved and putting sack on was canceled.";
		m_MessageCancel = "I stopped putting sack on.";
		//m_Animation = "INJECTEPIPENS";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override int GetType()
	{
		return AT_COVER_HEAD_S;
	}

	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "put on head";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( IsWearingHeadgear(player) ) 
			return false;

		return true;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		//setaperture will be called from here, or from statemachine
		//GetGame().GetWorld().SetAperture(10000);
		player.GetInventory().CreateInInventory("BurlapSackCover");
		item.TransferModifiers(player);
		item.Delete();
		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}


	bool IsWearingHeadgear( PlayerBase player )
	{
		if ( player.GetInventory().FindAttachment(InventorySlots.HEADGEAR) )
		{
			return true;
		}
		return false;		
	}
};