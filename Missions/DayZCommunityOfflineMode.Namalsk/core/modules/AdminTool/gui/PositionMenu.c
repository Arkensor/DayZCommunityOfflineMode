class PositionMenu extends PopupMenu
{
	protected TextListboxWidget m_LstPositionList;
	protected EditBoxWidget m_TxtSelectedX;
	protected EditBoxWidget m_TxtSelectedY;
	protected EditBoxWidget m_TxtCurrentX;
	protected EditBoxWidget m_TxtCurrentY;
	protected ButtonWidget m_TeleportButton;
	protected ButtonWidget m_CancelButton;

	autoptr map< string, vector > Positions = new map< string, vector >;

    protected bool m_bOverCurrentPos;

	void PositionMenu()
	{
	    m_bOverCurrentPos = false;

		Positions.Insert( "Nemsk", "9043.1 9.31 10134.0" );
		Positions.Insert( "Jalovisko", "8177.9 16.12 10827.0" );
		Positions.Insert( "BK-T08", "3929.8 6.15 10010.0" );
		Positions.Insert( "Lubjansk", "4450.1 9.24 11238.0" );
		Positions.Insert( "BK-T07", "4414.0 20.94 10746" );
		Positions.Insert( "BK-M06", "4808.1 13.96 10848.0" );
		Positions.Insert( "Vorkuta", "6815.5 16.6 11306.0" );
		Positions.Insert( "Alakit Old Yard", "6153.5 36.43 10874.0" );
		Positions.Insert( "Alakit", "5766.0 16.86 10768.0" );
		Positions.Insert( "Sebjan Dam", "5707.3 22.05 9851.5" );
		Positions.Insert( "Sebjan Airfield", "6340.5 22.2 9342.2" );
		Positions.Insert( "Sebjan Factory East", "6481.0 16.45 9299.9" );
		Positions.Insert( "Tara Farcory", "7688.5 72.67 8724.1" );
		Positions.Insert( "Tara Hospital", "7302.3 84.83 7991.4" );
		Positions.Insert( "Tara Harbor", "7868.4 10.1 7715.9" );
		Positions.Insert( "Tara Rail Yard", "7671.7 42.05 7402.6" );
		Positions.Insert( "Tara", "7241.9 238.26 7026.8" );
		Positions.Insert( "BK-L16", "6004.3 20.3 6623.2" ); 
		Positions.Insert( "Sawmill", "7047.7 39.6 5823.5" );
		Positions.Insert( "Tara Bridge", "5913.0 41.0 5872.4" );
		Positions.Insert( "Brensk", "4350.5 5.03 4772.9" );
		Positions.Insert( "Brensk Factory", "4855.4 41.3 6230.7" );
		Positions.Insert( "Brensk Bridge", "4885.1 41.74 6082.0" );
		Positions.Insert( "Athena-2", "4980.0 47.7 6612.6" );
		Positions.Insert( "BK-L02", "3440.7 216.4 6706.1" );
		Positions.Insert( "C-130J Mohawk", "3180.1 117.31 7516.7" );
		Positions.Insert( "Athena-1", "3755.5 147.3 8243.6" );
		Positions.Insert( "Warehouses", "4686.5 23.63 8933.7" );
		Positions.Insert( "Sebjan Refugee Camp", "5170.5 74.13 8976.3" );
		Positions.Insert( "Sebjan Factory", "5817.3 43.99 8715.0" );
		Positions.Insert( "Sebjan", "5185.5 30.89 8535.1" );
		Positions.Insert( "Sebjan uranium Mine", "4792.9 50.648 8019.0" );
		Positions.Insert( "Athena Research institute", "4291.1 78.91 8049.2" );
		Positions.Insert( "Norinsk", "3919.5 50.44 7519.8" );
		Positions.Insert( "BK-L01", "4146.8 381.48 6573.7" );
	}

	void ~PositionMenu()
	{
	}

	void Init()
	{
		m_LstPositionList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("tls_ppp_pm_positions_list") );
		m_TxtSelectedX = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_selected_x_value") );
		m_TxtSelectedY = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_selected_y_value") );
		m_TxtCurrentX = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_current_x_value") );
		m_TxtCurrentY = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_current_y_value") );
		m_TeleportButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_ppp_pm_teleport") );
		m_CancelButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_ppp_pm_cancel") );

        for ( int nPosition = 0; nPosition < Positions.Count(); nPosition++ )
        {
            m_LstPositionList.AddItem( Positions.GetKey( nPosition ), NULL, 0 );
        }
	}

	override void OnShow()
	{
        vector player_pos = COM_GetPB().GetPosition();

        m_TxtCurrentX.SetText( player_pos[0].ToString() );
		m_TxtCurrentY.SetText( player_pos[2].ToString() );
	}

	override void OnHide()
	{

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
		        PlayerBase oPlayer = COM_GetPB();
		        oPlayer.MessageStatus( "Invalid teleportation position!" );

		        return true;
		    }

			vector vPlayerPos;
			vPlayerPos[0] = pos_x;
			vPlayerPos[2] = pos_y;

			vPlayerPos = COM_SnapToGround( vPlayerPos );

			COM_GetPB().SetPosition( vPlayerPos );

			return true;
		}

		return false;
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
		vector position = "0 0 0";

		if( !Positions.Find( GetCurrentPositionName(), position ) )
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
}