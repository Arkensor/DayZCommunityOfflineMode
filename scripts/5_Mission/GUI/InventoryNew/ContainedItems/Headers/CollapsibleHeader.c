class CollapsibleHeader: Header
{
	void CollapsibleHeader( ContainerBase parent, string function_name ) { }

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.CollapsibleHeader;
	}
}
