class PortableGasLamp extends ItemBase
{
	private const string GAS_LIGHT_MATERIAL_ON 		= "dz\\gear\\cooking\\data\\GasLightOn.rvmat";
	private const string GAS_LIGHT_MATERIAL_OFF 	= "dz\\data\\data\\default.rvmat";

	//sound
	protected SoundOnVehicle m_SoundGasFlameLoop;
	const string SOUND_BURNING_GAS 	= "gasFlame";
	
	
	//--- POWER EVENTS
	override void OnWorkStart()
	{
		SetPilotLight( true );
		
		//refresh visual
		SetObjectMaterial( 0, GAS_LIGHT_MATERIAL_ON );
		
		//sound
		SoundBurningStart();
	}

	override void OnWorkStop()
	{
		SetPilotLight( false );
		
		//refresh visual
		SetObjectMaterial( 0, GAS_LIGHT_MATERIAL_OFF );
		
		//sound
		SoundBurningStop();
	}
	
	//================================================================
	// SOUNDS
	//================================================================
	//Gas burning
	protected void SoundBurningStart()
	{
		if ( !m_SoundGasFlameLoop && GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			m_SoundGasFlameLoop = PlaySoundLoop( SOUND_BURNING_GAS, 50 );
		}
	}
	
	protected void SoundBurningStop()
	{
		if ( m_SoundGasFlameLoop && GetGame() && ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) )
		{
			GetGame().ObjectDelete( m_SoundGasFlameLoop );
			m_SoundGasFlameLoop = NULL;			
		}
	}	
}