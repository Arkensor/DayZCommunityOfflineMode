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

        Positions.Insert( "Slowin", "1284 0 12693" );
        Positions.Insert( "Kolyzovka", "2757 0 13208" );
        Positions.Insert( "Bychau", "5858 0 13113" );
        Positions.Insert( "Domnowo Mili", "8744 0 13444" );
        Positions.Insert( "Domnowo Well", "8695 0 13286" );
        Positions.Insert( "Borowoje West", "11278 0 12630" );
        Positions.Insert( "Borowoje Industrial", "11820 0 12636" );
        Positions.Insert( "Alekssevka", "13802 0 11140" );
        Positions.Insert( "Ostrovez Industrial", "9617 0 10959" );
        Positions.Insert( "Ostrovez Central", "9582 0 10498" );
        Positions.Insert( "Ostrovez Airfield", "10562 0 10454" );
        Positions.Insert( "Adamowsk", "11659 0 8917" );
        Positions.Insert( "Dobrowo", "12793 0 8389" );
        Positions.Insert( "Olgowka", "13405 0 7153" );
        Positions.Insert( "Orschka", "6793 0 10908" );
        Positions.Insert( "Orschka Prison", "7912 0 4081" );
        Positions.Insert( "Orschka Castle", "7134 0 12382" );
        Positions.Insert( "Old Orschka", "5968 0 10572" );
        Positions.Insert( "Jelsk", "831 0 8370" );
        Positions.Insert( "Babrujsk", "4735 0 11390" );
        Positions.Insert( "Orlesko", "8070 0 7641" );
        Positions.Insert( "Orlesko Radio Tower", "7881 0 7909" );
        Positions.Insert( "Orlesko", "4735 0 11390" );
        Positions.Insert( "Rezoni", "9986 0 6781" );
        Positions.Insert( "Rezoni Sportsfacility", "10643 0 6607" );
        Positions.Insert( "Rezoni Castle", "9724 0 6692" );
        Positions.Insert( "Berasino", "3060 0 9363" );
        Positions.Insert( "Berasino Industrial", "3122 0 8566" );
        Positions.Insert( "Dribin", "4874 0 9259" );
        Positions.Insert( "Klettzk", "976 0 4025" );
        Positions.Insert( "Klettzk Old Mine", "1733 0 3047" );
        Positions.Insert( "Klettzk Radiotower", "634 0 2671" );
        Positions.Insert( "Krasnapolle South", "8590 0 1937" );
        Positions.Insert( "Krasnapolle North", "8943 0 2436" );
        Positions.Insert( "Krasnapolle West", "8641 0 2356" );
        Positions.Insert( "Prudnoje", "10192 0 1336" );
        Positions.Insert( "Mieva", "6793 0 10908" );
        Positions.Insert( "Mieva Airbase", "4521 0 5960" );
        Positions.Insert( "Mieva Caste", "2952 0 5630" );
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