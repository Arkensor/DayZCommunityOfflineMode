class ActionSingleUseBaseCB : ActionBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUse;
	}

	override void OnAnimationEvent(int pEventID)	
	{
		if ( !m_Interrupted && pEventID == UA_ANIM_EVENT ) 
		{
			m_ActionData.Do(this,UA_ANIM_EVENT,m_ActionComponent,m_Player,m_Target,m_Item);
		}	
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
		m_State = UA_PROCESSING;
		RegisterAnimationEvent("ActionExec", UA_ANIM_EVENT);
		m_SoundObject = m_ActionData.PlayActionSound(m_Player);			
	}	
	
	override void EndActionComponent()
	{
		m_State = UA_FINISHED;
	}	
};

class ActionSingleUseBase : ActionBase
{
	void ActionSingleUseBase()
	{
		m_CallbackClass = ActionSingleUseBaseCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP;
	}
	
	override int GetActionCategory()
	{
		return AC_SINGLE_USE;
	}
};