class DeveloperTeleport
{
	protected static const int TELEPORT_DISTANCE_MAX = 1000;
	
	static void TeleportAtCursor()
	{
		if ( !DeveloperFreeCamera.IsFreeCameraEnabled() )
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			vector pos_player = player.GetPosition();
			
			vector rayStart = GetGame().GetCurrentCameraPosition();
			vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 1000;		
			vector hitPos;
			vector hitNormal;
			int hitComponentIndex;		
			DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, NULL, NULL, player);
				
			float distance = vector.Distance( pos_player, hitPos );
			
			if ( distance < TELEPORT_DISTANCE_MAX )
			{
				SetPlayerPosition(PlayerBase.Cast( GetGame().GetPlayer() ), hitPos);
			}
			else
			{
				Debug.LogWarning("Distance for teleportation is too far!");
			}				
		}
	}
	
	// Set Player Position (MP support)
	static void SetPlayerPosition(PlayerBase player, vector position)
	{
		if ( GetGame().IsServer() )
		{
			player.SetPosition(position);
		}
		else
		{
			ref Param3<float, float, float> params = new Param3<float, float, float>(position[0], position[1], position[2]);
			player.RPCSingleParam(ERPCs.DEV_RPC_TELEPORT, params, true);
		}
	}
	
	// Set Player Direction (MP support)
	static void SetPlayerDirection(PlayerBase player, vector direction)
	{
		if ( GetGame().IsServer() )
		{
			player.SetDirection(direction);
		}
		else
		{
			ref Param3<float, float, float> params = new Param3<float, float, float>(direction[0], direction[1], direction[2]);
			player.RPCSingleParam(ERPCs.DEV_RPC_SET_PLAYER_DIRECTION, params, true);
		}
	}
	
	static void OnRPC(PlayerBase player, int rpc_type, ParamsReadContext ctx)
	{
		if ( !GetGame().IsDebug() )
		{
			return;
		}
	
		if ( rpc_type == ERPCs.DEV_RPC_TELEPORT )
		{
			OnRPCSetPlayerPosition(player, ctx);
		}
		else if ( rpc_type == ERPCs.DEV_RPC_SET_PLAYER_DIRECTION )
		{
			OnRPCSetPlayerDirection(player, ctx);
		}
	}
	
	static protected void OnRPCSetPlayerPosition(PlayerBase player, ParamsReadContext ctx)
	{
		ref Param3<float, float, float> p = new Param3<float, float, float>(0, 0, 0);
		if (ctx.Read(p))
		{
			vector v = "0 0 0";
			v[0] = p.param1;
			v[1] = p.param2;
			v[2] = p.param3;
			SetPlayerPosition(player, v);
		}
	}
	
	static protected void OnRPCSetPlayerDirection(PlayerBase player, ParamsReadContext ctx)
	{
		ref Param3<float, float, float> p = new Param3<float, float, float>(0, 0, 0);
		if (ctx.Read(p))
		{
			vector v = "0 0 0";
			v[0] = p.param1;
			v[1] = p.param2;
			v[2] = p.param3;
			SetPlayerDirection(player, v);
		}
	}
}