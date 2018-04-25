class ActionPullOutPlug: ActionInteractBase
{
	void ActionPullOutPlug()
	{
		m_MessageSuccess = "I've unplugged the device.";
		m_MessageStart = "";
	}

	override int GetType()
	{
		return AT_PULL_OUT_PLUG;
	}
		
	override string GetText()
	{
		/*
		string return_string = "unplug";
		string selection;
		PlayerBase m_Player = GetGame().GetPlayer();
		g_Game.GetPlayerCursorObjectComponentName( m_Player, selection );
		EntityAI target_EAI = g_Game.GetPlayerCursorObject(m_Player);
		EntityAI device = target_EAI.GetCompEM().GetPlugOwner(selection);
		
		if ( device )
		{
			return_string = return_string + " " + device.GetName();
		}
		
		return return_string;
		*/
		
		return "pull out plug";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		EntityAI target_EAI = EntityAI.Cast( target.GetObject() ); // cast to EntityAI
		
		if ( target_EAI.HasEnergyManager() )
		{
			string selection = targetObject.GetActionComponentName(target.GetComponentIndex());
			EntityAI device = target_EAI.GetCompEM().GetPlugOwner(selection);
			
			if ( device )
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		Object targetObject = target.GetObject();
		if ( targetObject )
		{
			EntityAI target_EAI = EntityAI.Cast( targetObject );
			string selection = targetObject.GetActionComponentName(target.GetComponentIndex());
			
			EntityAI device = EntityAI.Cast( target_EAI.GetCompEM().GetPlugOwner(selection) );
			if ( device )
			{
				device.GetCompEM().UnplugThis();
			}
			
			// Disable Advanced Placement
			if ( player.IsPlacingServer() )
			{
				player.PlacingCancelServer();
			}
		}
	}
};