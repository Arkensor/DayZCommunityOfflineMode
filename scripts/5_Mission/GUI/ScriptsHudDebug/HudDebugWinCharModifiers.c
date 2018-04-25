class DebugModifierData
{
	string m_Name;
	int m_ID;
	
	void DebugModifierData( string name, int id )
	{
		m_Name = name;
		m_ID = id;
	}
	
	string GetName()
	{
		return m_Name;
	}
	
	int GetID()
	{
		return m_ID;
	}
}

class HudDebugWinCharModifiers extends HudDebugWinBase
{	
	protected Widget										m_WgtModifiersContent;
	protected ref array<ref Widget>							m_ModifierWidgets;
	protected ref map<Widget, ref DebugModifierData>		m_ModifierWidgetData;
	protected PluginDeveloperSync m_PluginDeveloperSync;
	
	//============================================
	// HudDebugWinCharModifiers
	//============================================
	void HudDebugWinCharModifiers( Widget widget_root )
	{	
		m_WgtRoot = widget_root;
		m_WgtModifiersContent = Widget.Cast( m_WgtRoot.FindAnyWidget( "pnl_CharModifiers_Values" ) );
		m_ModifierWidgets = new array<ref Widget>;
		m_ModifierWidgetData = new map<Widget, ref DebugModifierData>;
		m_PluginDeveloperSync = PluginDeveloperSync.Cast( GetPlugin( PluginDeveloperSync ) );
	}

	void ~HudDebugWinCharModifiers()
	{	
		SetUpdate( false );
	}

	//============================================
	// GetWinType
	//============================================
	override int GetType()
	{
		return HudDebug.HUD_WIN_CHAR_MODIFIERS;
	}
	
	//============================================
	// Update
	//============================================
	void SetUpdate( bool state )
	{
		//Disable update on server (PluginDeveloperSync)
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		//if client, send RPC
		if ( GetGame().IsClient() )
		{
			ref Param1<bool> params = new Param1<bool>( state );
			if ( player )
			{
				player.RPCSingleParam( ERPCs.DEV_MODS_UPDATE, params, true );
			}
		}
		//else set directly
		else
		{
			if ( m_PluginDeveloperSync )
			{
				m_PluginDeveloperSync.EnableUpdate( state, ERPCs.DEV_MODS_UPDATE, player );	
			}
		}
	}

	override void Update()
	{
		super.Update();
		
		Refresh();
	}
	
	//============================================
	// Show / Hide
	//============================================
	override void Show()
	{
		super.Show();
		
		//Print("Show()");
		
		SetUpdate( true );
	}

	override void Hide()
	{
		super.Hide();
		
		//Print("Hide()");
		
		SetUpdate( false );
	}
	
	void Refresh()
	{
		SetModifiers();		
	}
	
	//============================================
	// FitWindow
	//============================================
	void FitWindow()
	{
		float title_size = 20;
		float spacing = 20;
		
		//get wgt content size values
		float wgt_content_size_x;
		float wgt_content_size_y;
		m_WgtModifiersContent.GetSize( wgt_content_size_x, wgt_content_size_y );
		
		//get wgt root size values
		float wgt_root_size_x;
		float wgt_root_size_y;
		m_WgtRoot.GetSize( wgt_root_size_x, wgt_root_size_y );
		
		//calculate new Y size
		float new_size_y = title_size + wgt_content_size_y + spacing;
		
		//set size
		m_WgtRoot.SetSize( wgt_root_size_x, new_size_y );
	}
	
	//============================================
	// Display Modifiers
	//============================================	
	void SetModifiers()
	{
		//clear window
		ClearModifiers();
		
		if ( m_PluginDeveloperSync.m_PlayerModsSynced.Count() > 0 )
		{
			//set active mods
			for ( int i = 0; i < m_PluginDeveloperSync.m_PlayerModsSynced.Count(); ++i )
			{
				SyncedValue synced_value = m_PluginDeveloperSync.m_PlayerModsSynced.Get( i );
				AddModifier( synced_value.GetName(), synced_value.GetValue(), synced_value.GetState() );
			}
		}
		
		FitWindow();
	}
	
