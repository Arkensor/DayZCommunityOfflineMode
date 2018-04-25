enum StateIDs {

	STATE_COUGH = 1,
	STATE_VOMIT,
	STATE_BLINDNESS,
	STATE_BULLET_HIT,
	STATE_BLEEDING_SOURCE,
	STATE_BLOODLOSS,
	STATE_SNEEZE,
	STATE_FEVERBLUR,
	STATE_LAUGHTER,
};

enum StateTypes 
{
	PRIMARY,
	SECONDARY
};

const int DEBUG_PADDING_OFFSET = 2;
const int MAX_QUEUE_SIZE = 5; 

class StateManager
{
	PlayerBase m_Player;
	ref map<int, ref StateBase> m_AvailableStates;
	
	ref array<ref StateBase> m_StateQueuePrimary;
	ref array<ref StateBase> m_StateQueueSecondary;

	ref map<int , StateBase > m_StatesUIDs;

	
	ref array<ref Param> m_StateQueueServerDbg;
	ref array<ref Param> m_StateQueueServerDbgPrimary;
	ref array<ref Param> m_StateQueueServerDbgSecondary;
	//ref array<string> m_StateQueueSecondaryServerDbg;

	int m_AnimID;
	int m_StateUID;
	int m_StanceMask;
	float m_Duration;
	bool m_IsAnimSet;
	
	ref Timer m_Timer;
	
	bool m_ShowDebug = false;
	bool m_ShowDebug2 = false;
	
	void Init()
	{
		RegisterState(new CoughState);
		RegisterState(new VomitState);
		RegisterState(new BlindnessState);
		RegisterState(new BulletHitState);
		RegisterState(new BleedingSourceState);
		RegisterState(new SneezeState);
		RegisterState(new FeverBlurState);
		RegisterState(new BloodLoss);
		RegisterState(new LaughterState);
		
	}

	void AutoactivateStates()
	{
		if( GetGame().IsClient() ) 
		{
			return;
		}
		
		QueueUpSecondaryState(StateIDs.STATE_BLOODLOSS);
	}
	
	void StateManager(PlayerBase player)
	{
		m_Timer = new Timer;
		m_StatesUIDs = new map<int , StateBase >;
		m_StateQueuePrimary = new array<ref StateBase>;
		m_StateQueueSecondary = new array<ref StateBase>;
		m_StateQueueServerDbg = new array<ref Param>;
		m_StateQueueServerDbgPrimary = new array<ref Param>;
		m_StateQueueServerDbgSecondary = new array<ref Param>;
		//m_StateQueueSecondaryServerDbg = new array<ref Param>;
		m_AvailableStates = new map<int, ref StateBase>;
		m_Player = player;
		Init();
		AutoactivateStates();
	}
	
	void OnPlayerKilled()
	{
		for(int i = 0; i < m_StateQueuePrimary.Count(); i++)
		{
			m_StateQueuePrimary.Get(i).OnOwnerKilled();
		}
		
		for(i = 0; i < m_StateQueueSecondary.Count(); i++)
		{
			if(m_StateQueueSecondary.Get(i)) m_StateQueueSecondary.Get(i).OnOwnerKilled();
		}
	}

	StateBase GetStateByUID(int state_uid)
	{
		return m_StatesUIDs.Get(state_uid);
	}
	
	
	void ~StateManager()
	{
	}

	PlayerBase GetPlayer()
	{
		return m_Player;
	}
	
	//! Removes all  states of a given type
	/*
	void RemoveSecondaryStatesByType( int type )
	{
		for(int i = 0; i < m_StateQueueSecondary.Count(); i++)
		{
			int current_type = m_StateQueueSecondary.Get(i).GetType();
			
			if( current_type == type )
			{
				m_StateQueueSecondary.Get(i).Destroy();
				i--;
			}
		}
	}
	*/
	
	void RegisterState(StateBase state)
	{
		state.Init(this, m_Player,0);
		int id = state.GetType();
		
		if( m_AvailableStates.Contains(id) )
		{
			Error("StateBase state already registered !");
			return;
		}
		
		m_AvailableStates.Insert(id, state);
		//PrintString("inserting id: "+ToString(id));
		
	
	}

