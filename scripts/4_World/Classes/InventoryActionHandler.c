//! Client only - manage set up crafting on client 
class InventoryActionHandler
{
	
	PlayerBase m_player;
	int m_actionID;

	ItemBase m_itemTarget;
	ItemBase m_itemInHand;
	
	bool m_isActive;
	vector m_actionStartPos;
	
	const float MIN_DISTANCE_TO_INTERRUPT = 1.0;	
	const int IAH_SINGLE_USE = 1;
	const int IAH_CONTINUOUS = 2;


	void InventoryActionHandler(PlayerBase player)
	{
		m_itemTarget = NULL;
		m_itemInHand = NULL;
		m_isActive = false;
		m_player = player;
		m_actionID = -1;
	}

	void SetAction(int type, int ActionID, ItemBase actionTargetItem )
	{
		m_actionID = ActionID;
		m_itemTarget = actionTargetItem;
		m_itemInHand = m_player.GetItemInHands();
		m_actionStartPos = m_player.GetPosition();
		m_isActive = true;
		
		ActionManagerClient mngr;
		Class.CastTo(mngr, m_player.GetActionManager());
		
		ActionTarget at;
		at = new ActionTarget(actionTargetItem, null, -1, vector.Zero, -1);
		switch( type )
		{
			case IAH_SINGLE_USE:
				mngr.InjectSingleUseAction(ActionID, at, m_itemInHand);
				break;
			case IAH_CONTINUOUS:
				mngr.InjectContinuousAction(ActionID, at, m_itemInHand);
				break;
		}
		mngr.ForceTarget(actionTargetItem);
		
		GetGame().GetUIManager().HideScriptedMenu(GetGame().GetUIManager().FindMenu(MENU_INVENTORY));
	}
	
	int GetActionID()
	{
		return m_actionID;
	}
	
	bool IsActiveAction()
	{
		return m_isActive;
	}
	
	void OnUpdate()
	{
		
		if( !m_isActive ) return;
		
		if( m_player.IsRaised() || m_player.GetCommand_Melee() )
		{
			DeactiveAction();
			return;			
		}
		
		if( GetGame().IsInventoryOpen() )
		{
			DeactiveAction();
			return;				
		}
		
		ItemBase handItem = m_player.GetItemInHands();
		
		if( handItem != m_itemInHand )
		{
			DeactiveAction();
			return;
		}
			
		if( Math.AbsFloat( vector.Distance(m_actionStartPos, m_player.GetPosition())) > MIN_DISTANCE_TO_INTERRUPT )
		{
			DeactiveAction();
			return;
		}
		
		
	}

	void DeactiveAction()
	{
		if( !m_isActive ) return;
		m_itemTarget = NULL;
		m_itemInHand = NULL;
		m_actionID = -1;
		m_isActive = false;
		
		ActionManagerClient mngr;
		Class.CastTo(mngr, m_player.GetActionManager());
		mngr.EjectActions();
		mngr.ClearForceTarget();
	}
}
