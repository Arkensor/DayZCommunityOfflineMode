class BlindnessState extends StateBase
{
	Material m_MatGauss;
	const int BLUR_DURATION = 3000;
	ref Param1<vector> m_Position = new Param1<vector>("0 0 0");
	
	//this is just for the state parameters set-up and is called even if the state doesn't execute, don't put any gameplay code in here
	override void OnInit()
	{
		m_StateType = StateTypes.SECONDARY;
		m_Priority = 0;
		m_ID = StateIDs.STATE_BLINDNESS;
		m_DestroyOnAnimFinish = true;
		m_IsPersistent = true;
		MakeParamObjectPersistent(m_Position);
	}
	
	//!gets called every frame
	override void OnUpdateServer(PlayerBase player, float deltatime)
	{
	}

	override void OnUpdateClient(PlayerBase player, float deltatime)
	{

		
	}
	
	//!gets called once on an state which is being activated
	override void OnGetActivatedServer(PlayerBase player)
	{

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
		PPEffects.DisableBurlapSackBlindness();
	}
	
}