	void OnAnimationFinished(int state_uid)
	{
		StateBase state = GetStateByUID(state_uid);
		if( state ) state.OnAnimationFinish();
	}
	
	int CreateUniqueID()
	{
		int uid = Math.RandomInt( 1, 2147483647);
		if( !IsUIDUsed(uid) ) return uid;
		else return CreateUniqueID();
	}

	bool IsUIDUsed(int uid)
	{
		if( m_StatesUIDs.Contains(uid) ) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	string GetStateName(int state_id)
	{
		return m_AvailableStates.Get(state_id).GetName();
	}
		
	//! Exits a specific state with a given UID
	void RequestStateExit(int state_uid)
	{
		if( m_StatesUIDs.Get(state_uid) ) m_StatesUIDs.Get(state_uid).RequestDestroy();
	}

	bool IsStatePrimary(int state_id)
	{
		return m_AvailableStates.Get(state_id).IsPrimary();
	}

	void OnInputUserDataReceived(ParamsReadContext ctx)
	{	
	}

	void OnScheduledTick(float deltatime)
	{
		UpdateActiveStates(deltatime);
		if(m_IsAnimSet) 
		{
			PlayAnimation();
		}
		#ifdef DEVELOPER
		if( GetGame().IsMultiplayer() && GetGame().IsServer() ) return;//must be here !!!
		if ( DiagMenu.GetBool(DiagMenuIDs.DM_PLAYER_STATES_SHOW) )
		{
			//DisplayDebug(true);
			array<ref Param>  primary_debug = PrepareClientDebug(m_StateQueuePrimary);
			array<ref Param>  secondary_debug = PrepareClientDebug(m_StateQueueSecondary);

			DisplayDebug1("States Client", 50,primary_debug, secondary_debug);
			DisplayDebug2("States Server", 300,m_StateQueueServerDbgPrimary, m_StateQueueServerDbgSecondary);
			ShowAvailableStates();
		}
		else
		{
			CleanDebug1("States Client",50);
			CleanDebug2("States Server",300);
			CleanAvailableStates();
			
		}
		#endif
	}

	void SetAnimation(ParamsReadContext ctx)
	{
		m_IsAnimSet = true;
		DayZPlayerSyncJunctures.ReadPlayerStateAnimParams(ctx, m_AnimID, m_StateUID, m_StanceMask, m_Duration);
	}
	
	void PlayAnimation()
	{
		StateCB	anim_callback = StateCB.Cast(GetPlayer().StartCommand_Action(m_AnimID, StateCB, m_StanceMask));
		anim_callback.Init(m_StateUID, m_Duration, GetPlayer());
		m_IsAnimSet = false;

	}
	
	void UpdateActiveStates(float deltatime)
	{	
		//if( GetGame().IsClient() && !m_Player.IsPlayer() ) return;
		//primary
		if( GetCurrentPrimaryActiveState() ) GetCurrentPrimaryActiveState().Update(deltatime);
		
		//secondary
		for(int i = 0; i < m_StateQueueSecondary.Count(); i++)
		{
			if(m_StateQueueSecondary.Get(i)) m_StateQueueSecondary.Get(i).Update(deltatime);
		}
	}
	
	
	void OnStateExit(StateBase state, int uid)
	{
		bool is_primary;
		if( state ) is_primary = state.IsPrimary();
		
		if( m_StatesUIDs.Contains(uid) )
		{
			m_StatesUIDs.Remove(uid);		
		}
		else
		{
			Debug.LogError("state with this UID does not exist", "PlayerStates");
		}
		if(is_primary)
		{
			for(int i = 0; i < m_StateQueuePrimary.Count(); i++)
			{
				if( m_StateQueuePrimary.Get(i) == state )
				{
					m_StateQueuePrimary.RemoveOrdered(i);
					break;
				}
			}
		}
		else
		{
			for(i = 0; i < m_StateQueueSecondary.Count(); i++)
			{
				if( m_StateQueueSecondary.Get(i) == state )
				{
					m_StateQueueSecondary.RemoveOrdered(i);
					break;
				}
			}
		}
		if( GetGame() ) SendServerDebugToClient();
	}

	StateBase SpawnState( int state_id, int uid = -1 )
	{
		if( m_AvailableStates.Get(state_id) )
		{
			StateBase state = m_AvailableStates.Get(state_id).ClassName().ToType().Spawn();
			if(uid == -1)
			{
				uid = CreateUniqueID();
			}
			state.Init(this, m_Player,uid);
			if( m_StatesUIDs.Contains(uid) ) Error("States: Unique ID already exists !");
			m_StatesUIDs.Insert(uid, state);
			return state;
		}
		
		if( !state ) 
		{
			Error("state not registered");
		}
		
		return NULL;
	}
	
	void CleanUpPrimaryQueue()
	{
		m_StateQueuePrimary.Clear();
	}
	/*
	StateBase QueueUpPrimaryState(int state_id, int uid = -1)
	{
		StateBase state;
		for(int i = 0; i < m_StateQueuePrimary.Count(); i++)
		{
			if( ComparePriority( GetStatePriority(state_id), m_StateQueuePrimary.Get(i).GetPriority() ) == 1 )
			{
				state = SpawnState( state_id, uid );
			
				m_StateQueuePrimary.InsertAt(state,i);

				if( m_StateQueuePrimary.Count() > MAX_QUEUE_SIZE )
				{
					delete m_StateQueuePrimary.Get(MAX_QUEUE_SIZE);//delete the last element in the queue, no need to remove from the array, that's done via state callback on destruct
				}
				SendServerDebugToClient();
				Debug.Log("queing up a new primary state with state_id: "+state_id.ToString(), "PlayerStates");
				return state;
			}
		}
		if( m_StateQueuePrimary.Count() < MAX_QUEUE_SIZE) 
		{
			state = SpawnState( state_id, uid );
			m_StateQueuePrimary.Insert(state);
			SendServerDebugToClient();
			Debug.Log("queing up a new primary state with state_id: "+state_id.ToString(), "PlayerStates");
			return state;
		}
		
		return NULL;
	}
*/
	StateBase QueueUpPrimaryState(int state_id, int uid = -1)
	{
		StateBase state;
		for(int i = 0; i < m_StateQueuePrimary.Count(); i++)
		{
			if( ComparePriority( GetStatePriority(state_id), m_StateQueuePrimary.Get(i).GetPriority() ) == 1 )
			{
				state = SpawnState( state_id, uid );
				m_StateQueuePrimary.InsertAt(state,i);

				if( m_StateQueuePrimary.Count() > MAX_QUEUE_SIZE )
				{
					delete m_StateQueuePrimary.Get(MAX_QUEUE_SIZE);//delete the last element in the queue, no need to remove from the array, that's done via state callback on destruct
				}
				break;
			}
		}
		if( !state && m_StateQueuePrimary.Count() < MAX_QUEUE_SIZE) 
		{
			state = SpawnState( state_id, uid );
			m_StateQueuePrimary.Insert(state);
		}
		SendServerDebugToClient();
		return state;
	}
	
	void QueueUpSecondaryState(int state_id, int uid = -1)
	{
		StateBase state = SpawnState( state_id, uid);
		
		if( state.IsPrimary() ) 
		{
			Error("trying to queue up a primary state into a secondary state queue");
			delete state;
			return;
		}
		m_StateQueueSecondary.Insert(state);
	}
	
	//! Removes a single state of a given type
	void RemoveSecondaryState(int state_id)
	{
		for(int i = 0; i < m_StateQueueSecondary.Count();i++)
		{
			if( m_StateQueueSecondary.Get(i) && m_StateQueueSecondary.Get(i).GetType() == state_id )
			{
				m_StateQueueSecondary.Get(i).RequestDestroy();
				return;
			}
		}
	}
	

	StateBase GetCurrentPrimaryActiveState()
	{
		if(m_StateQueuePrimary.Count() > 0 ) return m_StateQueuePrimary.Get(0);
		else return NULL;
	}
	
	private int ComparePriority( int prio1, int prio2 )
	{
		if( prio1 > prio2 )
		{
			return 1;
		}
		else if ( prio2 > prio1 )
		{
			return 2;
		}
		return 0;
	}
	
	int GetStatePriority(int state_id)
	{
		return m_AvailableStates.Get(state_id).GetPriority();
	}
	
	void OnRPC(int rpc_type, ParamsReadContext ctx)
	{
		ctx.Read(CashedObjectsParams.PARAM2_INT_INT);
		int state_id = CashedObjectsParams.PARAM2_INT_INT.param1;
		int uid = CashedObjectsParams.PARAM2_INT_INT.param2;
		
		bool is_primary = m_AvailableStates.Get(state_id).IsPrimary();
		
		if( rpc_type == ERPCs.RPC_PLAYER_STATES_ON )
		{
			if( is_primary )
			{
				CleanUpPrimaryQueue();
				QueueUpPrimaryState(state_id,uid);
			}
			else
			{
				QueueUpSecondaryState(state_id,uid);
			}
		}
		else if( rpc_type == ERPCs.RPC_PLAYER_STATES_OFF )
		{
			if( is_primary )
			{
				CleanUpPrimaryQueue();
			}
			else
			{
				RemoveSecondaryState( state_id );
			}
		}
	}

	void OnRPCDebug(int rpc_type, ParamsReadContext ctx)
	{
		if( !GetGame().IsDebug() ) return;
		if( rpc_type == ERPCs.RPC_PLAYER_STATES_DEBUG )
		{
			int primary_states_count;
			int secondary_states_count;
			
			int state_id;
			

			if ( ctx.Read(CashedObjectsParams.PARAM1_INT) )
			{
				primary_states_count = CashedObjectsParams.PARAM1_INT.param1;
			}
			
			if ( ctx.Read(CashedObjectsParams.PARAM1_INT) )
			{
				secondary_states_count = CashedObjectsParams.PARAM1_INT.param1;
			}
			
			m_StateQueueServerDbg.Clear();
			m_StateQueueServerDbgPrimary.Clear();
			m_StateQueueServerDbgSecondary.Clear();

			int overall_count = primary_states_count + secondary_states_count;
			for(int i = 0; i < overall_count ; i++)
			{
				Param3<bool, int, int> p3 = new Param3<bool, int, int>(false,0,0);
				ctx.Read(p3);
				bool is_primary = p3.param1;
				
				if(is_primary)  m_StateQueueServerDbgPrimary.Insert(p3);//   m_StateQueueServerDbg.Insert(p3);				
				else 			m_StateQueueServerDbgSecondary.Insert(p3);
			}
			/*
			for(i = 0; i < secondary_states_count; i++)
			{
				ctx.Read(CashedObjectsParams.PARAM1_STRING);
				m_StateQueueSecondaryServerDbg.Insert(CashedObjectsParams.PARAM1_STRING.param1);				
			}*/
			
		}
		else if( rpc_type == ERPCs.RPC_PLAYER_STATES_EXIT )
		{
			ctx.Read(CashedObjectsParams.PARAM1_INT);
			int state_uid = CashedObjectsParams.PARAM1_INT.param1;
			RequestStateExit(state_uid);
		}

		else if( rpc_type == ERPCs.RPC_PLAYER_STATES_DEBUG_ON)
		{
			ctx.Read(CashedObjectsParams.PARAM1_INT);
			int id = CashedObjectsParams.PARAM1_INT.param1;
			if( IsStatePrimary(id) )
			{
				QueueUpPrimaryState(id);
			}
			else
			{
				QueueUpSecondaryState(id);
			}
		}
	
	}
	
	void SendServerDebugToClient()
	{
		array<ref Param> debug_list = new array<ref Param>;
		
		Param1<int> p1 = new Param1<int>(0);
		Param1<int> p2 = new Param1<int>(0);
		
		p1.param1 = m_StateQueuePrimary.Count();
		p2.param1 = m_StateQueueSecondary.Count();
		
		debug_list.Insert(p1);
		debug_list.Insert(p2);
		
		Param3<bool,int,int> p;
		

		bool is_primary;
		int state_id;
		int state_uid;

		for( int i = 0; i < m_StateQueuePrimary.Count(); i++ )
		{
			is_primary = m_StateQueuePrimary.Get(i).IsPrimary();
			state_id = m_StateQueuePrimary.Get(i).GetType();
			state_uid = m_StateQueuePrimary.Get(i).GetUID();
			
			p = new Param3<bool,int,int>(is_primary, state_id, state_uid );
			debug_list.Insert(p);
		}

		for( i = 0; i < m_StateQueueSecondary.Count(); i++ )
		{

			is_primary = m_StateQueueSecondary.Get(i).IsPrimary();
			state_id = m_StateQueueSecondary.Get(i).GetType();
			state_uid = m_StateQueueSecondary.Get(i).GetUID();
			
			p = new Param3<bool,int,int>(is_primary, state_id, state_uid );
			debug_list.Insert(p);
		}
		GetGame().RPC(GetPlayer(), ERPCs.RPC_PLAYER_STATES_DEBUG, debug_list, true);
	}

	void OnStoreSave( ParamsWriteContext ctx )
	{
		ref array<int> 		m_SaveQueue = new array<int>;

		for( int i = 0; i < m_StateQueuePrimary.Count(); i++ )
		{
			if( m_StateQueuePrimary.Get(i).IsPersistent() )
			{
				m_SaveQueue.Insert( m_StateQueuePrimary.Get(i).GetType() );
			}
		}

		for( i = 0; i < m_StateQueueSecondary.Count(); i++ )
		{
			if( m_StateQueueSecondary.Get(i).IsPersistent() )
			{
				m_SaveQueue.Insert( m_StateQueueSecondary.Get(i).GetType() );
			}
		}

		ctx.Write( m_SaveQueue );

	}

	void OnStoreLoad( ParamsReadContext ctx )
	{
		ref array<int> m_SaveQueue;
		
		if(ctx.Read(m_SaveQueue))
		{
		
			for( int i = 0; i < m_SaveQueue.Count(); i++ )
			{
				int id = m_SaveQueue.Get(i);
	
				if( IsStatePrimary(id) )
				{
					QueueUpPrimaryState( id );
				}
				else
				{
					QueueUpSecondaryState( id );
				}
			}
		}
	}
	
	void DebugRequestExitState(int state_uid)
	{
		Param1<int> p1 = new Param1<int>(state_uid);
		GetGame().RPCSingleParam(GetPlayer(), ERPCs.RPC_PLAYER_STATES_EXIT, p1, true, GetPlayer().GetIdentity());
		//return;
		/*
		if( GetGame().IsClient() || ( GetGame().IsClient() && !GetGame().IsMultiplayer() )  )
		{
			Param1<int> p1 = new Param1<int>(state_uid);
			GetGame().RPCSingleParam(GetPlayer(), ERPCs.RPC_PLAYER_STATES_EXIT, p1, true, GetPlayer().GetIdentity());		
		}
		*/
	}

	
	array<ref Param> PrepareClientDebug(array<ref StateBase> states)
	{
		array<ref Param> debug_array = new array<ref Param>;
		
		Param3<bool, int, int> p3;
		
		for(int i = 0; i < states.Count(); i++)
		{
			bool is_primary		= states.Get(i).IsPrimary();
			int state_id	 	= states.Get(i).GetType();
			int state_uid 		= states.Get(i).GetUID();
			
			p3 = new Param3<bool,int,int>(is_primary, state_id, state_uid );
			debug_array.Insert(p3);
		}
		return debug_array;
	}



	void DisplayDebug1(string name, int y_offset, array<ref Param> states_primary, array<ref Param> states_secondary)
	{
		string primary;
		Param3<bool, int, int> p3 = new Param3<bool, int, int>(false,0,0);
		
		DbgUI.BeginCleanupScope();     
        DbgUI.Begin(name, 50, y_offset);
		DbgUI.Text("Primary: ");
		
		bool is_primary;
		int state_id;
		int state_uid;
		string state_name;
		
		for(int i = 0; i < states_primary.Count(); i++)
		{
			p3 = Param3<bool, int, int>.Cast(states_primary.Get(i));
			
			is_primary = p3.param1;
			state_id = p3.param2;
			state_uid = p3.param3;
			state_name = GetStateName(state_id);
			
			primary += state_name + " | ";
		}
		DbgUI.Text(primary);
		DbgUI.Text("Secondary: ");
		for(i = 0; i < states_secondary.Count(); i++)
		{
			p3 = Param3<bool, int, int>.Cast(states_secondary.Get(i));
			
			is_primary = p3.param1;
			state_id = p3.param2;
			state_uid = p3.param3;
			state_name = GetStateName(state_id);
			DbgUI.Text(state_name);
		}
		


        DbgUI.End();
        DbgUI.EndCleanupScope();		
	}
		

	void DisplayDebug2(string name, int y_offset, array<ref Param> states_primary, array<ref Param> states_secondary)
	{
		string primary;
		Param3<bool, int, int> p3 = new Param3<bool, int, int>(false,0,0);
		
		DbgUI.BeginCleanupScope();     
        DbgUI.Begin(name, 50, y_offset);
		DbgUI.Text("Primary: ");
		
		bool is_primary;
		int state_id;
		int state_uid;
		string state_name;
		
		for(int i = 0; i < states_primary.Count(); i++)
		{
			p3 = Param3<bool, int, int>.Cast(states_primary.Get(i));
			
			is_primary = p3.param1;
			state_id = p3.param2;
			state_uid = p3.param3;
			state_name = GetStateName(state_id);
			
			if(DbgUI.Button( i.ToString() + " " +state_name, 80))
			{
				PrintString("state: "+ state_uid.ToString());
				DebugRequestExitState(state_uid);
			}
		}

		DbgUI.Text("Secondary: ");
		for(i = 0; i < states_secondary.Count(); i++)
		{
			p3 = Param3<bool, int, int>.Cast(states_secondary.Get(i));
			
			is_primary = p3.param1;
			state_id = p3.param2;
			state_uid = p3.param3;
			state_name = GetStateName(state_id);
			
			if(DbgUI.Button( i.ToString() + " " +state_name, 80))
			{
				PrintString("state: "+ state_uid.ToString());
				DebugRequestExitState(state_uid);
			}
		}



        DbgUI.End();
        DbgUI.EndCleanupScope();		
	}

	void ShowAvailableStates()
	{
		DbgUI.BeginCleanupScope();
		DbgUI.Begin("available states", 300, 50);
		
		for(int i = 0; i < m_AvailableStates.Count(); i++)
		{
			StateBase state = m_AvailableStates.GetElement(i);
			string state_name = state.GetName();
			int state_id = state.GetType();
			
			if(DbgUI.Button( i.ToString() + " " + state_name, 80))
			{
				DebugRequestActivateState(state_id);
			}			
		}
		
		DbgUI.End();
		DbgUI.EndCleanupScope();
	}
	

	void CleanAvailableStates()
	{
		DbgUI.BeginCleanupScope();
		DbgUI.Begin("available states", 300, 50);
		DbgUI.End();
		DbgUI.EndCleanupScope();
	}
	
	void CleanDebug1(string name, int y_offset)
	{
		DbgUI.BeginCleanupScope();
		DbgUI.Begin(name, 50, y_offset);
		DbgUI.End();
		DbgUI.EndCleanupScope();
	}
	
	void CleanDebug2(string name, int y_offset)
	{
		DbgUI.BeginCleanupScope();
		DbgUI.Begin(name, 50, y_offset);
		DbgUI.End();
		DbgUI.EndCleanupScope();
	}

	void DebugRequestActivateState(int state_id)
	{
		CashedObjectsParams.PARAM1_INT.param1 = state_id;
		GetGame().RPCSingleParam(GetPlayer(), ERPCs.RPC_PLAYER_STATES_DEBUG_ON , CashedObjectsParams.PARAM1_INT, true, GetPlayer().GetIdentity());
	}

}