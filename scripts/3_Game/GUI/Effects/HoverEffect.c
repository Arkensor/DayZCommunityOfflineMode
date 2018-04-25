// -----------------------------------------------------------
class HoverEffect : ScriptedWidgetEventHandler
{
	reference float speed;
	reference float amount;
	protected float m_orginal_width;
	protected float m_orginal_height;
	protected Widget m_root;
	protected ref AnimatorTimer m_anim;
	
	void HoverEffect()
	{
		m_anim = new AnimatorTimer();
	}
	
	// -----------------------------------------------------------
	void OnWidgetScriptInit(Widget w)
	{
		m_root = w;
		m_root.SetHandler(this);
	}
	
	// -----------------------------------------------------------
	protected void Update()
	{
		float p = amount * m_anim.GetValue();
		m_root.SetSize(m_orginal_width + (m_orginal_width * p), m_orginal_height + (m_orginal_height * p));
	
		float c = 1.0 - (0.5 * m_anim.GetValue());
		m_root.SetColor(ARGBF(1, 1, c, c));
	}
	
	// -----------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( !m_anim.IsRunning() ) m_root.GetSize(m_orginal_width, m_orginal_height);
		m_anim.Animate(1.0, speed);
		
		return false;
	}
	
	// -----------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		m_anim.Animate(0.0, speed);
		return false;
	}

};