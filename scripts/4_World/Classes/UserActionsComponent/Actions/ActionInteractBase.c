class ActionInteractBaseCB : ActionBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAInteract;
	}

	override void OnAnimationEvent(int pEventID)	
	{
#ifdef DEVELOPER
		Print("ActionInteractBase.c | OnAnimationEvent | OnAnimationEvent called");
#endif
		if ( !m_Interrupted && pEventID == UA_ANIM_EVENT ) 
		{
			m_ActionData.Do(this,UA_ANIM_EVENT,m_ActionComponent,m_Player,m_Target,m_Item);
		}	
	}
	
	override void InitActionComponent()
	{
#ifdef DEVELOPER
		Print("ActionInteractBase.c | InitActionComponent | inited");
#endif
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
		SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
		m_State = UA_FINISHED;
	}	
};



class ActionInteractBase : ActionBase
{
	string m_HUDCursorIcon;
	
	void ActionInteractBase() 
	{
		m_CallbackClass = ActionInteractBaseCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP;
		m_HUDCursorIcon = CursorIcons.Cursor;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}
	
	string GetHUDCursorIcon()
	{
		return m_HUDCursorIcon;
	}
	
	override int GetActionCategory()
	{
		return AC_INTERACT;
	}
	
};