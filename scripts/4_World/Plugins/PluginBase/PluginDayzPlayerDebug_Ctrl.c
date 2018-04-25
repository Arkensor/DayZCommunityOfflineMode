

// *************************************************************************************
// ! PluginDayzPlayerDebug_Ctrl
// *************************************************************************************
class PluginDayzPlayerDebug_Ctrl
{	
	Widget 									m_MainWnd;

	XComboBoxWidget 						m_ForceStance;
	// 

	int 									m_ApplyStanceChange;


	//---------------------------------------------------
  // gui stuff

	void PluginDayzPlayerDebug_Ctrl(Widget pMainWnd)
	{
		m_MainWnd 			= pMainWnd;
		m_ApplyStanceChange = -2;

		CreateModuleWidgets();
	}
	
	
	void ~PluginDayzPlayerDebug_Ctrl()
	{
		DestroyModuleWidgets();
	}
	
	
	void CreateModuleWidgets()
	{
		m_ForceStance = XComboBoxWidget.Cast( m_MainWnd.FindAnyWidget("StanceV") );

	}
	
	void DestroyModuleWidgets()
	{
	}

	

	//---------------------------------------------------
    // main update

	
	/* void Tick()
	{
		DayZPlayer player = DayZPlayer.Cast( GetGame().GetPlayer() );
		if (!player)
		{
			return;
		}
		
		if (player)
		{
		}
	} */
	

	//---------------------------------------------------
    // window ui clicks

	//! buttons clicks
	bool	OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_ForceStance)
		{
			Print("Stance changed");
			m_ApplyStanceChange = m_ForceStance.GetCurrentItem() - 1;
			return true;
		}
		
	
		return false;
	}


	//---------------------------------------------------
    // Global handler to handle commands from player

	void 	CommandHandler()
	{
		if (m_ApplyStanceChange != -2)
		{
			DayZPlayer player = DayZPlayer.Cast( GetGame().GetPlayer() );
			if (!player)
			{	
				return;
			}
			
			HumanCommandMove cm = player.GetCommand_Move();
			if (cm)
			{
				cm.ForceStance(m_ApplyStanceChange);
				m_ApplyStanceChange = -2;
			}
		}

	}
}
