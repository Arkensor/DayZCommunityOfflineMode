
class LogoutMenu extends UIScriptedMenu
{	
	private TextWidget m_logoutTimetext;
	private ButtonWidget m_bLogoutNow;
	private ButtonWidget m_bCancel;
	private int m_iTime;

	void LogoutMenu()
	{
		m_iTime = 0;
		g_Game.SetKeyboardHandle(this);
	}

	void ~LogoutMenu()
	{
		g_Game.SetKeyboardHandle(NULL);
	}
	
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_logout_dialog.layout");
		
		m_logoutTimetext = TextWidget.Cast( layoutRoot.FindAnyWidget("logoutTimeText") );
		m_bLogoutNow = ButtonWidget.Cast( layoutRoot.FindAnyWidget("bLogoutNow") );
		m_bCancel = ButtonWidget.Cast( layoutRoot.FindAnyWidget("bCancel") );

		return layoutRoot;
	}

	void Show()
	{
		if (layoutRoot)
			layoutRoot.Show(true);
	}
	
	void Hide()
	{
		if (layoutRoot)
			layoutRoot.Show(false);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		if (w == m_bLogoutNow)
		{
			GetGame().GetMission().AbortMission();
			
			return true;
		}
		else if (w == m_bCancel)
		{
			Cancel();
			return true;
		}

		return false;
	}

	override bool OnKeyDown(Widget w, int x, int y, int key)
	{
		super.OnKeyDown( w, x, y, key);
		
		switch(key)
		{
			// TODO: just temporary until new inputs are done
			case KeyCode.KC_W:
			case KeyCode.KC_S:
			case KeyCode.KC_A:
			case KeyCode.KC_D:
			case KeyCode.KC_UP:
			case KeyCode.KC_DOWN:
			case KeyCode.KC_LEFT:
			case KeyCode.KC_RIGHT:
			case KeyCode.KC_ESCAPE:
				Cancel();
		}
		
		return true;
	}
	
	void SetTimeLoading()
	{
		m_logoutTimetext.SetText(" ");
	}
	
	void SetTime(int time)
	{
		m_iTime = time;
		m_logoutTimetext.SetText(m_iTime.ToString());
	}
	
	void UpdateTime()
	{
		if (m_iTime > 0)
		{
			m_iTime -= 1;
			m_logoutTimetext.SetText(m_iTime.ToString());	
		}
		else
		{
			Exit();
		}
	}

	void Exit()
	{		
		// exit menu and logout screen
		GetGame().GetMission().Continue();	

		// stop updating of logout screen
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.UpdateTime);

		// go back to main menu
		GetGame().GetMission().AbortMission();
	}

	void Cancel()
	{
		GetGame().GetMission().Continue();	
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.UpdateTime);
		
		// request logout cancel from server
		GetGame().LogoutRequestCancel();
	}
}