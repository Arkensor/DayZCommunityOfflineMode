class Trap_FishNet extends TrapSpawnBase
{
	void Trap_FishNet()
	{
		m_InitWaitTime = Math.RandomFloat(45,90);
		m_BaitNeeded = false;
		m_IsFoldable = true;

		m_AnimationPhaseSet = "inventory";
		m_AnimationPhaseTriggered = "placing";
		
		m_WaterSurfaceForSetup = true;

		m_CatchesPond = new multiMap<string, float>;
		m_CatchesPond.Insert("Carp",1);
		m_CatchesPond.Insert("Carp",1);
		m_CatchesPond.Insert("Carp",2);
		
		m_CatchesSea = new multiMap<string, float>;
		m_CatchesSea.Insert("Sardines",1);
		m_CatchesSea.Insert("Sardines",1);
		m_CatchesSea.Insert("Sardines",2);

		m_CatchesSea.Insert("Sardines",1);
		m_CatchesSea.Insert("Sardines",1);
		m_CatchesSea.Insert("Sardines",2);

		m_CatchesSea.Insert("Mackerel",1);
		m_CatchesSea.Insert("Mackerel",1);
		m_CatchesSea.Insert("Mackerel",2);
		
		m_CatchesGroundAnimal = new multiMap<string, float>;
	}
	
	// ITEM CANNOT BE TAKEN WHEN CONTAINS CARGO
	/*override*/ bool CanPutInInventory ( EntityAI  player ) 
	{
		
		return IsTakeable();
	}
	
	override bool CanPutIntoHands ( EntityAI player ) 
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		return IsTakeable();
	}

	override bool CanRemoveFromHands ( EntityAI player ) 
	{
		return IsTakeable();
	}

	override bool CanReceiveItemIntoCargo( EntityAI child )
	{
		if ( GetHierarchyRootPlayer() == NULL )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override bool CanRelaseCargo( EntityAI child )
	{
		if ( GetHierarchyRootPlayer() == NULL )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

class FishNetTrap extends Trap_FishNet 
{

}