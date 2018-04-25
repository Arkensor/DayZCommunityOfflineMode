class Trap_SmallFish extends TrapSpawnBase
{
	void Trap_SmallFish()
	{
		m_InitWaitTime = 200;
		m_BaitNeeded = true;

		m_AnimationPhaseSet = "inventory";
		m_AnimationPhaseTriggered = "placing";

		m_WaterSurfaceForSetup = true;

		m_CatchesPond = new multiMap<string, float>;
		
		m_CatchesSea = new multiMap<string, float>;
		m_CatchesSea.Insert("Sardines",1);
		m_CatchesSea.Insert("Sardines",1);
		m_CatchesSea.Insert("Sardines",1);
		
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

class SmallFishTrap extends Trap_SmallFish 
{
	ref RainProcurementManager m_RainProcurement;

	// ----------------------------------------------------------------------------------------
	override void OnPlacementComplete( Man player )
	{
		// fill the bottle when placed in water
		vector position = this.GetPosition();

		if ( GetGame().SurfaceIsSea( position[0], position[2] ) || GetGame().SurfaceIsPond( position[0], position[2] ) )
		{
			this.SetQuantity( this.GetQuantityMax() );
			SetupTrapPlayer( PlayerBase.Cast( player ), false );
		}	
		else
		{
			m_RainProcurement = new RainProcurementManager( this );
			m_RainProcurement.InitRainProcurement();
		}
	}
	
	// ----------------------------------------------------------------------------------------
	// When the item is picked up by a player, stop the rain procurement
	override void EEItemLocationChanged( EntityAI old_owner, EntityAI new_owner ) 
	{
		super.EEItemLocationChanged(old_owner, new_owner);
		
		if (m_RainProcurement != NULL) 
		{
			if (new_owner)
			{
				m_RainProcurement.StopRainProcurement();
				m_RainProcurement = NULL;
			}
		}
	}
}