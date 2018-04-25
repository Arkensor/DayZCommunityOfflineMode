class StanceIndicator
{	
	protected ref HumanMovementState	m_State;
	protected PlayerBase					m_Player;
	
	void StanceIndicator(PlayerBase player)
	{		
		m_State = new HumanMovementState();
		m_Player = player;
	}
	
	void Update()
	{
		if ( m_Player )
		{
			m_Player.GetMovementState(m_State);
			int player_stance = m_State.m_iStanceIdx;
			int hud_stance_id = 1;
			//if ( player_stance == DayZPlayerConstants.STANCEIDX_ERECT  || player_stance == DayZPlayerConstants.STANCEIDX_RAISEDERECT)  hud_stance_id = 1; 
			if ( player_stance == DayZPlayerConstants.STANCEIDX_CROUCH || player_stance == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH)
			{
				hud_stance_id = 2;
			}
			if ( player_stance == DayZPlayerConstants.STANCEIDX_PRONE  || player_stance == DayZPlayerConstants.STANCEIDX_RAISEDPRONE) 
			{
				hud_stance_id = 3;
			}
			DisplayStance(hud_stance_id);
			//Debug
			//m_Player.MessageStatus(ToString(player) + "StanceIndicator.c || stance: " + ToString(player_stance));
		}
	}
	
	void DisplayStance(int stance)
	{	
		if ( m_Player )
		{
			m_Player.GetDisplayStatus().SetValue(eDisplayElements.DELM_STANCE, stance);
		}
	}
};