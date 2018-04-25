// -----------------------------------------------------------
class AutoAspect : ScriptedWidgetEventHandler
{
	protected ImageWidget m_root;

	void Update()
	{
		int x, y;
		m_root.GetImageSize( 0, x, y );
		m_root.SetSize( x/100, y/100 );
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		m_root = ImageWidget.Cast(w);
		m_root.SetHandler(this);
		Update();
	}
	
	override bool OnUpdate( Widget  w )
	{
		Update();
		return false;
	}
};