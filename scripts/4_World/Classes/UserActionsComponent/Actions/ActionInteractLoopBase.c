class ActionInteractLoopBaseCB : ActionInteractBaseCB
{
	
	bool CancelCondition()
	{
		//SetCommand(DayZPlayerConstants.CMD_ACTIONINT_ACTIONLOOP);
		//Print("cancel condition enabled: " + GetState().ToString() );
		if ( !m_Interrupted && (GetState() == STATE_LOOP_LOOP || GetState() == STATE_LOOP_LOOP2) )
		{	
			m_ActionData.Do(this,m_State,m_ActionComponent,m_Player,m_Target,m_Item);
		}
		return DefaultCancelCondition(); 
	}
	
	override void OnAnimationEvent(int pEventID)	
	{
	}
	
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAInteract;
		
	}
	
	override void InitActionComponent()
	{
		super.InitActionComponent();
		EnableCancelCondition(true);
	}
	
};

class ActionInteractLoopBase : ActionInteractBase
{	
	void ActionInteractLoopBase() 
	{
		m_CallbackClass = ActionInteractLoopBaseCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BERRIES;
		m_HUDCursorIcon = CursorIcons.Cursor;
	}
};