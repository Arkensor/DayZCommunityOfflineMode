class GameMenu extends PopupMenu
{
	TextListboxWidget m_gameScriptList;
	Widget 			  m_checkboxPanel;
	ButtonWidget 	  m_gameScriptButton;

	protected ref map< string, string > checkBoxMap = new map< string, string >; // store widget name

	string checkboxLayout = "$CurrentDir:missions\\CommunityOfflineModeCore\\core\\modules\\ComEditor\\gui\\layouts\\CheckboxTemplate.layout";

	override void Init()
	{
		m_checkboxPanel = layoutRoot.FindAnyWidget("game_checkbox_panel");
		m_gameScriptList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("game_list_box"));
		m_gameScriptButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("game_script_button"));

		m_gameScriptList.AddItem( "Spawn ADA 4x4",     	new Param1< string >( "SpawnOffroadHatchback" ),    0 );
		m_gameScriptList.AddItem( "Spawn Gunter", 		new Param1< string >( "SpawnHatchback_02" ), 		0 );
		m_gameScriptList.AddItem( "Spawn HMWWV", 	 	new Param1< string >( "SpawnOffroad_02" ), 	   		0 );
        m_gameScriptList.AddItem( "Spawn Olga",  		new Param1< string >( "SpawnCivilianSedan" ),    	0 );
        m_gameScriptList.AddItem( "Spawn Sarka",     	new Param1< string >( "SpawnSedan_02" ),     		0 );
        m_gameScriptList.AddItem( "Spawn Covered M3S",  new Param1< string >( "SpawnTruck_01_Covered" ),    0 );

		CheckBoxWidget checkBoxGodmode = CheckBoxWidget.Cast(GetGame().GetWorkspace().CreateWidgets( checkboxLayout, m_checkboxPanel ));
		checkBoxGodmode.SetName( "Godmode" );
		checkBoxGodmode.SetText( "Godmode" );

		checkBoxMap.Insert( checkBoxGodmode.GetName(), "ToggleGodMode" );
	}

	override void OnShow()
	{
		// Update checkbox checked
		UpdateCheckBoxes();
	}

	void UpdateCheckBoxes() 
	{
		foreach( string widgetName, string function : checkBoxMap ) 
		{
			Widget widget = m_checkboxPanel.FindWidget( widgetName );

			if ( widget.IsInherited( CheckBoxWidget )) 
			{
				CheckBoxWidget checkbox = CheckBoxWidget.Cast( widget );
				bool checked;
				GetGame().GameScript.CallFunction( this , function , checked, NULL );
				checkbox.SetChecked( checked );
			}
		}
	}

	bool ToggleOldAiming( CheckBoxWidget widget )
	{
		if ( widget ) // Temp work around. I'm lazy xd
		{
			m_COM_OldAiming = !m_COM_OldAiming;

			COM_GetPB().OverrideShootFromCamera( !m_COM_OldAiming );
		}
		return m_COM_OldAiming;
	}

	bool ToggleGodMode( CheckBoxWidget widget ) 
	{
		if ( widget ) 
		{
			m_COM_GodMode = !m_COM_GodMode;

			COM_GetPB().SetAllowDamage( !m_COM_GodMode );
		}
		return m_COM_GodMode;
	}

	bool ToggleLaser( CheckBoxWidget widget ) 
	{
		if ( widget ) 
		{
			COM_bc_Visible = !COM_bc_Visible;
		}
		return COM_bc_Visible;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		string param;
		Param1<string> param1;

		if ( w == m_gameScriptButton ) 
		{
			int selectRow = m_gameScriptList.GetSelectedRow();

			if ( selectRow == -1 ) return false;

			m_gameScriptList.GetItemData( selectRow, 0, param1 );

			if ( param1 ) 
			{
				GetGame().GameScript.CallFunction( this , param1.param1 , NULL, 0 );
			}
		}

		if ( w.IsInherited( CheckBoxWidget ) ) 
		{
			param = checkBoxMap.Get( w.GetName() );

			if ( param ) 
			{
				GetGame().GameScript.CallFunction( this , param , NULL, CheckBoxWidget.Cast( w ) );
			}
		}

		return false;
	}

	void SpawnOffroadHatchback() 
	{
		TStringArray attArr = {
		"HeadlightH7", "HeadlightH7",
		"CarBattery", "CarRadiator", "SparkPlug",
		"HatchbackHood","HatchbackTrunk", "HatchbackDoors_Driver", "HatchbackDoors_CoDriver",
		"HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "HatchbackWheel",
		};

		SpawnVehicle( "OffroadHatchback", attArr );
	}

	void SpawnHatchback_02()
	{
		TStringArray attArr = {
		"HeadlightH7", "HeadlightH7",
		"CarBattery", "CarRadiator", "SparkPlug",
		"Hatchback_02_Door_1_1","Hatchback_02_Door_1_2", "Hatchback_02_Door_2_1","Hatchback_02_Door_2_2","Hatchback_02_Trunk", "Hatchback_02_Hood",
		"Hatchback_02_Wheel", "Hatchback_02_Wheel",	"Hatchback_02_Wheel", "Hatchback_02_Wheel", "Hatchback_02_Wheel",
		};

		SpawnVehicle( "Hatchback_02", attArr );
	}

	void SpawnOffroad_02() 
	{
		TStringArray attArr = {
		"HeadlightH7", "HeadlightH7",
		"CarBattery", "GlowPlug", 
		"Offroad_02_Hood", "Offroad_02_Trunk", "Offroad_02_Door_1_1", "Offroad_02_Door_1_2", "Offroad_02_Door_2_1", "Offroad_02_Door_2_2",
        "Offroad_02_Wheel", "Offroad_02_Wheel", "Offroad_02_Wheel", "Offroad_02_Wheel", "Offroad_02_Wheel",
		};

		SpawnVehicle( "Offroad_02", attArr );
	}

	void SpawnCivilianSedan() 
	{
		TStringArray attArr = {
		"HeadlightH7", "HeadlightH7",
		"CarBattery", "CarRadiator", "SparkPlug",
		"CivSedanHood", "CivSedanTrunk", "CivSedanDoors_Driver","CivSedanDoors_CoDriver", "CivSedanDoors_BackLeft", "CivSedanDoors_BackRight",
		"CivSedanWheel", "CivSedanWheel", "CivSedanWheel", "CivSedanWheel", "CivSedanWheel",
		};

		SpawnVehicle( "CivilianSedan", attArr );
	}

	void SpawnSedan_02()
	{
		TStringArray attArr = {
		"HeadlightH7", "HeadlightH7",
		"CarBattery", "CarRadiator", "SparkPlug", "Sedan_02_Hood",
		"Sedan_02_Hood","Sedan_02_Trunk","Sedan_02_Door_1_1","Sedan_02_Door_2_1","Sedan_02_Door_1_2","Sedan_02_Door_2_2",
		"Sedan_02_Wheel","Sedan_02_Wheel","Sedan_02_Wheel","Sedan_02_Wheel","Sedan_02_Wheel",
		};

		SpawnVehicle( "Sedan_02", attArr );
	}

	void SpawnTruck_01_Covered() 
	{
		TStringArray attArr = {
		"HeadlightH7", "HeadlightH7",
		"TruckBattery",
		"Truck_01_Hood","Truck_01_Door_1_1","Truck_01_Door_2_1",
		"Truck_01_Wheel","Truck_01_Wheel", "Truck_01_Wheel","Truck_01_Wheel",
		"Truck_01_WheelDouble","Truck_01_WheelDouble", "Truck_01_WheelDouble","Truck_01_WheelDouble",
		};

		SpawnVehicle( "Truck_01_Covered", attArr );
	}

	void SpawnVehicle( string vehicle, TStringArray attachments) 
	{
		Car oCar = Car.Cast( GetGame().CreateObject( vehicle, COM_GetCursorPos(), false, false ) );

		for (int j = 0; j < attachments.Count(); j++) { oCar.GetInventory().CreateAttachment( attachments.Get(j) ); }

		oCar.Fill( CarFluid.FUEL, 1000 );
		oCar.Fill( CarFluid.OIL, 1000 );
		oCar.Fill( CarFluid.BRAKE, 1000 );
		oCar.Fill( CarFluid.COOLANT, 1000 );
	}
};
