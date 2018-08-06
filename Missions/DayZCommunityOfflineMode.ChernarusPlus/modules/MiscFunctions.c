class MiscFunctions extends Module 
{
	bool IsHudVisible = true;
	
	void MiscFunctions( CommunityOfflineMode m_Mission )
	{
	}

	void ~MiscFunctions()
	{
	}
	
	override void Init() 
	{
		super.Init();
	}
	
	override void onUpdate( int timeslice )
	{
	}
	
	override void RegisterKeyMouseBindings() 
	{
	}
	
	void ToggleCursor() 
	{
		if ( GetGame().GetInput().HasGameFocus(INPUT_DEVICE_MOUSE) ) 
		{
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_MOUSE);
			GetGame().GetUIManager().ShowUICursor(true);	
		} 
		else 
		{
			GetGame().GetUIManager().ShowUICursor(false);
			GetGame().GetInput().ResetGameFocus(INPUT_DEVICE_MOUSE);
		}
	}
	
	void ToggleUI() 
	{
		//IsHudVisible = !IsHudVisible;
		//m_Mission.GetHud().Show(IsHudVisible);
		
		// Camera ui test
		GetGame().GetUIManager().ShowScriptedMenu( new CameraSettings(), NULL );
	}
	
	void TeleportCursor() 
	{
		if ( CameraTool.Cast( m_Mission.GetModule(CameraTool) ).IsUsingCamera() ) 
		{
			GetPlayer().MessageStatus( "You can not teleport while you are inside the freecam!" );
			return;
		}

		vector hitPos = GetCursorPos();

		float distance = vector.Distance( GetPlayer().GetPosition(), hitPos );
		
		if ( distance < 5000 )
		{
			EntityAI oVehicle = GetPlayer().GetDrivingVehicle();

			if( oVehicle )
			{
				GetPlayer().MessageStatus("Get out of the vehicle first!");
			}
			else
			{
				GetPlayer().SetPosition( hitPos );
				GetPlayer().MessageStatus("Teleported!");
			}
		}
		else
		{
			GetPlayer().MessageStatus( "Distance for teleportation is too far!" );
		}
	}
	
	void Reload() 
	{
		EntityAI oWeapon = GetPlayer().GetHumanInventory().GetEntityInHands();
				
		if( oWeapon )
		{
			Magazine oMag = ( Magazine ) oWeapon.GetAttachmentByConfigTypeName( "DefaultMagazine" );
			
			if( oMag && oMag.IsMagazine() )
			{
				oMag.LocalSetAmmoMax();
			}					
			
			Object oSupressor = ( Object ) oWeapon.GetAttachmentByConfigTypeName( "SuppressorBase" );
			
			if( oSupressor )
			{
				oSupressor.SetHealth( oSupressor.GetMaxHealth( "", "" ) );
			}
		}
	}
	
}