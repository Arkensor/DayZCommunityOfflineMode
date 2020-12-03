class WelcomeMenu extends UIScriptedMenu
{
    MultilineTextWidget m_InfoBox;
    ButtonWidget m_BtnOk;

	void WelcomeMenu()
	{
	}	
	
	void ~WelcomeMenu()
	{
	}

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Namalsk\\core\\modules\\ComMenu\\gui\\layouts\\WelcomeMenu.layout" );

        m_InfoBox = TextWidget.Cast( layoutRoot.FindAnyWidget( "infotext" ) );
        m_BtnOk = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_ok" ) );

		string infotext;
		
		infotext = "Enjoy the DayZ Community Sandbox Mode.";
		infotext += "\n \n \nThe current game build is still ALPHA!";
		infotext += "\nCrashes and bugs can occur.";
		infotext += "\nNot all features are implemented or working yet!";
		infotext += "\nProvide feedback on the game via feedback.dayz.com";
		infotext += "\n \n \nCreated and Managed by Arkensor, DannyDog and Jacob_Mango.";
		infotext += "\n \n \nContributions from:";
		infotext += "\n DuhOneZ, Watchman, n8m4re, wriley and gallexme.";

        m_InfoBox.SetText( infotext );

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
			Close();

			return true;
		}

		return false;
	}

	override bool OnKeyPress( Widget w, int x, int y, int key )
	{
        if( key == KeyCode.KC_ESCAPE )
        {
			Close();
        }

		return false;
	}
}
