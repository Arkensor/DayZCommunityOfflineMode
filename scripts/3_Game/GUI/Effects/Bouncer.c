// -----------------------------------------------------------
class Bouncer
{
	reference float amount;
	reference float speed;

	protected float m_orginal_width;
	protected float m_orginal_height;
	protected Widget m_root;
	protected ref AnimatorTimer m_anim;

	// -----------------------------------------------------------
	void Bouncer()
	{
		m_anim = new AnimatorTimer();
	}
	
	// -----------------------------------------------------------
	protected void Update()
	{
		if (m_root)
		{
			float p = amount * m_anim.GetValue();
			m_root.SetSize(m_orginal_width + (m_orginal_width * p), m_orginal_height - (m_orginal_height * p));
		}
	}
	
	// -----------------------------------------------------------
	void OnWidgetScriptInit(Widget w)
	{
		m_root = w;
		m_root.GetSize(m_orginal_width, m_orginal_height);
		m_anim.AnimateLoop(speed);
	}
};