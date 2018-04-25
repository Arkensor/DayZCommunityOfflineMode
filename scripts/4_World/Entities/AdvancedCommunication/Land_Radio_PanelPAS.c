class Land_Radio_PanelPAS extends PASBroadcaster
{
	//--- BASE
	override bool IsStaticTransmitter()
	{
		return true;
	}

	//--- ACTION CONDITION
	bool CanOperate()
	{
		return GetCompEM().IsSwitchedOn();
	}
	
	//--- POWER EVENTS
	override void OnWorkStop()
	{
		//turn off device
		SwitchOn ( false );
	}
	
	//--- ACTION EVENTS
	override void OnSwitchOn()
	{
		//turn device on
		SwitchOn ( true );	
	}
	
	override void OnSwitchOff()
	{
		//turn device off
		SwitchOn ( false );
		
		//turn device off
		GetCompEM().SwitchOff();
	}
}
