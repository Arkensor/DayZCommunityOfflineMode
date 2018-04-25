class TentBase extends ItemBase
{
	static const int PACKED 	= 0;
	static const int PITCHED 	= 1;
	
	private int m_State;
	
	protected ref map< ref ToggleSelections, bool> m_ToggleSelections;
	protected ref array<string> m_ShowSelectionsWhenPitched;
	protected ref array<string> m_HideSelectionsWhenPacked;
	
	void TentBase()
	{
		m_ToggleSelections = new map<ref ToggleSelections, bool>;
		m_ShowSelectionsWhenPitched = new array<string>;
		m_HideSelectionsWhenPacked = new array<string>;
	}
	
	void Init()
	{
		Pack( false );
	}
	
	bool ConditionIntoInventory( EntityAI player )
	{
		return CanBeManipulated();
	}

	override bool CanPutIntoHands( EntityAI parent )
	{
		if( !super.CanPutIntoHands( parent ) )
		{		
			return false;
		}
				
		return CanBeManipulated();
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo( parent ) )
		{			
			return false;
		}
				
		return CanBeManipulated();
	}
	
	bool ConditionOutOfHands( EntityAI player )
	{
		return CanBeManipulated();
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		
		if ( !g_Game.IsServer() ) return;

		if ( item.IsInherited(XmasLights) ) 
		{
			XmasLights fence_light = XmasLights.Cast( item );
			fence_light.AttachToObject(this);
			SetAnimationPhase( "xlights", 0 );
		}
		else if ( item.IsKindOf ( "CamoNet" ) ) 
		{
			SetAnimationPhase( "camonet", 0 );
			
			RegenerateNavmesh();
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		if ( !g_Game.IsServer() ) return;
		
		if ( item.IsInherited(XmasLights) ) 
		{
			XmasLights fence_light = XmasLights.Cast( item );
			fence_light.DetachFromObject(this);
			SetAnimationPhase( "xlights", 1 );
			SetAnimationPhase( "placing", 0 );
		}
		else if ( item.IsKindOf ( "CamoNet" ) ) 
		{
			SetAnimationPhase( "camonet", 1 );
			SetAnimationPhase( "placing", 0 );
			
			RegenerateNavmesh();
		}
	}
	
	override void OnPlacementStarted( Man player )
	{
		super.OnPlacementStarted( player );
		
		Pitch( false );
	}
	
	override void OnPlacementComplete( Man player )
	{
		Pitch();
	}
	
	int GetState()
	{
		return m_State;
	}

	bool CanBePacked()
	{
		if ( GetState() == PITCHED )
		{
			if ( GetInventory().GetCargo().GetItemCount() == 0 && GetInventory().AttachmentCount() == 0 )
			{
				return true;
			}
		}
		
		return false;
	}

	bool CanBeManipulated()
	{
		if ( GetState() == PACKED )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	void Pack( bool update_navmesh = true )
	{
		//Print("PACKING");
		m_State = PACKED;
		if (GetInventory())
			DestroyInventory();
		
		SetAnimationPhase( "placing", 1 );
		SetAnimationPhase( "inventory", 0 );
		
		for ( int i = 0; i < m_HideSelectionsWhenPacked.Count(); i++ )
		{
			SetAnimationPhase( m_HideSelectionsWhenPacked.Get(i), 1 );
		}
		
		if ( update_navmesh ) 
		{
			//Print("Pack->RegenerateNavmesh");
			RegenerateNavmesh();	
		}
	}

	void Pitch( bool update_navmesh = true )
	{
		//Print("PITCHING");
		m_State = PITCHED;
		if (!GetInventory())
			CreateAndInitInventory();
		
		SetAnimationPhase( "placing", 0 );
		SetAnimationPhase( "inventory", 1 );
		
		for ( int i = 0; i < m_ShowSelectionsWhenPitched.Count(); i++ )
		{
			SetAnimationPhase( m_ShowSelectionsWhenPitched.Get(i), 0 );
		}
		
		if ( update_navmesh ) 
		{
			//Print("Pitch->RegenerateNavmesh");
			RegenerateNavmesh();	
		}
	}
	
	bool CanToggleSelection( string selection )
	{
		for ( int i = 0; i < m_ToggleSelections.Count(); i++ )
		{
			ToggleSelections toggle = m_ToggleSelections.GetKey(i);
			
			if ( toggle.GetToggleOff() == selection || toggle.GetToggleOn() == selection )
			{
				return true;
			}
		}
		
		return false;
	}

	void ToggleSelection( string selection )
	{
		//Print("TOGGLE SELLECTIONS");
		for ( int i = 0; i < m_ToggleSelections.Count(); i++ )
		{
			ToggleSelections toggle = m_ToggleSelections.GetKey(i);
			
			if ( toggle.GetToggleOff() == selection || toggle.GetToggleOn() == selection )
			{
				if ( m_ToggleSelections.GetElement(i) )
				{
					SetAnimationPhase( toggle.GetToggleOff(), 0 );
					SetAnimationPhase( toggle.GetToggleOn(), 1 );
					m_ToggleSelections.Set( toggle, false );
				}
				else
				{
					SetAnimationPhase( toggle.GetToggleOff(), 1 );
					SetAnimationPhase( toggle.GetToggleOn(), 0 );
					m_ToggleSelections.Set( toggle, true );
				}
			}
		}
	}
	
	void RegenerateNavmesh()
	{
		SetAffectPathgraph( true, false );
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, this);
	}
};
