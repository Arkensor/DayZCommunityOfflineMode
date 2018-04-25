// WIP! Unfinished script.
// SetPilotLight currently doesn't work so I'll finish this functionality later.

class Flashlight extends ItemBase
{
	ref Timer m_UpdateTimer;
	bool ignore1cycle = true;
	
	void Flashlight()
	{
		/*if ( !GetGame().IsMultiplayer() )
		{ // Temporal battery spawn for testing
			ItemBase battery = GetGame().CreateObject("Battery9V", GetPosition());
			TakeEntityAsAttachment(battery);
		}*/
	}
	
	//--- POWER EVENTS
	override void OnWorkStart()
	{
		ignore1cycle = true;
		SetPilotLight(true);
	}

	override void OnWorkStop()
	{
		ignore1cycle = true;
		SetPilotLight(false);
	}
	
	override void OnWork( float consumed_energy)
	{
		/*
		ItemBase battery = GetCompEM().GetEnergySource();
		
		if (battery)
		{
			float health = GetHealth("","");
			float max_health = GetMaxHealth("","");
			
			if ( GetHealthLabel() == 3 )
			{
				if ( Math.RandomFloat01() < 0.2 )
				{
					if (!ignore1cycle)
					{
						GetCompEM().UnplugThis();
						
						m_UpdateTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
						m_UpdateTimer.Run( 1 , this, "FixColdContact", NULL, false);
					}
					else
					{
						ignore1cycle = false;
					}
				}
			}
		}
		*/
	}
	
	void FixColdContact()
	{
		ItemBase battery = ItemBase.Cast( GetInventory().GetAttachmentFromIndex(0) );
		
		if (battery)
		{
			ignore1cycle = true;
			GetCompEM().PlugThisInto(battery);
		}
	}
}