class BatteryCharger extends ItemBase
{
	// Model selections
	static protected const string SEL_CLIPS_CAR			= "clips_car_battery";
	static protected const string SEL_CLIPS_TRUCK		= "clips_truck_battery";
	static protected const string SEL_CLIPS_DETACHED	= "clips_detached";
	static protected const string SEL_CLIPS_FOLDED		= "clips_folded";
	static protected const string SEL_SWITCH_ON			= "switch_on";
	static protected const string SEL_SWITCH_OFF		= "switch_off";
	static protected const string SEL_CORD_PLUGGED		= "cord_plugged";
	static protected const string SEL_CORD_FOLDED		= "cord_folded";
	static protected const string SEL_LIGHT_STATE_1		= "light_stand_by";
	static protected const string SEL_LIGHT_STATE_2		= "light_charging";
	static protected const string SEL_LIGHT_STATE_3		= "light_charged";
	
	// glow materials
	static protected const string RED_LIGHT_GLOW		= "dz\\gear\\camping\\data\\battery_charger_light_r.rvmat";
	static protected const string GREEN_LIGHT_GLOW		= "dz\\gear\\camping\\data\\battery_charger_light_g.rvmat";
	static protected const string YELLOW_LIGHT_GLOW		= "dz\\gear\\camping\\data\\battery_charger_light_y.rvmat";
	static protected const string SWITCH_LIGHT_GLOW		= "dz\\gear\\camping\\data\\battery_charger_light_switch_on.rvmat";
	static protected const string DEFAULT_MATERIAL 		= "dz\\gear\\camping\\data\\battery_charger.rvmat";
	
	protected const string 	ATTACHED_CLIPS_STATES[] 		= {SEL_CLIPS_CAR, SEL_CLIPS_TRUCK}; // TO DO: If it's required by design, add helicopter battery here and regieter its selection names.
	protected const int 	ATTACHED_CLIPS_STATES_COUNT		= 2;
	
	
	
	int 						m_BatteryEnergy0To100;
	protected float 			m_ChargeEnergyPerSecond;
	
	static protected 	float 	m_BlinkingStatusLightInterval = 0.4; // How often the lights blink
	ref 				Timer 	m_UpdateStatusLightsTimer;
	protected 			bool 	m_BlinkingStatusLightIsOn = false;	// Status of one blinking light
	
	void BatteryCharger()
	{
		m_ChargeEnergyPerSecond = GetGame().ConfigGetFloat ("CfgVehicles " + GetType() + " ChargeEnergyPerSecond");
		m_UpdateStatusLightsTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		SwitchLightOff();
		RegisterNetSyncVariableInt("m_BatteryEnergy0To100");
	}
	
	override void OnWork( float consumed_energy )
	{
		// Charging functionality
		ItemBase battery = ItemBase.Cast( GetCompEM().GetPluggedDevice() );
		
		if ( battery )
		{
			if ( GetGame().IsServer() )
			{
				float battery_capacity = battery.GetCompEM().GetEnergyMax();
				
				if ( battery.GetCompEM().GetEnergy() < battery_capacity )
				{
					// Heat up the items so players know they are working.
					this.SetTemperature(60); 
					battery.SetTemperature(60);
					
					float charger_health = GetHealth("", "");
					float energy_add = m_ChargeEnergyPerSecond * ( consumed_energy / GetCompEM().GetEnergyUsage() );
					
					if ( GetCompEM().ConsumeEnergy(energy_add) ) // consumes energy from the power source
					{
						// There is enough of energy to use
						energy_add = energy_add * ( 0.5 + charger_health*0.005 ); // Damaged charger works less efficiently - 50% damage causes 75% efficiency
					}
					else
					{
						// There is NOT enough of energy to use
						energy_add = 0;
					}
					
					battery.GetCompEM().AddEnergy( energy_add );
				}
				else
				{
					battery.GetCompEM().SetEnergy( battery_capacity );
				}
				
				m_BatteryEnergy0To100 = battery.GetCompEM().GetEnergy0To100();
				SetSynchDirty();
			}
		}
	}

