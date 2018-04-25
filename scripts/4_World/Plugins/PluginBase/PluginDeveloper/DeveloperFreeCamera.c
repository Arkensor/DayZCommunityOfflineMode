class DeveloperFreeCamera
{
	static void OnInit()
	{
	}
	
	//! Enable / Disable Free camera (Fly mod) - disable of camera will teleport player at current free camera position.
	static void FreeCameraToggle( PlayerBase player, bool teleport_player = false )
	{
		if ( !IsFreeCameraEnabled() )
		{
			EnableFreeCamera( player );
		}
		else 
		{
			DisableFreeCamera( player, teleport_player );
		}
	}
	
	static void EnableFreeCameraSceneEditor( PlayerBase player )
	{
		EnableFreeCamera(player);	
	}
	
	//! Is free camera enabled
	static bool IsFreeCameraEnabled()
	{
		return FreeDebugCamera.GetInstance().IsActive();
	}
	
	//---------------------------------------------
	// EnableFreeCamera
	//---------------------------------------------
	protected static void EnableFreeCamera( PlayerBase player )
	{
		if ( player )
		{
			FreeDebugCamera.GetInstance().SetActive(true);

			GetGame().GetMission().RefreshCrosshairVisibility();
			//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().GetMission(), "RefreshCrosshairVisibility", NULL);
		}
		else
		{
			Print("EnableFreeCamera( PlayerBase player ) -> player is NULL");
			Debug.LogError("EnableFreeCamera( PlayerBase player ) -> player is NULL", "DeveloperFreeCamera");
		}
	}

	//---------------------------------------------
	// DisableFreeCamera
	//---------------------------------------------
	protected static void DisableFreeCamera( PlayerBase player, bool teleport_player = false )
	{
		if ( player )
		{
			if ( FreeDebugCamera.GetInstance().IsActive() )
			{
				if ( teleport_player )
				{
					vector from = FreeDebugCamera.GetInstance().GetPosition(); 
					vector to = from + ( Vector(0,-1,0) * 10000 );   
					vector contact_pos;   
					vector contact_dir;   
					int contact_component; 
					
					if ( DayZPhysics.RaycastRV(from, to, contact_pos, contact_dir, contact_component) )   
					{
						DeveloperTeleport.SetPlayerPosition( player, contact_pos );
						DeveloperTeleport.SetPlayerDirection( player, FreeDebugCamera.GetInstance().GetDirection() );
					}
				}
				
				FreeDebugCamera.GetInstance().SetActive(false);

				if ( GetGame().GetMission() )
				{
					GetGame().GetMission().RefreshCrosshairVisibility();
				}
				//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().GetMission(), "RefreshCrosshairVisibility", NULL);
			}
		}
		else
		{
			Print("DisableFreeCamera( PlayerBase player, bool teleport_player ) -> player is NULL");
			Debug.LogError("DisableFreeCamera( PlayerBase player, bool teleport_player ) -> player is NULL", "DeveloperFreeCamera");
		}
	}
	
	//---------------------------------------------
	// OnSetFreeCameraEvent
	//---------------------------------------------
	static void OnSetFreeCameraEvent( PlayerBase player, FreeDebugCamera camera )
	{
		if ( player )
		{
			if ( !GetGame().IsMultiplayer() )
			{
				PluginSceneManager plugin_scene_manager = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
				
				if ( plugin_scene_manager.IsOpened() )
				{
					plugin_scene_manager.EditorToggle();
				}
			}
			
			GetGame().GetMission().RefreshCrosshairVisibility();
			//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().GetMission(), "RefreshCrosshairVisibility", NULL);
		}
	}
}