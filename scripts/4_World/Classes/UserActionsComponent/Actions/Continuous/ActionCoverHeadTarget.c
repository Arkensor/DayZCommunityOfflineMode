class ActionCoverHeadTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.COVER_HEAD);
	}
};

class ActionCoverHeadTarget: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.01;
	
	void ActionCoverHeadTarget()
	{
		m_CallbackClass = ActionCoverHeadTargetCB;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "Player started putting sack on your head.";
		m_MessageSuccess = "Player finished putting sack on your head.";
		m_MessageFail = "Player moved and putting sack on was canceled.";
		m_MessageCancel = "You stopped putting sack on.";
		//m_Animation = "INJECTEPIPENS";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);		
	}

	override int GetType()
	{
		return AT_COVER_HEAD_T;
	}
		
	override string GetText()
	{
		return "put on targets head";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		PlayerBase targetPlayer;
		Class.CastTo(targetPlayer, target.GetObject());
		if ( !IsWearingHeadgear(targetPlayer) ) 
		{
			return true;
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		//setaperture will be called from here, or from statemachine
		PlayerBase ntarget;
		Class.CastTo(ntarget, target.GetObject());
		ntarget.GetInventory().CreateInInventory("BurlapSackCover");
		//ntarget.CreateInInventory("BurlapSackCover","cargo_headgear");
		item.TransferModifiers(ntarget);
		item.Delete();

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}

	bool IsWearingHeadgear( PlayerBase player)
	{
		if ( player.GetInventory().FindAttachment(InventorySlots.HEADGEAR) )
		{
			return true;
		}
		return false;		
	}
};