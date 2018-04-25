class CrossHair
{
	protected string					m_Name;
	protected bool 						m_Faded;
	protected bool 						m_Current;

	protected Widget 					m_Widget;
	protected ref WidgetFadeTimer		m_FadeTimer;
	
	void CrossHair(Widget w)
	{
		m_Widget = w;
		m_Name = w.GetName();
		m_Faded = true;
		m_Current = false;
		
		m_FadeTimer = new WidgetFadeTimer;
	}

	void ~CrossHair() {}

	string GetName()
	{
		return m_Name;
	}

	bool IsCurrent()
	{
		return m_Current;
	}

	bool IsFaded()
	{
		return m_Faded;
	}

	void FadeIn(float fadeTime)
	{
		m_FadeTimer.FadeIn(m_Widget, fadeTime);
		m_Faded = false;
		m_Current = true;
		m_Widget.Show(true);
	}
	
	void FadeOut(float fadeTime)
	{
		m_FadeTimer.FadeOut(m_Widget, fadeTime);
		m_Faded = true;
		m_Current = false;
		m_Widget.Show(false);
	}
}

class CrossHairSelector extends ScriptedWidgetEventHandler
{
	reference float							FadeIn_Time;
	reference float							FadeOut_Time;

	protected PlayerBase 					m_Player;
	protected ActionManagerBase 			m_AM;
	
	protected Widget					m_Root;

	protected ref set<ref CrossHair>	m_CrossHairs;

	void CrossHairSelector()
	{
		m_Player 		= NULL;
		m_AM 			= NULL;
		m_CrossHairs 	= new set<ref CrossHair>;

		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
		
	void ~CrossHairSelector()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	protected void Init()
	{
		m_CrossHairs.Clear();
				
		Widget child = m_Root.GetChildren();
		while(child)
		{
			m_CrossHairs.Insert(CrossHair(child));
			child = child.GetSibling();
		}
	}
	
	protected void OnWidgetScriptInit(Widget w)
	{
		m_Root = w;
		m_Root.SetHandler(this);
		Init();
		m_Root.Update();
	}
	
	protected void Update()
	{
		//! don't show crosshair if it's disabled in profile or from server
		if(!g_Game.GetProfileOption(EDayZProfilesOptions.CROSSHAIR) || g_Game.GetWorld().IsCrosshairDisabled())
		{
			if(GetCurrentCrossHair())
				ShowCrossHair(null);
			
			return;
		};

		if(m_Player && !m_Player.IsAlive()) // handle respawn
		{
			m_Player = NULL;
			m_AM = NULL;
		}
		if(!m_Player) GetPlayer();
		if(!m_AM) GetActionManager();
		
		if(m_Player && m_Player.IsPlayerSelected() && !GetGame().IsInventoryOpen())
		{
			SelectCrossHair();
		}
	}
	
	// getters
 	protected void GetPlayer()
	{
		Class.CastTo(m_Player, GetGame().GetPlayer());
	}

	protected void GetActionManager()
	{
		if( m_Player && m_Player.IsPlayerSelected() )
		{
			Class.CastTo(m_AM, m_Player.GetActionManager());
		}
		else
			m_AM = NULL;
	}
	
	protected CrossHair GetCrossHairByName(string widgetName)
	{
		for(int i = 0; i < m_CrossHairs.Count(); i++)
		{
			if(m_CrossHairs.Get(i).GetName() == widgetName)
				return m_CrossHairs.Get(i);
		}

		return null;
	}

	protected CrossHair GetCurrentCrossHair()
	{
		for(int i = 0; i < m_CrossHairs.Count(); i++)
		{
			if(m_CrossHairs.Get(i).IsCurrent())
				return m_CrossHairs.Get(i);
		}

		return null;
	}
	
	protected void SelectCrossHair()
	{
		HumanInputController hic = m_Player.GetInputController();
		ActionBase action = m_AM.GetRunningAction();

		if ( m_Player.IsFireWeaponRaised() && !m_Player.IsInIronsights() && !hic.CameraIsFreeLook() )	// Firearms
			ShowCrossHair(GetCrossHairByName("cross_128x128"));
			//ShowCrossHair(GetCrossHairByName("cross_stretched"));
		else if (action && action.GetActionCategory() == AC_CONTINUOUS) // On Continuous Actions
		{
			int actionState = m_AM.GetActionState(action);

			if ( actionState != UA_NONE )
				ShowCrossHair(null);
		}
		else if ( m_Player.IsRaised() || m_Player.GetCommand_Melee() ) // Melee
			ShowCrossHair(null);
		else
			ShowCrossHair(GetCrossHairByName("dot"));
	}
	
	protected void ShowCrossHair(CrossHair crossHair)
	{
		//! no crosshair + clean + fade the previous
		if(!crossHair)
		{
			if(GetCurrentCrossHair())
				GetCurrentCrossHair().FadeOut(FadeOut_Time);
			
			return;
		}
		else //! fade out prev crosshair
		{
			if(GetCurrentCrossHair() && GetCurrentCrossHair() != crossHair)
				GetCurrentCrossHair().FadeOut(FadeOut_Time);
		}
		
		//! fadein the new one
		if(!crossHair.IsCurrent() && crossHair.IsFaded())
			crossHair.FadeIn(FadeIn_Time);
	}
}