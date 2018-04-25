class ProjectedCrosshair extends ScriptedWidgetEventHandler
{
	private Widget		m_Root;
	private vector		m_Position;
	private bool 			m_Visible;
	private bool			m_Debug;
	
	private PlayerBase	m_Player;
	private Weapon		m_Weapon;		

	void ProjectedCrosshair()
	{
		m_Player = NULL;
		m_Weapon = NULL;
		m_Visible = false;
		m_Debug = false;

		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(this.Update);
	}

	void ~ProjectedCrosshair()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(this.Update);
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		m_Root = w;
		m_Root.SetHandler(this);
		m_Root.Update();
	}
	
	//! Update
	private void Update()
	{
#ifdef DEVELOPER
		m_Debug = DiagMenu.GetBool( DiagMenuIDs.DM_WEAPON_DEBUG_ENABLE );
#endif
		if(!m_Debug) return;
		if(!m_Player) GetPlayer();

		if( m_Player && m_Player.IsPlayerSelected() && m_Player.IsRaised() && !m_Player.IsInIronsights() && !GetGame().IsInventoryOpen() )
		{
			float sx, sy;

			GetCrosshairPosition();
			vector screenSpace = GetGame().GetScreenPos(m_Position);

			m_Root.GetSize(sx, sy);
			screenSpace[0] = screenSpace[0] - sx/2;
			screenSpace[1] = screenSpace[1] - sy/2;
			
			m_Root.SetPos(screenSpace[0], screenSpace[1]);
			m_Root.Show(m_Visible);
		}
		else
		{
			m_Root.Show(false);
			m_Position = vector.Zero;
		}
	}

	// getters
  	private void GetPlayer()
	{
		Class.CastTo(m_Player, GetGame().GetPlayer());
	}

	private void GetCrosshairPosition()
	{
		m_Visible = false;
		ItemBase itemInHands;
		itemInHands = m_Player.GetItemInHands();
		if( itemInHands && itemInHands.IsWeapon() )
		{
			if( Class.CastTo(m_Weapon, itemInHands) )
			{
				//m_Visible = m_Weapon.GetProjectedCursorPos3d(m_Position);
			}
		}
	}
};