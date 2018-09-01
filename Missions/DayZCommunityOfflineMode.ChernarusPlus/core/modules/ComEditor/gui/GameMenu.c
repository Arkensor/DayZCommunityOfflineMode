class FunctionParam 
{
	string function;

	void FunctionParam( string param ) 
	{
		function = param;
	}
}

class GameMenu extends PopupMenu
{
	TextListboxWidget m_gameScriptList;
	Widget 			  m_checkboxPanel;
	ButtonWidget 	  m_gameScriptButton;

	protected ref array< CheckBoxWidget > checkBoxList = new array< CheckBoxWidget >;

	string checkboxLayout = "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\gui\\layouts\\CheckboxTemplate.layout";

	// Move these vars to own options object
	bool m_GodMode;
	bool m_OldAiming;

	void GameMenu()
	{
		GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Insert( Update );
	}

	void ~GameMenu()
	{
		GetGame().GetUpdateQueue( CALL_CATEGORY_SYSTEM ).Remove( Update );
	}

	override void Init()
	{
		m_checkboxPanel = layoutRoot.FindAnyWidget("game_checkbox_panel");
		//m_gameScriptList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("game_list_box"));
		m_gameScriptButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("game_script_button"));

		//m_gameScriptList.AddItem("Toggle Old Aiming", new FunctionParam("ToggleOldAiming"), 0);

		CheckBoxWidget checkBoxGodmode = CheckBoxWidget.Cast(GetGame().GetWorkspace().CreateWidgets( checkboxLayout, m_checkboxPanel ));
		checkBoxGodmode.SetUserData( new FunctionParam( "ToggleGodMode" ));
		checkBoxGodmode.SetText("Godmode");

		CheckBoxWidget checkBoxAiming = CheckBoxWidget.Cast(GetGame().GetWorkspace().CreateWidgets( checkboxLayout, m_checkboxPanel ));
		checkBoxAiming.SetUserData( new FunctionParam( "ToggleOldAiming" ));
		checkBoxAiming.SetText("Old Aiming");

		checkBoxList.Insert( checkBoxGodmode );
		checkBoxList.Insert( checkBoxAiming );
	}

	override void OnShow()
	{
		// Update checkbox checked
		UpdateCheckBoxes();
	}

	override void OnHide() 
	{
		
	}

	void UpdateCheckBoxes() 
	{
		foreach( CheckBoxWidget checkbox : checkBoxList ) 
		{
			FunctionParam function
			checkbox.GetUserData( function ); // TODO come back to this fix crashing. ugh kms

			bool checked = false; 
			if ( function ) 
			{
				GetPlayer().MessageStatus( function.ToString() );
				// GetGame().GameScript.CallFunction( this , funcParam.param1 , NULL, checkbox );
			}
			checkbox.SetChecked( checked );
		}
	}

	void Update() 
	{
		if ( m_GodMode ) 
		{
			GetPlayer().SetAllowDamage( false );

			GetPlayer().SetHealth( GetPlayer().GetMaxHealth( "", "" ) );
			GetPlayer().SetHealth( "","Blood", GetPlayer().GetMaxHealth( "", "Blood" ) );
			GetPlayer().SetHealth( "","Shock", GetPlayer().GetMaxHealth( "", "Shock" ) );
			GetPlayer().SetStamina(1000, 1000);
			GetPlayer().GetStatStamina().Set(1000);
			GetPlayer().GetStatEnergy().Set(1000);
			GetPlayer().GetStatWater().Set(1000);
			GetPlayer().GetStatStomachSolid().Set(300);		
			GetPlayer().GetStatStomachWater().Set(300);
			GetPlayer().GetStatStomachEnergy().Set(300);
			GetPlayer().GetStatHeatComfort().Set(0);
			
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

	bool ToggleOldAiming( CheckBoxWidget widget )
	{
		if ( widget ) // Temp work around. I'm lazy xd
		{
			m_OldAiming = widget.IsChecked();

			GetPlayer().OverrideShootFromCamera( m_OldAiming );
		} 
		else 
		{
			return m_OldAiming;
		}

		return false;
	}

	bool ToggleGodMode( CheckBoxWidget widget ) 
	{
		if ( widget ) 
		{
			m_GodMode = widget.IsChecked(); 

			GetPlayer().SetAllowDamage( !m_GodMode );
		}
		else 
		{
			return m_GodMode;
		}

		return false;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{

		FunctionParam param;

		if ( w == m_gameScriptButton ) 
		{
			int selectRow = m_gameScriptList.GetSelectedRow();

			m_gameScriptList.GetItemData( selectRow, 0, param );

			if ( param ) 
			{
				GetGame().GameScript.CallFunction( this , param.function , NULL, 0 );
			}
		}

		if ( w.IsInherited( CheckBoxWidget ) ) 
		{
			w.GetUserData( param );

			if ( param ) 
			{
				GetGame().GameScript.CallFunction( this , param.function , NULL, w );
			}
			//CheckBoxWidget checkBox = CheckBoxWidget.Cast( checkBox );
		}

		return false;
	}
}