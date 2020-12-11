class LoadText extends UIScriptedMenu
{
    private TextWidget bgText;
    private TextWidget fgText;

    void LoadText()
	{
	}

    void ~LoadText()
    {
    }

	Widget InitWithParent( Widget parent )
	{
		Print( "LoadText::InitWithParent" );

		string layoutFile = "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\Persistency\\gui\\layouts\\LoadText.layout";
		if ( parent )
		{
			layoutRoot = GetGame().GetWorkspace().CreateWidgets( layoutFile, parent );
		}
		else {
			layoutRoot = GetGame().GetWorkspace().CreateWidgets( layoutFile );
		}

        bgText = TextWidget.Cast( layoutRoot.FindAnyWidget( "Bg_LoadText_TextWidget" ) );
        fgText = TextWidget.Cast( layoutRoot.FindAnyWidget( "Fg_LoadText_TextWidget" ) );

        return layoutRoot;
	}
	
	override Widget Init()
	{
		Print( "LoadText::Init" );
		return InitWithParent( NULL );
    }

    void SetText( string text )
    {
        if ( bgText && fgText )
        {
            bgText.SetText( text );
            fgText.SetText( text );

		    bgText.SetColor(0xFF000000);
		    fgText.SetColor(0xFFFFFFFF);
        }
    }
}

ref LoadText g_LoadText;

static void SetLoadingText( string text )
{
    CloseLoadingText();

    if ( !g_LoadText )
    {
        g_LoadText = new LoadText;
    }

    GetGame().GetUIManager().ShowScriptedMenu( g_LoadText , NULL );

    g_LoadText.SetText( text );
}

static void CloseLoadingText()
{
    if ( !g_LoadText ) return;

	GetGame().GetUIManager().HideScriptedMenu( g_LoadText );
}