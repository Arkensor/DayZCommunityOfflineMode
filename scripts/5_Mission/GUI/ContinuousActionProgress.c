class ContinuousActionProgress extends ScriptedWidgetEventHandler
{
	reference string				RadialBarWidgetName;

	protected PlayerBase 			m_Player;
	protected ActionBase			m_Action;
	protected int 					m_ActionState;
	protected ActionManagerBase		m_AM;
	
	protected ref WidgetFadeTimer	m_FadeTimer;
	protected bool					m_Faded;
	
	protected float 				m_InitProgress;
	protected float					m_Speed;

	protected Widget				m_Root;
	protected Widget				m_RadialWidget;
	protected ImageWidget 			m_LoaderImage;
	ref RadialProgressBar			m_Radial;

	void ContinuousActionProgress()
	{
		m_Action = null;
		m_ActionState = -1;
		m_AM = null;
		m_RadialWidget = null;
		m_LoaderImage = null;
		m_Radial = null;
		m_Speed = 0;
		m_InitProgress = 100;
		
		m_FadeTimer = new WidgetFadeTimer;
		m_Faded = true;

		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~ContinuousActionProgress()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}

	protected void OnWidgetScriptInit(Widget w)
	{
		m_Root = w;
		m_Root.SetHandler(this);
		m_Root.Show(false);

		m_RadialWidget = m_Root.FindAnyWidget("PBRadial1");
		m_LoaderImage = ImageWidget.Cast( m_Root.FindAnyWidget("cap_init_loader") );
		if(m_RadialWidget)
			m_RadialWidget.GetScript(m_Radial);
		m_Root.Update();
	}

	protected void Update()
	{
		if(m_Player && !m_Player.IsAlive()) // handle respawn
		{
			m_Player = null;
			m_AM = null;
		}
		if(!m_Player) GetPlayer();
		if(!m_AM) GetActionManager();

		GetActions();

		if(m_Action && m_ActionState != UA_NONE && !GetGame().IsInventoryOpen())
		{
			if(m_ActionState == UA_INITIALIZE || m_ActionState == UA_AM_PENDING)
			{
				m_Speed += 0.02;
				m_LoaderImage.SetRotation(0, 0, m_Speed * Math.RAD2DEG);
				m_LoaderImage.Show(true);
			}
			else
			{
				m_Speed = 0.0;
				m_LoaderImage.SetRotation(0, 0, 0);
				m_LoaderImage.Show(false);
			}

			if(m_ActionState == UA_PROCESSING)
			{
				m_InitProgress = 100;
				m_LoaderImage.SetRotation(0, 0, 0);
				SetProgress(Math.AbsFloat(m_Action.GetActionComponentProgress() * 100));
			}
			m_Root.Show(true);
		}
		else
		{
			m_Speed = 0.0;
			m_Root.Show(false);
			m_LoaderImage.Show(false);
			SetProgress(0.0);
			m_LoaderImage.SetRotation(0, 0, 0);
		}
	}

	// getters
  	private void GetPlayer()
	{
		Class.CastTo(m_Player, GetGame().GetPlayer());
	}

	private void GetActionManager()
	{
		if( m_Player && m_Player.IsPlayerSelected() )
		{
			Class.CastTo(m_AM, m_Player.GetActionManager());
		}
		else
			m_AM = null;
	}

  	private void GetActions()
	{
		if(!m_AM) return;

		m_Action = null;
		m_ActionState = -1;
		m_Action = m_AM.GetRunningAction();
		if (m_Action && m_Action.GetActionCategory() == AC_CONTINUOUS)
			m_ActionState = m_AM.GetActionState(m_Action);
		else
			m_Action = null;
	}

	private void SetProgress(float progress)
	{
		if(m_Radial)
			m_Radial.SetProgress(progress);
	}
}