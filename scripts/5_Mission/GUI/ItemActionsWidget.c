class ItemActionsWidget extends ScriptedWidgetEventHandler
{
	protected PlayerBase 				m_Player;
	protected EntityAI 					m_EntityInHands;
	protected ActionBase				m_Single;
	protected ActionBase				m_Continuous;
	protected ActionManagerBase		 	m_AM;
	
	protected bool 						m_HealthEnabled;
	protected bool						m_QuantityEnabled;
	
	protected ref WidgetFadeTimer		m_FadeTimer;
	protected bool						m_Faded;

	protected Widget					m_Root;
	protected Widget					m_Container;
	protected Widget 					m_ItemLeft;
	ref AutoHeightSpacer				m_MainSpacer;
	ref AutoHeightSpacer				m_HealthQuantitySpacer;

	void ItemActionsWidget()
	{
		m_Single = NULL;
		m_Continuous = NULL;
		m_AM = NULL;
		
		m_FadeTimer = new WidgetFadeTimer;
		m_Faded = true;
		
		m_HealthEnabled = true;
		m_QuantityEnabled = true;

		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~ItemActionsWidget()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	void SetSingleXboxIcon( string imageset_name, string image_name )
	{
		SetXboxIcon( "ia_single", imageset_name, image_name );
	}
	
	void SetContinuousXboxIcon( string imageset_name, string image_name )
	{
		SetXboxIcon( "ia_continuous", imageset_name, image_name );
	}
	
	protected void SetXboxIcon( string name, string imageset_name, string image_name )
	{
		ImageWidget w = ImageWidget.Cast( m_Root.FindAnyWidget( name + "_btn_icon_xbox" ) );
		w.LoadImageFile( 0, "set:"+ imageset_name + " image:" + image_name );
	}

	protected void ShowXboxHidePCIcons( string widget, bool show_xbox_icon )
	{
		m_Root.FindAnyWidget( widget + "_btn_icon_xbox" ).Show( show_xbox_icon );
		m_Root.FindAnyWidget( widget + "_btn_icon" ).Show( !show_xbox_icon );
	}

	protected void OnWidgetScriptInit(Widget w)
	{
		m_Root = w;
		m_Root.SetHandler(this);
		m_Root.Show(false);

		m_Container = w.FindAnyWidget("ia_container");
		m_Container.GetScript(m_MainSpacer);
		m_ItemLeft = w.FindAnyWidget("ia_item_left");
		m_ItemLeft.GetScript(m_HealthQuantitySpacer);
		m_Root.Update();
		m_MainSpacer.Update();
		m_HealthQuantitySpacer.Update();
		
#ifdef PLATFORM_XBOX
		SetSingleXboxIcon("xbox_buttons", "RT");
		SetContinuousXboxIcon("xbox_buttons", "RT");
#endif
#ifdef PLATFORM_PS4
		SetSingleXboxIcon("playstation_buttons", "R2");
		SetContinuousXboxIcon("playstation_buttons", "R2");
#endif
	}

	protected void BuildCursor()
	{
		int health = -1;
		int q_type = 0;
		int q_min, q_max = -1;
		int q_chamber, q_mag = 0;
		float q_cur = -1.0;

		// item health
		health = GetItemHealth();
		SetItemHealth(health, "ia_item", "ia_item_health_mark", m_HealthEnabled);
		
		// quantity
		if(m_EntityInHands && m_EntityInHands.IsWeapon())
		{
			GetWeaponQuantity(q_chamber, q_mag);
			SetWeaponQuantity(q_chamber, q_mag, "ia_item", "ia_item_quantity_pb", "ia_item_quantity_text", m_QuantityEnabled);
			SetWeaponModeAndZeroing("ia_item_subdesc", "ia_item_subdesc_up", "ia_item_subdesc_down", true);
		}
		else
		{
			GetItemQuantity(q_type, q_cur, q_min, q_max);
			SetItemQuantity(q_type, q_cur, q_min, q_max, "ia_item", "ia_item_quantity_pb", "ia_item_quantity_text", m_QuantityEnabled);
			SetWeaponModeAndZeroing("ia_item_subdesc", "", "", false);
		}
	
		SetItemDesc(m_EntityInHands, GetItemDesc(m_EntityInHands), "ia_item", "ia_item_desc");
		SetActionWidget(m_Single, GetActionDesc(m_Single), "ia_single", "ia_single_action_name");
		SetActionWidget(m_Continuous, GetActionDesc(m_Continuous), "ia_continuous", "ia_continuous_action_name");
		
		m_MainSpacer.Update();
		m_HealthQuantitySpacer.Update();
	}
		
	protected void Update()
	{
		//! don't show crosshair if it's disabled in profile
		if(!g_Game.GetProfileOption(EDayZProfilesOptions.HUD))
		{
			m_Root.Show(false);
			return;
		};

		if(m_Player && !m_Player.IsAlive()) // handle respawn
		{
			m_Player = NULL;
			m_AM = NULL;
		}
		if(!m_Player) GetPlayer();
		if(!m_AM) GetActionManager();

		GetEntityInHands();
		GetActions();

		if((m_EntityInHands || m_Single || m_Continuous) && !GetGame().IsInventoryOpen())
		{
			BuildCursor();
			m_Root.Show(true);
		}
		else
		{
			m_Root.Show(false);
		}
	}
	
	// getters
  private void GetPlayer()
	{
		Class.CastTo(m_Player, GetGame().GetPlayer());
	}

	private void GetActionManager()
	{
		if( m_Player && m_Player.IsPlayerSelected() )
			Class.CastTo(m_AM, m_Player.GetActionManager());
		else
			m_AM = NULL;
	}

  private void GetActions()
	{
		m_Single = NULL;
		m_Continuous = NULL;

		if(!m_AM) return;
		//if(!m_EntityInHands) return false;
		if(m_Player.IsSprinting()) return;
				
		m_Single = m_AM.GetSingleUseAction();
		m_Continuous = m_AM.GetContinuousAction();
	}

	private void GetEntityInHands()
	{
		if(!m_Player) return;
	
		m_EntityInHands = m_Player.GetHumanInventory().GetEntityInHands();
	}
	
	private string GetActionDesc(ActionBase action)
	{
		string desc = "";
		if (action && action.GetText())
		{
			desc = action.GetText();
			return desc;
		}
		return desc;
	}
	
	private string GetItemDesc(EntityAI entity)
	{
		string desc = "";

		if(m_EntityInHands)
		{
			desc = m_EntityInHands.GetDisplayName();
			return desc;
		}
		return desc;
	}
	
	private int GetItemHealth()
	{
		int health = -1;

		if(m_EntityInHands)
		{
			health = m_EntityInHands.GetHealthLabel();
			return health;
		}
		
		return health;
	}

	private void GetItemQuantity(out int q_type, out float q_cur, out int q_min, out int q_max)
	{
		InventoryItem item = NULL;

		if( m_EntityInHands )
		{
			Class.CastTo(item, m_EntityInHands);
			q_type = QuantityConversions.HasItemQuantity(m_EntityInHands);
			if(q_type > 0)
			{
				if(q_type == QUANTITY_PROGRESS)
				{
					QuantityConversions.GetItemQuantity(item, q_cur, q_min, q_max);

					q_cur = (q_cur / q_max);
					// to percents
					q_min = 0;
					q_cur = q_cur * 100;
					q_max = 100;
				}
				else
				{
					QuantityConversions.GetItemQuantity(item, q_cur, q_min, q_max);
				}
			}
		}
	}
	
	private void GetWeaponQuantity(out int q_chamber, out int q_mag)
	{
		q_chamber = 0;
		q_mag = 0;

		Weapon_Base wpn;
		Magazine mag

		if ( Class.CastTo(wpn, m_EntityInHands ) )
		{
			int mi = wpn.GetCurrentMuzzle();
			mag = wpn.GetMagazine(mi);
			if (!wpn.IsChamberFiredOut(mi) && !wpn.IsChamberEmpty(mi))
			{
				q_chamber = 1;
			}
			
			if (mag)
			{
				q_mag = mag.GetAmmoCount();
			}
			else if (wpn.GetChamberCartridgeCount(mi) > 1)
			{
				q_mag = wpn.GetChamberCartridgeCount(mi) - 1;
			}
		}
	}
	
	// setters
	private void SetItemDesc(EntityAI entity, string descText, string itemWidget, string descWidget)
	{
		Widget widget;
		
		widget = m_Root.FindAnyWidget(itemWidget);
		
		if(entity)
		{
			descText.ToUpper();
			TextWidget itemName;
			Class.CastTo(itemName, widget.FindAnyWidget(descWidget));
			itemName.SetText(descText);
			widget.Show(true);
		}
		else
			widget.Show(false);
	}
	
	private void SetItemHealth(int health, string itemWidget, string healthWidget, bool enabled)
	{
		Widget widget;
		
		widget = m_Root.FindAnyWidget(itemWidget);
		
		if(enabled)
		{
			ImageWidget healthMark;
			Class.CastTo(healthMark, widget.FindAnyWidget(healthWidget));

			switch (health)
			{
				case -1 :
					healthMark.GetParent().Show(false);
					break;
				case STATE_PRISTINE :
					healthMark.SetColor(COLOR_PRISTINE);
					healthMark.SetAlpha(0.5);
					healthMark.GetParent().Show(true);
					break;
				case STATE_WORN :
					healthMark.SetColor(COLOR_WORN);
					healthMark.SetAlpha(0.5);
					healthMark.GetParent().Show(true);
					break;
				case STATE_DAMAGED :
					healthMark.SetColor(COLOR_DAMAGED);
					healthMark.SetAlpha(0.5);
					healthMark.GetParent().Show(true);
					break;
				case STATE_BADLY_DAMAGED:
					healthMark.SetColor(COLOR_BADLY_DAMAGED);
					healthMark.SetAlpha(0.5);
					healthMark.GetParent().Show(true);
					break;
				case STATE_RUINED :
					healthMark.SetColor(COLOR_RUINED);
					healthMark.SetAlpha(0.5);
					healthMark.GetParent().Show(true);
					break;
				default :
					healthMark.SetColor(0x00FFFFFF);
					healthMark.SetAlpha(0.5);
					healthMark.GetParent().Show(true);
					break;			
			}
			
			widget.Show(true);
		}
		else
			widget.Show(false);
	}
	
	private void SetItemQuantity(int type, float current, int min, int max, string itemWidget, string quantityPBWidget, string quantityTextWidget, bool enabled )
	{
		Widget widget;
		
		widget = m_Root.FindAnyWidget(itemWidget);
		
		if(enabled)
		{
			ProgressBarWidget progressBar;
			TextWidget textWidget;
			Class.CastTo(progressBar, widget.FindAnyWidget(quantityPBWidget));
			Class.CastTo(textWidget, widget.FindAnyWidget(quantityTextWidget));

			//check for volume vs. count and display progressbar or "bubble" with exact count/max text
			switch (type)
			{
				case QUANTITY_HIDDEN :
					progressBar.Show(false);
					textWidget.Show(false);
					textWidget.GetParent().Show(false);
					break;
				case QUANTITY_COUNT :
					string qty_text = string.Format("%1/%2", Math.Round(current).ToString(), max.ToString());
	
					progressBar.Show(false);
					textWidget.SetText(qty_text);
					textWidget.Show(true);
					textWidget.GetParent().Show(true);
					break;
				case QUANTITY_PROGRESS :	
					textWidget.Show(false);
					progressBar.SetCurrent(current);
					progressBar.Show(true);
					textWidget.GetParent().Show(true);
					break;
			}
			widget.Show(true);
		}
		else
			widget.Show(false);	
	}
	
	private void SetWeaponQuantity(int chamber, int mag, string itemWidget, string quantityPBWidget, string quantityTextWidget, bool enabled)
	{
		Widget widget;
		
		widget = m_Root.FindAnyWidget(itemWidget);
		
		if(enabled)
		{
			ProgressBarWidget progressBar;
			TextWidget textWidget;
			Class.CastTo(progressBar, widget.FindAnyWidget(quantityPBWidget));
			Class.CastTo(textWidget, widget.FindAnyWidget(quantityTextWidget));

			string wpn_qty = string.Format("%1 (+%2)", chamber, mag);
	
			progressBar.Show(false);
			textWidget.SetText(wpn_qty);
			textWidget.Show(true);
			textWidget.GetParent().Show(true);
		}
		else
			widget.Show(false);
	}
	
	private void SetWeaponModeAndZeroing(string itemWidget, string upWidget, string downWidget, bool enabled)
	{
		Widget widget;
		
		widget = m_Root.FindAnyWidget(itemWidget);

		if(enabled)
		{
			TextWidget txtModeWidget;
			TextWidget txtZeroingWidget;

			Class.CastTo(txtModeWidget, widget.FindAnyWidget(upWidget));
			Class.CastTo(txtZeroingWidget, widget.FindAnyWidget(downWidget));
			
			string zeroing	= string.Format("%1 m", m_Player.GetCurrentZeroing());

			txtModeWidget.SetText(m_Player.GetCurrentWeaponMode());
			txtZeroingWidget.SetText(zeroing);
			widget.Show(true);
		}
		else
			widget.Show(false);
	}
	
	private void SetActionWidget(ActionBase action, string descText, string actionWidget, string descWidget)
	{
		Widget widget;
		
		widget = m_Root.FindAnyWidget(actionWidget);
		
#ifdef PLATFORM_XBOX
		ShowXboxHidePCIcons( actionWidget, true );
#else
#ifdef PLATFORM_PS4
		ShowXboxHidePCIcons( actionWidget, true );
#else
		ShowXboxHidePCIcons( actionWidget, false );
#endif
#endif
		
		if(action && !action.HasTarget())
		{
			TextWidget actionName;
			Class.CastTo(actionName, widget.FindAnyWidget(descWidget));
			if(action.IsInherited(ActionContinuousBase))
			{
				descText = descText + " [HOLD]";
				actionName.SetText(descText);
			}
			else
				actionName.SetText(descText);
			widget.Show(true);
		}
		else
			widget.Show(false);
	}
}