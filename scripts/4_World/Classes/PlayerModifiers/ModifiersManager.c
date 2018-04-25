enum EActivationType {

	TRIGGER_EVENT_OFF,
	TRIGGER_EVENT_ON_ACTIVATION,
	TRIGGER_EVENT_ON_CONNECT
};

const int DEFAULT_TICK_TIME_ACTIVE = 3;
const int DEFAULT_TICK_TIME_INACTIVE = 3;

class ModifiersManager
{
	PlayerBase m_Player;
	ref map<int, ref ModifierBase> m_ModifierList;
	ref array<ref Param> m_ParamList;
	bool m_AllowModifierTick = false;
	
	void ModifiersManager(PlayerBase player)
	{
		m_ModifierList 	= new map<int, ref ModifierBase>;
		m_ParamList 	= new array<ref Param>;
		m_Player 		= player;
		
		Init();
	}

	void Init()
	{
		//AddModifier(new Testing);
		AddModifier(new BloodRegen);
		//AddModifier(new BoneRegen);
		//AddModifier(new Health);
		AddModifier(new Saline);
		AddModifier(new HealthRegen);
		AddModifier(new Hunger);
		AddModifier(new Shock);
		AddModifier(new ImmuneSystem);
		AddModifier(new Stomach);
		AddModifier(new Temperature);
		AddModifier(new Thirst);
		AddModifier(new BleedingCheck);
		//AddModifier(new Blinded);
		AddModifier(new BrokenArms);
		AddModifier(new BrokenLegs);
		AddModifier(new VomitModifier);
		AddModifier(new Burning);
		AddModifier(new Fever);
		AddModifier(new HeartAttack);
		AddModifier(new HemolyticReaction);
		AddModifier(new Hyperthermia);
		AddModifier(new Hypothermia);
		AddModifier(new Poisoning);
		AddModifier(new StuffedStomach);
		AddModifier(new Tremor);
		//AddModifier(new CommonCold);
		//AddModifier(new Cholera);
		//AddModifier(new Influenza);
		//AddModifier(new Salmonella);
		//AddModifier(new BrainDisease);
	}

	void SetModifiers(bool enable)
	{
		
		m_AllowModifierTick = enable;
		if( enable == false )
		{
			for(int i = 0; i < m_ModifierList.Count(); i++)
			{
				m_ModifierList.GetElement(i).ResetLastTickTime();
			}
		}
	}

	bool IsModifiersEnable()
	{
		return m_AllowModifierTick;
	}
	
	void AddModifier(ModifierBase modifier)
	{
		modifier.InitBase(m_Player,this);
		int id = modifier.GetModifierID();
		
		if(id < 1)
		{
			Error("modifiers ID must be 1 or higher(for debugging reasons)");			
		}
		
		//TODO: add a check for duplicity
		m_ModifierList.Insert(id, modifier);
	}
	
	bool IsModifierActive(eModifiers modifier_id)
	{
		return m_ModifierList.Get(modifier_id).IsActive();
	}

	void OnScheduledTick(float delta_time)
	{
		if(!m_AllowModifierTick) return;

		for(int i = 0; i < m_ModifierList.Count(); i++)
		{
			m_ModifierList.GetElement(i).Tick(delta_time);
		}

	}
	
	void ActivateModifier(int modifier_id, bool triggerEvent = EActivationType.TRIGGER_EVENT_ON_ACTIVATION)
	{
		m_ModifierList.Get(modifier_id).Activate(triggerEvent);
	}

	void DeactivateModifier(int modifier_id, bool triggerEvent = true)
	{
		m_ModifierList.Get(modifier_id).Deactivate(triggerEvent);
	}
	
	void OnStoreSave( ParamsWriteContext ctx )
	{
		CashedObjectsArrays.ARRAY_INT.Clear();
		
		int modifier_count;
		for(int x = 0; x < m_ModifierList.Count(); x++)
		{
			ModifierBase mdfr = m_ModifierList.GetElement(x);
			if( mdfr.IsActive() ) 
			{
				modifier_count++;
				//save the modifier id
				CashedObjectsArrays.ARRAY_INT.Insert( mdfr.GetModifierID() );
				if( mdfr.IsTrackAttachedTime() )
				{
					//save the overall attached time
					CashedObjectsArrays.ARRAY_INT.Insert( mdfr.GetAttachedTime() );
				}
			}
		}

		//write the count
		//CashedObjectsParams.PARAM1_INT.param1 = modifier_count;
		//PrintString("Saving modifiers count: "+ modifier_count);
		ctx.Write(modifier_count);

		//write the individual modifiers and respective attached times
		for(int i = 0; i < CashedObjectsArrays.ARRAY_INT.Count(); i++)
		{
			int item = CashedObjectsArrays.ARRAY_INT.Get(i);
			//PrintString( "saving item: "+item );
			ctx.Write(item);
		}

		for (int z = 0; z < m_ParamList.Count(); z++)
		{
			m_ParamList.Get(z).Serialize(ctx);
		}

	}
	
	void OnStoreLoad( ParamsReadContext ctx )
	{
		int modifier_count;
		ctx.Read(modifier_count);
		//PrintString("Loading modifiers count: "+ modifier_count);
		for(int i = 0; i < modifier_count; i++)
		{
			int modifier_id;
			ctx.Read(modifier_id);
			//PrintString( "loading item: "+modifier_id );
			//int modifier_id = CashedObjectsParams.PARAM1_INT.param1;
			ModifierBase modifier = GetModifier(modifier_id);
			
			if( modifier.IsTrackAttachedTime() )
			{
				int time;
				ctx.Read(time);//get the attached time
				modifier.SetAttachedTime( time );
			}
			
			ActivateModifier(modifier_id, EActivationType.TRIGGER_EVENT_ON_CONNECT);
		}
		
		for (int x = 0; x < m_ParamList.Count(); x++)
		{
			m_ParamList.Get(x).Deserializer(ctx);
		}
		
	}

	ModifierBase GetModifier(int modifier_id)
	{
		return m_ModifierList.Get(modifier_id);
	}
		
	PlayerBase GetPlayer()
	{
		return m_Player;
	}

	void SetModifierLock(int modifier_id, bool state)
	{
		m_ModifierList.Get(modifier_id).SetLock(state);
	}


	bool GetModifierLock(int modifier_id)
	{
		return m_ModifierList.Get(modifier_id).IsLocked();
	}

	void DbgGetModifiers(map<int, string> modifiers_map)
	{
		for(int i = 0;i < m_ModifierList.Count(); i++)
		{
			ModifierBase modifier = m_ModifierList.GetElement(i);
			int modifier_id = modifier.GetModifierID();
			string modifier_name = modifier.GetName();
			
			if( !modifier.IsActive() ) 
			{
				modifier_id = -modifier_id;
			}
			
			modifiers_map.Insert( modifier_id, modifier_name );
		}
	}
}
