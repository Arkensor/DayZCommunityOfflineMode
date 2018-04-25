class InventoryMenuNew extends UIScriptedMenu
{
	Widget main_container;
	ref Inventory m_Inventory;
	private ref ContextMenu m_context_menu;
	protected bool m_IsOpened;
	protected bool m_OnlyFirstTime;

	void InventoryMenuNew()
	{
		main_container = GetGame().GetWorkspace().CreateWidgets( WidgetLayoutName.Inventory );
		m_Inventory = new Inventory(NULL);
		m_Inventory.Reset();
		m_Inventory.UpdateInterval();
		m_context_menu = new ContextMenu;
		main_container.Show( false );
	}

	override Widget Init()
	{
		m_Inventory.Init();
		m_context_menu.Init(layoutRoot);
		return main_container;
	}
	
	void RefreshQuickbar()
	{
		m_Inventory.RefreshQuickbar();
	}
	
	void RefreshQuantity( EntityAI item_to_refresh )
	{
		m_Inventory.RefreshQuantity( item_to_refresh );
	}
	
	void RefreshItemPosition( EntityAI item_to_refresh )
	{
		m_Inventory.RefreshItemPosition( item_to_refresh );
	}
	
	override ContextMenu GetContextMenu()
	{
		return m_context_menu;
	}

	void InitContainers(EntityAI target)
	{
	}

	override void OnShow()
	{
		#ifdef PLATFORM_XBOX
			if( !m_OnlyFirstTime )
			{
				m_OnlyFirstTime = true;
				GetGame().GetUIManager().HideScriptedMenu( this );
			}
			else
			{
				super.OnShow();
				m_IsOpened = true;
				PPEffects.SetBlurInventory(1);
				main_container.Show( false );
				m_Inventory.OnShow();
			}
		#else
			super.OnShow();
			m_IsOpened = true;
			PPEffects.SetBlurInventory(1);
			main_container.Show( false );
			m_Inventory.OnShow();
		#endif
	}

	int Reset2asd()
	{
		//Print("testfunc");
	}

	int Reset()
	{
		m_Inventory.Reset();
		return 1;
	}
	
	bool IsOpened()
	{
		return m_IsOpened;
	}

	override void OnHide()
	{
		super.OnHide();
		m_IsOpened = false;
		PPEffects.SetBlurInventory(0);
		if(m_Inventory)
		m_Inventory.OnHide();
	}
}
