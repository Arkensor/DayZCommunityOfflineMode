class ObjectMenu
{

	Widget layoutRoot;
	protected TextListboxWidget m_classList;
	protected EditBoxWidget m_SearchBox;
	protected ButtonWidget m_btnSpawnGround;
	protected ButtonWidget m_btnSpawnCursor;
	protected ButtonWidget m_btnSpawnInventory;
	protected ButtonWidget m_btnCancel;

	void ObjectMenu( Widget parentWidget )
	{

		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "missions\\DayZCommunityOfflineMode.ChernarusPlus\\core\\modules\\ComEditor\\gui\\layouts\\ObjectMenu.layout", parentWidget );

		Init();
	}

	void ~ObjectMenu()
	{
	}

	Widget Init()
	{
		m_classList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget( "classlist" ) );
		m_SearchBox = EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "search_input" ) );
		m_btnSpawnGround = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawn_ground" ) );
		m_btnSpawnCursor = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawn_cursorpos" ) );
		m_btnSpawnInventory = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawn_inventory" ) );
		m_btnCancel = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_cancel" ) );

		return layoutRoot;
	}

	void Toggle()
	{
		layoutRoot.Show(!layoutRoot.IsVisible());

		if ( layoutRoot.IsVisible() ) 
		{
			UpdateList();
		}
		else 
		{

		}
	}

	bool OnChange( Widget w, int x, int y, bool finished )
	{
        if ( w == m_SearchBox )
        {
            UpdateList();
            return true;
        }

        return false;
    }

	bool OnClick( Widget w, int x, int y, int button )
	{
	    string strSelection = GetCurrentSelection();

        if( strSelection != "" )
        {
            if( w == m_btnSpawnCursor )
            {
                EntityAI oCursorObj = g_Game.CreateObject( strSelection, GetCursorPos(), false );

                if ( oCursorObj.IsInherited( ItemBase ) )
                {
                    ItemBase oCursorItem = ( ItemBase ) oCursorObj;
                    SetupSpawnedItem( oCursorItem, oCursorItem.GetMaxHealth(), 1 );

                    return true;
                }
            }
            else if ( w == m_btnSpawnGround )
            {
                EntityAI oObj = g_Game.CreateObject( strSelection, GetGame().GetPlayer().GetPosition(), false );

                if ( oObj.IsInherited( ItemBase ) )
                {
                    ItemBase oItem = ( ItemBase ) oObj;
                    SetupSpawnedItem( oItem, oItem.GetMaxHealth(), 1 );

                     return true;
                }
            }
            else if ( w == m_btnSpawnInventory )
            {
                EntityAI oInvItem = g_Game.GetPlayer().GetInventory().CreateInInventory( strSelection );

                oInvItem.SetHealth( oInvItem.GetMaxHealth() );

                 return true;
            }
        }

        return false;
	}

	bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
        //Todo use this for item preview change maybe?

		return true;
	}

    void UpdateList()
    {
        m_classList.ClearItems();

		TStringArray configs = new TStringArray;
		configs.Insert( CFG_VEHICLESPATH );
		configs.Insert( CFG_WEAPONSPATH );
		configs.Insert( CFG_MAGAZINESPATH );

        string strSearch = m_SearchBox.GetText();

		strSearch.ToLower();

		for ( int nConfig = 0; nConfig < configs.Count(); ++nConfig )
		{
			string strConfigPath = configs.Get( nConfig );

			int nClasses = g_Game.ConfigGetChildrenCount( strConfigPath );

			for ( int nClass = 0; nClass < nClasses; ++nClass )
			{
				string strName;

				g_Game.ConfigGetChildName( strConfigPath, nClass, strName );

				int scope = g_Game.ConfigGetInt( strConfigPath + " " + strName + " scope" );

				if ( scope == 0 )
				{
					continue;
				}

				string strNameLower = strName;

				strNameLower.ToLower();

				if ( strNameLower.Contains( strSearch ) || ( strSearch == "" ) )
				{
					m_classList.AddItem( strName, NULL, 0 );
				}
			}
		}
    }

	string GetCurrentSelection()
	{
		if ( m_classList.GetSelectedRow() != -1 )
		{
			string result;
			m_classList.GetItemText( m_classList.GetSelectedRow(), 0, result );
			return result;
		}

		return "";
	}
}