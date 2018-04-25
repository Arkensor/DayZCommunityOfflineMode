class Defibrillator extends ItemBase
{
	static const string	CHARGING_SOUND = 			"defibCharge";
	static const string	CHARGED_AND_READY_SOUND = 	"defibReady";
	static const string	SHOCK_SOUND = 				"defibShock";
	
	bool m_IsCharged = false;
	
	static float m_ChargeTime = 8;
	static float m_EnergyNeededToCharge = 20; 
	
	ref Timer m_ChargingTimer;
	SoundOnVehicle m_ChargedAlarm;
	
	void Defibrillator()
	{
		// Read all config parameters
		m_ChargeTime = GetTimeNeededToCharge();
		m_EnergyNeededToCharge = GetEnergyNeededToCharge();
		
		if (m_ChargeTime == 0)
			m_ChargeTime = 0.1;
		
	}
	
	float GetTimeNeededToCharge()
	{
		string cfg_path = "CfgVehicles " + GetType() + " ";
		return GetGame().ConfigGetFloat (cfg_path + "defibChargeTime");
	}
	
	float GetEnergyNeededToCharge()
	{
		string cfg_path = "CfgVehicles " + GetType() + " ";
		return GetGame().ConfigGetFloat (cfg_path + "defibEnergyNeededPerCharge");
	}
	
	override void OnWorkStart()
	{
		if (!GetGame().IsMultiplayer()  ||  GetGame().IsClient())
			PlaySound(CHARGING_SOUND, 20);
		
		float energy_needed = m_EnergyNeededToCharge / m_ChargeTime;
		GetCompEM().SetEnergyUsage(energy_needed);
		
		if (!m_ChargingTimer)
			m_ChargingTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		
		if ( !m_ChargingTimer.IsRunning() )
		{
			m_ChargingTimer.Run(m_ChargeTime, this, "OnIsCharged", NULL, false);
		}
	}
	
	override void OnWorkStop()
	{
		GetCompEM().SwitchOff();
		GetCompEM().ResetEnergyUsage();
		StopChargingTimer();
		StopChargedAlarm();
		
		m_IsCharged = false;
	}
	
	void OnIsCharged()
	{
		if ( GetCompEM().IsWorking() )
		{
			if (!GetGame().IsMultiplayer()  ||  GetGame().IsClient())
				m_ChargedAlarm = PlaySoundLoop(CHARGED_AND_READY_SOUND, 40);
			
			GetCompEM().ResetEnergyUsage();
			m_IsCharged = true;
		}
	}
	
	void StopChargedAlarm()
	{
		if (m_ChargedAlarm)
		{
			GetGame().ObjectDelete(m_ChargedAlarm);
			m_ChargedAlarm = NULL;
		}
	}
	
	void StopChargingTimer()
	{
		if (m_ChargingTimer)
		{
			m_ChargingTimer.Stop();
			m_ChargingTimer = NULL;
		}
	}
	
	bool IsCharged()
	{
		return m_IsCharged;
	}
	
	void Discharge(PlayerBase victim)
	{
		if (!GetGame().IsMultiplayer()  ||  GetGame().IsClient())
			PlaySound(SHOCK_SOUND, 40);
		
		bool has_heart_attack = victim.m_ModifiersManager.IsModifierActive(eModifiers.MDF_HEART_ATTACK);
		
		if ( has_heart_attack )
		{
			victim.m_ModifiersManager.DeactivateModifier ( eModifiers.MDF_HEART_ATTACK );
		}
		else
		{
			victim.m_ModifiersManager.ActivateModifier ( eModifiers.MDF_HEART_ATTACK );
			victim.GetStatShock().Add(500);
		}
		
		GetCompEM().SwitchOff();
	}
}