class ManBase extends DayZPlayerImplement // PSOVIS originaly extends Man
{	
	void ManBase()
	{
		//Print("PSOVIS: this is ManBase");
	}

	// -------------------------------------------------------------------------
	// ! On Client, 'true' if this instance of a character is controlled by the player(ie. not a remote player)
	bool IsControlledPlayer()
	{
		return( GetGame().GetPlayer() == this );
	}

	// -------------------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
	}
}