class Construction
{
	protected ref map<string, ref ConstructionPart> m_ConstructionParts;	//string - part name; int - 0-not constructed, 1-constructed
	protected EntityAI m_Parent;

	//============================================
	// Construction
	//============================================	
	void Construction( EntityAI parent )
	{
		m_ConstructionParts = new ref map<string, ref ConstructionPart>;
		
		//set parent object
		SetParent( parent );
	}
	
	void Init( array<string> base_parts )
	{
		UpdateConstructionParts();
		
		if ( base_parts )
		{
			for ( int i = 0; i < base_parts.Count(); ++i )
			{
				AddToConstructedParts( "", base_parts.Get( i ) );
			}
		}
		
		UpdateVisual();
	}
	
	void ~Construction()
	{	
	}
	
	//parent
	protected EntityAI GetParent()
	{
		return m_Parent;
	}
	protected void SetParent( EntityAI parent )
	{
		m_Parent = parent;
	}

	//move to basebuildingbase
	//============================================
	// OnStore Save/Load
	//============================================
	//on store save/load
	void OnStoreSave( ParamsWriteContext ctx )
	{
		
	}

	void OnStoreLoad( ParamsReadContext ctx )
	{
		
	}
	
	//============================================
	// Construction process
	//============================================
	//constructed parts
	protected void AddToConstructedParts( string slot_name, string part_name )
	{
		if ( !m_ConstructionParts.Contains( part_name ) ) 
		{
			m_ConstructionParts.Insert( part_name, new ConstructionPart( part_name, slot_name, true ) );
		}
		else
		{
			ConstructionPart constrution_part = m_ConstructionParts.Get( part_name );
			constrution_part.SetIsBuilt( true );
		}
	}
	protected void RemoveFromConstructedParts( string slot_name, string part_name )
	{
		if ( !m_ConstructionParts.Contains( part_name ) ) 
		{
			m_ConstructionParts.Insert( part_name, new ConstructionPart( part_name, slot_name, false ) );
		}
		else
		{
			ConstructionPart constrution_part = m_ConstructionParts.Get( part_name );
			constrution_part.SetIsBuilt( false );
		}
	}
	
	//constructPart
	void ConstructPart( string slot_name, string part_name )
	{
		//remove materials
		TakeMaterials( slot_name, part_name );

		//add part to constructed parts
		AddToConstructedParts( slot_name, part_name );
		
		//show proxy selection
		ShowConstructionPart( part_name );
	}
	
	//deconstructPart
	void DeconstructPart( string slot_name, string part_name )
	{
		//remove materials
		ReceiveMaterials( slot_name, part_name );

		//add part to constructed parts
		RemoveFromConstructedParts( slot_name, part_name );
		
		//show proxy selection
		HideConstructionPart( part_name );
	}
	
	//============================================
	// Update construction
	//============================================
	//update visual
	void UpdateVisual()
	{
		for ( int i = 0; i < m_ConstructionParts.Count(); ++i )
		{
			string key = m_ConstructionParts.GetKey( i );
			ConstructionPart value = m_ConstructionParts.Get( key );
		
			if ( value.IsBuilt() )
			{
				ShowConstructionPart( key );
			}
			else
			{
				HideConstructionPart( key );
			}
		}
	}
	
	//update construction parts
	protected void UpdateConstructionParts()
	{
		string construction_path = "cfgVehicles" + " " + GetParent().GetType() + " " + "Construction";
		
		if ( GetGame().ConfigIsExisting( construction_path ) )
		{
			//slots
			int	slots_count = GetGame().ConfigGetChildrenCount( construction_path );
			for ( int i = 0; i < slots_count; ++i )
			{
				string slot_name;
				GetGame().ConfigGetChildName( construction_path, i, slot_name );
				string slot_path = construction_path + " " + slot_name;
				
				//parts
				int parts_count = GetGame().ConfigGetChildrenCount( slot_path );
				for ( int j = 0; j < parts_count; ++j )
				{
					string part_name;
					GetGame().ConfigGetChildName( slot_path, j, part_name );
					m_ConstructionParts.Insert( part_name, new ConstructionPart( part_name, slot_name, false ) );
				}
			}
		}
	}
	
