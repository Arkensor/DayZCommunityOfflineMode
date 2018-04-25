class IconsContainer: ContainerBase
{
	int m_ColumnCount;
	void IconsContainer( ContainerBase parent ) 
	{
		//TODO: create layout for IconsContainer and set ARGB there
		m_ParentWidget.SetColor( ARGB( 175, 155, 155, 155 ) );
	}

	void SetColumnCount( int column_count )
	{
		m_ColumnCount = column_count;
	}

	int GetColumnCount()
	{
		return m_ColumnCount;
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.InventoryIconContainer;
	}
}
