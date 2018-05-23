class PositionManager extends UIScriptedMenu
{
	protected TextListboxWidget m_LstPositionList;
	protected EditBoxWidget m_TxtSelectedX;
	protected EditBoxWidget m_TxtSelectedY;
	protected EditBoxWidget m_TxtCurrentX;
	protected EditBoxWidget m_TxtCurrentY;
	protected ButtonWidget m_TeleportButton;
	protected ButtonWidget m_CancelButton;

	protected autoptr map< string, vector > m_oPositions = new map< string, vector >;

    protected bool m_bOverCurrentPos;

	void PositionManager()
	{
	    m_bOverCurrentPos = false;

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
	}

	void ~PositionManager()
	{
	}

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\gui\\PositionManager.layout" );

		m_LstPositionList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("tls_ppp_pm_positions_list") );
		m_TxtSelectedX = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_selected_x_value") );
		m_TxtSelectedY = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_selected_y_value") );
		m_TxtCurrentX = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_current_x_value") );
		m_TxtCurrentY = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_current_y_value") );
		m_TeleportButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_ppp_pm_teleport") );
		m_CancelButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_ppp_pm_cancel") );

        for ( int nPosition = 0; nPosition < m_oPositions.Count(); nPosition++ )
        {
            m_LstPositionList.AddItem( m_oPositions.GetKey( nPosition ), NULL, 0 );
        }

		return layoutRoot;
	}

	override void OnShow()
	{
	    super.OnShow();

        vector player_pos = GetGame().GetPlayer().GetPosition();

        m_TxtCurrentX.SetText( player_pos[0].ToString() );
		m_TxtCurrentY.SetText( player_pos[2].ToString() );
	}

	override void OnHide()
	{
		super.OnHide();
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
        if ( w == m_TxtCurrentX || w == m_TxtCurrentY )
        {
            m_bOverCurrentPos = true;
        }

		return false;
	}

	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
        if ( w == m_TxtCurrentX || w == m_TxtCurrentY )
        {
            m_bOverCurrentPos = false;
        }

		return false;
	}

	override bool OnKeyPress( Widget w, int x, int y, int key )
	{
		if ( m_bOverCurrentPos )
		{
            m_TxtSelectedX.SetText( "" );
            m_TxtSelectedY.SetText( "" );
		}

        if( key == KeyCode.KC_ESCAPE )
        {
            GetGame().GetUIManager().Back();
        }

		return false;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_TeleportButton )
		{
		    float pos_x = 0;
            float pos_y = 0;

		    if( ( m_TxtSelectedX.GetText() != "" ) && ( m_TxtSelectedY.GetText() != "" ) )
		    {
	            pos_x = m_TxtSelectedX.GetText().ToFloat();
                pos_y = m_TxtSelectedY.GetText().ToFloat();
		    }
		    else if( ( m_TxtCurrentX.GetText() != "" ) && ( m_TxtCurrentY.GetText() != "" ) )
		    {
                pos_x = m_TxtCurrentX.GetText().ToFloat();
                pos_y = m_TxtCurrentY.GetText().ToFloat();
		    }
		    else
		    {
		        PlayerBase oPlayer = GetGame().GetPlayer();
		        oPlayer.MessageStatus( "Invalid teleportation position!" );

		        return true;
		    }

			vector vPlayerPos;
			vPlayerPos[0] = pos_x;
			vPlayerPos[2] = pos_y;

			vPlayerPos = SnapToGround( vPlayerPos );

			GetGame().GetPlayer().SetPosition( vPlayerPos );

			GetGame().GetUIManager().Back();
			return true;
		}
		else if ( w == m_CancelButton )
		{
			GetGame().GetUIManager().Back();
			return true;
		}

		return false;
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
		vector position = "0 0 0";

		if( !m_oPositions.Find( GetCurrentPositionName(), position ) )
		{
			position = "0 0 0";
		}

		m_TxtSelectedX.SetText( position[0].ToString() );
		m_TxtSelectedY.SetText( position[2].ToString() );

		return true;
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

    vector SnapToGround(vector pos)
    {
        float pos_x = pos[0];
        float pos_z = pos[2];
        float pos_y = GetGame().SurfaceY( pos_x, pos_z );
        vector tmp_pos = Vector( pos_x, pos_y, pos_z );
        tmp_pos[1] = tmp_pos[1] + pos[1];

        return tmp_pos;
    }
	
	
	TVectorArray PlayerSpawnPositions() 
	{
		
		return { "15135.1 0 13901.1", "15017.8 0 13892.4", "14887.1 0 14547.9", "14749.7 0 13248.7",
		"14697.6 0 13418.4", "14537.3 0 14755.7", "14415.3 0 14025.2", "14338.0 0 12859.5",
		"14263.8 0 12748.7", "14172.2 0 12304.9", "14071.4 0 12033.3", "14054.9 0 11341.3",
		"14017.8 0 2959.1", "13905.5 0 12489.7", "13852.4 0 11686.0", "13846.6 0 12050.0",
		"13676.0 0 12262.1", "13617.4 0 12759.8", "13610.1 0 11223.6", "13594.3 0 4064.0",
		"13587.8 0 6026.5", "13571.1 0 3056.8", "13552.6 0 4653.7", "13529.9 0 3968.3",
		"13520.8 0 4223.7", "13504.0 0 5004.5", "13476.7 0 6136.3", "13441.6 0 5262.2",
		"13426.6 0 5747.3", "13416.8 0 11840.4", "13400.8 0 4120.7", "13395.8 0 5902.8",
		"13385.0 0 3946.6", "13374.4 0 6454.3", "13367.1 0 10837.1", "13366.3 0 4906.0",
		"13337.1 0 5120.8", "13326.7 0 5489.1", "13312.7 0 6771.1", "13288.7 0 11415.1",
		"13261.6 0 11785.2", "13171.6 0 6534.8", "13159.8 0 5401.7", "13155.2 0 5475.2",
		"13084.9 0 7938.6", "13056.8 0 4848.5", "13048.1 0 8357.6", "13048.1 0 3867.7",
		"12991.7 0 7287.1", "12983.0 0 5539.1", "12978.9 0 9727.8", "12950.2 0 5226.7",
		"12942.1 0 8393.1", "12891.5 0 3673.9", "12628.7 0 10495.2", "12574.3 0 3592.8",
		"12566.3 0 6682.6", "12465.2 0 8009.0", "12354.5 0 3480.0", "13262.8 0 7225.8" };
		
	};
}