	override void OnWorkStart()
	{
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			UpdateStatusLights();
			m_UpdateStatusLightsTimer.Run( m_BlinkingStatusLightInterval/2 , this, "UpdateStatusLights", NULL, true);
		}
	}
		
	override void OnWorkStop()
	{
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			UpdateStatusLights();
			m_UpdateStatusLightsTimer.Stop();
		}
	}
	
	void UpdateStatusLights()
	{
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			if (GetCompEM().IsWorking())
			{
				SwitchLightOn();
				ItemBase battery = ItemBase.Cast( GetCompEM().GetPluggedDevice() );
				
				if (battery)
				{
					RedLightOff();
					
					if (m_BatteryEnergy0To100 <= 33)
					{
						// Less than 1/3 charged, yellow status light must repeatedly blink
						
						if (m_BlinkingStatusLightIsOn)
							YellowLightOn();
						else
							YellowLightOff();
						
						m_BlinkingStatusLightIsOn = !m_BlinkingStatusLightIsOn;
					}
					else if (m_BatteryEnergy0To100 > 33  &&  m_BatteryEnergy0To100 <= 66)
					{
						// Less than 2/3 charged, yellow status light must glow
						
						YellowLightOn();
					}
					else if (m_BatteryEnergy0To100 > 66  &&  m_BatteryEnergy0To100 < 100)
					{
						// Less than 3/3 charged, yellow status light must glow, green light must blink
						
						YellowLightOn();
						
						if (m_BlinkingStatusLightIsOn)
							GreenLightOn();
						else
							GreenLightOff();
						
						m_BlinkingStatusLightIsOn = !m_BlinkingStatusLightIsOn;
					}
					else if (m_BatteryEnergy0To100 >= 100)
					{
						// Fully charged, green light must glow
						YellowLightOff();
						GreenLightOn();
					}
				}
				else
				{
					if (m_BlinkingStatusLightIsOn)
						RedLightOn();
					else
						RedLightOff();
					
					m_BlinkingStatusLightIsOn = !m_BlinkingStatusLightIsOn;
					
					GreenLightOff();
					YellowLightOff();
				}
			}
			else
			{
				SwitchLightOff();
				GreenLightOff();
				RedLightOff();
				YellowLightOff();
			}
		}
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		// No "Take" action if the item is connected
		if ( !GetCompEM().IsPlugged()  &&  !GetCompEM().GetPluggedDevice() )
		{
			return true;
		}
		
		return false;
	}

	override bool CanPutIntoHands ( EntityAI player ) 
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		// No "Take into hands" action if the item is connected
		if ( !GetCompEM().IsPlugged()  &&  !GetCompEM().GetPluggedDevice() )
		{
			return true;
		}
		
		return false;
	}

	override void OnOwnSocketTaken( EntityAI device )
	{
		string att_type = device.GetType();
		
		if ( att_type == "CarBattery" )
		{
			HideAttachedClipsStates();
			ShowSelection(SEL_CLIPS_CAR);
		}
		
		if ( att_type == "TruckBattery" )
		{
			HideAttachedClipsStates();
			ShowSelection(SEL_CLIPS_TRUCK);
		}
		
		HideSelection(SEL_CLIPS_DETACHED);
		HideSelection(SEL_CLIPS_FOLDED);
	}

	override void OnOwnSocketReleased( EntityAI device )
	{
		HideAttachedClipsStates();
		ShowSelection(SEL_CLIPS_DETACHED);
	}

	override bool CanReceiveAttachment( EntityAI attachment ) 
	{
		ItemBase ibase;
		Class.CastTo(ibase, attachment);
		
		// No attaching if the charger is in inventory!
		PlayerBase charger_owner = PlayerBase.Cast( GetHierarchyRootPlayer() );
		if ( charger_owner )
		{
			return false;
		}
		
		// Only one attachment allowed
		if ( GetCompEM().GetPluggedDevice() )
		{
			return false;
		}
		
		if ( ibase.HasEnergyManager()  &&  ibase.GetCompEM().GetPluggedDevicesCount() >= 1 ) // Make sure nothing is plugged into the battery
		{
			return false;
		}
		
		return true;
	}
	
	void HideAttachedClipsStates()
	{
		for ( int i = 0; i < ATTACHED_CLIPS_STATES_COUNT; i++ )
		{
			string selection = ATTACHED_CLIPS_STATES[i];
			HideSelection(selection);
		}
	}
	
	
	// Control of status lights
	// ON
	void RedLightOn()
	{
		SetObjectMaterial( 0, RED_LIGHT_GLOW );
	}
	void GreenLightOn()
	{
		SetObjectMaterial( 2, GREEN_LIGHT_GLOW );
	}
	void YellowLightOn()
	{
		SetObjectMaterial( 1, YELLOW_LIGHT_GLOW );
	}
	void SwitchLightOn()
	{
		SetObjectMaterial( 3, SWITCH_LIGHT_GLOW );
	}
	// OFF
	void RedLightOff()
	{
		SetObjectMaterial( 0, DEFAULT_MATERIAL );
	}
	void GreenLightOff()
	{
		SetObjectMaterial( 2, DEFAULT_MATERIAL );
	}
	void YellowLightOff()
	{
		SetObjectMaterial( 1, DEFAULT_MATERIAL );
	}
	void SwitchLightOff()
	{
		SetObjectMaterial( 3, DEFAULT_MATERIAL );
	}
	
	
	override void OnSwitchOn()
	{
		HideSelection(SEL_SWITCH_OFF);
		ShowSelection(SEL_SWITCH_ON);
	}
	
	override void OnSwitchOff()
	{
		HideSelection(SEL_SWITCH_ON);
		ShowSelection(SEL_SWITCH_OFF);
	}
	
	// Inventory manipulation
	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);
		
		HideAttachedClipsStates();
		HideSelection(SEL_CLIPS_FOLDED);
		ShowSelection(SEL_CLIPS_DETACHED);
	}
	
	override void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);
		
		HideAttachedClipsStates();
		HideSelection(SEL_CLIPS_DETACHED);
		ShowSelection(SEL_CLIPS_FOLDED);
	}
	
	override void OnPlacementStarted( Man player )
	{	
		super.OnPlacementStarted( player );
		
		SetAnimationPhase( SEL_CLIPS_DETACHED, 0);
		SetAnimationPhase( SEL_CLIPS_FOLDED, 1);
		SetAnimationPhase( SEL_SWITCH_ON, 1);
		SetAnimationPhase( SEL_SWITCH_OFF, 1);
		SetAnimationPhase( SEL_LIGHT_STATE_1, 1);
		SetAnimationPhase( SEL_LIGHT_STATE_2, 1);
		SetAnimationPhase( SEL_LIGHT_STATE_3, 1);
	
		ref array<string> array_of_selections = {SEL_CORD_PLUGGED, SEL_CORD_FOLDED, SEL_CLIPS_DETACHED, SEL_CLIPS_FOLDED};	
		PlayerBase player_PB = PlayerBase.Cast( player );
				
		if( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			player_PB.GetHologramServer().SetSelectionToRefresh( array_of_selections );
		}
		else
		{
			player_PB.GetHologramLocal().SetSelectionToRefresh( array_of_selections );
		}
	}
}