/*!
	Base native class of all vehicles in game.
*/
class Transport extends EntityAI
{
	ref TIntArray m_SingleUseActions;
	ref TIntArray m_ContinuousActions;
	ref TIntArray m_InteractActions;

	void Transport()
	{
		Init();
	}
	
	void Init()
	{
		m_SingleUseActions  = new TIntArray;
		m_ContinuousActions = new TIntArray;
		m_InteractActions   = new TIntArray;
		SetUserActions();
	}
	
	void SetUserActions()
	{
		g_Game.ConfigGetIntArray("cfgVehicles " +GetType() + " ContinuousActions", m_ContinuousActions);
		g_Game.ConfigGetIntArray("cfgVehicles " +GetType() + " SingleUseActions", m_SingleUseActions);
		g_Game.ConfigGetIntArray("cfgVehicles " +GetType() + " InteractActions", m_InteractActions);
	}

	override void GetSingleUseActions(out TIntArray actions)
	{	
		for ( int i = 0; i < m_SingleUseActions.Count(); i++ )
		{
			actions.Insert(m_SingleUseActions.Get(i));
		}
	}

	override void GetContinuousActions(out TIntArray actions)
	{
		for ( int i = 0; i < m_ContinuousActions.Count(); i++ )
		{
			actions.Insert(m_ContinuousActions.Get(i));
		}
	}

	override void GetInteractActions(out TIntArray actions)
	{
		if (m_InteractActions)
		{
			for ( int i = 0; i < m_InteractActions.Count(); i++ )
			{
				Print(m_InteractActions.Get(i));
				actions.Insert(m_InteractActions.Get(i));
			}
		}
	}
	
	//! Returns crew capacity of this vehicle.
	proto native int CrewSize();

	//! Returns crew member index based on action component index.
	//! -1 is returned when no crew position corresponds to given component index.
	proto native int CrewPositionIndex( int componentIdx );

	//! Returns crew member based on position index.
	//! Null can be returned if no Human is present on the given position.
	proto native Human CrewMember( int posIdx );

	//! Reads entry point and direction into vehicle on given position in model space.
	proto native void CrewEntry( int posIdx, out vector pos, out vector dir );

	//! Reads entry point and direction into vehicle on given position in world space.
	proto native void CrewEntryWS( int posIdx, out vector pos, out vector dir );

	//! Returns crew transformation indside vehicle in model space
	proto native void CrewTransform( int posIdx, out vector mat[4] );

	//! Returns crew transformation indside vehicle in world space
	proto native void CrewTransformWS( int posIdx, out vector mat[4] );

	//! Performs transfer of player from world into vehicle on given position.
	proto native void CrewGetIn( Human player, int posIdx );

	//! Performs transfer of player from vehicle into world from given position.
	proto native Human CrewGetOut( int posIdx );


	override bool IsTransport()
	{
		return true;
	}
	
	override bool IsHealthVisible()
	{
		return false;
	}
};
