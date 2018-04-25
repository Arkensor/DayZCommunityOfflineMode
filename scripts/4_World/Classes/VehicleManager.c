class VehicleManager
{		
	PlayerBase 						m_Player;
	HumanInputController 			m_HIC;
	protected ActionManagerBase	 	m_AM;
		
	void VehicleManager( PlayerBase player, ActionManagerBase mngr) 
	{
		m_Player = player;
		m_HIC = m_Player.GetInputController();
		m_AM = mngr;
	}
	
	//Called from players commandhandler each frame, checks input
	void Update( float deltaT )
	{
		PickAction();
	}
	
	protected void PerformAction( int actionID )
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			if ( !m_Player.GetCommandModifier_Action() && !m_Player.GetCommand_Action() )
			{
				ActionBase action = m_AM.GetAction(actionID);
				if ( action ) 
				{			
					//ActionTarget target = new ActionTarget( m_AM.FindActionTarget().GetObject(), -1, Vector(0,0,0), -1);
					ActionTarget target = new ActionTarget( m_Player.GetHierarchyParent(), null, -1, Vector(0,0,0), -1);
					if ( target && action.Can(m_Player, target, null ) )
					{
						action.Start(m_Player, target, null );
						if ( GetGame().IsClient() )
						{
							ScriptInputUserData ctx = new ScriptInputUserData;
							action.WriteToContext(ctx, target);
							ctx.Send();
						}
					}
				}
			}
		}
		else
		{
			m_AM.StartDeliveredAction();
		}
	}
	
	protected void PickAction()
	{
		Car car;
		if ( !Class.CastTo(car,  m_Player.GetHierarchyParent()) ) return;
		
		if ( m_Player )
		{
			/* // not synced to server
			if ( KeyState(KeyCode.KC_L) > 0 )
			{
				PerformAction( AT_VEH_SWITCH_LIGHTS );
			}
			*/
			switch ( m_HIC.IsGestureSlot() )
			{
				case 1 :
					PerformAction( AT_VEH_SWITCH_LIGHTS );
				break;
				
				case 2 :
					PerformAction( AT_VEH_ENGINE_START );
				break;
				
				case 3 :
					PerformAction( AT_VEH_ENGINE_STOP );
				break;
				/*
				case 4 :
					PerformAction( m_Player.m_ConfigEmotesProfile.GetEmoteSlot4() );
				break;
				
				case 5 :
					PerformAction( m_Player.m_ConfigEmotesProfile.GetEmoteSlot5() );
				break;
				
				case 6 :
					PerformAction( m_Player.m_ConfigEmotesProfile.GetEmoteSlot6() );
				break;
				
				case 7 :
					PerformAction( m_Player.m_ConfigEmotesProfile.GetEmoteSlot7() );
				break;
				*/
			}
		}
	}
};