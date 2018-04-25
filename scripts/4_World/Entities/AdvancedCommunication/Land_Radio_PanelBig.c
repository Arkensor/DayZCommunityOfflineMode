class Land_Radio_PanelBig extends StaticTransmitter
{
	//--- BASE
	override bool IsStaticTransmitter()
	{
		return true;
	}
	
	//--- ACTIONS
	bool CanOperate()
	{
		return GetCompEM().IsSwitchedOn();
	}
	
	void SetNextFrequency( PlayerBase player = NULL )
	{
		SetNextChannel();
		
		if ( player )
		{
			DisplayRadioInfo( GetTunedFrequency().ToString(), player );
		}
	}

	//--- POWER EVENTS
	override void OnWorkStop()
	{
		//turn off device
		GetCompEM().SwitchOff();
	}

	override void OnSwitchOn()
	{
		//turn device on
		SwitchOn( true );
		
		//turn on broadcasting/receiving
		EnableBroadcast( true );
		EnableReceive( false );
	}

	override void OnSwitchOff()
	{
		//turn device off
		SwitchOn ( false );
		
		//turn off broadcasting/receiving
		EnableBroadcast( false );
		EnableReceive( false );
	}
}
