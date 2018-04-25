
class DebugMonitorValues
{
	const int TYPE_HEALTH 		= 1;
	const int TYPE_BLOOD 		= 2;
	const int TYPE_BODY_TEMP 	= 3;
	const int TYPE_POSITION 	= 4;
	const int TYPE_LAST_DAMAGE 	= 5;
	
	const int LAST_DAMAGE_FALL	= 1;
	const int LAST_DAMAGE_HIT	= 2;

	const float VALUE_CHECK_INTERVAL	 	= 1;
	const float SENSITIVTY_PERCENTAGE 		= 1;//how much the value needs to change up/down from previous update to trigger a new update(in percent)

	PlayerBase m_Player;
	float m_TimeSinceLastTick = VALUE_CHECK_INTERVAL + 1;
	
	string m_CurrentLastDamage;
	
	float m_LastHealthUpdate;
	float m_LastBloodUpdate;
	string m_lastDamageUpdate;
	
	float m_HealthMaxValue;
	float m_BloodMaxValue;

	float m_BloodClient;
	float m_HealthClient;
	string m_LastDamageClient;
	
	void DebugMonitorValues(PlayerBase player)
	{
		m_Player = player;
		Init();
	}

	void Init()
	{
		m_HealthMaxValue 	= m_Player.GetMaxHealth("", "Health");
		m_BloodMaxValue 	= m_Player.GetMaxHealth("", "Blood");
	}
	
	void OnScheduledTick(float deltatime)
	{
		if( GetGame().IsClient() ) return;
		if( !GetGame().IsDebugMonitor() ) return; // turned off in server config
		
		m_TimeSinceLastTick += deltatime;
		
		if( m_TimeSinceLastTick > VALUE_CHECK_INTERVAL )
		{
			m_TimeSinceLastTick = 0;
			CheckValues();
		}
	}
	
	void CheckValues()
	{
		CheckHealth();
		CheckBlood();
		CheckLastDamage();
	}
	
	float GetBlood()
	{
		return m_BloodClient;
	}
	
	float GetHealth()
	{
		return m_HealthClient;
	}
	
	string GetLastDamage()
	{
		return m_LastDamageClient;
	}
	
	void SetLastDamage(string value)
	{
		m_CurrentLastDamage = value;
	}
	
	void CheckHealth()
	{
		float health_current = m_Player.GetHealth("","Health");
		float health_normalized = health_current / m_HealthMaxValue;
		float difference_normalized = health_normalized - m_LastHealthUpdate;
		float diff_abs = Math.AbsFloat(difference_normalized);
		
		if( diff_abs > ( SENSITIVTY_PERCENTAGE /100 ) )
		{
			Param1<float> param = new Param1<float>( health_current );

			SendValue(TYPE_HEALTH, param);
			m_LastHealthUpdate = health_normalized;
		}
	}
	
	void CheckBlood()
	{
		float blood_current = m_Player.GetHealth("","Blood");
		float blood_normalized = blood_current / m_BloodMaxValue;
		float difference_normalized = blood_normalized - m_LastBloodUpdate;
		float diff_abs = Math.AbsFloat(difference_normalized);
		
		if( diff_abs > ( SENSITIVTY_PERCENTAGE /100 ) )
		{
			Param1<float> param = new Param1<float>( blood_current );
			
			SendValue(TYPE_BLOOD, param);
			m_LastBloodUpdate = blood_normalized;
		}
	}
	
	void CheckLastDamage()
	{
		if (m_CurrentLastDamage != m_lastDamageUpdate)
		{
			Param1<string> param = new Param1<string>(m_CurrentLastDamage);
			SendValue(TYPE_LAST_DAMAGE, param);
			m_lastDamageUpdate = m_CurrentLastDamage;
		}
	}
	
	void SendValue(int value_type, Param param)
	{
		switch(value_type)
		{
		case DebugMonitorValues.TYPE_HEALTH:
		case DebugMonitorValues.TYPE_BLOOD:
			{	
				ref Param1<float> value_float = Param1<float>.Cast( param );

				CashedObjectsParams.PARAM2_INT_FLOAT.param1 = value_type;
				CashedObjectsParams.PARAM2_INT_FLOAT.param2 = value_float.param1;
				//Print("SendingValue type " + value_type.ToString() + " value "  + value_float.ToString());
				GetGame().RPCSingleParam(m_Player, ERPCs.RPC_DEBUG_MONITOR_FLT, CashedObjectsParams.PARAM2_INT_FLOAT, true, m_Player.GetIdentity());
			}
			break;
		case DebugMonitorValues.TYPE_LAST_DAMAGE:
			{
				ref Param1<string> value_string = Param1<string>.Cast( param );

				CashedObjectsParams.PARAM2_INT_STRING.param1 = value_type;
				CashedObjectsParams.PARAM2_INT_STRING.param2 = value_string.param1;
				//Print("SendingValue type " + value_type.ToString() + " value "  + value_string);
				GetGame().RPCSingleParam(m_Player, ERPCs.RPC_DEBUG_MONITOR_STR, CashedObjectsParams.PARAM2_INT_STRING, true, m_Player.GetIdentity());
			}
			break;
		}
	}

	void ReceiveValue(int value_type, Param param)
	{	
		GetGame().GetMission().CreateDebugMonitor();
		
		switch (value_type)
		{
		case DebugMonitorValues.TYPE_HEALTH:
			{
				ref Param1<float> valueHealth = Param1<float>.Cast( param );
				//Print("ReceivedValue health " + valueHealth.param1.ToString());
				m_HealthClient = valueHealth.param1;
			}
			break;
		case DebugMonitorValues.TYPE_BLOOD:
			{
				ref Param1<float> valueBlood = Param1<float>.Cast( param );
				//Print("ReceivedValue blood " + valueBlood.param1.ToString());
				m_BloodClient = valueBlood.param1;
			}
			break;
		case DebugMonitorValues.TYPE_LAST_DAMAGE:
			{
				ref Param1<string> valueLastDamage = Param1<string>.Cast( param );
				//Print("ReceivedValue lastdamage " + valueLastDamage.param1);
				m_LastDamageClient = valueLastDamage.param1;
			}
			break; 		
		}
	}

	void OnRPCFloat(ParamsReadContext ctx)
	{
		ref Param1<float> value = new Param1<float>(0);
		
		ctx.Read(CashedObjectsParams.PARAM2_INT_FLOAT);
		int value_type 	= CashedObjectsParams.PARAM2_INT_FLOAT.param1;
		value.param1 	= CashedObjectsParams.PARAM2_INT_FLOAT.param2;
		
		ReceiveValue(value_type, value);
	}
	
	void OnRPCString(ParamsReadContext ctx)
	{
		ref Param1<string> value = new Param1<string>("");

		ctx.Read(CashedObjectsParams.PARAM2_INT_STRING);
		int value_type 	= CashedObjectsParams.PARAM2_INT_STRING.param1;
		value.param1 	= CashedObjectsParams.PARAM2_INT_STRING.param2;
		
		ReceiveValue(value_type, value);
	}
};
