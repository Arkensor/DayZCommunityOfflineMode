class Megaphone extends ItemMegaphone
{
	override bool IsTransmitter()
	{
		return true;
	}	
	
	//--- POWER EVENTS
	override void OnWorkStop()
	{
		//turn off device
		this.GetCompEM().SwitchOff();
	}
	
	//--- ACTION EVENTS
	override void OnSwitchOn()
	{
		//turn device on
		this.SwitchOn ( true );
	}

	override void OnSwitchOff()
	{
		//turn device off
		this.SwitchOn ( false );
	}
}