	//============================================
	// Parts
	//============================================	
	//get construction part that can be constructed
	string GetConstructionPartName( string slot_name )
	{
		string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + slot_name;
		
		if ( GetGame().ConfigIsExisting( cfg_path ) )
		{
			int	child_count = GetGame().ConfigGetChildrenCount( cfg_path );
			
			for ( int i = 0; i < child_count; i++ )
			{
				string part_name;
				GetGame().ConfigGetChildName( cfg_path, i, part_name );
				
				if ( !IsPartConstructed( part_name ) && HasRequiredPart( slot_name, part_name ) && !HasConflictPart( slot_name, part_name )  )
				{
					return part_name;	
				}
			}
		}
		
		return "";
	}
	
	//get construction part that can be deconstructed
	string GetDeconstructionPartName( string slot_name )
	{
		string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + slot_name;
		
		if ( GetGame().ConfigIsExisting( cfg_path ) )
		{
			int	child_count = GetGame().ConfigGetChildrenCount( cfg_path );
			
			for ( int i = 0; i < child_count; i++ )
			{
				string part_name;
				GetGame().ConfigGetChildName( cfg_path, i, part_name );
				
				if ( IsPartConstructed( part_name ) && !HasDependentPart( part_name )  )
				{
					return part_name;	
				}
			}
		}
		
		return "";
	}
	
