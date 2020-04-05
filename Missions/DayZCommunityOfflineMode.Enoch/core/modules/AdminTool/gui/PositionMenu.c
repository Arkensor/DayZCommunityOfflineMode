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

        Positions.Insert( "Adamow", "3086.50 0 6810.62" );
        Positions.Insert( "Bagno", "11827.17 0 11123.91" );
        Positions.Insert( "Bielawa", "1476.83 0 9645.52" );
        Positions.Insert( "Branzow Castle", "1017.37 0 11439.59" );
        Positions.Insert( "Brena", "6615.27 0 11208.30" );
        Positions.Insert( "Borek", "9812.73 0 8467.18" );
        Positions.Insert( "Dambog", "644.65 0 1107.23" );
        Positions.Insert( "Dolnik", "11358.33 0 623.55" );
        Positions.Insert( "Drewniki", "5830.76 0 5047.80" );
        Positions.Insert( "Dubky", "6653.59 0 3674.47" );
        Positions.Insert( "Gieraltow", "11250.50 0 4347.67" );
        Positions.Insert( "Gliniska", "15152.52 0 13815.69" );
        Positions.Insert( "Gliniska Airfield", "3949.16 0 10298.76" );
        Positions.Insert( "Grabin", "10673.83 0 11047.34" );
        Positions.Insert( "Hedrykow", "4052.30 0 6598.08" );
        Positions.Insert( "Hrud", "6474.62 0 9351.71" );
        Positions.Insert( "Huta", "5215.02 0 5572.90" );
		Positions.Insert( "Jantar", "3634.26 0 8886.00" );
        Positions.Insert( "Jezurow", "2026.93 0 2347.30" );
        Positions.Insert( "Karlin", "10069.03 0 6960.65" );
        Positions.Insert( "Kolembrody", "8470.30 0 12006.89" );
		Positions.Insert( "Kopa", "5555.70 0 8758.64" );
		Positions.Insert( "Krsnik", "7848.31 0 9945.57" );
        Positions.Insert( "Kulno", "11103.60 0 2394.19" );
        Positions.Insert( "Lembork", "8796.92 0 6635.59" );
        Positions.Insert( "Lipina", "5930.77 0 6779.37" );
        Positions.Insert( "Lukow", "3529.55 0 11961.57" );
        Positions.Insert( "Muratyn", "4593.03 0 6376.17" );
        Positions.Insert( "Nadbor", "6093.30 0 4072.62" );
        Positions.Insert( "Nidek", "6093.30 0 8035.85" );
        Positions.Insert( "Olszanka", "4808.69 0 7657.65" );
        Positions.Insert( "Piorun", "532.91 0 12067.83" );
        Positions.Insert( "Polana", "3314.67 0 2047.25" );
        Positions.Insert( "Radacz", "4049.18 0 7935.83" );
        Positions.Insert( "Radunin", "7318.53 0 6479.31" );
        Positions.Insert( "Rodzanica", "8922.73 0 2058.97" );
        Positions.Insert( "Roztoka", "7731.10 0 5297.85" );
        Positions.Insert( "Sitnik", "11484.92 0 9561.13" );
        Positions.Insert( "Sobotka", "6299.59 0 10248.75" );
        Positions.Insert( "Sowa", "11644.32 0 12061.58" );
        Positions.Insert( "Swarog", "4980.60 0 2115.23" );
        Positions.Insert( "Tarnow", "9043.84 0 11555.24" );
        Positions.Insert( "Topolin", "1783.14 0 7379.48" );
        Positions.Insert( "Tymbark", "2858.34 0 1222.10" );
        Positions.Insert( "Widok", "10256.57 0 2172.27" );
        Positions.Insert( "Wrzeszcz", "9006.34 0 4419.56" );
        Positions.Insert( "Zalesie", "882.97 0 5547.89" );
        Positions.Insert( "Zapadlisko", "8049.91 0 8710.97" );
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
        vector player_pos = GetGame().GetPlayer().GetPosition();

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
		        PlayerBase oPlayer = GetGame().GetPlayer();
		        oPlayer.MessageStatus( "Invalid teleportation position!" );

		        return true;
		    }

			vector vPlayerPos;
			vPlayerPos[0] = pos_x;
			vPlayerPos[2] = pos_y;

			vPlayerPos = SnapToGround( vPlayerPos );

			GetGame().GetPlayer().SetPosition( vPlayerPos );

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