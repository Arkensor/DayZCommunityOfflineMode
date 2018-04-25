class AdvancedCommunication extends EntityAI
{
	ref TIntArray m_SingleUseActions;
	ref TIntArray m_ContinuousActions;
	ref TIntArray m_InteractActions;
	
	void AdvancedCommunication()
	{
		//user actions
		m_SingleUseActions = new TIntArray;
		m_ContinuousActions = new TIntArray;
		m_InteractActions = new TIntArray;
		SetUserActions();		
	}
			
	//User actions
	void SetUserActions()
	{
		g_Game.ConfigGetIntArray("cfgVehicles " +GetType() + " ContinuousActions", m_ContinuousActions);	
		g_Game.ConfigGetIntArray("cfgVehicles " +GetType() + " SingleUseActions", m_SingleUseActions);	
		g_Game.ConfigGetIntArray("cfgVehicles " +GetType() + " InteractActions", m_InteractActions);	
	}

	override void GetSingleUseActions(out TIntArray actions)
	{	
		if ( m_SingleUseActions )
		{			
			for ( int i = 0; i < m_SingleUseActions.Count(); i++ )
			{
				actions.Insert(m_SingleUseActions.Get(i));
			}
		}
	}
	
	override void GetContinuousActions(out TIntArray actions)
	{
		if ( m_ContinuousActions )
		{
			for ( int i = 0; i < m_ContinuousActions.Count(); i++ )
			{
				actions.Insert(m_ContinuousActions.Get(i));
			}
		}
	}
	
	override void GetInteractActions(out TIntArray actions)
	{
		if ( m_InteractActions )
		{			
			for ( int i = 0; i < m_InteractActions.Count(); i++ )
			{
				actions.Insert(m_InteractActions.Get(i));
			}
		}
	}

	//HUD
	protected Hud GetHud( PlayerBase player )
	{
		if ( !player ) 
		{
			return NULL;
		}
		
		return player.m_HUD;
	}
	
	void DisplayRadioInfo( string message, PlayerBase player )
	{
		Hud hud;
		if ( player )
		{
			hud = GetHud( player );
		}
		
		if ( hud ) 
		{	
			hud.SetWalkieTalkieText( message );
			hud.ShowWalkieTalkie( 3 );
		}
	}
}

class PASReceiver extends AdvancedCommunication
{
};

class PASBroadcaster extends AdvancedCommunication
{
	proto native void SwitchOn(bool onOff);
	proto native bool IsOn();
};

class StaticTransmitter extends AdvancedCommunication
{
	proto native void SwitchOn(bool onOff);
	proto native bool IsOn();
	proto native void SetNextChannel();
	proto native void SetPrevChannel();
	proto native float GetTunedFrequency();
	proto native void EnableBroadcast(bool state);
	proto native void EnableReceive(bool state);
	proto native bool IsBroadcasting();
	proto native bool IsReceiving();
};