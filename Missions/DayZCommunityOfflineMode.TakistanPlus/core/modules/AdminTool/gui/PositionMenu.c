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

        Positions.Insert( "Laschkar_Radarbase", "11243.09 0 9108.92" );
        Positions.Insert( "Rasman Airbase", "5754.95 0 11372.86" );
        Positions.Insert( "LoyManara AirBase", "8059.4 0 1856.03" );
        Positions.Insert( "FeeruzAbad RadioTower", "4116.05 0 6754.08" );
        Positions.Insert( "FeeruzAbad MilitaryBase", "5151.2 0 6076.96" );
        Positions.Insert( "FeeruzAbad FOB", "5247.09 0 5796.93" );
        Positions.Insert( "Rustaq Military Base", "2632.2 0 5100.09" );
        Positions.Insert( "Garmsar Military Base", "10422.45 0 6434.39" );
        Positions.Insert( "Farakkah", "8861.27 0 5271.47" );
        Positions.Insert( "AzZablah", "11856.08 0 2612.1" );
        Positions.Insert( "AzZablah Outpost", "11874.14 0 2543.67" );
        Positions.Insert( "LoyManara Refinery ", "7115.06 0 1025.43" );
        Positions.Insert( "Behbaman", "6082.98 0 1144.36" );
        Positions.Insert( "Dhimran", "3606.67 0 8544.91" );
        Positions.Insert( "Al Falludiyah", "1496.45 0 3602.79" );
        Positions.Insert( "Zayda", "141.58 0 2795.6" );
        Positions.Insert( "Al Hamiqqah", "1563.4 0 5721.32" );
        Positions.Insert( "El Thoubaroh", "2039.67 0 7659.63" );
        Positions.Insert( "Marastar", "4146.03 0 11758.76" );
        Positions.Insert( "Al Babair", "10477.13 0 11086.94" );
        Positions.Insert( "Marmamsiyah", "12328.32 0 10408.79" );
        Positions.Insert( "Hammaqiyah", "11502.87 0 8238.27" );
        Positions.Insert( "Al Hajasra", "11152.05 0 7636.76" );
        Positions.Insert( "Qaikkah", "10200.38 0 2350.73" );
        Positions.Insert( "Chaman", "876.04 0 3099.05" );
        Positions.Insert( "Ad Dawi", "3238.29 0 3636.3" );
        Positions.Insert( "Qarma", "6792.44 0 6089.24" );
        Positions.Insert( "Al Falqas", "4217.27 0 10747.8" );
        Positions.Insert( "Al-Falqas Oil Fields", "3650.62 0 11103.28" );
        Positions.Insert( "Gospandi", "3933.13 0 9373.13" );
        Positions.Insert( "Bastam", "5622.66 0 8899.43" );
        Positions.Insert( "Karachinar", "12333.8 0 11110.64" );
        Positions.Insert( "Jalelam", "6832.69 0 12294.08" );
        Positions.Insert( "Mahahwit", "5670.26 0 9935.64" );
        Positions.Insert( "Garmarud", "9149.89 0 6716.39" );
        Positions.Insert( "Dodr", "5035.86 0 5428.01" );
        Positions.Insert( "FeeruzAbad", "5072 0 6158" );
        Positions.Insert( "Nagara", "3057 0 9808" );
        Positions.Insert( "Rasman", "6092 0 11018" );
        Positions.Insert( "Garmsar", "10603.58 0 6410.72" );
        Positions.Insert( "Nur", "1890.7 0 11867.49" );
        Positions.Insert( "Zavarak", "9951.81 0 11651.26" );
        Positions.Insert( "Falar", "6063.8 0 7256.97" );
        Positions.Insert( "Anar", "6030.72 0 5659.43" );
        Positions.Insert( "ChakChak", "4375.92 0 754.72" );
        Positions.Insert( "Sakhee", "3588.99 0 4284.47" );
        Positions.Insert( "LoyManara", "8622.05 0 2454.22" );
        Positions.Insert( "Mulladoost", "2009.21 0 7675.97" );
        Positions.Insert( "Jilavur", "2537.21 0 5026.47" );
        Positions.Insert( "Shukurkalay", "1529.11 0 3582.72" );
        Positions.Insert( "Imarat", "8238.39 0 7847.22" );
        Positions.Insert( "Ravanay", "11539.21 0 8318.97" );
        Positions.Insert( "Kakaru", "5313.46 0 4740.72" );
        Positions.Insert( "Landay", "1950.86 0 349.72" );
        Positions.Insert( "Huzrutimam", "5932.74 0 1245.22" );
        Positions.Insert( "Sultansafee", "6578.67 0 1955.22" );
        Positions.Insert( "Band-E Kirmiz", "3531.56 0 6884.1" );
        Positions.Insert( "Silsila-Ye Siah Kuh", "4812.84 0 2556.02" );
        Positions.Insert( "Reegshur Valley", "4246.14 0 5568.12" );
        Positions.Insert( "Lalezar Valley", "4401.77 0 9763.12" );
        Positions.Insert( "Darre-Ye Sabz", "8587.86 0 3204.21" );
        Positions.Insert( "Dawlatsah Valley", "7532.3 0 8283.12" );
        Positions.Insert( "Zabo Valley", "2774.33 0 8016.48" );
        Positions.Insert( "Dawlatsah Valley", "10104.34 0 6651.83" );

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