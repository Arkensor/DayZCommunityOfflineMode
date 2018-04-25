class Refridgerator extends ItemBase
{	
	SoundOnVehicle 	m_SoundLoopEntity;
	
	override void OnInitEnergy()
	{
		m_SoundLoopEntity = NULL;
	}
	
	//--- POWER EVENTS
	override void OnWorkStart()
	{
		// TO DO: Slow down or stop the aging of food inside of the fridge when such functionality is created.
		
		m_SoundLoopEntity = PlaySoundLoop("powerGeneratorLoop", 50); // using placeholder sound
	}
	
	override void OnWork( float consumed_energy )
	{
		
	}
	
	override void OnWorkStop()
	{
		// TO DO: Allow food inside the fridge to start aging again.
		
		GetGame().ObjectDelete(m_SoundLoopEntity);
		m_SoundLoopEntity = NULL;
	}
	
	//--- ACTION EVENTS
	override void OnSwitchOn()
	{
		
	}

	override void OnSwitchOff()
	{
		
	}
}