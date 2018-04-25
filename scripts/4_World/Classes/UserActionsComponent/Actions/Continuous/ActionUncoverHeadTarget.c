class ActionUncoverHeadTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.COVER_HEAD);
	}
};

class ActionUncoverHeadTarget: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.01;
	
	void ActionUncoverHeadTarget()
	{
		m_CallbackClass = ActionUncoverHeadTargetCB;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "Player started removing sack from your head.";
		m_MessageSuccess = "Player finished removing sack from your head.";
		m_MessageFail = "Player moved and removing sack from was canceled.";
		m_MessageCancel = "You stopped removing sack.";
		//m_Animation = "INJECTEPIPENS";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERRACTPLAYER;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_UNCOVER_HEAD_T;
	}
		
	override string GetText()
	{
		return "uncover player's head";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		PlayerBase targetPlayer;
		if( Class.CastTo(targetPlayer, target.GetObject()) )
		{
			if ( IsWearingBurlap(targetPlayer) ) 
			{
				return true;
			}
		}
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		//setaperture will be called from here, or from statemachine
		PlayerBase ntarget = PlayerBase.Cast( target.GetObject() );
		
		EntityAI attachment;
		Class.CastTo(attachment, ntarget.GetInventory().FindAttachment(InventorySlots.HEADGEAR));
		
		if ( attachment && attachment.GetType() == "BurlapSackCover" )
		{
			attachment.Delete();
			player.GetInventory().CreateInInventory("BurlapSack");
			player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		}
		


		
	}

	bool IsWearingBurlap( PlayerBase player )
	{
		EntityAI attachment;
		Class.CastTo(attachment, player.GetInventory().FindAttachment(InventorySlots.HEADGEAR));
		if ( attachment && attachment.GetType() == "BurlapSackCover" )
		{
			return true;
		}
		return false;		
	}
};