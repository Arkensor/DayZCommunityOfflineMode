class DisplayElement extends MessageReceiverBase
{
	int		m_Value;
	int 	m_Key = 0;
	int		m_Type = 0;
	int 	m_Range = 0;
	int		m_Colors[7];
	string 	m_Labels[7];
	int		m_Pos = 1;
	int		m_NumOfBits = 0;
	bool 	m_IsClientOnly = false;
	
	PluginPlayerStatus 	m_ModulePlayerStatus;

	void DisplayElement()
	{
		m_System = "DisplayStatus";
		m_ModulePlayerStatus = PluginPlayerStatus.Cast(GetPlugin(PluginPlayerStatus));
		//SetMaxValue();
	}
	
	//for elements which get their value client side and can be updated much more quickly
	bool IsClientOnly()
	{
		return m_IsClientOnly;
	}
	/*
	void ImmediateUpdate()
	{
		
		if ( m_Type == TYPE_BAR )//<<<<--------- bar( hardcoded for stamina and stamina range, no generic Bar UI support)
		{
			m_ModulePlayerStatus.SetStamina( m_Value, m_Range );
		}
		if ( m_Type == TYPE_STANCE )
		{
			m_ModulePlayerStatus.SetStance( m_Value );
		}
		if ( m_Type == TYPE_PROGRESS_BAR )
		{
			m_ModulePlayerStatus.SetProgressBarState( m_Value );
		}
		if ( m_Type == TYPE_SINGLE_USE_ACTION_NAME )
		{
			m_ModulePlayerStatus.SetSingleUseActionName( m_Labels[ 0 ] );
		}
		if ( m_Type == TYPE_CONTINUOUS_ACTION_NAME )
		{
			m_ModulePlayerStatus.SetContinuousActionName( m_Labels[ 0 ] );
		}
		if ( m_Type == TYPE_SHOW_SINGLE_USE_ACTION )
		{
			m_ModulePlayerStatus.ShowSingleUseAction( (bool)m_Value );
		}
		if ( m_Type == TYPE_SHOW_CONTINUOUS_ACTION )
		{
			m_ModulePlayerStatus.ShowContinuousAction( (bool)m_Value );
		}
		
	}
	*/
	
	void UpdateHUD(){}

	
	int GetNumberOfBits()
	{
		return m_NumOfBits;
	}

	void SetValue(int value, int range = 0)
	{
		m_Value = value;
		if (range != 0 )
		{
			m_Range = range;
		}
	}
	
	void SetLevel( DSLevels level )
	{
		int new_value = level;
		new_value = new_value | m_Value;
		m_Value = new_value;
	}

	bool IsBlinking()
	{
		if(m_Value & 2)
		{
			return true;
		}
		return false;
	}
	
	int GetValue()
	{
		return m_Value;
	}

	int BitToDec(int mask, int index, int length)
	{
		int value = mask & (GetCompareMask() << index);
		value = value >> index;
		return value;
	}

	int GetCompareMask()
	{
		int mask = Math.Pow(2, GetNumberOfBits() ) - 1;
		return mask;
	}
	
	int GetType()
	{
		return m_Type;
	}
	
	int TranslateLevelToStatus(int level)
	{
		if( level == DSLevels.WARNING ) return 2;
		if( level == DSLevels.CRITICAL ) return 3;
		if( level == DSLevels.BLINKING ) return 4;
		return 1;
	}
}