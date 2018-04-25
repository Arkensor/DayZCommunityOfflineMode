class AITargetCallbacksPlayer : AITargetCallbacks
{
	void AITargetCallbacksPlayer(PlayerBase pPlayer)
	{
		m_Player = pPlayer;
		
		m_iHeadBoneIndex = m_Player.GetBoneIndexByName("Head");
		m_iChestBoneIndex = m_Player.GetBoneIndexByName("Spine3");
	}
	
	override vector GetHeadPositionWS() 
	{ 
		if( m_iHeadBoneIndex != -1 )
			return m_Player.GetBonePositionWS(m_iHeadBoneIndex);
		else
			return m_Player.GetPosition() + "0 1.7 0";
	}
	
	override vector GetVisionPointPositionWS(EntityAI pApplicant) 
	{ 
		DayZInfected infected = DayZInfected.Cast(pApplicant);
		if( infected )
		{
			DayZInfectedInputController ic = infected.GetInputController();
			if( ic )
			{
				int mindState = ic.GetMindState();
				if( mindState >= DayZInfectedConstants.MINDSTATE_ALERTED )
				{
					return GetHeadPositionWS();
				}
				else
				{
					if( m_iChestBoneIndex != -1 )
						return m_Player.GetBonePositionWS(m_iChestBoneIndex);
				}				
			}
		}
		
		return m_Player.GetPosition() + "0 1 0";
	}
	
	override float GetMaxVisionRangeModifier(EntityAI pApplicant)
	{ 
		HumanMovementState state = new HumanMovementState;
		m_Player.GetMovementState(state);
		
		float mod = 1.0;

		//! stance modification
		switch(state.m_iStanceIdx)
		{
		case DayZPlayerConstants.STANCEIDX_CROUCH:
		case DayZPlayerConstants.STANCEIDX_RAISEDCROUCH:
			mod *= 0.66;
			break;
			
		case DayZPlayerConstants.STANCEIDX_PRONE:
		case DayZPlayerConstants.STANCEIDX_RAISEDPRONE:
			mod *= 0.33;
			break;
		}
		
		//! clothes modification
		
		
		return mod; 
	}

	private PlayerBase m_Player;
	private int m_iHeadBoneIndex;
	private int m_iChestBoneIndex;
};