class SneezeState extends StateBase
{
	//this is just for the state parameters set-up and is called even if the state doesn't execute, don't put any gameplay code in here
	override void OnInit()
	{
		m_StateType = StateTypes.PRIMARY;
		m_Priority = 0;
		m_ID = StateIDs.STATE_SNEEZE;
		m_DestroyOnAnimFinish = true;
		m_SyncToClient = false;
	}
	
	//!gets called every frame
	override void OnUpdateServer(PlayerBase player, float deltatime)
	{

	}

	override void OnUpdateClient(PlayerBase player, float deltatime)
	{
		int i = 1 + 1;
	}
	
	//!gets called once on an state which is being activated
	override void OnGetActivatedServer(PlayerBase player)
	{
		PlaySound("sneeze");
		ManTrigger trigger = ManTrigger.Cast(GetGame().CreateObject("ManTrigger", player.GetPosition(),true, false));
		PluginTransmissionAgents plugin = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));

		
		for(int i = 0; i < trigger.GetInsiders().Count(); i++)
		{
			PlayerBase other_player = PlayerBase.Cast(trigger.GetInsiders().Get(i).GetObject());
			if(other_player != player)
			{
				plugin.TransmitAgents(player, other_player, AGT_AIRBOURNE, 20);
			}
			
		}
		trigger.Delete();
	}

	//!gets called once on an state which is being activated
	override void OnGetActivatedClient(PlayerBase player)
	{
	}

	//!only gets called once on an active state that is being deactivated
	override void OnGetDeactivatedServer(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated SneezeState called", "PlayerState");
	}

	//!only gets called once on an active state that is being deactivated
	override void OnGetDeactivatedClient(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated SneezeState called", "PlayerState");
	}
}
