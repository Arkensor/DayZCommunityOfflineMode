class HescoBox extends Inventory_Base
{
	static const int FOLDED 		= 0;
	static const int UNFOLDED 		= 1;
	static const int FILLED 		= 2;
	
	ref array<string> m_SurfaceForSetup;

	protected int m_State;
	
	void HescoBox()
	{
		m_State = FOLDED;
		m_SurfaceForSetup = new array<string>;
		
		//synchronized variables
		RegisterNetSyncVariableInt( "m_State", FOLDED, FILLED );
	}

	override bool CanPutIntoHands( EntityAI parent )
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		return CanBeManipulated();
	}

	override void OnPlacementComplete( Man player )
	{
		Unfold();
	}

	void Synchronize()
	{
		SetSynchDirty();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		//refresh visuals
		RefreshVisuals();
	}
	
	void RefreshVisuals()
	{
		if( GetState() == FOLDED )
		{
			m_SurfaceForSetup.Clear();		
		}
		else if( GetState() == UNFOLDED )
		{
			InsertMaterialForSetup();
		}
	}

	int GetState()
	{
		return m_State;
	}
	
	void SetState( int state )
	{
		m_State = state;
	}

	bool CanBeFilledAtPosition( vector position )
	{
		string surface_type;
		GetGame().SurfaceGetType( position[0], position[2], surface_type );

		if ( m_SurfaceForSetup.Find(surface_type) > -1 )
		{
			return true;
		}
		
		return false;
	}

	bool CanBeManipulated()
	{
		if ( GetState() == FOLDED )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	void Fold()
	{
		this.ShowSelection( "inventory" );
		this.HideSelection( "placing" );
		this.HideSelection( "filled" );
		
		m_SurfaceForSetup.Clear();

		SetState( FOLDED );
		
		Synchronize();
		
		DecreaseHealth( "", "", 5 ); //TODO Daniel implement soft skill bonus via useraction
	}

	void Unfold()
	{
		this.HideSelection( "inventory" );
		this.ShowSelection( "placing" );
		this.HideSelection( "filled" );
	
		InsertMaterialForSetup();

		SetState( UNFOLDED );
		
		Synchronize();
		
		DecreaseHealth( "", "", 5 ); //TODO Daniel implement soft skill bonus via useraction
	}

	void InsertMaterialForSetup()
	{	
		m_SurfaceForSetup.Insert("cp_dirt");
		m_SurfaceForSetup.Insert("cp_broadleaf_dense1");
		m_SurfaceForSetup.Insert("cp_broadleaf_dense2");
		m_SurfaceForSetup.Insert("cp_broadleaf_sparse1");
		m_SurfaceForSetup.Insert("cp_broadleaf_sparse2");
		m_SurfaceForSetup.Insert("cp_conifer_common1");
		m_SurfaceForSetup.Insert("cp_conifer_common2");
		m_SurfaceForSetup.Insert("cp_conifer_moss1");
		m_SurfaceForSetup.Insert("cp_conifer_moss2");
		m_SurfaceForSetup.Insert("cp_grass");
		m_SurfaceForSetup.Insert("cp_grass_tall");
		m_SurfaceForSetup.Insert("cp_gravel");
	}

	void Fill()
	{
		this.HideSelection( "inventory" );
		this.HideSelection( "placing" );
		this.ShowSelection( "filled" );
		
		SetState( FILLED );

		Synchronize();
		
		DecreaseHealth( "", "", 5 ); //TODO Daniel implement soft skill bonus via useraction
	}
}
