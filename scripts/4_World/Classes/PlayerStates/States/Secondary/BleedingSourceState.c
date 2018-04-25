const float BLEED_PER_SEC = -4;
class BleedingSourceState extends StateBase
{
	ref Param1<vector> m_Position = new Param1<vector>("0 0 0");
	//this is just for the state parameters set-up and is called even if the state doesn't execute, don't put any gameplay code in here

	void ~BleedingSourceState()
	{

	}

	override void OnInit()
	{
		m_StateType = StateTypes.SECONDARY;
		m_Priority = 0;
		m_ID = StateIDs.STATE_BLEEDING_SOURCE;
		m_DestroyOnAnimFinish = false;
		m_IsPersistent = false;
		m_SyncToClient = true;
	}
	
	//!gets called every frame
	override void OnUpdateServer(PlayerBase player, float deltatime)
	{
		player.AddHealth("GlobalHealth","Blood", (BLEED_PER_SEC * deltatime) );
	}

	override void OnUpdateClient(PlayerBase player, float deltatime)
	{
		//int i = 1 + 1;
	}
	
	//!gets called once on an state which is being activated
	override void OnGetActivatedServer(PlayerBase player)
	{
		/*
		Param1<vector> pv3 = m_Param_01;
		if(pv3) m_Position.param1 = pv3.param1;
		Print("OnGetActivatedServer called");
		Print( m_Position.param1.ToString() );
		m_BloodParticle = Particle.Create( ParticleList.BLEEDING_SOURCE, GetPlayer(),m_Position.param1 );
		m_BloodParticle.Play();
		*/
		
	}

	override void OnGetActivatedClient(PlayerBase player)
	{
	}

	override void OnGetDeactivatedServer(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated CoughState called", "PlayerState");

	}
	
	//!only gets called once on an active state that is being deactivated
	override void OnGetDeactivatedClient(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated CoughState called", "PlayerState");
	}
	
}
