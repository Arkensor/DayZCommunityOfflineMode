class UIPopupScriptPositionManager extends UIPopupScript
{
	private TextListboxWidget m_LstPositionList;
	private EditBoxWidget m_TxtSelectedX;
	private EditBoxWidget m_TxtSelectedY;
	private EditBoxWidget m_TxtCurrentX;
	private EditBoxWidget m_TxtCurrentY;
	private PluginConfigDebugProfileFixed m_ConfigDebugProfileFixed;
	private ButtonWidget m_TeleportButton;
	private ButtonWidget m_CancelButton;
	
	autoptr map< string, vector > m_oPositions = new map< string, vector >;
	
	//================================================
	// UIPopupScriptPositionManager
	//================================================	
	void UIPopupScriptPositionManager( Widget wgt )
	{
		m_ConfigDebugProfileFixed	= PluginConfigDebugProfileFixed.Cast( GetPlugin( PluginConfigDebugProfileFixed ) );
		m_LstPositionList = TextListboxWidget.Cast( wgt.FindAnyWidget("tls_ppp_pm_positions_list") );
		m_TxtSelectedX = EditBoxWidget.Cast( wgt.FindAnyWidget("pnl_ppp_pm_selected_x_value") );
		m_TxtSelectedY = EditBoxWidget.Cast( wgt.FindAnyWidget("pnl_ppp_pm_selected_y_value") );
		m_TxtCurrentX = EditBoxWidget.Cast( wgt.FindAnyWidget("pnl_ppp_pm_current_x_value") );
		m_TxtCurrentY = EditBoxWidget.Cast( wgt.FindAnyWidget("pnl_ppp_pm_current_y_value") );
		m_TeleportButton = ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_pm_teleport") );
		m_CancelButton = ButtonWidget.Cast( wgt.FindAnyWidget("btn_ppp_pm_cancel") );

		// m_ConfigDebugProfileFixed.GetAllPositionsNames( positions_array );
		
		m_oPositions.Insert( "Severograd", "8428.0 0.0 12767.1" );
		m_oPositions.Insert( "Krasnostav", "11172.0 0.0 12314.1" );
		m_oPositions.Insert( "Mogilevka", "7583.0 0.0 5159.4" );
		m_oPositions.Insert( "Stary Sobor", "6072.0 0.0 7852.5" );
		m_oPositions.Insert( "Msta", "11207.0 0.0 5380.54" );
		m_oPositions.Insert( "Vybor", "3729.51 0.0 8935.56" );
		m_oPositions.Insert( "Gorka", "9787.1 0.0 8767.19" );
		m_oPositions.Insert( "Solnicni", "13393.1 0.0 6159.8" );
		m_oPositions.Insert( "Airport Vybor", "4828.9 0.0 10219.5" );
		m_oPositions.Insert( "Airport Balota", "4791.53 0.0 2557.47" );
		m_oPositions.Insert( "Airport Krasnostav", "12004.3 0.0 12655.3" );
		m_oPositions.Insert( "Chernogorsk Center", "6893.07 0.0 2618.13" );
		m_oPositions.Insert( "Chernogorsk West", "6546.03 0.0 2317.56" );
		m_oPositions.Insert( "Chernogorsk East", "7105.76 0.0 2699.53" );
		m_oPositions.Insert( "Elektrozavodsk Center", "10459.8 0.0 2322.72" );
		m_oPositions.Insert( "Elektrozavodsk East", "10414.5 0.0 2569.42" );
		m_oPositions.Insert( "Berezino Center", "12296.9 0.0 9470.51" );
		m_oPositions.Insert( "Berezino West", "12035.4 0.0 9143.49" );
		m_oPositions.Insert( "Berezino East", "12932.3 0.0 10172.7" );
		m_oPositions.Insert( "Svetlojarsk Center", "13835.3 0.0 13202.3" );
		m_oPositions.Insert( "Zelenogorsk Center", "2660.99 0.0 5299.28" );
		m_oPositions.Insert( "Zelenogorsk West", "2489.45 0.0 5080.41" );
		
		for ( int i = 0; i < m_oPositions.Count(); i++ )
		{
			m_LstPositionList.AddItem( m_oPositions.GetKey(i), NULL, 0); //new PresetParams ( m_oPositions.GetKey(i), true, false)
		}
	}
	
	//================================================
	// OnOpen
	//================================================	
	override void OnOpen( Param param )
	{		
		vector player_pos = GetGame().GetPlayer().GetPosition();
		
		m_TxtSelectedX.SetText( player_pos[0].ToString() );
		m_TxtSelectedY.SetText( player_pos[2].ToString() );
	}
	
	vector SnapToGround(vector pos)
	{
		float pos_x = pos[0];
		float pos_z = pos[2];
		float pos_y = GetGame().SurfaceY(pos_x, pos_z);
		vector tmp_pos = Vector(pos_x, pos_y, pos_z);
		tmp_pos[1] = tmp_pos[1] + pos[1];
	
		return tmp_pos;
	}

	//================================================
	// OnClick
	//================================================
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_TeleportButton )
		{
			float pos_x = m_TxtSelectedX.GetText().ToFloat();
			float pos_y = m_TxtSelectedY.GetText().ToFloat();
			
			vector vPlayerPos;
			vPlayerPos[0] = pos_x;
			vPlayerPos[2] = pos_y;
			
			vPlayerPos = SnapToGround( vPlayerPos );
			
			GetGame().GetPlayer().SetPosition( vPlayerPos );
			
			PopupBack();
			return true;
		}
		else if ( w == m_CancelButton )
		{
			PopupBack();
			return true;
		}
		return false;
	}
	
	void OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{	
		vector position = "0 0 0";
		
		if( !m_oPositions.Find( GetCurrentPositionName(), position ) )
		{
			position = "0 0 0";
		}

		m_TxtSelectedX.SetText( position[0].ToString() );
		m_TxtSelectedY.SetText( position[2].ToString() );
	}
	
	string GetCurrentPositionName()
	{
		if ( m_LstPositionList.GetSelectedRow() != -1 )
		{
			string position_name;
			m_LstPositionList.GetItemText( m_LstPositionList.GetSelectedRow(), 0, position_name );
			return position_name;
		}
		return "";
	}
}
