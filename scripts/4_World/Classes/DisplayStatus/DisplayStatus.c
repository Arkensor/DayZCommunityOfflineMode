enum DSLevels
{
	NORMAL = 0,//no bit, default
	WARNING = 8,//fourth bit
	CRITICAL = 16//fifth bit
	BLINKING = 24//fourth + fifth bit
}

class DisplayStatus
{
	const int FLOAT_USABLE_SIZE = 31;
	const int NUMBER_OF_MASKS = 2;//how many INT numbers we need to accommodate all elements
	ref array<int> m_LastSentArray;
	//ref map<int, ref DisplayElement> m_Elements;
	const int NUMBER_OF_ELEMENTS = eDisplayElements.COUNT;
	ref DisplayElement m_Elements[NUMBER_OF_ELEMENTS];
	ref Timer myTimer1;
	Mission mission;
	Hud m_Hud;
	int m_LastTick;
	PlayerBase m_Player;
	string m_System 		= "DisplayStatus";
	
	ref array<ref Param> rpcParams;
	
	void DisplayStatus(PlayerBase player)
	{

		m_Player 		= player;
		//m_Elements 		= new map<int,ref DisplayElement>;
		m_LastTick		= 0;
		//rpcParams 		= new array<ref Param>;
		/*
		for(int i = 0; i < NUMBER_OF_MASKS; i++)
		{
			rpcParams.Insert( new Param1<int>(0) );
		}
		*/

		//myTimer2.Run(3, this, "Debug", NULL, true);
		//myTimer2.Run(10, this, "PrintElements", NULL, true);
		
		// !!!!!! don't add new stuff unless you really really REALLY know what you're doing !!!!!!
		AddElement(new BadgeStuffed);// size 2
		AddElement(new BadgeWet);// size 2
		AddElement(new BadgeSick);// size 2
		AddElement(new BadgePoisoned);// size 2
		AddElement(new BadgeFracture);// size 2
		AddElement(new TendencyHealth);// size 5
		AddElement(new TendencyBlood);// size 5
		AddElement(new TendencyTemperature);// size 5
		AddElement(new TendencyHunger);// size 5
		// sum 30/31
		AddElement(new TendencyThirst);// size 5
		AddElement(new TendencyBacteria);// size 5
		// sum 10/31

		AddElement(new ElementStance);// size 0(client only)
		//AddElement(new ElementStamina);// size 0 (client only)


		mission = GetGame().GetMission();
		if ( mission )
		{
			m_Hud = mission.GetHud();
		}
	}
	
	void OnScheduledTick()
	{
		if( GetGame().IsServer() )
		{
			if(GetGame().GetTime() > (m_LastTick + DISPLAY_STATUS_UPDATE_INTERVAL))
			{
				SendRPC();
				m_LastTick = GetGame().GetTime();
			}
		}
		else
		{
			ImmediateUpdate();
			DisplayPresence();
		}
		
		
	}

	DisplayElement GetElement(int id)
	{
		return m_Elements[id];
	}
	
	void AddElement(DisplayElement element)
	{
		int id = element.GetType();
		m_Elements[id] = element;
		Log("adding element:"+id.ToString());
	}

	void SetBlinking(int id, bool blink)
	{
		// for badges
		// this will set the second bit of the element value to either 1 or 0 (the first bit sets the badge on/off)
		int blinking_mask = (int)blink << 1;
		int off_mask = ~blinking_mask;//set all bits except the blinking bit to 1, blinking bit is set to 0
		int blinking_bit_zero_mask = GetElement(id).m_Value & off_mask;//using the off_mask, switch off the blinking bit
		GetElement(id).m_Value = blinking_bit_zero_mask | blinking_mask;//now simply apply the blinking mask
	}
	
	void SetLevel(int id, DSLevels level)
	{
		if( GetElement(id) ) GetElement(id).SetLevel(level);
	}

	void SetStatus(int id, int level)
	{
		if(level<0) level = Math.AbsInt(level)+3;//REWORK.V: hacking, remove in the final version !!!!

		if( GetElement(id) ) GetElement(id).SetValue(level);
	}

	void SetValue(int id, int value)
	{
		if( GetElement(id) ) GetElement(id).SetValue(value);
	}

	void SetBar(int id, int value, int range)
	{
		if( GetElement(id) ) GetElement(id).SetValue(value,range);
	}
	
	void SetLabel(int id, string name)
	{
			// GetElement(id).m_Labels[0] = name; // commented out for now because it does not work with server-client; it will be implemented diffrently
	}
/*
	void DebugRandValues()
	{
		for(int i = 0; i < m_Elements.Count();i++)
		{
			if(GetElement(i).GetType() == TYPE_BADGE)
			{
				GetElement(i).m_Value = Math.RandomInt(0,2);
			}
			else if(GetElement(i).GetType() == TYPE_BAR)
			{
				GetElement(i).m_Value = Math.RandomInt(0,65);
			}
			else
			{
				GetElement(i).m_Value = Math.RandomInt(0,7);
			}
		}
	}*/

