class Land_Misc_Greenhouse_TO_BE_RELEASED extends GardenBase // Greenhouses are temporarily disabled for the first 063 exps.
{
	ref Timer m_Timer;
	
	void Land_Misc_Greenhouse()
	{
		//SetAnimationPhase("slotCovered_01", 1);
		
		m_Timer = new Timer( CALL_CATEGORY_GAMEPLAY );
		//m_Timer.Run( Math.RandomFloat(1, 20), this, "OnTimer", NULL, false ); // Temporal solution for initial lag. Commented out as a quick fix for DAYZ-30633
	}
	
	void OnTimer()
	{
		FIRST_SLOT_COMPONENT_INDEX = 115;
		
		m_BaseFertility = 1.0;
		InitializeSlots();
		DigAllSlots();
	}	
	
	override int GetGardenSlotsCount()
	{
		return 9;
	}
}

/*class Land_Misc_Greenhouse extends Building
{
	
	ref Timer m_Timer;
	
	void Land_Misc_Greenhouse()
	{
		Print(this);
		m_Timer = new Timer( CALL_CATEGORY_GAMEPLAY );
		m_Timer.Run( 1, this, "OnTimer", NULL, false );
	}
	
	void OnTimer()
	{
		if (GetGame())
		{
			PlayerBase plr = GetGame().GetPlayer();
			
			if (plr)
			{
				vector pos = GetPosition();
				plr.SetPosition(pos);
			}
		}
	}
}*/

/*
// Just testing something. Temporal code.
class Land_Misc_Greenhouse extends Building
{
	void Land_Misc_Greenhouse()
	{
		Print("GREENHOUSE CREATED!");
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		return false;
	}

	override bool CanPutIntoHands( EntityAI player )
	{
		return false;
	}

	override bool CanRemoveFromHands( EntityAI player )
	{
		return false;
	}
}
*/