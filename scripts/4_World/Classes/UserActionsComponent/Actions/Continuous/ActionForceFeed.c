class ActionForceFeedCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.EAT_SMALL,UATimeSpent.DEFAULT);
	}
};

class ActionForceFeed: ActionForceConsume
{
	void ActionForceFeed()
	{
		m_CallbackClass = ActionForceFeedCB;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "I started feeding.";
		m_MessageSuccess = "I finished feeding.";
		m_MessageFail = "Player moved and feeding was canceled.";
		m_MessageCancel = "I stopped feeding.";
		m_MessageStartTarget = "Player started feeding you.";
		m_MessageSuccessTarget = "Player finished feeding you.";
		m_MessageCancelTarget = "Player stopped feeding.";
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_FORCEFEED;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		
		
		//m_Animation = "feed";
	}
	
	override void CreateConditionComponents() 
	{
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINonRuined;
	}

	override int GetType()
	{
		return AT_FEED;
	}
		
	override string GetText()
	{
		return "feed";
	}
};