	void SerializeElements(ref array<int> mask_array)
	//this will serialize all elements and 'compresses' them into integer(s) through bit shifting, these integers are placed into an array
	{
		int offset = 0;
		int mask = 0;
		
		for(int i = 0; i < NUMBER_OF_ELEMENTS; i++)
		{
			if(  GetElement(i) && !GetElement(i).IsClientOnly() )
			{
				if( (GetElement(i).GetNumberOfBits() + offset) > FLOAT_USABLE_SIZE )
				{
					mask_array.Insert(mask);
					offset = 0;
					mask = 0;
				}
				mask = mask | (GetElement(i).GetValue() << offset);
				offset = offset + GetElement(i).GetNumberOfBits();
			}
		}
		mask_array.Insert(mask);
	}



	void DeserializeElements(ref array<int> mask_array)//extracts elements from mask
	{
		int maskArrayIndex = 0;
		int offset = 0;
		int mask = 0;
		
		for(int i = 0; i < NUMBER_OF_ELEMENTS;i++)
		{
			if( GetElement(i) && !GetElement(i).IsClientOnly() )
			{
				//Log("entity> " + ToString(GetElement(i)) );
				if(offset + GetElement(i).GetNumberOfBits() > FLOAT_USABLE_SIZE)
				{
					maskArrayIndex++;
					offset = 0;
				}
				mask = mask_array.Get(maskArrayIndex);
				int value = BitToDec( mask, offset, GetElement(i).GetCompareMask() );
				offset = offset + GetElement(i).GetNumberOfBits();
				GetElement(i).SetValue( value );
			}
		}
	}


	int BitToDec(int mask, int index, int compareMask)
	{
		int value = mask & (compareMask << index);
		value = value >> index;
		return value;
	}


	void PrintMasks()
	{/*
		for(int i = 0; i < m_MaskArray.Count();i++)
		{
			Log("mask"+i.ToString()+">"+m_MaskArray.Get(i).ToString() );
		}
	*/
	}

	void PrintElements()
	{
		for(int i = 0; i < NUMBER_OF_ELEMENTS; i++)
		{
			PrintString(i.ToString() +": "+ GetElement(i).m_Value.ToString() );
		}
	}
	/*
	void SendRPC()//on Server
	{
		array<int> mask_array = new array<int>;
		DeserializeElements(mask_array);
		
		Param1<int> paramTmp;
		for(int i = 0; i < mask_array.Count();i++)
		{
			paramTmp = Param1<int>.Cast( rpcParams.Get(i) );
			paramTmp.param1 = mask_array.Get(i);
		}
		GetGame().RPC(m_Player, ERPCs.RPC_SYNC_DISPLAY_STATUS, rpcParams, false, m_Player.GetIdentity());
	}

*/
	void SendRPC()
	{
		array<int> mask_array = new array<int>;
		SerializeElements(mask_array);
		if( !m_LastSentArray || !AreArraysSame(m_LastSentArray, mask_array)  )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write(mask_array);
			rpc.Send(m_Player, ERPCs.RPC_SYNC_DISPLAY_STATUS, false, m_Player.GetIdentity());
			m_LastSentArray = mask_array;
		}
	}

	bool AreArraysSame( notnull array<int> array_a, notnull array<int> array_b )
	{
		if( array_a.Count() != array_b.Count() ) return false;
		for(int i = 0; i  <array_a.Count(); i++)
		{
			if(	array_a.Get(i) != array_b.Get(i) )
			{
				return false;
			}
		}
		return true;
	}

	void ImmediateUpdate()
	{
		for(int i = 0; i < NUMBER_OF_ELEMENTS;i++)
		{
			if( GetElement(i) && GetElement(i).IsClientOnly() ) GetElement(i).UpdateHUD();
		}
	}
	
	void DisplayPresence()
	{
		if ( m_Hud )
		{
			m_Hud.DisplayPresence();
		}
	}

	void UpdateStatus()
	{
		//Log("UpdateStatus called for entity: "+ToString(m_Player));
		for(int i = 0; i < NUMBER_OF_ELEMENTS;i++)
		{
			if(  GetElement(i) && !GetElement(i).IsClientOnly() ) GetElement(i).UpdateHUD();
		}
	}

	void OnRPC(ParamsReadContext ctx)//on Client 			REWORK.V: NUMBER_OF_MASKS should ideally be transmitted with the masks, ie. saved when sending, and then read on receive as first item
	{
		//Log("OnRPC called");
		array<int> mask_array = new array<int>;
		ctx.Read(mask_array); 
		DeserializeElements(mask_array);
		UpdateStatus();
	}

	void Debug()
	{
		Log("debug");
		PluginPlayerStatus 	m_ModulePlayerStatus = PluginPlayerStatus.Cast(GetPlugin(PluginPlayerStatus));
		m_ModulePlayerStatus.DisplayTendency(NTFKEY_HUNGRY, 2);
	}
}