	//checks if construction part has dependent part (that is already built) because of which it cannot be deconstruct
	bool HasDependentPart( string part_name )
	{
		for ( int i = 0; i < m_ConstructionParts.Count(); ++i )
		{
			string key = m_ConstructionParts.GetKey( i );
			ConstructionPart construction_part = m_ConstructionParts.Get( key );
			
			if ( construction_part.IsBuilt() )
			{
				string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " " + "Construction" + " " + construction_part.GetSlotName() + " " + construction_part.GetPartName() + " " + "RequiredParts";
				ref array<string> required_parts = new array<string>;
				GetGame().ConfigGetTextArray( cfg_path, required_parts );
				
				if ( required_parts.Find( construction_part.GetPartName() ) > 0 )
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	//checks if construction part has required part already built
	protected bool HasRequiredPart( string slot_name, string part_name )
	{
		string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + slot_name + " " + part_name + " " + "RequiredParts";
		ref array<string> required_parts = new array<string>;
		GetGame().ConfigGetTextArray( cfg_path, required_parts );
		
		//if no parts are required
		if ( required_parts.Count() == 0 )
		{
			return true;
		}
		
		//check if parts are already built
		for ( int i = 0; i < required_parts.Count(); i++ )
		{
			if ( !IsPartConstructed( required_parts.Get( i ) ) )
			{
				return false;
			}
		}
		
		return true;
	}
	
	//checks if there are conflict parts already built
	protected bool HasConflictPart( string slot_name, string part_name )
	{
		string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + slot_name + " " + part_name + " " + "ConflictParts";
		ref array<string> conflict_parts = new array<string>;
		GetGame().ConfigGetTextArray( cfg_path, conflict_parts );

		//if no parts are required
		if ( conflict_parts.Count() == 0 )
		{
			return false;
		}
		
		//check if parts are already built
		for ( int i = 0; i < conflict_parts.Count(); i++ )
		{
			if ( IsPartConstructed( conflict_parts.Get( i ) ) )
			{
				return true;
			}
		}
		
		return false;
	}
	
	
	//============================================
	// Conditions
	//============================================
	//can construct part
	bool CanConstructPart( string slot_name )
	{
		string construction_part = GetConstructionPartName( slot_name );
		
		if ( construction_part.Length() > 0 )
		{
			return true;
		}
		
		return false;
	}

	//can deconstruct part
	bool CanDeconstructPart( string slot_name )
	{
		string deconstruction_part = GetDeconstructionPartName( slot_name );
		
		if ( deconstruction_part.Length() > 0 )
		{
			return true;
		}
		
		return false;
	}
	
	//show/hide construction part
	protected void ShowConstructionPart( string part_name )
	{
		GetParent().SetAnimationPhase( part_name, 0 );
		GetParent().AddProxyPhysics( part_name );
	}
	protected void HideConstructionPart( string part_name )
	{
		GetParent().SetAnimationPhase( part_name, 1 );
		GetParent().RemoveProxyPhysics( part_name );
	}
	
	
	//is part constructed
	protected bool IsPartConstructed( string part_name )
	{
		if ( m_ConstructionParts.Contains( part_name ) && m_ConstructionParts.Get( part_name ).IsBuilt() )
		{
			return true;
		}
		
		return false;
	}
	//============================================
	// Materials for construction
	//============================================
	//has materials
	bool HasMaterials( string slot_name, string part_name )
	{
		string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + slot_name + " " + part_name + " " + "Materials";
		
		if ( GetGame().ConfigIsExisting( cfg_path ) )
		{
			int	child_count = GetGame().ConfigGetChildrenCount( cfg_path );
			
			for ( int i = 0; i < child_count; i++ )
			{
				string child_name;
				GetGame().ConfigGetChildName( cfg_path, i, child_name );
				
				//get type, quantity from material
				string material_path;
				string type;
				material_path = cfg_path + " " + child_name + " " + "type";
				GetGame().ConfigGetText( material_path, type );
				material_path = cfg_path + " " + child_name + " " + "quantity";
				float quantity = GetGame().ConfigGetFloat( material_path );			//!TODO add quantity as non-required parameter in config (default value will be provided)
				
				//if the selected material (or its quantity) is not available
				if ( !HasMaterialAttached( type, quantity ) )
				{
					return false;
				}
			}
		}
		
		return true;	//return true even if no material required
	}
	
	//check if parent object has attachment of required quantity attached to it
	protected bool HasMaterialAttached( string type, float quantity )
	{
		ItemBase attachment = ItemBase.Cast( GetParent().GetAttachmentByConfigTypeName( type ) );
		
		if ( attachment && attachment.GetQuantity() >= quantity )
		{
			return true;
		}
		
		return false;
	}
	
	//take materials when constructing
	protected void TakeMaterials( string slot_name, string part_name )
	{
		string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + slot_name + " " + part_name + " " + "Materials";
		
		if ( GetGame().ConfigIsExisting( cfg_path ) )
		{
			int	child_count = GetGame().ConfigGetChildrenCount( cfg_path );
			
			for ( int i = 0; i < child_count; i++ )
			{
				string child_name;
				GetGame().ConfigGetChildName( cfg_path, i, child_name );
				
				//get type, quantity from material
				string material_path;
				string type;
				material_path = cfg_path + " " + child_name + " " + "type";
				GetGame().ConfigGetText( material_path, type );
				material_path = cfg_path + " " + child_name + " " + "quantity";
				float quantity = GetGame().ConfigGetFloat( material_path );			//!TODO add quantity as non-required parameter in config (default value will be provided)
				
				//subtract quantity
				ItemBase attachment = ItemBase.Cast( GetParent().GetAttachmentByConfigTypeName( type ) );
				attachment.AddQuantity( -quantity );
			}
		}
	}
	
	//receive materials when deconstructing
	protected void ReceiveMaterials( string slot_name, string part_name )
	{
		string cfg_path = "cfgVehicles" + " " + GetParent().GetType() + " "+ "Construction" + " " + slot_name + " " + part_name + " " + "Materials";
		
		if ( GetGame().ConfigIsExisting( cfg_path ) )
		{
			int	child_count = GetGame().ConfigGetChildrenCount( cfg_path );
			
			for ( int i = 0; i < child_count; i++ )
			{
				string child_name;
				GetGame().ConfigGetChildName( cfg_path, i, child_name );
				
				//get type, quantity from material
				string material_path;
				string type;
				material_path = cfg_path + " " + child_name + " " + "type";
				GetGame().ConfigGetText( material_path, type );
				material_path = cfg_path + " " + child_name + " " + "quantity";
				float quantity = GetGame().ConfigGetFloat( material_path );			//!TODO add quantity as non-required parameter in config (default value will be provided)
				
				//TODO add quantity to material item if it's attached to parent
				//set quantity
				ItemBase received_material = ItemBase.Cast( GetGame().CreateObject( type, GetParent().GetPosition() ) );
				received_material.SetQuantity( quantity );
			}
		}
	}
}
