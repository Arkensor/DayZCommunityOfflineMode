class PowerGenerator extends ItemBase
{
	float						m_Fuel;
	private static float		m_FuelTankCapacity; // Capacity in ml.
	private static float		m_FuelToEnergyRatio; // Conversion ratio of 1 ml of fuel to X Energy
	private int					m_FuelPercentage;
	
	static const string			START_SOUND = "powerGeneratorTurnOn";
	static const string			LOOP_SOUND = "powerGeneratorLoop";
	static const string			STOP_SOUND = "powerGeneratorTurnOff";
	
	SoundOnVehicle 				m_SoundLoopEntity;
	ref Timer 					m_SoundLoopStartTimer;
	ref protected Effect 		m_Smoke;
	
	ItemBase	m_SparkPlug; // Attached spark plug item
	
	// Constructor
	void PowerGenerator()
	{
		/*if ( !GetGame().IsMultiplayer() ) // Temporal sparkplug spawn for testing
		{
			ItemBase sparkplug = GetGame().CreateObject("Sparkplug", GetPosition());
			
			if (sparkplug)
			{
				TakeEntityAsAttachment(sparkplug);
			}
		}*/
		m_FuelPercentage = 100;
		RegisterNetSyncVariableInt("m_FuelPercentage");
	}

	// Play the loop sound
	void StartLoopSound()
	{
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			m_SoundLoopEntity = PlaySoundLoop(LOOP_SOUND, 50);
			
			// Particle
			vector local_pos = "0.3 0.21 0.4";
			vector local_ori = "270 0 0";
			m_Smoke = new EffGeneratorSmoke();
			SEffectManager.PlayOnObject(m_Smoke, this, local_pos, local_ori);
		}
	}
	
	// Taking item into inventory
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		return CanManipulate();
	}

	// Taking item into inventory
	override bool CanPutIntoHands ( EntityAI player ) 
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		return CanManipulate();
	}

	// Returns true/false if this item can be moved into inventory/hands
	bool CanManipulate()
	{
		if ( GetCompEM().GetPluggedDevicesCount() == 0  &&  !GetCompEM().IsWorking() )
		{
			return true;
		}
		
		return false;
	}
	
	/*===================================
					EVENTS
	===================================*/
	
	// Init
	override void OnInitEnergy()
	{
		m_FuelTankCapacity = GetGame().ConfigGetFloat ("CfgVehicles " + GetType() + " fuelTankCapacity");
		m_FuelToEnergyRatio = GetCompEM().GetEnergyMax() / m_FuelTankCapacity; // Conversion ratio of 1 ml of fuel to X Energy
		m_SparkPlug = NULL;
		m_SoundLoopEntity = NULL;
		
		UpdateFuelMeter();
	}
	
	// Generator is working
	override void OnWorkStart()
	{
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			PlaySound(START_SOUND, 50);
			
			if (!m_SoundLoopStartTimer)
				m_SoundLoopStartTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
			
			if ( !m_SoundLoopStartTimer.IsRunning() ) // Makes sure the timer is NOT running already
			{
				m_SoundLoopStartTimer.Run(1.5, this, "StartLoopSound", NULL, false);
			}
		}
	}

	// Do work
	override void OnWork( float consumed_energy )
	{
		if ( GetGame().IsServer() )
		{
			m_FuelPercentage = GetCompEM().GetEnergy0To100();
			SetSynchDirty();
		}
		
		UpdateFuelMeter();
	}

	// Turn off when this runs out of fuel
	override void OnWorkStop()
	{
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			// Sound
			PlaySound(STOP_SOUND, 50);
			GetGame().ObjectDelete(m_SoundLoopEntity);
			m_SoundLoopEntity = NULL;
			
			// particle
			if (m_Smoke)
				delete m_Smoke;
			
			// Fuel meter
			UpdateFuelMeter();
		}
	}
	
	// Called when this generator is picked up
	override void EEItemLocationChanged  ( EntityAI old_owner, EntityAI new_owner ) 
	{
		super.EEItemLocationChanged(old_owner, new_owner);
		UpdateFuelMeter();
	}
	
	override void EEItemAttached ( EntityAI item, string slot_name )
	{
		super.EEItemAttached ( item, slot_name );
		
		ItemBase item_IB = ItemBase.Cast( item );
		
		if ( item_IB.IsKindOf("Sparkplug") )
		{
			ShowSelection("sparkplug_installed");
			m_SparkPlug = item_IB;
		}
	}
	
	override void EEItemDetached ( EntityAI item, string slot_name )
	{
		super.EEItemDetached ( item, slot_name );
		
		ItemBase item_IB = ItemBase.Cast( item );
		
		if ( item_IB.IsKindOf("Sparkplug") )
		{
			HideSelection("sparkplug_installed");
			m_SparkPlug = NULL;
			GetCompEM().SwitchOff();
		}
	}
	
	/*================================
				FUNCTIONS
	================================*/
	
	void UpdateFuelMeter()
	{
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			SetAnimationPhase("dial_fuel", m_FuelPercentage * 0.01);
		}
	}
	
	// Adds energy to the generator
	void SetFuel(float fuel_amount)
	{
		if (m_FuelTankCapacity > 0)
		{
			m_FuelToEnergyRatio = GetCompEM().GetEnergyMax() / m_FuelTankCapacity;
			GetCompEM().SetEnergy(fuel_amount * m_FuelToEnergyRatio);
			UpdateFuelMeter();
		}
		else
		{
			string error = "ERROR! Item " + this.GetType() + " has fuel tank with 0 capacity! Add parameter 'fuelTankCapacity' to its config and set it to more than 0!";
			DPrint(error);
		}
	}

	// Adds fuel (energy) to the generator
	// Returns how much fuel was accepted
	float AddFuel(float available_fuel)
	{
		if ( available_fuel == 0 )
		{
			return 0;
		}
		
		float needed_fuel = GetMaxFuel() - GetFuel();
		
		if ( needed_fuel > available_fuel )
		{
			SetFuel(GetFuel() + available_fuel);
			return available_fuel; // Return used fuel amount
		}
		else
		{
			SetFuel(GetMaxFuel());
			return needed_fuel;
		}
	}

	// Check the bottle if it can be used to fill the tank
	bool CanAddFuel( ItemBase container )
	{
		if (container)
		{
			// Get the liquid
			int liquid_type	= container.GetLiquidType();
			
			// Do all checks
			if ( container.GetQuantity() > 0  &&  GetCompEM().GetEnergy() < GetCompEM().GetEnergyMax()  &&  (liquid_type & LIQUID_GASOLINE) )
			{
				return true;
			}
		}
		
		return false;
	}

	// Returns fuel amount
	float GetFuel()
	{
		return GetCompEM().GetEnergy() / m_FuelToEnergyRatio;
	}

	// Returns max fuel amount
	float GetMaxFuel()
	{
		return m_FuelTankCapacity;
	}

	// Checks sparkplug
	bool HasSparkplug()
	{
		if ( m_SparkPlug )
		{
			if ( !m_SparkPlug.IsRuined() > 0 )
			{
				return true;
			}
		}
		
		return false;
	}
}
