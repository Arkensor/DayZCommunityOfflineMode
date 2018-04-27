class ActionDigGardenPlotCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(UATimeSpent.DIG_GARDEN);
	}
};

class ActionDigGardenPlot: ActionContinuousBase
{	
	private const float ROUGH_SPECIALTY_WEIGHT = 0.01;

	void ActionDigGardenPlot()
	{
		m_CallbackClass = ActionDigGardenPlotCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGSHOVEL;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		/*
		m_MessageStart = "Player started placing object.";
		m_MessageSuccess = "Player placed the object."
		m_MessageFail = "Player failed to place the object.";
		m_MessageCancel = "You canceled action.";
		m_MessageStartFail = "Failed to start action: Object collides with environment.";
		*/
		m_MessageFail = "Digging tile failed because tile is colliding with environment.";
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override bool Can ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//Client
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			if ( player.IsPlacingLocal() )
			{
				Hologram hologram = player.GetHologramLocal();
				GardenPlot_TO_BE_RELEASED item_GP;
				Class.CastTo(item_GP,  hologram.GetProjectionEntity() );	
				CheckSurfaceBelowGardenPlot(player, item_GP, hologram);
	
				if ( !hologram.IsColliding() )
				{
					if ( item.IsInherited(Shovel) )
					{
						m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGSHOVEL;
					}	
					else
					{
						m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGHOE;
					}
					return true;
				}
			}
			return false;
		}
		//Server
		return true;
	}

	void CheckSurfaceBelowGardenPlot(PlayerBase player, GardenPlot_TO_BE_RELEASED item_GP, Hologram hologram)
	{
		if ( item_GP.CanBePlaced(player, item_GP.GetPosition() )  )
		{
			if( item_GP.CanBePlaced(NULL, item_GP.CoordToParent(hologram.GetLeftCloseProjectionVector())) )
			{
				if( item_GP.CanBePlaced(NULL, item_GP.CoordToParent(hologram.GetRightCloseProjectionVector())) )
				{
					if( item_GP.CanBePlaced(NULL, item_GP.CoordToParent(hologram.GetLeftFarProjectionVector())) )
					{
						if( item_GP.CanBePlaced(NULL, item_GP.CoordToParent(hologram.GetRightFarProjectionVector())) )
						{
							hologram.SetIsCollidingGPlot( false );

							return;
						}
					}
				}
			}
		}
		
		hologram.SetIsCollidingGPlot( true );
		
	}
	
	override int GetType()
	{
		return AT_DIG_GARDEN_PLOT;
	}
	
	override bool HasTarget()
	{
		return false;
	}
		
	override string GetText()
	{
		return "Make garden plot";
	}

	override void Start( PlayerBase player, ActionTarget target, ItemBase item ) //Setup on start of action
	{
		super.Start(player, target, item);	
		
		if( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{	
			player.PlacingStartServer();
		}
		else
		{
			//local singleplayer
			player.GetHologramLocal().SetUpdatePosition( false );
		}		
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
	
	override void OnCompleteServer( PlayerBase player, ActionTarget target, ItemBase item, Param acdata )
	{	
		EntityAI entity_for_placing = EntityAI.Cast( item );

		if( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{	
			player.GetHologramServer().PlaceEntity( entity_for_placing, player.GetLocalProjectionPosition(), player.GetLocalProjectionOrientation() );
			player.PlacingCompleteServer();
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

	override void WriteToContext(ParamsWriteContext ctx,ActionTarget target)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		ctx.Write(player.GetHologramLocal().GetProjectionPosition());
		ctx.Write(player.GetHologramLocal().GetProjectionOrientation());
		
		player.PlacingCompleteLocal();
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
