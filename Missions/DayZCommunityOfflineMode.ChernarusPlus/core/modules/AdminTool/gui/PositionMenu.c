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
        Positions.Insert(new Param2<string, vector>("Altar", "8090.49 0 9326.95"));
        Positions.Insert(new Param2<string, vector>("Balota", "4393.72 0 2412.05"));
        Positions.Insert(new Param2<string, vector>("Balota Airstrip", "5025.65 0 2363.44"));
        Positions.Insert(new Param2<string, vector>("Bashnya", "4156.51 0 11747.29"));
        Positions.Insert(new Param2<string, vector>("Bay Mutnaya", "5634.24 0 1947.33"));
        Positions.Insert(new Param2<string, vector>("Bay Tikhaya", "1190.83 0 2194.76"));
        Positions.Insert(new Param2<string, vector>("Bay Zelenaya", "11140.75 0 3090.17"));
        Positions.Insert(new Param2<string, vector>("Belaya Polana", "14148.24 0 15004.23"));
        Positions.Insert(new Param2<string, vector>("Berezhki", "15152.52 0 13815.69"));
        Positions.Insert(new Param2<string, vector>("Berezino", "11856.29 0 8991.54"));
        Positions.Insert(new Param2<string, vector>("Berezino Lumberyard", "12661.27 0 9590.91"));
        Positions.Insert(new Param2<string, vector>("Berezino Shipyard", "13178.96 0 10213.62"));
        Positions.Insert(new Param2<string, vector>("Black Forest", "9083.59 0 7555.09"));
        Positions.Insert(new Param2<string, vector>("Black Lake", "13360.76 0 11902.36"));
        Positions.Insert(new Param2<string, vector>("Black Mtn", "10246.34 0 11984.03"));
        Positions.Insert(new Param2<string, vector>("Bogatyrka", "1504.85 0 8940.5"));
        Positions.Insert(new Param2<string, vector>("Bor", "3317.02 0 4014.26"));
        Positions.Insert(new Param2<string, vector>("Cap Golova", "8350.07 0 2443.64"));
        Positions.Insert(new Param2<string, vector>("Cernaya Polana", "12199.96 0 13666.45"));
        Positions.Insert(new Param2<string, vector>("Chernogorsk", "6572.9 0 2447.53"));
        Positions.Insert(new Param2<string, vector>("Chernogorsk Factory", "6406.17 0 2710.03"));
        Positions.Insert(new Param2<string, vector>("Chyornaya Bay", "7706.96 0 3087.25"));
        Positions.Insert(new Param2<string, vector>("Crown Castle", "1430 0 9218.07"));
        Positions.Insert(new Param2<string, vector>("Deep Lake", "1853.38 0 14860.82"));
        Positions.Insert(new Param2<string, vector>("Devil's Castle", "6909.76 0 11371.53"));
        Positions.Insert(new Param2<string, vector>("Dichina Military", "4622.19 0 8283.76"));
        Positions.Insert(new Param2<string, vector>("Dobroe", "13096.32 0 14953.18"));
        Positions.Insert(new Param2<string, vector>("Dolina", "11327.42 0 6645.57"));
        Positions.Insert(new Param2<string, vector>("Drakon Island", "11185.96 0 2491.77"));
        Positions.Insert(new Param2<string, vector>("Drozhino", "3446.32 0 4929.12"));
        Positions.Insert(new Param2<string, vector>("Dubky", "6653.59 0 3674.47"));
        Positions.Insert(new Param2<string, vector>("Dubrovka", "10397.6 0 9795.56"));
        Positions.Insert(new Param2<string, vector>("Dubrovka Vyshnaya", "9952.95 0 10392.8"));
        Positions.Insert(new Param2<string, vector>("Electrozavodsk", "10502.8 0 2336.88"));
        Positions.Insert(new Param2<string, vector>("Electrozavodsk Power", "10358.14 0 2593.85"));
        Positions.Insert(new Param2<string, vector>("Electrozavodsk Shipyard", "9961.48 0 1788.85"));
        Positions.Insert(new Param2<string, vector>("Gnome Castle", "7410.93 0 9114.04"));
        Positions.Insert(new Param2<string, vector>("Gorka", "9646.16 0 8810.25"));
        Positions.Insert(new Param2<string, vector>("Green Mtn", "3744.3 0 6013.15"));
        Positions.Insert(new Param2<string, vector>("Grishino", "6018.26 0 10256.4"));
        Positions.Insert(new Param2<string, vector>("Grozovoy Pass", "3237.3 0 15293.46"));
        Positions.Insert(new Param2<string, vector>("Guba", "14609.06 0 13283.4"));
        Positions.Insert(new Param2<string, vector>("Guglovo", "8411.8 0 6687.87"));
        Positions.Insert(new Param2<string, vector>("Gvozdno", "8706.38 0 11895.07"));
        Positions.Insert(new Param2<string, vector>("Kabanino", "5298.35 0 8563.76"));
        Positions.Insert(new Param2<string, vector>("Kamenka", "1892.27 0 2155.87"));
        Positions.Insert(new Param2<string, vector>("Kamensk", "6633.17 0 14466.59"));
        Positions.Insert(new Param2<string, vector>("Kamyshovo", "12170.79 0 3447.95"));
        Positions.Insert(new Param2<string, vector>("Karmanovka", "12519.8 0 14679.5"));
        Positions.Insert(new Param2<string, vector>("Khelm", "12286.8 0 10794.4"));
        Positions.Insert(new Param2<string, vector>("Klen Mtn", "11491.72 0 11331.18"));
        Positions.Insert(new Param2<string, vector>("Komarovo", "3619.86 0 2342.53"));
        Positions.Insert(new Param2<string, vector>("Kozlova Mtn", "8826.96 0 2858.41"));
        Positions.Insert(new Param2<string, vector>("Kozlovka", "4451.57 0 4577.18"));
        Positions.Insert(new Param2<string, vector>("Krasnoe", "6486.37 0 14981.87"));
        Positions.Insert(new Param2<string, vector>("Krasnostav", "11228.8 0 12223.2"));
        Positions.Insert(new Param2<string, vector>("Krasnostav Airstrip", "12016.22 0 12490.07"));
        Positions.Insert(new Param2<string, vector>("Krutoy Cap", "13624.71 0 3892.25"));
        Positions.Insert(new Param2<string, vector>("Kumyrna", "8403.05 0 6030.65"));
        Positions.Insert(new Param2<string, vector>("Lopatino", "2749.87 0 9991.8"));
        Positions.Insert(new Param2<string, vector>("Mamino", "7981.69 0 13057.3"));
        Positions.Insert(new Param2<string, vector>("Map Edge NE", "15321.19 0 15327.49"));
        Positions.Insert(new Param2<string, vector>("Map Edge NW", "44.12 0 15330.89"));
        Positions.Insert(new Param2<string, vector>("Map Edge SW", "19.82 0 1584.2"));
        Positions.Insert(new Param2<string, vector>("Mogilevka", "7502.8 0 5237.32"));
        Positions.Insert(new Param2<string, vector>("Msta", "11258.3 0 5511.28"));
        Positions.Insert(new Param2<string, vector>("Myshkino", "2056.57 0 7425.29"));
        Positions.Insert(new Param2<string, vector>("Myshkino Military", "1266.66 0 7225.99"));
        Positions.Insert(new Param2<string, vector>("Myshkino Trainyard", "885.72 0 7696.21"));
        Positions.Insert(new Param2<string, vector>("Nadezhdino", "5821.39 0 4764.33"));
        Positions.Insert(new Param2<string, vector>("Nagornoe", "9333.93 0 14666.87"));
        Positions.Insert(new Param2<string, vector>("Nizhnoye", "12895.4 0 8040.79"));
        Positions.Insert(new Param2<string, vector>("Novaya Petrovka", "3471.91 0 12988.1"));
        Positions.Insert(new Param2<string, vector>("Novaya Trainyard", "3526.6 0 12579.7"));
        Positions.Insert(new Param2<string, vector>("Novodmitrovsk", "11595 0 14696.5"));
        Positions.Insert(new Param2<string, vector>("Novoselky", "6052.29 0 3285.59"));
        Positions.Insert(new Param2<string, vector>("Novy Sobor", "7159.26 0 7688.51"));
        Positions.Insert(new Param2<string, vector>("NW Airfield ATC", "4618.3 0 10439.17"));
        Positions.Insert(new Param2<string, vector>("NW Airfield Range", "5373.21 0 9979.8"));
        Positions.Insert(new Param2<string, vector>("NW Airfield South", "4520.11 0 9421.26"));
        Positions.Insert(new Param2<string, vector>("NW Airfield Tents", "4163.31 0 11014.24"));
        Positions.Insert(new Param2<string, vector>("Olha", "13320.9 0 12893.05"));
        Positions.Insert(new Param2<string, vector>("Oreshka Pass", "9877.39 0 6009.26"));
        Positions.Insert(new Param2<string, vector>("Orlovets", "12136.28 0 7281.89"));
        Positions.Insert(new Param2<string, vector>("Orlovets Factory", "11489.7 0 7592.98"));
        Positions.Insert(new Param2<string, vector>("Otmel Island", "11642.89 0 3095.52"));
        Positions.Insert(new Param2<string, vector>("Pavlovo", "1626.37 0 3846.56"));
        Positions.Insert(new Param2<string, vector>("Pavlovo Military", "2101.77 0 3290.93"));
        Positions.Insert(new Param2<string, vector>("Pogorevka", "4376.71 0 6413.21"));
        Positions.Insert(new Param2<string, vector>("Polana", "10678.7 0 8090.18"));
        Positions.Insert(new Param2<string, vector>("Polesovo", "5817 0 13490"));
        Positions.Insert(new Param2<string, vector>("Prigorodki", "7984.89 0 3302.17"));
        Positions.Insert(new Param2<string, vector>("Prud Lake", "6627.34 0 9288.07"));
        Positions.Insert(new Param2<string, vector>("Pulkovo", "4902.36 0 5650.65"));
        Positions.Insert(new Param2<string, vector>("Pusta", "9176.44 0 3858.71"));
        Positions.Insert(new Param2<string, vector>("Pustoshka", "2992.21 0 7805.54"));
        Positions.Insert(new Param2<string, vector>("Ratnoe", "6305.55 0 12708.4"));
        Positions.Insert(new Param2<string, vector>("Rify", "13783.18 0 11233"));
        Positions.Insert(new Param2<string, vector>("Rog Castle", "11255 0 4293.29"));
        Positions.Insert(new Param2<string, vector>("Rogovo", "4731 0 6775.61"));
        Positions.Insert(new Param2<string, vector>("Romashka", "8148.5 0 11015.2"));
        Positions.Insert(new Param2<string, vector>("Schadenfreude Island", "24000 0 0"));
        Positions.Insert(new Param2<string, vector>("Severograd", "7893.4 0 12576.3"));
        Positions.Insert(new Param2<string, vector>("Shakhovka", "9670.8 0 6495.85"));
        Positions.Insert(new Param2<string, vector>("Silence Lake", "1203.95 0 6507.42"));
        Positions.Insert(new Param2<string, vector>("Sinystok", "1447.49 0 11924.24"));
        Positions.Insert(new Param2<string, vector>("Skalisty Castle", "13493.95 0 3254.96"));
        Positions.Insert(new Param2<string, vector>("Skalisty Island", "13661.17 0 2960.38"));
        Positions.Insert(new Param2<string, vector>("Skvsch Biathlon Arena", "488.57 0 11118.7"));
        Positions.Insert(new Param2<string, vector>("Smirnovo", "11629.3 0 15023.1"));
        Positions.Insert(new Param2<string, vector>("Solnichniy", "13469.1 0 6239"));
        Positions.Insert(new Param2<string, vector>("Solnichniy Factory", "13069.59 0 7161.34"));
        Positions.Insert(new Param2<string, vector>("Sosnovka", "2512.53 0 6371.41"));
        Positions.Insert(new Param2<string, vector>("Sosnovy Pass", "2736.13 0 6757.38"));
        Positions.Insert(new Param2<string, vector>("Staroye", "10164.18 0 5460.93"));
        Positions.Insert(new Param2<string, vector>("Stary Sobor", "6072.64 0 7748.56"));
        Positions.Insert(new Param2<string, vector>("Stary Yar", "4935.61 0 15068"));
        Positions.Insert(new Param2<string, vector>("Storozh Island (Prison)", "2611.2 0 1306.15"));
        Positions.Insert(new Param2<string, vector>("Svergino", "9533.2 0 13728.4"));
        Positions.Insert(new Param2<string, vector>("Svetlojarsk", "13931.93 0 13231.39"));
        Positions.Insert(new Param2<string, vector>("Three Crosses Gas", "312.94 0 9362.44"));
        Positions.Insert(new Param2<string, vector>("Three Valleys", "12842.1 0 5500.33"));
        Positions.Insert(new Param2<string, vector>("Tisy", "3413.75 0 14810.27"));
        Positions.Insert(new Param2<string, vector>("Tisy Military", "1681.3 0 14116.6"));
        Positions.Insert(new Param2<string, vector>("Topolka Dam", "10280.5 0 3507.5"));
        Positions.Insert(new Param2<string, vector>("Topolniki", "2821.19 0 12366.5"));
        Positions.Insert(new Param2<string, vector>("Troitskoe", "7559.44 0 13515.9"));
        Positions.Insert(new Param2<string, vector>("Troitskoe Military", "7866.4 0 14676.1"));
        Positions.Insert(new Param2<string, vector>("Tulga", "12808.7 0 4457.13"));
        Positions.Insert(new Param2<string, vector>("Turovo", "13576.5 0 14128.6"));
        Positions.Insert(new Param2<string, vector>("Vavilovo", "2211.83 0 11016.4"));
        Positions.Insert(new Param2<string, vector>("Veresnik Mtn", "4439.42 0 8078.14"));
        Positions.Insert(new Param2<string, vector>("Vybor", "3800 0 8844.25"));
        Positions.Insert(new Param2<string, vector>("Vyshnoye", "6532.61 0 6090.53"));
        Positions.Insert(new Param2<string, vector>("Vysotovo", "5616.26 0 2561.77"));
        Positions.Insert(new Param2<string, vector>("Willow Lake", "13254.2 0 11607.8"));
        Positions.Insert(new Param2<string, vector>("Zabolotye", "1156.26 0 10006.2"));
        Positions.Insert(new Param2<string, vector>("Zaprudnoe", "4927.47 0 13027.2"));
        Positions.Insert(new Param2<string, vector>("Zelenogorsk", "2713.4 0 5391.75"));
        Positions.Insert(new Param2<string, vector>("Zelenogorsk Trainyard", "2490.65 0 5151.76"));
        Positions.Insert(new Param2<string, vector>("Zub Castle", "6541.79 0 5591.69"));
        Positions.Insert(new Param2<string, vector>("Zvir", "477.35 0 5202.98"));
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