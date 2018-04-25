class MainMenuButtonEffect : ScriptedWidgetEventHandler
{
	reference float speed;
	reference float amount;
	protected float m_textProportion;
	protected float m_textProportion2;
	protected ButtonWidget m_root;
	protected ref AnimatorTimer m_anim;
	
	// -----------------------------------------------------------
	void MainMenuButtonEffect()
	{
		if (GetGame()) GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(this.Update);
		m_anim = new AnimatorTimer();
	}
	
	// -----------------------------------------------------------
	void ~MainMenuButtonEffect()
	{
		if (GetGame()) GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(this.Update);
	}
	
	// -----------------------------------------------------------
	void OnWidgetScriptInit(ButtonWidget w)
	{
		m_root = w;
		m_root.SetHandler(this);
	}
	
	// -----------------------------------------------------------
	protected void Update(float tDelta)
	{
		m_anim.Tick(tDelta);
		float p = amount * m_anim.GetValue();
		//m_root.SetTextProportion( m_textProportion + (p * 0.5) );
		m_root.SetTextOffset( p * 4, 0 );
	
		float c = 1.0 - m_anim.GetValue();
		m_root.SetTextColor(ARGBF(1, 1, c, c));
	}
	
	// -----------------------------------------------------------
	override bool OnFocus(Widget w, int x, int y)
	{
		//if ( !m_anim.IsRunning() ) m_textProportion = m_root.GetTextProportion();
		if ( !m_anim.IsRunning() )
		{
			m_root.GetPos( m_textProportion, m_textProportion2 );
		}
		m_anim.Animate(1.0, speed);

		return false;
	}
	
	// -----------------------------------------------------------
	override bool OnFocusLost(Widget w, int x, int y)
	{
		m_anim.Animate(0.0, speed);
		return false;
	}
};