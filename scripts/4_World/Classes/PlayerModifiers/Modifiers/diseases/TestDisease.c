class TestDisease: ModifierBase
{
	float m_Interval;
	ref Param1<float> m_SomeValue;

	override void Init()
	{
		m_TrackActivatedTime			= false;
		m_ID 							= 9999;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
		m_SomeValue		= new Param1<float>(0);
		MakeParamObjectPersistent(m_SomeValue);
	}
	
	override bool ActivateCondition(PlayerBase player)
	{
		return false;
	}

	override void OnActivate(PlayerBase player)
	{
		//player.GetStateManager().QueueUpSecondaryState( StateIDs.STATE_BLINDNESS );
		//player.GetStateManager().QueueUpSecondaryState( StateIDs.STATE_BULLET_HIT );
		/*
		array<int> arr0 = BitArray.CreateMaskArray(128);
		array<int> arr = BitArray.CreateArrayWithID(36);
		array<int> arr3 = BitArray.CreateArrayWithID(64);
		Debug.LogArrayInt(arr);
		Debug.LogArrayInt(arr3);
		bool rslt1 = BitArray.IsMaskContainID(arr3,1);
		bool rslt2 = BitArray.IsMaskContainID(arr3,31);
		bool rslt3 = BitArray.IsMaskContainID(arr3,64);
		Print(rslt1.ToString());
		Print(rslt2.ToString());
		Print(rslt3.ToString());
		BitArray.AddIDToMask(arr0,1);
		BitArray.AddIDToMask(arr0,65);
		BitArray.AddIDToMask(arr0,128);
		rslt1 = BitArray.IsMaskContainID(arr0,1);
		rslt2 = BitArray.IsMaskContainID(arr0,31);
		rslt3 = BitArray.IsMaskContainID(arr0,65);
		bool rslt4 = BitArray.IsMaskContainID(arr0,128);
		Print(rslt1.ToString());
		Print(rslt2.ToString());
		Print(rslt3.ToString());
		Debug.Log("+----------+");
		int i = 4 + 3;
		*/

		//player.GetStateManager().QueueUpSecondaryState( StateIDs.STATE_BLINDNESS );
		//player.GetStateManager().QueueUpSecondaryState( StateIDs.STATE_BLEEDING_SOURCE, new Param1<vector>("1 1 1") );

		/*
		vector pos123 = "1 1 1";
		Particle p = Particle.Create( ParticleList.BLEEDING_SOURCE, GetPlayer(), pos123 );
		p.Play();
		*/
		

		player.GetStateManager().QueueUpSecondaryState( StateIDs.STATE_BULLET_HIT );
		//player.GetStateManager().QueueUpSecondaryState( StateIDs.STATE_BLINDNESS );
		
		player.GetBleedingManager().AddBleedingSource("RightArm", "1 1 1");
		player.GetBleedingManager().AddBleedingSource("LeftArm", "1.2 1.2 1.2");
		player.GetBleedingManager().AddBleedingSource("RightLeg", "1.3 1.3 1.3");
		
		int i = 1 + 1;
	}
	
	override void OnReconnect(PlayerBase player)
	{
		//OnActivate(player);
		//player.GetStateManager().QueueUpSecondaryState( StateIDs.STATE_BLINDNESS );
	}

	override void OnDeactivate(PlayerBase player)
	{
	}

	override bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	override void OnTick(PlayerBase player, float deltaT)
	{
	
	}
};