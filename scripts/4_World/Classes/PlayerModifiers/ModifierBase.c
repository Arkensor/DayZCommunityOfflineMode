// #include "Scripts\Classes\PlayerModifiers\Modifiers\_constants.c"
class ModifierBase
{
	int					m_ID = 0;
	ModifiersManager 	m_Manager;//the manager instance
	string 				m_System = "Modifiers";
	float 				m_ActivatedTime;//overall time this modifier was active
	bool				m_TrackActivatedTime;//should this modifier track overall time it was active ?
	PlayerBase			m_Player;
	float				m_TickIntervalInactive = 5;
	float				m_TickIntervalActive = 3;
	bool				m_IsActive;
	float				m_AccumulatedTimeActive;
	float				m_AccumulatedTimeInactive;
	float				m_LastTickedActive;
	float				m_LastTickedInactive;
	bool				m_IsLocked = false;
	
	PluginPlayerStatus 		m_ModulePlayerStatus;

	void ModifierBase()
	{
		Class.CastTo(m_ModulePlayerStatus, GetPlugin(PluginPlayerStatus));
	}
	
	void InitBase(PlayerBase player, ModifiersManager manager)
	{
		m_Manager 				= manager;
		m_Player 				= player;
		Init();
	}
	
	void Init(){}


	PlayerBase GetPlayer()
	{
		return m_Player;
	}


	void MakeParamObjectPersistent(Param object)
	{
		m_Manager.m_ParamList.Insert(object);
	}


	void ResetLastTickTime()
	{
		m_LastTickedActive = 0;
	}


	void Tick(float delta_time)
	{
		if( IsActive() )
		{
			m_AccumulatedTimeActive += delta_time;
			if( m_AccumulatedTimeActive > m_TickIntervalActive )
			{
				//------
				/*
				float time = GetGame().GetTime();
				Print("ticking modifier "+ this.ClassName() +" for player " + m_Player.ToString() + " at time:" + time.ToString());
				*/
				//------
				if( DeactivateCondition(m_Player) )
				{
					if( !IsLocked() ) 
					{
						Deactivate();
					}
				}
				else
				{
					m_ActivatedTime += m_AccumulatedTimeActive;
					OnTick(m_Player, m_AccumulatedTimeActive);
				}
				m_AccumulatedTimeActive = 0;
			}
		}
		else
		{
			m_AccumulatedTimeInactive += delta_time;
			if( m_AccumulatedTimeInactive > m_TickIntervalInactive )
			{
				if( ActivateCondition(m_Player) )
				{
					if( !IsLocked() ) 
					{
						Activate(true);
					}
				}
				m_AccumulatedTimeInactive = 0;
			}
		}
	}
	
	bool IsActive()
	{
		return m_IsActive;
	}
	
	void SetLock(bool state)
	{
		m_IsLocked = state;
	}

	bool IsLocked()
	{
		return m_IsLocked;		
	}
	
	bool IsTrackAttachedTime()
	{
		return m_TrackActivatedTime;
	}
	
	float GetAttachedTime()
	{
		return m_ActivatedTime;
	}
	
	void SetAttachedTime(float time)
	{
		m_ActivatedTime = time;
	}
	
	int GetModifierID()
	{
		return m_ID;
	}
	
	string GetName()
	{
		return this.ClassName() + " modifier";
	}

	bool ActivateCondition(PlayerBase player)
	{
		return false;
	}

	bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	//! is called when an inactive modifier gets activated during gameplay, is NOT called on activation upon player server connection(see OnReconnect)
	void OnActivate(PlayerBase player)
	{

	}
	
	//! is called when a modifier is being re-activated upon player server connection, use to activate systems which are not persistent and need to run alongside active modifiers
	void OnReconnect(PlayerBase player)
	{

	}

	void OnDeactivate(PlayerBase player)
	{

	}

	void Activate(EActivationType trigger)
	{
		m_IsActive = true;
		if( trigger == EActivationType.TRIGGER_EVENT_ON_ACTIVATION ) OnActivate(m_Player);
		else if(trigger == EActivationType.TRIGGER_EVENT_ON_CONNECT ) OnReconnect(m_Player);
	}

	void Deactivate(bool trigger = true)
	{
		m_IsActive = false;
		m_ActivatedTime = 0;
		if(trigger) OnDeactivate(m_Player);
	}


	void OnStoreSave( ParamsWriteContext ctx )
	{
	}


	private void OnTick(PlayerBase player, float deltaT)
	{
		
	}
}
