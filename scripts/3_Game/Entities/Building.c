// in configs all the structures(Houses, Buildings) inherits from config class House
typedef Building House;

class Building extends EntityAI
{
	proto native int GetLaddersCount();
	proto native vector GetLadderPosTop(int ladderIndex);
	proto native vector GetLadderPosBottom(int ladderIndex);

	proto native int GetDoorIndex(int componentIndex);
	proto native bool IsDoorOpen(int index);
	proto native bool IsDoorLocked(int index);
	proto native void OpenDoor(int index);
	proto native void CloseDoor(int index);
	proto native void LockDoor(int index);
	proto native void UnlockDoor(int index);
	proto native vector GetDoorSoundPos(int index);
	
	override bool IsBuilding()
	{
		return true;
	}
	
	override bool IsHealthVisible()
	{
		return false;
	}

	ref TIntArray m_InteractActions;
	
	void Building()
	{
		m_InteractActions = new TIntArray;
		g_Game.ConfigGetIntArray("cfgVehicles " +GetType() + " InteractActions", m_InteractActions);
	}
	
	override void GetInteractActions( out TIntArray actions)
	{
		if ( m_InteractActions )
		{		
			for( int i = 0; i < m_InteractActions.Count(); i++ )
			{
				actions.Insert(m_InteractActions.Get(i));
			}
		}
	}
};