	void AddModifier( string name, int id, bool state )
	{
		//create widget
		Widget widget = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/day_z_hud_debug_modifier.layout", m_WgtModifiersContent );
		
		//add to widget array (for clearing purposes)
		m_ModifierWidgets.Insert( widget );
		
		//set widget name
		TextWidget mod_name_text = TextWidget.Cast( widget.FindAnyWidget( "TextModifierName" ) );
		mod_name_text.SetText( name );
		if ( id > 0 )
		{
			mod_name_text.SetColor( ARGB( 255, 0, 255, 0 ) );
		}
		else
		{
			mod_name_text.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
		
		//set set data for interactive parts (modifier ID should be enough)
		DebugModifierData data = new DebugModifierData( name, id );
		//Activate button
		Widget activate_button = widget.FindAnyWidget( "ButtonActivate" );
		m_ModifierWidgetData.Insert( activate_button, data );
		
		//Deactivate button
		Widget deactivate_button = widget.FindAnyWidget( "ButtonDeactivate" );
		m_ModifierWidgetData.Insert( deactivate_button, data );

		//Lock checkbox
		Widget checkbox_widget = widget.FindAnyWidget( "CheckBoxLock" );
		m_ModifierWidgetData.Insert( checkbox_widget, data );
		//set lock based on checkbox value
		CheckBoxWidget checkbox = CheckBoxWidget.Cast( checkbox_widget );
		checkbox.SetChecked( state );
	}

	void ClearModifiers()
	{
		//clear widget data
		m_ModifierWidgetData.Clear();
		
		//destroy all modifier widgets
		for ( int i = 0; i < m_ModifierWidgets.Count(); ++i )
		{
			delete m_ModifierWidgets.Get( i );
		}
		m_ModifierWidgets.Clear();
	}
	
	//============================================
	// OnClick
	//============================================
	bool OnClick( Widget w, int x, int y, int button )
	{	
		if ( w )
		{
			//Button activate
			if ( w.GetName() == "ButtonActivate" )
			{
				DebugModifierData ba_data = m_ModifierWidgetData.Get( w );
				
				//activate
				ActivateModifier( ba_data.GetID() );
				
				//force update
				m_PluginDeveloperSync.Update();
				
				return true;
			}
			//Button deactivate
			else if ( w.GetName() == "ButtonDeactivate" )
			{
				DebugModifierData bd_data = m_ModifierWidgetData.Get( w );
				
				//deactivate
				DeactivateModifier( bd_data.GetID() );

				//force update
				m_PluginDeveloperSync.Update();
				
				return true;
			}
			//Lock checkbox
			else if ( w.GetName() == "CheckBoxLock" )
			{
				DebugModifierData lcb_data = m_ModifierWidgetData.Get( w );
				CheckBoxWidget checkbox = CheckBoxWidget.Cast( w );
				
				//set lock
				LockModifier( lcb_data.GetID(), checkbox.IsChecked() );
				
				//force update
				m_PluginDeveloperSync.Update();
				
				return true;
			}			
		}
		
		return false;
	}
	
	//============================================
	// Actions
	//============================================
	void ActivateModifier( int id )
	{
		//Disable update on server (PluginDeveloperSync)
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		//if client, send RPC
		if ( GetGame().IsClient() )
		{
			ref Param1<int> params = new Param1<int>( id );
			if ( player )
			{
				player.RPCSingleParam( ERPCs.DEV_RPC_MODS_ACTIVATE, params, true );
			}
		}
		//else set directly
		else
		{
			m_PluginDeveloperSync.ActivateModifier( id );
		}
	}
	
	void DeactivateModifier( int id )
	{
		//Disable update on server (PluginDeveloperSync)
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		//if client, send RPC
		if ( GetGame().IsClient() )
		{
			ref Param1<int> params = new Param1<int>( id );
			if ( player )
			{
				player.RPCSingleParam( ERPCs.DEV_RPC_MODS_DEACTIVATE, params, true );
			}
		}
		//else set directly
		else
		{
			m_PluginDeveloperSync.DeactivateModifier( id );
		}
	}
	
	void LockModifier( int id, bool state )
	{
		//Disable update on server (PluginDeveloperSync)
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		//if client, send RPC
		if ( GetGame().IsClient() )
		{
			ref Param2<int, bool> params = new Param2<int, bool>( id, state );
			if ( player )
			{
				player.RPCSingleParam( ERPCs.DEV_RPC_MODS_LOCK, params, true );
			}
		}
		//else set directly
		else
		{
			m_PluginDeveloperSync.LockModifier( id, state );
		}
	}
}
