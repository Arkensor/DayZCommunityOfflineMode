#include "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\gui\\EditorMenu.c"

class COMModule extends Module 
{
	bool IsHudVisible = true;
	
	void COMModule( CommunityOfflineMode m_Mission )
	{
		m_GUIWindow = new EditorMenu( this );
	}

	void ~COMModule()
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
		KeyMouseBinding toggleCursor 	= new KeyMouseBinding( GetModuleType() , "ToggleCursor"		, "[U]"			, "Toggles the cursor."   );
		KeyMouseBinding toggleUI     	= new KeyMouseBinding( GetModuleType() , "ToggleUI"			, "[HOME]"		, "Toggles the UI."       );
		KeyMouseBinding teleport	 	= new KeyMouseBinding( GetModuleType() , "TeleportCursor"	, "[T]"			, "Teleport to cursor position." );
		KeyMouseBinding reload       	= new KeyMouseBinding( GetModuleType() , "Reload"			, "[R]"			, "Instantly reloads mag.");
		KeyMouseBinding spawnSedan	 	= new KeyMouseBinding( GetModuleType() , "SpawnSedan"		, "[N]"			, "Spawns a sedan at cursor position." );
		KeyMouseBinding spawnWolf	 	= new KeyMouseBinding( GetModuleType() , "SpawnWolf"		, "[CTRL]+[O]"	, "Spawns a wolf at cursor position." );
		KeyMouseBinding spawnAnimal	 	= new KeyMouseBinding( GetModuleType() , "SpawnAnimal"		, "[SHIFT]+[O]"	, "Spawns a random animal at cursor position." );
		KeyMouseBinding spawnInfected	= new KeyMouseBinding( GetModuleType() , "SpawnInfected"	, "[O]"			, "Spawns a random infected at cursor position." );
		KeyMouseBinding openEditor   	= new KeyMouseBinding( GetModuleType() , "OpenEditor"		, "[Y]"			, "Opens the editor."     ); 
		
		toggleCursor	.AddKeyBind( KeyCode.KC_U,    KB_EVENT_RELEASE );
		toggleUI		.AddKeyBind( KeyCode.KC_HOME, KB_EVENT_RELEASE );
		openEditor		.AddKeyBind( KeyCode.KC_Y,    KB_EVENT_RELEASE );
		teleport		.AddKeyBind( KeyCode.KC_T,    KB_EVENT_PRESS   );
		spawnSedan		.AddKeyBind( KeyCode.KC_O,    KB_EVENT_RELEASE );
		spawnWolf		.AddKeyBind( KeyCode.KC_O,    KB_EVENT_PRESS   );
		spawnAnimal		.AddKeyBind( KeyCode.KC_O,    KB_EVENT_PRESS   );
		spawnInfected	.AddKeyBind( KeyCode.KC_O,    KB_EVENT_PRESS   );
		reload			.AddKeyBind( KeyCode.KC_R,    KB_EVENT_RELEASE );
		
		RegisterKeyMouseBinding( toggleCursor );
		RegisterKeyMouseBinding( toggleUI );
		RegisterKeyMouseBinding( openEditor );
		RegisterKeyMouseBinding( teleport );
		RegisterKeyMouseBinding( spawnSedan );
		RegisterKeyMouseBinding( spawnWolf );
		RegisterKeyMouseBinding( spawnAnimal );
		RegisterKeyMouseBinding( spawnInfected );
		RegisterKeyMouseBinding( reload );
	}
	
	void OpenEditor() 
	{
		ShowWindow( NULL );
	}

	void SpawnSedan()
	{
		TStringArray attArr = {
			"CivSedanWheel","CivSedanWheel","CivSedanWheel","CivSedanWheel",
			"CarBattery","CarRadiator","EngineBelt","SparkPlug","CivSedanHood",
			"CivSedanTrunk","CivSedanDoors_Driver","CivSedanDoors_CoDriver",
			"CivSedanDoors_BackLeft","CivSedanDoors_BackRight",
		}; 
		
		EntityAI oCar = EntityAI.Cast( GetGame().CreateObject( "CivilianSedan", GetCursorPos(), false, true ) );
		
		for (int j = 0; j < attArr.Count(); j++) { oCar.GetInventory().CreateAttachment( attArr.Get(j) ); }
		
		oCar.SetAllowDamage( false );
	}

	void SpawnWolf()
	{
		GetGame().CreateObject( "Animal_CanisLupus_Grey", GetCursorPos(), false, true );
	}

	void SpawnAnimal()
	{
		GetGame().CreateObject( GetRandomChildFromBaseClass( "cfgVehicles", "AnimalBase" ), GetCursorPos(), false, true );
	}
	
	void SpawnInfected()
	{
		GetGame().CreateObject( WorkingZombieClasses().GetRandomElement(), GetCursorPos(), false, true );
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
		if ( CameraModule.Cast( m_oManager.GetModule(CameraModule) ).IsUsingCamera() ) 
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