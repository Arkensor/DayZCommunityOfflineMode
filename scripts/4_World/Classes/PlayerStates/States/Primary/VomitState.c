class VomitState extends StateBase
{
	//just for the state parameters set-up and gets called even if the state doesn't execute, don't put any gameplay code in here
	override void OnInit()
	{
		m_StateType = StateTypes.PRIMARY;
		m_Priority = 100;
		m_ID = StateIDs.STATE_VOMIT;
		m_DestroyOnAnimFinish = true;
		m_SyncToClient = false;
		m_Duration = 5;
		m_QuitAnimOnStateDestroy = true;
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
		//timer.Run(10, this, "Destroy");
		Debug.Log("OnGetActivated VomitState called", "PlayerState");
		player.GetStatStomachSolid().Set(0);
		player.GetStatStomachWater().Set(0);
		//player.GetStatEnergy().Set(0);
		PlayAnimation(DayZPlayerConstants.CMD_ACTIONFB_VOMIT,DayZPlayerConstants.STANCEMASK_CROUCH, GetDuration() );
	}

	override void OnGetActivatedClient(PlayerBase player)
	{
		
	}

	//!only gets called once on an active state that is being deactivated
	override void OnGetDeactivatedServer(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated VomitState called", "PlayerState");
	}

	override void OnGetDeactivatedClient(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated VomitState called", "PlayerState");
	}
}