
class DayZPlayerImplementSwimming
{

	DayZPlayer 					m_pPlayer;
	SHumanCommandSwimSettings	m_pSettings;

	void DayZPlayerImplementSwimming(DayZPlayer pPlayer)
	{
		m_pPlayer 	= pPlayer;
		m_pSettings	= pPlayer.GetDayZPlayerType().CommandSwimSettingsW();
	}

	float  GetWaterDepth()
	{
		vector 	pp = m_pPlayer.GetPosition();
		vector  wl = HumanCommandSwim.WaterLevelCheck(m_pPlayer, pp);

		return wl[0];
	}

	float  GetCharacterDepth()
	{
		vector 	pp = m_pPlayer.GetPosition();
		vector  wl = HumanCommandSwim.WaterLevelCheck(m_pPlayer, pp);

		return wl[1];
	}



	//! ha
	bool HandleSwimming(int pCurrentCommandID, HumanCommandMove pCMove, HumanMovementState pState)
	{
		//! get water level 
		if (pCurrentCommandID != DayZPlayerConstants.COMMANDID_SWIM)
		{
			vector 	pp = m_pPlayer.GetPosition();
			vector  wl = HumanCommandSwim.WaterLevelCheck(m_pPlayer, pp);
			
			//! if total water depth >= 1.5m && character is 1.5m in water 
			if (wl[0] >= m_pSettings.m_fWaterLevelIn && wl[1] >= m_pSettings.m_fWaterLevelIn)
			{
				m_pPlayer.StartCommand_Swim();
				return true;
			}
			
			//! now movement
			if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
			{
				if (wl[1] > m_pSettings.m_fToErectLevel)
				{
					pCMove.ForceStanceUp(DayZPlayerConstants.STANCEIDX_ERECT);
				}
				else if (wl[1] > m_pSettings.m_fToCrouchLevel)
				{
					pCMove.ForceStanceUp(DayZPlayerConstants.STANCEIDX_CROUCH);
				}
			}

			// not handled 
			return false;
		}
		else // if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_SWIM)
		{
			if (GetWaterDepth() < m_pSettings.m_fWaterLevelOut)
			{
				HumanCommandSwim hcs = m_pPlayer.GetCommand_Swim();
				hcs.StopSwimming();
				return true;
			}
		
			//! handled !
			return true;
		}
	}
}