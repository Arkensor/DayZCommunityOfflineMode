class WelcomeManager extends UIScriptedMenu
{
    MultilineTextWidget m_InfoBox;
    ButtonWidget m_BtnOk;

	void WelcomeManager()
	{
	}	
	
	void ~WelcomeManager()
	{
	}

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:\\missions\\DayZCommunityOfflineMode.ChernarusPlus\\gui\\WelcomeManager.layout" );

        m_InfoBox = TextWidget.Cast( layoutRoot.FindAnyWidget( "infotext" ) );
        m_BtnOk = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_ok" ) );

        m_InfoBox.SetText( "Enjoy the DayZ Community Offline Mode.\n \n \nThe current game build is still ALPHA!\nCrashes and bugs can occur.\nNot all 0.63 features are impletemented yet!\nProvide feedback on the game via feedback.bistudio.com\n \n \nCreated by Arkensor.\nContributions from DuhOneZ, Watchman and gallexme" );

        return layoutRoot;
    }

    override void OnShow()
    {
        super.OnShow();
    }

    override void OnHide()
    {
        super.OnHide();
    }

	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick(w, x, y, button);

		if ( w == m_BtnOk )
		{
			GetGame().GetUIManager().Back();

			return true;
		}

		return false;
	}

	override bool OnKeyPress( Widget w, int x, int y, int key )
	{
        if( key == KeyCode.KC_ESCAPE )
        {
            GetGame().GetUIManager().Back();
        }

		return false;
	}
}