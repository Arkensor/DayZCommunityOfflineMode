// -----------------------------------------------------------
class Rotator
{
	reference float speed;

	protected Widget m_root;	
	protected ref AnimatorTimer m_anim;

	// -----------------------------------------------------------
	void Rotator()
	{
		m_anim = new AnimatorTimer();
	}
	
	// -----------------------------------------------------------
	protected void Update()
	{
		if (m_root)
		{
			m_root.SetRotation(0, 0, m_anim.GetTargetValue() * Math.RAD2DEG);
		}
	}
	
	// -----------------------------------------------------------
	void OnWidgetScriptInit(Widget w)
	{
		m_root = w;
		m_anim.AnimateLoop(speed);
	}
};