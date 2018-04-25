class ActionTakeFireplaceIndoor: ActionInteractBase
{
	void ActionTakeFireplaceIndoor()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageSuccess = "I took the fireplace.";
	}

	override int GetType()
	{
		return AT_TAKE_FIREPLACE_INDOOR;
	}

	override string GetText()
	{
		return "take fireplace";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object target_object = target.GetObject();
		//empty hands	--interact action base condition
		if ( target_object )
		{
			FireplaceIndoor fireplace_indoor = FireplaceIndoor.Cast( target_object );
	
			if ( !fireplace_indoor.HasAshes() && !fireplace_indoor.IsBurning() && fireplace_indoor.IsCargoEmpty() && !fireplace_indoor.GetCookingEquipment() )
			{
				return true;
			}
		}

		return false;
	}

	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{
		Object target_object = target.GetObject();
		FireplaceIndoor fireplace_indoor = FireplaceIndoor.Cast( target_object );
		
		MiscGameplayFunctions.TurnItemIntoItemEx(player, new FireplaceIndoorLambda(fireplace_indoor, "Fireplace", player));

		/*
		//create fireplace object
		vector spawn_pos = player.GetPosition();
		Fireplace fireplace = Fireplace.Cast( GetDayZGame().CreateObject( "Fireplace", Vector(spawn_pos[0]+1, spawn_pos[1]+1, spawn_pos[2]+1) ) );
		player.TakeEntityToHands(true, fireplace );
		
		//transfer all required parameters to this object (damage, wetness)
		fireplace.SetHealth( "", "", fireplace_indoor.GetHealth("", "") );
		fireplace.SetWet( fireplace_indoor.GetWet() );
		
		//transfer all cargo items to this object, position of all items must be preserved
		//fireplace in hands cannot have any items in it but this can be changed in the future
		Cargo cargo = fireplace_indoor.GetInventory().GetCargo();
		for ( int i = 0; i < cargo.GetItemCount(); i++ )
		{
			int col;
			int row;
			cargo.GetItemPos( i, col, row );
			fireplace.SynchronizedTakeEntityToCargoEx(true,  cargo.GetItem ( i ), fireplace.GetInventory().GetCargo(), row, col );
		}
		
		//transfer all attachments to this object
		int item_attachments_count = fireplace_indoor.GetInventory().AttachmentCount();
		for ( int j = 0; j < item_attachments_count; j++ )
		{
			EntityAI entity = fireplace_indoor.GetInventory().GetAttachmentFromIndex( 0 );
			fireplace.SynchronizedTakeEntityAsAttachment(true,  entity );
		}*/
	}
}

class FireplaceIndoorLambda : TurnItemIntoItemLambda
{
	float m_SpillModifier;
	float m_SpecialtyWeight;
	void FireplaceIndoorLambda (EntityAI old_item, string new_item_type, PlayerBase player) { }

	override void CopyOldPropertiesToNew (notnull EntityAI old_item, notnull EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);

		Fireplace fireplace = Fireplace.Cast(new_item);
		fireplace.SetHealth( "", "", old_item.GetHealth("", "") );
		fireplace.SetWet( old_item.GetWet() );
		
		//@TODO: move to TurnItemIntoItemLambda?
		//transfer all cargo items to this object, position of all items must be preserved
		//fireplace in hands cannot have any items in it but this can be changed in the future
		Cargo cargo = old_item.GetInventory().GetCargo();
		for ( int i = 0; i < cargo.GetItemCount(); i++ )
		{
			int col;
			int row;
			cargo.GetItemPos( i, col, row );
			fireplace.LocalTakeEntityToCargoEx(cargo.GetItem ( i ), 0, row, col );
		}
	}
};
