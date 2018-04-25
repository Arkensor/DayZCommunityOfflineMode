class ActionPlaceFireplaceIndoor: ActionSingleUseBase
{
	private ref FireplacePoint 	m_ActualFireplacePoint;
	private const float 		ROUGH_SPECIALTY_WEIGHT = -0.01;
	private const float 		FIRE_POINT_MAX_DISTANCE = 2;		//maximum distance from the fireplace interact point

	void ActionPlaceFireplaceIndoor()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "I placed the fireplace inside.";
		m_MessageFail = "I was unable to place the fireplace inside.";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override int GetType()
	{
		return AT_PLACE_FIREPLACE_INDOOR;
	}

	override string GetText()
	{
		return "place fireplace";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !target ) return false;
		if ( !IsInReach(player, target, UAMaxDistances.DEFAULT) ) return false;

		Object target_object = target.GetObject();
		string action_selection = target_object.GetActionComponentName(target.GetComponentIndex());
		
		if ( target_object && target_object.IsInherited( BuildingWithFireplace ) )
		{
			BuildingWithFireplace building = BuildingWithFireplace.Cast( target_object );
			
			if ( item && building.IsFireplaceActionSelection( action_selection )  )
			{
				FireplacePoint fireplace_point = building.GetFirePointByActionSelection( action_selection );
				string fire_point = fireplace_point.GetFirePoint();
				vector fire_point_pos = building.GetSelectionPosition( fire_point );
				vector fire_point_pos_world = building.ModelToWorld( fire_point_pos );				
				float fire_point_dist = vector.Distance( fire_point_pos_world, player.GetPosition() );
				
				//check point availability and distance from player
				if ( building.IsPointAvailable( action_selection ) && fire_point_dist <= FIRE_POINT_MAX_DISTANCE )
				{
					return true;
				}
			}			
		}
		
		return false;
	}
	
	override void OnStartServer( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//get actual action_selection
		BuildingWithFireplace building = BuildingWithFireplace.Cast( target.GetObject() );
		string action_selection = building.GetActionComponentName(target.GetComponentIndex());
		
		//get and set FireplacePoint reference
		m_ActualFireplacePoint = building.GetFirePointByActionSelection( action_selection );
	}
	
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item /* fireplace in hands */, Param acdata )
	{	
		FireplaceBase fireplace_in_hands = FireplaceBase.Cast( item );
		
		//get 'fire point pos' from 'fire point' memory point
		string fire_point = m_ActualFireplacePoint.GetFirePoint();
		BuildingWithFireplace building = BuildingWithFireplace.Cast( target.GetObject() );
		//
		//set position and direction to fireplace_pointX
		vector fire_point_pos = building.GetSelectionPosition( fire_point );
		vector fire_point_pos_world = building.ModelToWorld( fire_point_pos );

		//create fireplace indoor
		FireplaceIndoor fireplace_indoor = FireplaceIndoor.Cast( GetDayZGame().CreateObject( "FireplaceIndoor", fire_point_pos_world ) );

		//transfer all required parameters to this object (damage, wetness)
		//fireplace_indoor.SetPosition( fire_point_pos_world );
		fireplace_indoor.SetHealth( "", "", fireplace_in_hands.GetHealth("", "") );
		fireplace_indoor.SetWet( fireplace_in_hands.GetWet() );
		
		//transfer all cargo items to this object, position of all items must be preserved
		//fireplace in hands cannot have any items in it but this can be changed in the future
		Cargo cargo = fireplace_in_hands.GetInventory().GetCargo();
		for ( int i = 0; i < cargo.GetItemCount(); i++ )
		{	
			EntityAI c = cargo.GetItem(i);
			InventoryLocation srcc = new InventoryLocation;
			if (c.GetInventory().GetCurrentInventoryLocation(srcc))
			{
				InventoryLocation dstc = new InventoryLocation;
				dstc.SetCargo(fireplace_indoor, c, srcc.GetIdx(), srcc.GetRow(), srcc.GetCol());
				player.ServerTakeEntityToTargetCargoEx(fireplace_indoor, c, srcc.GetIdx(), srcc.GetRow(), srcc.GetCol());
			}
		}
		
		//transfer all attachments to this object
		int item_attachments_count = fireplace_in_hands.GetInventory().AttachmentCount();
		for ( int j = 0; j < item_attachments_count; j++ )
		{
			EntityAI a = fireplace_in_hands.GetInventory().GetAttachmentFromIndex( 0 );
			InventoryLocation srca = new InventoryLocation;
			if (a.GetInventory().GetCurrentInventoryLocation(srca))
			{
				InventoryLocation dsta = new InventoryLocation;
				dsta.SetAttachment(fireplace_indoor, a, srca.GetSlot());
				player.ServerTakeEntityToTargetAttachmentEx(fireplace_indoor, a, srca.GetSlot());
			}
		}
		
		//set building settings (m_FireplacePoints) and fireplaceindoor settings
		m_ActualFireplacePoint.SetObject( fireplace_indoor );
		fireplace_indoor.SetFireplacePoint( m_ActualFireplacePoint );

		player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
}