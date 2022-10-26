
// Saving positions to $saves:CommunityOfflineMode/COMPositions.json.
// COMPositions.json can be customized to the users liking.
// COMPositions.json can be renamed or deleted to restore the default COMPositions.json file.
// Positions are now inserted into the Positions menu in the same order as the COMPositions.json file.
// Added positions for Namalsk.
// Disabled teleporting while in vehicle to prevent player glitching.
// - Brandon10x15.

class PositionMenu extends PopupMenu
{
	protected TextListboxWidget m_LstPositionList;
	protected EditBoxWidget m_TxtSelectedX;
	protected EditBoxWidget m_TxtSelectedY;
	protected EditBoxWidget m_TxtCurrentX;
	protected EditBoxWidget m_TxtCurrentY;
	protected ButtonWidget m_TeleportButton;
	protected ButtonWidget m_CancelButton;

    ref array<ref PositionData> Positions = new ref array< ref PositionData>;

    protected bool m_bOverCurrentPos;
    protected bool m_positionMenuOpen = false;

	void PositionMenu()
	{
	    m_bOverCurrentPos = false;
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

		SetupPositions();

        foreach(auto position : Positions)
        {
            m_LstPositionList.AddItem(position.name, NULL, 0);
        }
        GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( Update, 1000, true );
	}

	void SetupPositions()
	{
		string positionsPath = BASE_COM_DIR + "COMPositions.json";
		ref PositionSave positions = NULL;
        if ( !FileExist( positionsPath ) )
        {
			positions = new PositionSave();
			positions.New();
			JsonFileLoader<PositionSave>.JsonSaveFile( positionsPath, positions);
        }
    	if(positions == NULL)
    	{
    	    JsonFileLoader<PositionSave>.JsonLoadFile( positionsPath, positions );
    	}
		if(worldMap == "ChernarusPlus") {
            Positions = positions.ChernarusPlus;
        }
        else if(worldMap == "Enoch")
        {
            Positions = positions.Enoch;
        }
        else if(worldMap == "Namalsk")
        {
            Positions = positions.Namalsk;
        }
	}

    void Update()
    {
        if(isPositionMenuOpen())
        {
            vector player_pos = COM_GetPB().GetPosition();
            m_TxtCurrentX.SetText(player_pos[0].ToString());
            m_TxtCurrentY.SetText(player_pos[2].ToString());
        }
    }

	bool isPositionMenuOpen() {
		return m_positionMenuOpen;
	}

	override void OnShow() {
		m_positionMenuOpen = true;
	}

	override void OnHide() {
		m_positionMenuOpen = false;
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
            if(COM_GetPB().IsInVehicle())
            {
                COM_Message("Exit the vehicle before teleporting.");
                return false;
            }

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
        string posName = GetCurrentPositionName();
        if (posName) {
            foreach(auto pos : Positions) {
                if (pos.name == posName) {
                    position = pos.pos;
                    break;
                }
            }
        }
        m_TxtSelectedX.SetText(position[0].ToString());
        m_TxtSelectedY.SetText(position[2].ToString());
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