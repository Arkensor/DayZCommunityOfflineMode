class Fireplace extends FireplaceBase
{
	void Fireplace()
	{
		//Particles - default for FireplaceBase
		PARTICLE_FIRE_START 	= ParticleList.CAMP_FIRE_START;
		PARTICLE_SMALL_FIRE 	= ParticleList.CAMP_SMALL_FIRE;
		PARTICLE_NORMAL_FIRE	= ParticleList.CAMP_NORMAL_FIRE;
		PARTICLE_SMALL_SMOKE 	= ParticleList.CAMP_SMALL_SMOKE;
		PARTICLE_NORMAL_SMOKE	= ParticleList.CAMP_NORMAL_SMOKE;
		PARTICLE_FIRE_END 		= ParticleList.CAMP_FIRE_END;
		PARTICLE_STEAM_END		= ParticleList.CAMP_STEAM_2END;
	}

	override bool IsBaseFireplace()
	{
		return true;
	}
	
	//attachments
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
		if ( item.Type() == ATTACHMENT_COOKING_POT )
		{
			if ( IsItemTypeAttached( ATTACHMENT_TRIPOD ) || IsOven() ) 
			{
				return true;
			}
		}
		
		//tripod
		if ( item.Type() == ATTACHMENT_TRIPOD )
		{
			if ( !IsOven() && GetHierarchyParent() == NULL )
			{
				return true;
			}
		}
		
		//stones
		if ( item.Type() == ATTACHMENT_STONES )
		{
			if ( GetHierarchyParent() || IsBurning() )
			{
				return false;
			}
			
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
		if ( IsKindling( item ) && !IsBurning() )
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
		if ( item.Type() == ATTACHMENT_COOKING_POT )
		{
			return true;
		}
		
		//tripod
		if ( item.Type() == ATTACHMENT_TRIPOD && !GetCookingEquipment() )
		{
			return true;
		}
		
		//stones
		if ( item.Type() == ATTACHMENT_STONES )
		{
			if ( IsOven() || IsBurning() )
			{
				return false;
			}
			
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
			AddToFireConsumables( item_base );
		}
		
		//cookware
		if ( item_base.Type() == ATTACHMENT_COOKING_POT )
		{
			SetCookingEquipment( item_base );
			
			//rotate handle (if not in 'Oven' stage)
			if ( !IsOven() )
			{
				item_base.SetAnimationPhase( ANIMATION_COOKWARE_HANDLE, 0 );
			}
		}
		
		//refresh fireplace visuals
		RefreshFireplaceVisuals();
	}

	override void EEItemDetached( EntityAI item, string slot_name ) 
	{
		super.EEItemDetached( item, slot_name );
		
		ItemBase item_base = ItemBase.Cast( item );
		
		//kindling / fuel
		if ( IsKindling( item_base ) || IsFuel( item_base ) )
		{
			//remove from consumables
			RemoveFromFireConsumables( GetFireConsumableByItem( item_base ) );
			
			//no attachments left, no cargo items & no ashes are present
			if ( GetInventory().AttachmentCount() == 0 && !HasAshes() )
			{
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
		
		//refresh fireplace visuals
		RefreshFireplaceVisuals();
	}
	
	//CONDITIONS
	//this into/outo parent.Cargo
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo( parent ) ) 
		{
			return false;
		}
		
		if ( HasAshes() || IsBurning() || IsOven() || IsInAnimPhase( ANIMATION_TRIPOD ) || IsInAnimPhase( ANIMATION_STONES ) || !IsCargoEmpty() )
		{
			return false;
		}
		
		return true;
	}

	override bool CanRemoveFromCargo( EntityAI parent )
	{
		return true;
	}
	
	//cargo item into/outo this.Cargo
	override bool CanReceiveItemIntoCargo( EntityAI cargo )
	{
		if ( GetHierarchyParent() )
		{
			return false;
		}
		
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
		
		if ( HasAshes() || IsBurning() || IsOven() || IsInAnimPhase( ANIMATION_TRIPOD ) || IsInAnimPhase( ANIMATION_STONES ) || !IsCargoEmpty() )
		{
			return false;
		}
		
		return true;
	}
}
