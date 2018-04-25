class Radio extends ItemRadio
{
	override bool IsTransmitter()
	{
		return true;
	}
	
	//--- COMMON
	bool CanOperate()
	{
		return GetCompEM().IsSwitchedOn();	
	}
	
	//--- POWER EVENTS
	override void OnSwitchOn()
	{
		//switch device on
		SwitchOn ( true );
	}

	override void OnSwitchOff()
	{
		//switch device off
		SwitchOn ( false );
	}

	override void OnWorkStop()
	{
		//turn off device
		GetCompEM().SwitchOff();
	}
	
	//--- RADIO ACTIONS
	void TuneNextStation()
	{
		//tune next station
		TuneNext();
	}

	void TunePreviousStation()
	{
		//tune previous station
		TunePrev();
	}
}
