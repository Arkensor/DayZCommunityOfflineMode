class InventoryContainerVicinity: InventoryContainer
{
	protected ref map<EntityAI, ref InventoryViewBase> m_views;
	protected ref map<EntityAI, ref InventoryContainerAttachmentCategories> m_viewsAttCat;
	protected Widget m_parent;
	
	void InventoryContainerVicinity(Widget parent, bool ownedByPlayer, bool isContentGrid = true )
	{
		m_views = new map<EntityAI, ref InventoryViewBase>;
		m_viewsAttCat = new map<EntityAI, ref InventoryContainerAttachmentCategories>;
		m_parent = parent;
	}

	void Init()
	{	
	}
	
	override void UpdateItems()
	{
		Man player = GetGame().GetPlayer();
		vector pos = player.GetPosition();
		vector dir = player.GetDirection();
		int i, j, c, x;
		EntityAI entity;
		Cargo cargo;
		Object obj;
		EntityAI e;
		ref array<Object> objects = new array<Object>;
		ref array<Cargo> proxyCargos = new array<Cargo>;
		GetGame().GetObjectsAtPosition(pos + (dir * 0.75), 1.0, objects, proxyCargos);
		InventoryManager manager = InventoryManager.GetInstance();

		c = proxyCargos.Count();

		for (i = 0; i < c; i++)
		{
			cargo = proxyCargos.Get(i);
			objects.Insert(cargo.GetParent());
		}
		
		// remove unnecesary entities
		c = m_views.Count();
		for (i = 0; i < c; i++)
		{
			entity = m_views.GetKey(i);
			if (objects.Find(entity) == INDEX_NOT_FOUND)
			{
				m_views.RemoveElement(i);
				c--;
				i--;
			}
		}
		
		c = m_viewsAttCat.Count();
		for (i = 0; i < c; i++)
		{
			entity = m_viewsAttCat.GetKey(i);
			if (objects.Find(entity) == INDEX_NOT_FOUND)
			{
				m_viewsAttCat.RemoveElement(i);
				c--;
				i--;
			}
		}

		// add new entities
		for (i = 0; i < objects.Count(); i++)
		{
			obj = objects.Get(i);

			if ( CanShowItemInInventory(PlayerBase.Cast( player ), obj) && !obj.IsInherited(PlayerBase) && !obj.IsInherited(ZombieBase) )
			{
				e = EntityAI.Cast(obj);
				if ( m_views.Contains(e) == false && m_viewsAttCat.Contains(e) == false )
				{	
					bool addEntity = true;
					if ( e.IsInherited(Building) )
					{
						if ( e.GetInventory().GetCargo() == NULL || e.GetSlotsCountCorrect() < 1 )
						{
							addEntity = false;
						}
					}

					if ( addEntity )
					{	
						InventoryViewBase view;
						if ( e.GetInventory().GetCargo() == NULL && e.GetSlotsCountCorrect() < 1 )
						{
							view = new InventoryViewGridContainer(this, e, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);
							m_views.Set(e, view );
						}
						else if ( GetGame().IsInventoryTreeLocked(EntityAI.Cast( obj ), player) && e.IsInherited(TentBase) )
						{
							view = new InventoryViewGridContainer(this, e, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);
							m_views.Set(e, view );
						}
						else
						{
							string config = "CfgVehicles " + e.GetType() + " GUIInventoryAttachmentsProps";
							
							if( GetGame().ConfigIsExisting( config ) )
							{
								InventoryContainerAttachmentCategories view_cat_att;
								view_cat_att = new InventoryContainerAttachmentCategories(m_parent, false, true );
								view_cat_att.Init(e);
								view_cat_att.SetHeaderText(e.GetDisplayName());	
								view_cat_att.UpdateItems();
								m_viewsAttCat.Set( e, view_cat_att );
							}
							else
							{
								view = new InventoryView(this, e, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);
							}
						}
					
						c = proxyCargos.Count();
						for (j = 0; j < c; j++)
						{
							cargo = proxyCargos.Get(j);
							if (cargo.GetParent() == e)
							{
								view.AddProxyCargo(cargo);
							}
						}
						
						if( view != NULL )
						{
							view.CreateLayout();
							m_views.Set(e, view );
						}
					}
				}
			}
		}

		// remove not accessible items
		c = m_views.Count();
		for (i = 0; i < c; i++) 
		{
			e = m_views.GetKey(i);
			if ( !CanShowItemInInventory( PlayerBase.Cast( player ), e) )
			{
				m_views.RemoveElement(i);
				c--;
				i--;
			}
		}
		
		c = m_viewsAttCat.Count();
		for (i = 0; i < c; i++) 
		{
			e = m_viewsAttCat.GetKey(i);
			if ( !CanShowItemInInventory(PlayerBase.Cast( player ), e) )
			{
				m_viewsAttCat.RemoveElement(i);
				c--;
				i--;
			}
		}
		
		// update all
		c = m_views.Count();
		for (i = 0; i < c; i++) 
		{
			m_views.GetElement(i).UpdateItems();
		}
		
		c = m_viewsAttCat.Count();
		for (i = 0; i < c; i++) 
		{
			m_viewsAttCat.GetElement(i).UpdateItems();
		}

		if (m_spacer) m_spacer.Update();
	}

	// ScriptedWidgetEventHandler override	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if (manager.IsDragging() == false || dragged_item == NULL) return false;
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if (dragged_item && !(player.GetHumanInventory().GetEntityInHands() != dragged_item || player.GetHumanInventory().CanRemoveEntityInHands()))
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
		
		if (manager.IsDragging() == false || dragged_item == NULL ) return false;
		
		// InventoryMenu menu = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
		// if (menu)
		// {
		// 	LogInfo(ToString( "DROPITEM3"), LogTemplates.TEMPLATE_JANOSIK );
		// 	menu.UpdatePlayerPreviewAnim( player.GetHumanInventory().GetEntityInHands() );
		// }	

		manager.DropItem(dragged_item);
		manager.Drop();
		return true;
	}
}
