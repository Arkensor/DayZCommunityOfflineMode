class AttachmentCategoriesIconsContainer: Container
{
  
  ref IconsContainer m_ItemsCont;
	void AttachmentCategoriesIconsContainer( ContainerBase parent )
	{
    m_ItemsCont = new IconsContainer(this);
    m_Body.Insert(m_ItemsCont);
    for(int k = 0; k < 7; k++)
    {
      WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_ItemsCont.GetMainPanel().FindAnyWidget("Icon"+k),  m_Parent, "OnDropReceivedFromHeader" );
      WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_ItemsCont.GetMainPanel().FindAnyWidget("GhostSlot"+k),  m_Parent, "OnDropReceivedFromHeader" );
      WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_ItemsCont.GetMainPanel().FindAnyWidget("PanelWidget"+k),  m_Parent, "OnDropReceivedFromHeader" );
      
      WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_ItemsCont.GetMainPanel().FindAnyWidget("Icon"+k),  m_Parent, "DraggingOverHeader" );
      WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_ItemsCont.GetMainPanel().FindAnyWidget("GhostSlot"+k),  m_Parent, "DraggingOverHeader" );
      WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_ItemsCont.GetMainPanel().FindAnyWidget("PanelWidget"+k),  m_Parent, "DraggingOverHeader" );
    }
  }
  
  override void SetLayoutName()
  {
    m_LayoutName = WidgetLayoutName.ClosableContainer;
  }
  
  IconsContainer GetIconsContainer()
  {
    return m_ItemsCont;
  }
}
