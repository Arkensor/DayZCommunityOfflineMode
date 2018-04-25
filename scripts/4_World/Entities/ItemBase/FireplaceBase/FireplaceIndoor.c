class FireplaceIndoor extends FireplaceBase
{
	//Fireplace point for Indoor fireplaces
	protected ref FireplacePoint 	m_FireplacePoint;
	//saved/loaded storage data 
	protected string 				m_FirePoint;
	
	//Client side variables
	protected float 				m_SmokePosX;
	protected float 				m_SmokePosY;
	protected float 				m_SmokePosZ;
	protected vector				m_SmokePosition;
	
	void FireplaceIndoor()
	{
		//Particles - default for FireplaceBase
		PARTICLE_FIRE_START 	= ParticleList.HOUSE_FIRE_START;
		PARTICLE_SMALL_FIRE 	= ParticleList.HOUSE_SMALL_FIRE;
		PARTICLE_NORMAL_FIRE	= ParticleList.HOUSE_NORMAL_FIRE;
		PARTICLE_SMALL_SMOKE 	= ParticleList.HOUSE_SMALL_SMOKE;
		PARTICLE_NORMAL_SMOKE	= ParticleList.HOUSE_NORMAL_SMOKE;
		PARTICLE_FIRE_END 		= ParticleList.HOUSE_FIRE_END;
		PARTICLE_STEAM_END		= ParticleList.HOUSE_FIRE_STEAM_2END;		
		
		//register sync variables
		RegisterNetSyncVariableFloat( "m_SmokePosX", -100000, 100000, 2 );
		RegisterNetSyncVariableFloat( "m_SmokePosY", -100000, 100000, 2 );
		RegisterNetSyncVariableFloat( "m_SmokePosZ", -100000, 100000, 2 );
	}

	//================================================================
	// ONSTORESAVE/LOAD/AFTERLOAD
	//================================================================
/*
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );

		//fire point name
		ctx.Write( m_FirePoint );
	}

	override void OnStoreLoad( ParamsReadContext ctx )
	{
		super.OnStoreLoad( ctx );

		//fire point name
		ctx.Read( m_FirePoint );
	}
	
	override void EEOnAfterLoad()
	{
		super.EEOnAfterLoad();
		
		//set fireplace point data
		SetBuildingOnLoad();
	}
*/
		
	void SetBuildingOnLoad()
	{
		ref array<Object> nearest_objects = new array<Object>;
		ref array<Cargo> proxy_cargos = new array<Cargo>;
		vector position = GetPosition();
		GetGame().GetObjectsAtPosition ( position, 50, nearest_objects, proxy_cargos );
		
		Print("SetBuildingOnLoad");
		Print(nearest_objects.Count());
		for ( int i = 0; i < nearest_objects.Count(); ++i )
		{
			Object object = nearest_objects.Get( i );
			Print(object.GetType() + " idx = " + i.ToString());
			Print(object.ClassName() + " idx = " + i.ToString());
			
			if ( object.IsInherited( BuildingWithFireplace ) )
			{
				BuildingWithFireplace building = BuildingWithFireplace.Cast( object );
				
				//check distance
				vector fire_point_pos = building.GetSelectionPosition( m_FirePoint );
				vector fire_point_pos_world = building.ModelToWorld( fire_point_pos );
				
				float distance = vector.Distance( position, fire_point_pos_world );
				
				Print( position );
				Print( fire_point_pos_world );
				Print( distance );
				
				if ( distance < 1 )
				{
					ref FireplacePoint fireplace_point = building.GetFirePointByFirePoint( m_FirePoint );
					fireplace_point.SetObject( this );
					SetFireplacePoint( fireplace_point );
					
					break;
				}
			}
		}
	}

	//================================================================
	// FIREPLACE POINT (INDOOR FIREPLACES)
	//================================================================
	FireplacePoint GetFireplacePoint()
	{
		return m_FireplacePoint;
	}
	
	void SetFireplacePoint( FireplacePoint fireplace_point )
	{
		m_FireplacePoint = fireplace_point;
		
		if ( fireplace_point )
		{
			SetSmokePosition( fireplace_point );
			m_FirePoint = fireplace_point.GetFirePoint();
		}
		else
		{
			m_FirePoint = "";
		}
		
		//synchronize
		Synchronize();
	}
	
	void SetSmokePosition( FireplacePoint fireplace_point )
	{
		string memory_point = fireplace_point.GetSmokePoint();
		vector smoke_point_local_pos = fireplace_point.GetBuilding().GetSelectionPosition( memory_point );
		vector smoke_point_world_pos = fireplace_point.GetBuilding().ModelToWorld( smoke_point_local_pos );
		
		vector smoke_pos = WorldToModel( smoke_point_world_pos );
		m_SmokePosX = smoke_pos[0];
		m_SmokePosY = smoke_pos[1];
		m_SmokePosZ = smoke_pos[2];
	}
	
	void ClearFireplacePoint()
	{
		SetFireplacePoint( NULL );
	}
	
	//================================================================
	// PARTICLES
	//================================================================
	override protected vector GetSmokeEffectPosition()
	{
		return Vector( m_SmokePosX, m_SmokePosY, m_SmokePosZ );
	}

	//================================================================
	// STATE
	//================================================================
	override bool IsFireplaceIndoor()
	{
		return true;
	}

	//================================================================
	// ATTACHMENTS
	//================================================================	
	override bool CanReceiveAttachment( EntityAI attachment )
	{
		ItemBase item = ItemBase.Cast( attachment );
		
		//kindling items
		if ( IsKindling ( item ) )
		{
			return true;
		}
		
		//fuel items
		if ( IsFuel ( item ) )
		{
			return true;
		}
		
		//cookware
		if ( item.Type() == ATTACHMENT_COOKING_POT || item.Type() == ATTACHMENT_FRYING_PAN )
		{
			return true;
		}
		
		return false;
	}

	override bool CanReleaseAttachment( EntityAI attachment )
	{
		ItemBase item = ItemBase.Cast( attachment );
		
		//has last attachment and there are still items in cargo
		if ( GetInventory().AttachmentCount() == 1 && GetInventory().GetCargo().GetItemCount() != 0 )
		{
			return false;
		}
		
		//kindling items
		if ( IsKindling ( item ) && !IsBurning() )
		{
			if ( HasLastFuelKindlingAttached() )
			{
				if ( HasLastAttachment() )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		
		//fuel items
		if ( IsFuel( item ) && !IsBurning() )
		{
			if ( HasLastFuelKindlingAttached() )
			{	
				if ( HasLastAttachment() )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		
		//cookware
		if ( item.Type() == ATTACHMENT_COOKING_POT || item.Type() == ATTACHMENT_FRYING_PAN )
		{
			return true;
		}
		
		return false;
	}

	override void EEItemAttached( EntityAI item, string slot_name ) 
	{
		super.EEItemAttached( item, slot_name );
		
		ItemBase item_base = ItemBase.Cast( item );
		
		//kindling / fuel
		if ( IsKindling( item_base ) || IsFuel( item_base ) )
		{
			//add to consumables
			AddToFireConsumables ( item_base );
		}
		
		//cookware
		if ( item_base.Type() == ATTACHMENT_COOKING_POT )
		{
			SetCookingEquipment( item_base );
			
			//rotate handle
			item_base.SetAnimationPhase( ANIMATION_COOKWARE_HANDLE, 1 );
		}
		//TODO
		//frying pan 
		if ( item_base.Type() == ATTACHMENT_FRYING_PAN )
		{
			SetCookingEquipment( item_base );
		}
		
		
		//refresh fireplace visuals
		RefreshFireplaceVisuals();
	}

	override void EEItemDetached( EntityAI item, string slot_name ) 
	{
		super.EEItemDetached ( item, slot_name );
		
		ItemBase item_base = ItemBase.Cast( item );
		
		//kindling / fuel
		if ( IsKindling( item_base ) || IsFuel( item_base ) )
		{
			//remove from consumables
			RemoveFromFireConsumables( GetFireConsumableByItem( item_base ) );
			
			//no attachments left & no ashes are present
			if ( GetInventory().AttachmentCount() == 0 && !HasAshes() )
			{
				//Clear point
				if ( GetGame().IsServer() )
				{
					if ( GetFireplacePoint() )
					{
						GetFireplacePoint().ClearObject();
						ClearFireplacePoint();						
					}
				}

				//destroy fireplace
				DestroyFireplace();
			}
		}

		//cookware
		if ( item_base.Type() == ATTACHMENT_COOKING_POT )
		{
			ClearCookingEquipment();
			
			//rotate handle
			item_base.SetAnimationPhase( ANIMATION_COOKWARE_HANDLE, 1 );
			
			//stop steam particle
			ParticleCookingEquipmentSteamStop();
		}	
		//TODO
		//frying pan		
		if ( item_base.Type() == ATTACHMENT_FRYING_PAN )
		{
			ClearCookingEquipment();
		}
		
		//refresh fireplace visuals
		RefreshFireplaceVisuals();
	}
		
	//================================================================
	// CONDITIONS
	//================================================================
	//this into/outo parent.Cargo
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}
		return false;
	}

	override bool CanRemoveFromCargo( EntityAI parent )
	{
		return true;
	}
	
	//cargo item into/outo this.Cargo
	override bool CanReceiveItemIntoCargo( EntityAI cargo )
	{
		return true;
	}

	override bool CanRelaseCargo( EntityAI cargo )
	{
		if ( IsBurning() )
		{
			return false;
		}
		
		return true;
	}
	
	//hands
	override bool CanPutIntoHands( EntityAI parent )
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		return false;
	}
	
	override bool CanRemoveFromHands ( EntityAI player ) 
	{
		return false;
	}	
}
