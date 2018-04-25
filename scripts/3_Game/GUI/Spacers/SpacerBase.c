// -----------------------------------------------------------
class SpacerBase : ScriptedWidgetEventHandler
{
	protected Widget m_root;
	protected int m_count;

	// -----------------------------------------------------------
	void OnWidgetScriptInit(Widget w)
	{
		m_root = w;
		m_count = 0;
	
		Widget child = m_root.GetChildren();
		while (child)
		{
			m_count++;
			child.SetFlags(WidgetFlags.EXACTPOS | WidgetFlags.EXACTSIZE, false);
			child = child.GetSibling();
		}
	
		m_root.SetHandler(this);
	}
	
	// -----------------------------------------------------------
	override bool OnUpdate(Widget w)
	{
		if (w == m_root) UpdateLayout();
		return false;
	}
	
	// -----------------------------------------------------------
	override bool OnChildAdd( Widget  w, Widget  child)
	{
		m_count++;
		child.SetFlags(WidgetFlags.EXACTPOS | WidgetFlags.EXACTSIZE, false);
		return false;
	}
	
	// -----------------------------------------------------------
	override bool OnChildRemove( Widget  w, Widget  child)
	{
		m_count--;
		return false;
	}
	
	// -----------------------------------------------------------
	protected int GetChildIndex(Widget w)
	{
		Widget child = m_root.GetChildren();
	
		int index = 0;
		while (child)
		{
			if (child == w) return index;
	
			index++;
			child = child.GetSibling();
		}
	
		return INDEX_NOT_FOUND;
	}
	
	// -----------------------------------------------------------
	void UpdateLayout()
	{
		if (m_count == 0) return;
	
		float width;
		float height;
		m_root.GetScreenSize(width, height);
	
		Widget child = m_root.GetChildren();
		
		int index = 0;
		while (child)
		{
			UpdateChild(child, width, height, index);
			index++;
			child = child.GetSibling();
		}
	}
	protected void UpdateChild(Widget child, float w, float h, int index) {}
};