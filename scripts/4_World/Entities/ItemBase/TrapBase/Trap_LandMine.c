class LandMineTrap extends TrapBase
{
	SoundOnVehicle m_TimerLoop;
	
	void LandMineTrap()
	{
		m_DefectRate = 15;
		m_DamagePlayers = 0; 			//How much damage player gets when caught
		m_InitWaitTime = 10; 			//After this time after deployment, the trap is activated
		m_InfoActivationTime = "#STR_LandMineTrap0" + m_InitWaitTime.ToString() + "#STR_LandMineTrap1"; 
		
		m_AddDeactivationDefect = true;
	}
	
	override void StartActivate( PlayerBase player )
	{
		super.StartActivate( player );
		
		if (GetGame().IsClient())
		{
			PlaySound("landmine_safetyPin", 3);
			m_TimerLoop = PlaySoundLoop("landmine_timer2", 3);
		}
	}
	
	override void OnActivate()
	{
		if (GetGame().IsClient())
		{
			PlaySound("landmine_ready", 3);
		}
		
		GetGame().ObjectDelete(m_TimerLoop);
		m_TimerLoop = NULL;
	}
	
	override void OnPlacementComplete( Man player ) 
	{
		PlayerBase player_PB = PlayerBase.Cast( player );
		StartActivate( player_PB );
	}
	
	override bool CanExplodeInFire()
	{
		return true;
	}
	
	override void OnSteppedOn(EntityAI victim)
	{
		this.Explode();
	}
	
	override void EEItemLocationChanged  ( EntityAI old_owner, EntityAI new_owner ) 
	{
		super.EEItemLocationChanged(old_owner, new_owner);
		
		if ( g_Game.IsServer() )
		{
			
		}
	}
}
