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

        Positions.Insert( "Бункер Янтарь", "2513.145264 37.772495 6201.842773" );
        Positions.Insert( "Мертвый город", "2835.305420 122.214401 8240.073242" );
        Positions.Insert( "Рыжий лес", "3495.384766 97.980606 10778.959961" );
        Positions.Insert( "Армейские склады", "5833.546387 146.983841 7998.777344" );
        Positions.Insert( "Северная свалка", "6580.170898 97.652870 9410.355469" );
        Positions.Insert( "Северные топи", "8195.773438 132.623276 9718.948242" );
        Positions.Insert( "Кладбище техники", "7898.414551 160.728973 6553.560059" );
        Positions.Insert( "Сотка", "4955.398438 126.822639 6615.037598" );
        Positions.Insert( "Депо", "4997.834961 119.176666 4643.126953" );
        Positions.Insert( "Темная долина", "7747.113770 116.232506 4545.838379" );
        Positions.Insert( "Темная Лощина", "8957.650391 68.412491 1510.518066" );
        Positions.Insert( "Предбанник", "6286.533203 79.834671 545.352234" );
        Positions.Insert( "Южное КПП", "4693.636719 59.965763 1313.338989" );
        Positions.Insert( "ДереBASS", "4611.537109 76.162506 1813.046143" );
        Positions.Insert( "АТП", "6875.557617 95.346390 2971.814453" );
        Positions.Insert( "Южный Агропром", "1737.193359 42.562496 4072.360840" );
        Positions.Insert( "Южные Болота", "1867.939819 80.779274 1649.102173" );

        Positions.Insert( "Baza", "16776.972656 204.1 19704.855469" );
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