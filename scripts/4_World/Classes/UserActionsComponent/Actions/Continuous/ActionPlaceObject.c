class ActionPlaceObjectCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_PLACE);
	}
};

class ActionPlaceObject: ActionContinuousBase
{
	//SOFT SKILLS
	private const float ROUGH_SPECIALTY_WEIGHT = -0.01;
		
	void ActionPlaceObject()
	{
		m_CallbackClass		= ActionPlaceObjectCB;
		m_CommandUID		= DayZPlayerConstants.CMD_ACTIONFB_BERRIES;
		m_FullBody			= true;
		m_StanceMask		= DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;

		m_SpecialtyWeight 	= ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_PLACE_OBJECT;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "Place object";
	}
	
	override bool Can( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		//Client
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			if ( player.IsPlacingLocal() )
			{		
				if ( !player.GetHologramLocal().IsColliding() )
				{
					if ( item.CanBePlaced(player, player.GetHologramLocal().GetProjectionEntity().GetPosition()) )
					{
						return true;
					}
				}
			}
			return false;
		}
		//Server
		return true; 
	}
			
	override void Start( PlayerBase player, ActionTarget target, ItemBase item ) //Setup on start of action
	{
		super.Start(player, target, item);	
		
		if( GetGame().IsMultiplayer() )
		{
			if( GetGame().IsServer() )
			{
				player.PlacingStartServer();
			}
			else
			{
				player.PlacingCompleteLocal();
			}
		}
		else
		{	
			//local singleplayer
			player.GetHologramLocal().SetUpdatePosition( false );
		}
	}
		
	override void OnCancelClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata  )
	{
		//local singleplayer
		player.PlacingCancelLocal();
	}
		
	override void OnInterruptClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata  )
	{
		//local singleplayer
		player.PlacingCancelLocal();
	}
	
	override void OnCancelServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata  )
	{
		if( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{	
			player.PlacingCancelServer();
		}
		else
		{
			//local singleplayer
			player.PlacingCancelLocal();
		}
	}
		
	override void OnInterruptServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata  )
	{
		player.PlacingCancelServer();
		
		if( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{	
			player.PlacingCancelServer();
		}
		else
		{
			//local singleplayer
			player.PlacingCancelLocal();
		}
	}

	override void OnCompleteLoopServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		if( !GetGame().IsMultiplayer() && GetGame().IsServer() )
		{	
			//local singleplayer
			EntityAI entity_for_placing = EntityAI.Cast( item );
			vector position = player.GetHologramLocal().GetProjectionPosition();
			vector orientation = player.GetHologramLocal().GetProjectionOrientation();
			vector rotation_matrix[3];
			float direction[4];
			InventoryLocation source = new InventoryLocation;
			InventoryLocation destination = new InventoryLocation;
			
			Math3D.YawPitchRollMatrix( orientation, rotation_matrix );
			Math3D.MatrixToQuat( rotation_matrix, direction );
			
			if (entity_for_placing.GetInventory().GetCurrentInventoryLocation( source ))
			{
				destination.SetGroundEx( entity_for_placing, position, direction );
				entity_for_placing.LocalTakeToDst( source, destination );
			}
		}
	}
	
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		EntityAI entity_for_placing = EntityAI.Cast( item );

		if( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{	
			player.GetHologramServer().PlaceEntity( entity_for_placing, player.GetLocalProjectionPosition(), player.GetLocalProjectionOrientation() );
			player.GetHologramServer().CheckPowerSource();
			player.PlacingCompleteServer();
			entity_for_placing.OnPlacementComplete( player );
			player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		}
		else
		{
			//local singleplayer
			player.GetHologramLocal().PlaceEntity( entity_for_placing, player.GetHologramLocal().GetProjectionPosition(), player.GetHologramLocal().GetProjectionOrientation() );
			player.PlacingCompleteLocal();
			entity_for_placing.OnPlacementComplete( player );
			player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		}
	}

	override void OnCompleteLoopClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		EntityAI entity_for_placing = EntityAI.Cast( item );
		vector position = player.GetLocalProjectionPosition();
		vector orientation = player.GetLocalProjectionOrientation();
		vector rotation_matrix[3];
		float direction[4];
		InventoryLocation source = new InventoryLocation;
		InventoryLocation destination = new InventoryLocation;
		
		Math3D.YawPitchRollMatrix( orientation, rotation_matrix );
		Math3D.MatrixToQuat( rotation_matrix, direction );
		
		if (entity_for_placing.GetInventory().GetCurrentInventoryLocation( source ))
		{
			destination.SetGroundEx( entity_for_placing, position, direction );
			entity_for_placing.PredictiveTakeToDst( source, destination );
		}
	}
	
	override void OnCompleteClient( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		EntityAI entity_for_placing = EntityAI.Cast( item );

		entity_for_placing.OnPlacementComplete( player );
	}
	
	override void WriteToContext(ParamsWriteContext ctx,ActionTarget target)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		vector player_position = player.GetHologramLocal().GetProjectionPosition();
		vector player_orientation = player.GetHologramLocal().GetProjectionOrientation();
		ctx.Write( player_position );
		ctx.Write( player_orientation );
		
		player.SetLocalProjectionPosition( player_position );
		player.SetLocalProjectionOrientation( player_orientation );
	}
	override bool ReadFromContext(ParamsReadContext ctx, out ActionReceived actionReceived)
	{
		vector entity_position = "0 0 0";
		vector entity_orientation = "0 0 0";
		if (!ctx.Read(entity_position))
			return false;
		if (!ctx.Read(entity_orientation))
			return false;
		
		actionReceived.entity_position = entity_position;
		actionReceived.entity_orientation = entity_orientation;
		
		return true;
	}
};
