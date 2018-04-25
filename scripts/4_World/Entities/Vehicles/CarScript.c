/*!
	Base script class for all motorized wheeled vehicles.
*/
class CarScript extends Car
{
	protected float m_Time;

	//! keeps ammount of each fluid
	protected float m_FuelAmmount;
	protected float m_CoolantAmmount;
	protected float m_OilAmmount;
	protected float m_BrakeAmmount;
	
	//!
	protected float m_RadiatorHealth;
	protected float m_FuelTankHealth;


	void CarScript()
	{
		Init();
	}
	
	override void Init()
	{
		super.Init();
		SetEventMask(EntityEvent.POSTSIMULATE);

		m_Time = 0;

		// sets max health for all components at init
		m_RadiatorHealth = 1;
		m_FuelTankHealth = 1;
	}

	override void EOnPostSimulate(IEntity other, float timeSlice)
	{
		m_Time += timeSlice;
		//! move it to constants.c const float CAR_UPDATE_INTERVAL = 1.0
		if ( m_Time >= 1 )
		{
			m_Time = 0;
			CarPartsHealthCheck();
			//! leaking of coolant from radiator when damaged
			if( GetFluidFraction(CarFluid.COOLANT) > 0 && m_RadiatorHealth < 1 )
				LeakFluid(CarFluid.COOLANT);

			//! leaking of fuel from damaged fuel tank
			//if( GetFluidFraction(CarFluid.FUEL) > 0 && m_FuelTankHealth < 1 )
			//	LeakFluid(CarFluid.FUEL);
	
			//! actions runned when the engine on
			if ( IsEngineOn() )
			{
			}
		}
	}

	/*!
		Gets called everytime when the specified vehicle's fluid
		changes its current value eg. when car is consuming fuel.

		This callback is called on both server and client.
	*/
	void OnFluidChanged( CarFluid fluid, float newValue, float oldValue )
	{
		switch ( fluid )
		{
			case CarFluid.FUEL:
				m_FuelAmmount = newValue;
				break;

			case CarFluid.OIL:
				m_OilAmmount = newValue;
				break;

			case CarFluid.BRAKE:
				m_BrakeAmmount = newValue;
				break;

			case CarFluid.COOLANT:
				m_CoolantAmmount = newValue;
				break;
		}
	}

	/*!
		Gets called everytime the game wants to start the engine.
		This callback is called on server only.

		\return true if the engine can start, false otherwise.
	*/
	bool OnBeforeEngineStart()
	{
		// todo :: check if the battery is plugged-in
		//         check if we have enough power to start the engine

		//if ( GetHealth01("engine", "") <= 0 )
		//	return false;

		//if ( GetFluidFraction(CarFluid.FUEL) <= 0 )
		//	return false;

		return true;
	}

	//! Gets called everytime the engine starts.
	void OnEngineStart()
	{
	}

	//! Gets called everytime the engine stops.
	void OnEngineStop()
	{
	}

	/*!
		Gets called everytime the game wants to switch the lights.
		\return true when lights can be switched, false otherwise.
	*/
	bool OnBeforeSwitchLights( bool toOn )
	{
		if ( toOn )
		{
			// todo :: check if the battery is plugged-in
			//         check if we have enough power to turn on the lights

			// this is the case from off -> on
			return true;
		}

		// this is the case on -> off
		return true;
	}
	
	protected void LeakFluid(CarFluid fluid)
	{
		float ammount = 0;
		
		switch (fluid)
		{
			case CarFluid.COOLANT:
				Print(m_RadiatorHealth);
				//! move this to constant.c
 				//! CAR_COOLANT_LEAK_PER_SEC_MIN = 0.02; CAR_COOLANT_LEAK_PER_SEC_MAX = 0.12;
				ammount = m_RadiatorHealth * Math.RandomFloat(0.02, 0.12);
				Print("coolant is leaking for: " + ammount);
				Leak(fluid, ammount);
			break;
		}
	}

	protected void CarPartsHealthCheck()
	{
		if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			m_RadiatorHealth = GetHealth01("radiator", "");
		}
	}
};
