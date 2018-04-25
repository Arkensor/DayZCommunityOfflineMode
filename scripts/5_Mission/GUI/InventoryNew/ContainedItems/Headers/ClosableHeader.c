class ClosableHeader: Header
{
	void ClosableHeader( ContainerBase parent, string function_name ) { }

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.ClosableHeader;
	}
}
