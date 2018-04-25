class BarrelHoles_ColorBase extends FireplaceBase
{
	//Visual animations
	const string ANIMATION_OPENED 			= "LidOff";
	const string ANIMATION_CLOSED			= "LidOn";
	
	protected bool m_IsOpened 				= false;
	
	void BarrelHoles_ColorBase()
	{
		//Particles - default for FireplaceBase
		PARTICLE_FIRE_START 	= ParticleList.BARREL_FIRE_START;
		PARTICLE_SMALL_FIRE 	= ParticleList.BARREL_SMALL_FIRE;
		PARTICLE_NORMAL_FIRE	= ParticleList.BARREL_NORMAL_FIRE;
		PARTICLE_SMALL_SMOKE 	= ParticleList.BARREL_SMALL_SMOKE;
		PARTICLE_NORMAL_SMOKE	= ParticleList.BARREL_NORMAL_SMOKE;
		PARTICLE_FIRE_END 		= ParticleList.BARREL_FIRE_END;
		PARTICLE_STEAM_END		= ParticleList.BARREL_FIRE_STEAM_2END;		
	}
	
	override bool IsBarrelWithHoles()
	{
		return true;
	}
	
	//ATTACHMENTS
	override bool CanReceiveAttachment( EntityAI attachment )
	{
		ItemBase item = ItemBase.Cast( attachment );
		
		//kindling items
		if ( IsKindling ( item ) && IsOpened() )
		{
			return true;
		}
		
		//fuel items
		if ( IsFuel ( item ) && IsOpened() )
		{
			return true;
		}
		
		//cookware
		if ( item.Type() == ATTACHMENT_COOKING_POT && !IsOpened() )
		{
			return true;
		}
		
		return false;
	}

	override bool CanReleaseAttachment( EntityAI attachment )
	{
		ItemBase item = ItemBase.Cast( attachment );
		
		//kindling items
		if ( IsKindling ( item ) && !IsBurning() && IsOpened() )
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
		if ( IsFuel ( item ) && !IsBurning() && IsOpened() )
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
		if ( item.Type() == ATTACHMENT_COOKING_POT )
		{
			return true;
		}

		
		return false;
	}

	override void EEItemAttached ( EntityAI item, string slot_name ) 
	{
		super.EEItemAttached ( item, slot_name );
		
		ItemBase item_base = ItemBase.Cast( item );
		
		//kindling / fuel
		if ( IsKindling ( item_base ) || IsFuel ( item_base ) )
		{
			//add to consumables
			AddToFireConsumables ( item_base );
		}
		
		//cookware
		if ( item_base.Type() == ATTACHMENT_COOKING_POT )
		{
			SetCookingEquipment( item_base );
			
			//rotate handle
			item_base.SetAnimationPhase ( ANIMATION_COOKWARE_HANDLE, 0 );
		}
		
		//refresh fireplace visuals
		RefreshFireplaceVisuals();
	}

	override void EEItemDetached ( EntityAI item, string slot_name ) 
	{
		super.EEItemDetached ( item, slot_name );
		
		ItemBase item_base = ItemBase.Cast( item );
		
		//kindling / fuel
		if ( IsKindling ( item_base ) || IsFuel ( item_base ) )
		{
			//remove from consumables
			RemoveFromFireConsumables ( GetFireConsumableByItem( item_base ) );
		}

		//cookware
		if ( item_base.Type() == ATTACHMENT_COOKING_POT )
		{
			ClearCookingEquipment();
			
			//rotate handle
			item_base.SetAnimationPhase ( ANIMATION_COOKWARE_HANDLE, 1 );
			
			//stop steam particle
			ParticleCookingEquipmentSteamStop();	
		}
		
		//refresh fireplace visuals
		RefreshFireplaceVisuals();
	}
	
	//CONDITIONS
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
		if ( !IsOpened() || GetHierarchyParent() )
		{
			return false;
		}
		
		return true;
	}

	override bool CanRelaseCargo( EntityAI cargo )
	{
		if ( IsBurning() && IsOpened() )
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
		if ( HasAshes() || IsBurning() || !IsCargoEmpty() || IsItemTypeAttached( ATTACHMENT_COOKING_POT ) )
		{
			return false;
		}
		
		return true;
	}
		
	//ACTIONS
	bool IsOpened()
	{
		return m_IsOpened;
	}
	
	void SetOpenState( bool state )
	{
		m_IsOpened = state;
	}
	
	override void Open()
	{
		//update visual
		SetAnimationPhase ( ANIMATION_OPENED, 0 );
		SetAnimationPhase ( ANIMATION_CLOSED, 1 );
		
		//set open state
		SetOpenState( true );
		
		//refresh
		RefreshFireParticlesAndSounds();
	}
	
	override void Close()
	{
		//update visual
		SetAnimationPhase ( ANIMATION_OPENED, 1 );
		SetAnimationPhase ( ANIMATION_CLOSED, 0 );
		
		//set open state
		SetOpenState( false );
		
		//refresh
		RefreshFireParticlesAndSounds();
	}
	
	//particles
	override void RefreshFireParticlesAndSounds()
	{
		FireplaceFireState fire_state = GetFireState();
		
		if ( m_LastFireState != fire_state )
		{
			if ( fire_state == FireplaceFireState.START_FIRE )
			{
				//particles
				ParticleFireStartStart();
				
				//sounds
				SoundBurningStop();
				SoundBurningSmallStart();
			}
			else if ( fire_state == FireplaceFireState.SMALL_FIRE )
			{
				//particles
				ParticleFireStartStop();
				
				ParticleSmallFireStart();
				if ( IsOpened() ) 
				{
					ParticleSmallSmokeStart();
				}
				else
				{
					ParticleSmallSmokeStop();
				}
				
				ParticleNormalFireStop();
				ParticleNormalSmokeStop();
				
				//sounds
				SoundBurningStop();
				SoundBurningSmallStart();
			}
			else if ( fire_state == FireplaceFireState.NORMAL_FIRE )
			{
				//particles
				ParticleFireStartStop();
				
				ParticleSmallFireStop();
				ParticleSmallSmokeStop();
				
				ParticleNormalFireStart();
				if ( IsOpened() ) 
				{
					ParticleNormalSmokeStart();
				}
				else
				{
					ParticleNormalSmokeStop();
				}
				
				//sounds
				SoundBurningStop();
				SoundBurningNormalStart();
			}
			else if ( fire_state == FireplaceFireState.END_FIRE )
			{
				//particles
				ParticleFireStartStop();
				
				ParticleSmallFireStop();
				ParticleSmallSmokeStop();

				ParticleNormalFireStop();
				ParticleNormalSmokeStop();

				ParticleFireEndStart();
				
				//sounds
				SoundBurningStop();
				SoundBurningNoFuelStart();
			}
			else if ( fire_state == FireplaceFireState.EXTINGUISH_FIRE )
			{
				//particles
				ParticleFireStartStop();
				
				ParticleSmallFireStop();
				ParticleSmallSmokeStop();

				ParticleNormalFireStop();
				ParticleNormalSmokeStop();	

				ParticleFireEndStop();
				
				ParticleSteamEndStart();
				
				//sounds
				SoundBurningStop();
				SoundBurningNoFuelStart();
			}
			else if ( fire_state == FireplaceFireState.NO_FIRE )
			{
				//particles
				ParticleFireStartStop();
				
				ParticleSmallFireStop();
				ParticleSmallSmokeStop();

				ParticleNormalFireStop();
				ParticleNormalSmokeStop();	

				ParticleFireEndStop();
				
				ParticleSteamEndStop();
				
				//sounds
				SoundBurningStop();
			}

			m_LastFireState = fire_state;
		}
	}
}
