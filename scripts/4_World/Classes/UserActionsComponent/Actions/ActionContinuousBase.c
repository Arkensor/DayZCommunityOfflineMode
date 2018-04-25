class ActionContinuousBaseCB : ActionBaseCB
{	
	bool CancelCondition()
	{
		//SetCommand(DayZPlayerConstants.CMD_ACTIONINT_ACTIONLOOP);
		//Print("cancel condition enabled: " + GetState().ToString() );
		if ( !m_Interrupted && (GetState() == STATE_LOOP_LOOP || GetState() == STATE_LOOP_LOOP2) )
		{	
			if( m_State == UA_INITIALIZE )
				m_State = UA_PROCESSING;
			
			m_ActionData.Do(this,m_State,m_ActionComponent,m_Player,m_Target,m_Item);
		}
		return DefaultCancelCondition(); 
	}
	
	override void InitActionComponent()
	{
		m_Interrupted = false;
		m_Canceled = false;

		CreateActionComponent();
		if ( m_ActionComponent ) 
		{
			m_ActionComponent.Init(m_ActionData, m_Player, m_Target, m_Item);		
		}
		m_State = UA_INITIALIZE;
		EnableCancelCondition(true);
		m_SoundObject = m_ActionData.PlayActionSound(m_Player);
	}
	
	override void EndActionComponent()
	{
		if ( m_State == UA_FINISHED )
		{
			SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
		}
		else
		{
			m_Canceled = true;
			SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
			m_State = UA_CANCEL;
			return;
			//Cancel();
		}
		m_State = UA_FINISHED;
	}	
	
	void UserEndsAction()
	{
		if ( m_ActionComponent ) 
		{
			m_State = m_ActionComponent.Cancel(m_Player, m_Target, m_Item);
		}
		EndActionComponent();
	}
};

class ActionContinuousBase : ActionBase
{
	void ActionContinuousBase() 
	{
		m_CallbackClass = ActionContinuousBaseCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EAT;
	}
	
	override int GetActionCategory()
	{
		return AC_CONTINUOUS;
	}
};