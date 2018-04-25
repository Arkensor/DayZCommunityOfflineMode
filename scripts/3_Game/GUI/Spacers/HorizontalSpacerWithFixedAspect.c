// -----------------------------------------------------------
class HorizontalSpacerWithFixedAspect: ScriptedWidgetEventHandler
{
	protected Widget m_root;
	reference int border;
	reference int gap;
	reference float coef;
	float itemWidth;
	float itemHeight;
	
	// -----------------------------------------------------------
	void OnWidgetScriptInit(Widget w)
	{
		m_root = w;
		m_root.SetHandler(this);
	}
	
	// -----------------------------------------------------------
	override bool OnUpdate(Widget w)
	{
		if (w == m_root) UpdateLayout();
		return false;
	}
	
	protected void UpdateLayout()
	{
		Widget child = m_root.GetChildren();
		
			int index = 0;
			while (child)
			{
				if( index == 0 )
				{
					child.GetScreenSize(itemWidth, itemHeight);
				}
				else
				{
					child.SetFlags( WidgetFlags.EXACTPOS, false);
					child.SetPos(itemWidth+(itemWidth*coef), 0);
				}
				
				index++;
				child = child.GetSibling();
		}
	}
};