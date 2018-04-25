class InventoryContainerHands: InventoryContainer
{
	protected ref InventoryViewBase m_view;
	
	void InventoryContainerHands(Widget parent, bool ownedByPlayer, bool isContentGrid = true )
	{
	}

	void Init()
	{
	}
	
	override void UpdateItems()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( player == NULL )
		{
			return;
		}
		
		EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();
		
		if ( item_in_hands == NULL )
		{
			m_view = new InventoryViewHands(this, item_in_hands, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);	
			m_view.CreateLayout();
		}

		if ( item_in_hands && m_view && m_view.GetEntity() != item_in_hands)	
		{
			m_view = NULL;
		}
		
		if ( item_in_hands && m_view == NULL )
		{
			m_view = new InventoryViewHands(this, item_in_hands, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);	
			m_view.CreateLayout();
		}
		
		if (m_view) m_view.UpdateItems();
		if (m_spacer) m_spacer.Update();
	}

	// InventoryContainer override
	override Widget CreateLayout(Widget parent)
	{
		return GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryContainerHands.layout", parent);
	}
	
	override void OnItemDoubleClick(InventoryViewBase view, InventoryItem item)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( player == NULL )
		{
			return;
		}
		
		InventoryManager manager = InventoryManager.GetInstance();
		EntityAI item_in_hands = player.GetHumanInventory().GetEntityInHands();
		
		if (item_in_hands == item)
		{
			manager.TakeToHands(NULL);
		}
		else
		{
			super.OnItemDoubleClick(view, item);
		}
	}

	// ScriptedWidgetEventHandler override	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if (manager.IsDragging() == false || dragged_item == NULL) return false;
		
		if (dragged_item && dragged_item.GetInventory().CanRemoveEntity() == false)
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
		}
		else
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
		}
		return true;
	}

	override bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if (manager.IsDragging() == false || dragged_item == NULL) return false;
		
		if ( player.GetHumanInventory().CanAddEntityInHands(dragged_item) && dragged_item.GetInventory().CanRemoveEntity() )
		{
			manager.TakeToHands(dragged_item);
			manager.Drop();	
		}
		
		return true;
	}
}
