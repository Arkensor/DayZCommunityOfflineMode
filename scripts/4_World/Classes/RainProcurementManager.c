class RainProcurementManager
{
	protected 			ItemBase 	m_ProcuringItem;	
	protected 			int 		m_IsUnderRoof;
	protected	ref	Timer 		m_UpdateTimer;
	protected 	const 	int 		RAIN_COEFFICIENT = 10;

	void RainProcurementManager( ItemBase procuring_item )
	{
		m_ProcuringItem = procuring_item;
	}
	
	// ----------------------------------------------------------------------------------------
	void InitRainProcurement()
	{
		m_IsUnderRoof = IsUnderRoof();

		//m_ProcuringItem.SetQuantity(0); /*set to 0 for debug purposses*/

		if( !m_IsUnderRoof )
		{
			m_UpdateTimer = new Timer();
			m_UpdateTimer.Run( 10, this, "RainProcurementCheck", NULL, true ); 
		}
	}

	// ----------------------------------------------------------------------------------------
	void RainProcurementCheck()
	{		
		float rain_intensity = GetGame().GetWeather().GetRain().GetActual();
		float fill_per_update = RAIN_COEFFICIENT * rain_intensity;

		if ( rain_intensity > 0 ) 
		{
			if ( m_ProcuringItem.GetQuantity() < m_ProcuringItem.GetQuantityMax() )
			{
				m_ProcuringItem.AddQuantity( fill_per_update );

				//Print( "Quantity of " + m_ProcuringItem + " is: " +  m_ProcuringItem.GetQuantity() );
			}
			else
			{		
				//Print("vesel full");		
				StopRainProcurement();				
			}
		}
	}

	// ----------------------------------------------------------------------------------------
	bool IsUnderRoof() 
	{
		vector minMax[2];
		m_ProcuringItem.GetCollisionBox(minMax);

		vector size = Vector(0,0,0);
		size[1] = minMax[1][1] - minMax[0][1];

		vector from = m_ProcuringItem.GetPosition() + size;  
		vector ceiling = "0 20 0";
		vector to = from + ceiling;
		vector contact_pos;
		vector contact_dir;

		int contact_component;	
	
		return DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contact_component, NULL, NULL, m_ProcuringItem );	
	}

	// ----------------------------------------------------------------------------------------
	void StopRainProcurement()
	{
		if( !m_IsUnderRoof )
		{
			m_UpdateTimer.Stop();	
		}
		
	}
};