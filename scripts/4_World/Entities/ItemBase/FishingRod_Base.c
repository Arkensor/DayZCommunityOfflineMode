class FishingRod_Base : ItemBase
{
	const string    ANIM_PHASE_OPENED = "OpenRod";	
	const string    ANIM_PHASE_CLOSED = "CloseRod";
	
	const float 	BREAK_PULL = 36;
	const float 	TOO_MUCH_PULL = 32;
	const float 	HIGH_PULL = 22;
	const float 	LOW_PULL = 18;
	const float 	TOO_FEW_PULL = 8;
	const float 	LOOSE_PULL = 0;
	const int 		LOSS_BREAK = 0;
	const int 		LOSS_LOOSE = 1;
	const int 		WIN = 2;
	const int 		INTERUPTED = 3;
	const float		INIT_LINE_STRETCH = 15;
	const float		MIN_FISHING_TIME = 100;
	const float		MAX_FISHING_TIME = 150;
	const int		FISH_PULL_CHANCE = 15;
	const int		FISH_AGILITY = 15;
	
	int				m_FishProximity;
	bool			m_Fishing;
	ref Timer 		m_CyclerCatching;
	ref Timer 		m_Cycler;
	float 			m_Pull;
	float			m_DeltaT;
	bool			m_WasPulled;
	bool			m_IsFishPulling;
	float			m_PullTime;
	float			m_FishPull;
	int				m_FishingTime;
	PlayerBase 		m_Player;
	int				m_ChanceRange;
	
	void FishingRod_Base()
	{
		m_Fishing = false;
	}
	
	void Init()
	{
		m_Player = PlayerBase.Cast( GetGame().GetPlayer() );
		m_Pull = INIT_LINE_STRETCH;
		m_IsFishPulling = false;
		//TIMERDEPRECATED - randomized time of fishing
		m_FishingTime = Math.RandomInt(MIN_FISHING_TIME,MAX_FISHING_TIME);
		m_ChanceRange = FISH_AGILITY;
		m_FishProximity = FISH_AGILITY;
		//TIMERDEPRECATED	
		if(!m_Cycler)
		{
			m_Cycler = new Timer();
		}
		if(!m_CyclerCatching)
		{
			m_CyclerCatching = new Timer();
		}
		m_CyclerCatching.Run(4, this, "Catching", NULL,true);
		m_Player.Message("I have started fishing.", "colorFriendly");
	}

	bool IsFishingActive()
	{
		return m_Fishing;
	}
		
	void ActivateFishing()
	{
		m_Fishing = true;
		Init();
	}

	void DeactivateFishing()
	{
		End(INTERUPTED);
	}



	void FishPull()
	{

		if ( !m_IsFishPulling )
		{
			float rand = Math.RandomInt(1, FISH_PULL_CHANCE);
			if ( rand <= 2 && m_Pull <= TOO_MUCH_PULL && m_Pull >= TOO_FEW_PULL )
			{
				//TIMERDEPRECATED - randomizing timeframe to pull fish out
				if ( Math.RandomInt(0, 1) )
				{
					m_FishPull = rand;
				}
				else
				{
					m_FishPull = -rand;
				}
				//TIMERDEPRECATED - randomizing timeframe to pull fish out
				m_PullTime = Math.RandomInt(2, 6);
				m_IsFishPulling = true;
			}
		}
		else
		{
			m_PullTime--;
			if (m_PullTime == 0)
			{
				m_IsFishPulling = false;
			}
		}
	}

	void Catching()
	{
		if(m_FishProximity == 0 && m_WasPulled)
		{
			m_CyclerCatching.Stop();
			m_Cycler.Run(0.3, this, "Pulling", NULL,true);
		}
		else
		{
			if (m_WasPulled)
			{
				m_WasPulled = false;
				m_ChanceRange++;
			}
			//TIMERDEPRECATED - randomized proximity of fish to 
			m_FishProximity = Math.RandomInt(0,m_ChanceRange);
			string fline = "";
			for (int i = 0; i < m_FishProximity; i++)
			{
				fline += " .";
			}
			m_Player.Message("", "colorStatusChannel");
			m_Player.Message("HOLD right mouse button to pull the fishing line", "colorAction");
			m_Player.Message("", "colorStatusChannel");
			m_Player.Message("", "colorStatusChannel");
			m_Player.Message("", "colorStatusChannel");
			m_Player.Message("Start pulling when J and <*)))>< meet:", "colorStatusChannel");
			m_Player.Message("", "colorStatusChannel");
			m_Player.Message("J"+fline+" <*)))><", "colorImportant");
			if(m_ChanceRange > 0)
			{
				m_ChanceRange--;
			}
		}
	}




	void Pulling()
	{	
		float agent_speed_distance = vector.Distance("0 0 0",m_Player.GetModelSpeed());
		if (m_Player.GetItemInHands() != this || !m_Player.IsWaterContact() || agent_speed_distance > 1 )
		{
			End(INTERUPTED);
		}
		else
		{
			if ( m_FishingTime <= 0)
			{ 
				ItemBase pcatch = ItemBase.Cast( m_Player.GetInventory().CreateInInventory("Carp") );
				//ItemBase pcatch = ItemBase.Cast( m_Player.CreateInInventory("Carp","cargo_bait") );
				pcatch.SetQuantity(800,false);
				End(WIN);
			}
			else
			{
				FishPull();
				if ( m_IsFishPulling )
				{
					m_Pull += m_FishPull;
				}
				if (m_WasPulled)
				{
					m_Pull += 1;
				}
				else
				{
					m_Pull -= 1;
				}
				m_WasPulled = false;
				m_FishingTime--;
				DisplayState();
				if ( m_Pull <= LOOSE_PULL)
				{
					End(LOSS_LOOSE);
				}
				if ( m_Pull >= BREAK_PULL) 
				{
					End(LOSS_BREAK);
				}
			}
		}
	}


	void End( int result )
	{
		m_Cycler.Stop();
		m_CyclerCatching.Stop();
		m_Fishing = false;
		
		switch (result)
		{
			case LOSS_BREAK:
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("The fish had broken the fishing line and swam away.", "colorImportant");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
			break;
			
			case LOSS_LOOSE:
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("The fish escaped.", "colorImportant");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
			break;
			
			case WIN:
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("I caught the fish!", "colorFriendly");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
			break;
			
			case INTERUPTED:
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("Fishing time is over.", "colorFriendly");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
				m_Player.Message("", "colorStatusChannel");
			break;
			
			default:
				Print("Wrong number");
			break;
		}
	}

	void AddPull(float delta)
	{
		m_DeltaT = delta;
		m_WasPulled = true;
	}

	void DisplayState()
	{
		string fline = "";
		for (int i = 0; i < m_Pull; i++)
		{
			fline += "-";
		}
		m_Player.Message("", "colorStatusChannel");
		m_Player.Message("HOLD right mouse button to pull the fishing line", "colorAction");
		m_Player.Message("RELEASE right mouse button to loosen the fishing line", "colorAction");
		m_Player.Message("", "colorStatusChannel");
		m_Player.Message("", "colorStatusChannel");
		m_Player.Message("Fishing line stretch :", "colorStatusChannel");
		m_Player.Message("", "colorStatusChannel");
		if (m_Pull >= TOO_MUCH_PULL)
		{
			m_Player.Message("<"+fline+">", "colorImportant");
		}
		if ( m_Pull >= HIGH_PULL && m_Pull < TOO_MUCH_PULL)
		{
			m_Player.Message("<"+fline+">", "colorAction");
		}
		if ( m_Pull < HIGH_PULL && m_Pull > LOW_PULL)
		{
			m_Player.Message("<"+fline+">", "colorFriendly");
		}
		if ( m_Pull <= LOW_PULL && m_Pull > TOO_FEW_PULL)
		{
			m_Player.Message("<"+fline+">", "colorStatusChannel");
		}
		if (m_Pull <= TOO_FEW_PULL)
		{
			m_Player.Message("<"+fline+">", "colorImportant");
		}
	}
	
	// Conditions
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		if ( GetAnimationPhase(ANIM_PHASE_CLOSED) > 0.5 )
		{
			return true;
		}
		return false;
	}
}
