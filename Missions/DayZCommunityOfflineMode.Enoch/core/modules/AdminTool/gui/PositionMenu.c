class PositionMenu extends PopupMenu
{
	protected TextListboxWidget m_LstPositionList;
	protected EditBoxWidget m_TxtSelectedX;
	protected EditBoxWidget m_TxtSelectedY;
	protected EditBoxWidget m_TxtCurrentX;
	protected EditBoxWidget m_TxtCurrentY;
	protected ButtonWidget m_TeleportButton;
	protected ButtonWidget m_CancelButton;

	//autoptr map< string, vector > Positions = new map< string, vector >;
    ref array<ref Param2<string, vector>> Positions = new array< ref Param2<string, vector>>;

    protected bool m_bOverCurrentPos;
    static bool positionMenuOpen = false;

	void PositionMenu()
	{
	    m_bOverCurrentPos = false;
		Positions.Insert(new Param2<string, vector>("Adamow", "3175.23 0 6805.85"));
        Positions.Insert(new Param2<string, vector>("Bagno", "11970.94 0 11220.80"));
        Positions.Insert(new Param2<string, vector>("Bielawa", "1392.87 0 9664.53"));
        Positions.Insert(new Param2<string, vector>("Borek", "9768.98 0 8497.65"));
        Positions.Insert(new Param2<string, vector>("Branzow Castle", "1075.98 0 11365.36"));
        Positions.Insert(new Param2<string, vector>("Brena", "6641.84 0 11130.16"));
        Positions.Insert(new Param2<string, vector>("Dambog", "554.79 0 1127.55"));
        Positions.Insert(new Param2<string, vector>("Drewniki", "5790.12 0 5087.65"));
        Positions.Insert(new Param2<string, vector>("Dolnik", "11390.37 0 612.61"));
        Positions.Insert(new Param2<string, vector>("Gieraltow", "11345.83 0 4397.68"));
        Positions.Insert(new Param2<string, vector>("Gliniska", "4993.88 0 9916.66"));
        Positions.Insert(new Param2<string, vector>("Grabinskie", "10959.82 0 10998.11"));
        Positions.Insert(new Param2<string, vector>("Hedrykow", "4507.86 0 4803.22"));
        Positions.Insert(new Param2<string, vector>("Hrud", "6479.31 0 9235.63"));
        Positions.Insert(new Param2<string, vector>("Huta", "5228.30 0 5567.43"));
        Positions.Insert(new Param2<string, vector>("Jantar", "3668.64 0 8889.91"));
        Positions.Insert(new Param2<string, vector>("Jezurow", "2104.29 0 2330.90"));
        Positions.Insert(new Param2<string, vector>("Karlin", "10078.41 0 6885.64"));
        Positions.Insert(new Param2<string, vector>("Kolembrody", "8417.17 0 11968.60"));
        Positions.Insert(new Param2<string, vector>("Kopa", "5555.70 0 8724.25"));
        Positions.Insert(new Param2<string, vector>("Krsnik", "7813.15 0 10044.03"));
        Positions.Insert(new Param2<string, vector>("Kulno", "11194.24 0 2453.57"));
        Positions.Insert(new Param2<string, vector>("Lipina", "5937.02 0 6777.02"));
        Positions.Insert(new Param2<string, vector>("Lukow", "3755.38 0 11720.90"));
        Positions.Insert(new Param2<string, vector>("Lembork", "8837.56 0 6614.49"));
        Positions.Insert(new Param2<string, vector>("Muratyn", "4586.78 0 6385.54"));
        Positions.Insert(new Param2<string, vector>("Nadbor", "6171.44 0 4091.37"));
        Positions.Insert(new Param2<string, vector>("Nidek", "6291.00 0 8077.26"));
        Positions.Insert(new Param2<string, vector>("Olszanka", "4757.12 0 7590.45"));
        Positions.Insert(new Param2<string, vector>("Piorun", "643.09 0 12108.47"));
        Positions.Insert(new Param2<string, vector>("Polana", "3362.77 0 2095.52"));
        Positions.Insert(new Param2<string, vector>("Radacz", "3962.44 0 7895.98"));
        Positions.Insert(new Param2<string, vector>("Radunin", "7291.96 0 6487.13"));
        Positions.Insert(new Param2<string, vector>("Rodzanica", "8900.85 0 2077.72"));
        Positions.Insert(new Param2<string, vector>("Roztoka", "7688.82 0 5238.29"));
        Positions.Insert(new Param2<string, vector>("Sitnickie", "11323.00 0 10018.24"));
        Positions.Insert(new Param2<string, vector>("Skala", "2997.42 0 1958.17"));
        Positions.Insert(new Param2<string, vector>("Sobotka", "6300.11 0 10161.06"));
        Positions.Insert(new Param2<string, vector>("Sowa", "11665.42 0 12014.70"));
        Positions.Insert(new Param2<string, vector>("Swarog", "4888.40 0 2211.34"));
        Positions.Insert(new Param2<string, vector>("Tarnow", "9268.88 0 10790.26"));
        Positions.Insert(new Param2<string, vector>("Topolin", "1873.00 0 7341.97"));
        Positions.Insert(new Param2<string, vector>("Tymbark", "2979.45 0 1129.89"));
        Positions.Insert(new Param2<string, vector>("Widok", "10256.57 0 2122.26"));
        Positions.Insert(new Param2<string, vector>("Wrzeszcz", "9037.59 0 4390.65"));
        Positions.Insert(new Param2<string, vector>("Zalesie", "865.18 0 5498.05"));
        Positions.Insert(new Param2<string, vector>("Zapadlisko", "8111.64 0 8693.00"));
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

        foreach(auto param : Positions) { m_LstPositionList.AddItem(param.param1, NULL, 0); }
        GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( Update, 1000, true );
        /*for ( int nPosition = 0; nPosition < Positions.Count(); nPosition++ )
        {
            m_LstPositionList.AddItem( Positions.GetKey( nPosition ), NULL, 0 );
        }*/
	}
    void Update() {
        if(positionMenuOpen) { vector player_pos = COM_GetPB().GetPosition(); m_TxtCurrentX.SetText(player_pos[0].ToString()); m_TxtCurrentY.SetText(player_pos[2].ToString()); }
    }

	override void OnShow() { positionMenuOpen = true; }

	override void OnHide() { positionMenuOpen = false; }

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
            if(COM_GetPB().IsInVehicle()) { COM_Message("Exit the vehicle before teleporting."); return false; }
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

			vector vPlayerPos; vPlayerPos[0] = pos_x; vPlayerPos[2] = pos_y;
			vPlayerPos = COM_SnapToGround(vPlayerPos);

			COM_GetPB().SetPosition(vPlayerPos);
			Camera cam = CameraTool.Cast(COM_GetModuleManager().GetModule(CameraTool)).m_oCamera;
			if(cam) { vPlayerPos[1] = vPlayerPos[1] + 2; cam.SetPosition(vPlayerPos); }

			return true;
		}

		return false;
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
		vector position = "0 0 0";
        string posname = GetCurrentPositionName();
        if (posname) {
            foreach(auto param : Positions) {
                if (param.param1 == posname) {
                    position = param.param2;
                }
            }

            m_TxtSelectedX.SetText(position[0].ToString());
            m_TxtSelectedY.SetText(position[2].ToString());
            return true;
        }
        return false;
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