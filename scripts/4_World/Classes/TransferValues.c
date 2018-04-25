

class TransferValues extends Managed
{

	const int TYPE_HEALTH 	= 1;
	const int TYPE_BLOOD 	= 2;
	
	const float VALUE_CHECK_INTERVAL	 	= 5;
	const float SENSITIVTY_PERCENTAGE 		= 1;//how much the value needs to change up/down from previous update to trigger a new update(in percent)

	PlayerBase m_Player;
	float m_TimeSinceLastTick = VALUE_CHECK_INTERVAL + 1;
	
	float m_LastHealthUpdate;
	float m_LastBloodUpdate;

	float m_HealthMaxValue;
	float m_BloodMaxValue;
	
	float m_BloodClient;
	float m_HealthClient = 1;
	
	void TransferValues(PlayerBase player)
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
		#ifdef DEVELOPER
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_TRANSFER_VALUES_SHOW) )
		{
			ShowDebugValues(true);
		}
		else
		{
			ShowDebugValues(false);
		}
		#endif
		
		if( GetGame().IsClient() ) return;
		
		m_TimeSinceLastTick += deltatime;
		
		if( m_TimeSinceLastTick > VALUE_CHECK_INTERVAL )
		{
			/*
			Print(m_TimeSinceLastTick.ToString());
			Print(VALUE_CHECK_INTERVAL.ToString());
			Print("--------------");
			*/
			m_TimeSinceLastTick = 0;
			CheckValues();
			
			// send sync junctures if necessary
			// TODO: !!!! event is sent too often, please fix it
			/*
			float damage = 1 - m_Player.GetHealth("", "") / 100;
			DayZPlayerSyncJunctures.SendInjury(m_Player, true, damage);
			*/
		}
	}
	
	void CheckValues()
	{
		CheckHealth();
		CheckBlood();
	}
	
	float GetBlood()
	{
		return m_BloodClient;
		
	}
	
	float GetHealth()
	{
		return m_HealthClient;
	}
	
	void CheckHealth()
	{
		float health_current = m_Player.GetHealth("","Health");
		float health_normalized = health_current / m_HealthMaxValue;
		float difference_normalized = health_normalized - m_LastHealthUpdate;
		float diff_abs = Math.AbsFloat(difference_normalized);
		
		if( diff_abs > ( SENSITIVTY_PERCENTAGE /100 ) )
		{
			SendValue(TYPE_HEALTH, health_normalized);
			m_LastHealthUpdate = health_normalized;
		}
		
	}
	
	void CheckBlood()
	{
		float blood_current = m_Player.GetHealth("","Blood");
		//float blood_normalized = blood_current / m_BloodMaxValue;
		float blood_normalized = Math.InverseLerp(ModifierConstants.BLOOD_THRESHOLD_FATAL, m_BloodMaxValue, blood_current);
		float difference_normalized = blood_normalized - m_LastBloodUpdate;
		float diff_abs = Math.AbsFloat(difference_normalized);
		
		if( diff_abs > ( SENSITIVTY_PERCENTAGE /100 ) )
		{
			SendValue(TYPE_BLOOD, blood_normalized);
			m_LastBloodUpdate = blood_normalized;
		}
		
	}
	
	
	void SendValue(int value_type, float value)
	{
		CashedObjectsParams.PARAM2_INT_FLOAT.param1 = value_type;
		CashedObjectsParams.PARAM2_INT_FLOAT.param2 = value;
		
		GetGame().RPCSingleParam(m_Player, ERPCs.RPC_DAMAGE_VALUE_SYNC, CashedObjectsParams.PARAM2_INT_FLOAT, true, m_Player.GetIdentity());
	}

	void ReceiveValue(int value_type, float value)
	{
		if( value_type == TYPE_HEALTH )
		{
			m_HealthClient = value;
		}
		else if( value_type == TYPE_BLOOD )
		{
			m_BloodClient = value;
		}
	}

	void OnRPC(ParamsReadContext ctx)
	{
		ctx.Read(CashedObjectsParams.PARAM2_INT_FLOAT);
		
		int value_type 	= CashedObjectsParams.PARAM2_INT_FLOAT.param1;
		float value 	= CashedObjectsParams.PARAM2_INT_FLOAT.param2;
		
		ReceiveValue(value_type, value);
	}
	
	void ShowDebugValues(bool show)
	{
		if( GetGame().IsMultiplayer() && GetGame().IsServer() ) return;
		
		if( show )
		{
			
			DbgUI.BeginCleanupScope();     
	        DbgUI.Begin("Values", 50, 50);
	        DbgUI.Text("Blood: " + m_BloodClient.ToString());
			DbgUI.Text("Health: " + m_HealthClient.ToString());

	        DbgUI.End();
	        DbgUI.EndCleanupScope();
		}
		else
		{
			DbgUI.BeginCleanupScope();
			DbgUI.Begin("Values", 50, 50);
			DbgUI.End();
	        DbgUI.EndCleanupScope();   
		}
		
	}
}
