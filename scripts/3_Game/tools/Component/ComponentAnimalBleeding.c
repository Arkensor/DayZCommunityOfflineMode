//-----------------------------
// ANIMAL BLEEDING
//-----------------------------
/*
Animal bleeding is handled by this component.
*/

class ComponentAnimalBleeding : Component
{
	// Member variables
	protected ref Timer m_BleedTimer;
	protected const float BASE_BLEED_RATE = 500;
	protected const float PASS_OUT_AMOUT = 500;

	// Constructor
	void ComponentAnimalBleeding()
	{
		
	}

	void CreateWound( string zone_name, string ammo )
	{
		float bleed_chance = GetGame().ConfigGetFloat( "CfgVehicles " + m_ThisEntityAI.GetType() + " DamageSystem " + "DamageZones " + zone_name + " bleedChance" );
		//Print("bleed_chance: " + bleed_chance );
		float chance = Math.RandomFloat01();
		//Print("chance: " + chance );
		
		if( chance < bleed_chance )
		{
			m_BleedTimer = new Timer();
			float wound_intensity = GetWoundIntensity( ammo );
			m_BleedTimer.Run( 1, this, "Bleed", new Param1<float>( wound_intensity ), true ); 
		}
		/*
		else
		{
			Print("Not bleeding");
		}
		*/
	}

	void Bleed( float wound_intensity )
	{
		if( m_ThisEntityAI.IsAlive() )
		{
			float bleeding_intensity = BASE_BLEED_RATE * wound_intensity;
			//Print("bleeding_intensity: " + bleeding_intensity);
			float global_blood_lvl = m_ThisEntityAI.GetHealth( "", "Blood" );

			m_ThisEntityAI.DecreaseHealth( "", "Blood", bleeding_intensity );

			if( global_blood_lvl < PASS_OUT_AMOUT )
			{
				m_ThisEntityAI.SetHealth( "", "", 0 );
				//Print("global_blood_lvl < PASS_OUT_AMOUT => Zabijam zviera.");	
			}
		
			/*
			Print( "GetHealth Global Health: " + m_ThisEntityAI.GetHealth( "", "Health" ));
			Print( "GetHealth Global Blood: " + m_ThisEntityAI.GetHealth( "", "Blood" ));
			Print( "GetHealth Global Shock: " + m_ThisEntityAI.GetHealth( "", "Shock" ));
			Print( "GetHealth Health @: " + zone_name + " " + m_ThisEntityAI.GetHealth( zone_name, "Health" ));
			Print( "GetHealth Blood @: " + zone_name + " " + m_ThisEntityAI.GetHealth( zone_name, "Blood" ));
			Print( "GetHealth Shock @: " + zone_name + " " + m_ThisEntityAI.GetHealth( zone_name, "Shock" ));
			*/
		}
		else
		{
			m_BleedTimer.Stop();	
			//Print("Vypinam timer.");		
		}
	}

	float GetWoundIntensity( string ammo )
	{
		ref map<string, float> caliber_bleed_rate = new map<string, float>;
		float bleed_rate;

		// lower bleeding rate
		caliber_bleed_rate.Set("Bullet_9x19", 0.5 );
		// default bleeding rate
		caliber_bleed_rate.Set("Bullet_45ACP", 1 );
		// higher bleeding rate
		caliber_bleed_rate.Set("Bullet_762x39", 2 );

		caliber_bleed_rate.Find( ammo, bleed_rate );

		return bleed_rate;
	}
}