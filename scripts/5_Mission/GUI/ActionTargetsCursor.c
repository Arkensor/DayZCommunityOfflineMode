class ATCCachedObject
{
	protected Object 	m_CachedObject;
	protected vector 	m_CursorWPos;
	protected vector	m_ScreenPos;
	
	void ATCCachedTarget()
	{
		m_CachedObject = null;
		m_ScreenPos = vector.Zero;
	}

	//! cache object and its world pos
	void Store(Object obj, vector pos)
	{
		if(!m_CachedObject)
		{
			m_CachedObject = obj;
			m_CursorWPos = pos;
		}
	}

	//! invalidate cached objec
	void Invalidate(bool cached)
	{
		if ( m_CachedObject )
		{
			m_CachedObject = null;
			m_CursorWPos = vector.Zero;
		}
	}

	Object Get()
	{
		return m_CachedObject; 	
	}
	
	vector GetCursorWorldPos()
	{
		return m_CursorWPos;
	}
};

class ActionTargetsCursor extends ObjectFollower
{
	protected PlayerBase 					m_Player;
	protected ActionTarget 					m_Target;
	protected ref ATCCachedObject			m_CachedObject;

	protected ActionBase					m_Interact;
	protected ActionBase					m_Single;
	protected ActionBase					m_Continuous;
	protected ActionManagerClient 			m_AM;

	protected int							m_InteractActionsNum;
	protected bool 							m_IsCached;
	protected bool 							m_HealthEnabled;
	protected bool							m_QuantityEnabled;	
	protected bool							m_FixedOnPosition;

	// for KeyToUIElements conversion
	ref TIntArray 							m_ActionIndices;
	ref TIntArray 							m_Keys;
	ref TStringArray						m_Actions;
	
	protected Widget						m_Container;
	protected Widget 						m_ItemLeft;
	ref AutoHeightSpacer					m_MainSpacer;
	ref AutoHeightSpacer					m_HealthQuantitySpacer;

	void ActionTargetsCursor()
	{
		m_Interact = null;
		m_Single = null;
		m_Continuous = null;
		m_AM = null;
		
		m_CachedObject = new ATCCachedObject;
		m_IsCached = false;
		
		m_ActionIndices = new TIntArray;
		m_Keys = new TIntArray;
		m_Actions = new TStringArray;
		
		GetActionGroup(4); // 4 - "Interact"
		KeysToUIElements.Init(); // Initialiaze of KeysToUIElements
	}
	
	// Controls appearance of the builded cursor
	void SetHealthVisibility( bool state)
	{
		m_HealthEnabled = state;
	}

	void SetQuantityVisibility( bool state )
	{
		m_QuantityEnabled = state;
	}
		
	void SetInteractXboxIcon( string imageset_name, string image_name )
	{
		SetXboxIcon( "interact", imageset_name, image_name );
	}
	
	void SetSingleXboxIcon( string imageset_name, string image_name )
	{
		SetXboxIcon( "single", imageset_name, image_name );
	}
	
	void SetContinuousXboxIcon( string imageset_name, string image_name )
	{
		SetXboxIcon( "continuous", imageset_name, image_name );
	}
	
	protected void SetXboxIcon( string name, string imageset_name, string image_name )
	{
		ImageWidget w = ImageWidget.Cast( m_Root.FindAnyWidget( name + "_btn_icon_xbox" ) );
		w.LoadImageFile( 0, "set:"+ imageset_name + " image:" + image_name );
	}

	override protected void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);

		m_Root = w;
		m_Root.Show(false);

		m_Container = w.FindAnyWidget("container");
		m_Container.GetScript(m_MainSpacer);
		m_ItemLeft = w.FindAnyWidget("item_left");
		m_ItemLeft.GetScript(m_HealthQuantitySpacer);
		m_Root.Update();
		m_MainSpacer.Update();
		m_HealthQuantitySpacer.Update();

#ifdef PLATFORM_XBOX		
		SetSingleXboxIcon("xbox_buttons", "RT");
		SetContinuousXboxIcon("xbox_buttons", "RT");
		SetInteractXboxIcon("xbox_buttons", "X");
#endif

#ifdef PLATFORM_PS4		
		SetSingleXboxIcon("playstation_buttons", "R2");
		SetContinuousXboxIcon("playstation_buttons", "R2");
		SetInteractXboxIcon("playstation_buttons", "square");
#endif
	}
	
	protected void PrepareCursorContent()
	{
		int health = -1;
		int cargoCount = 0;
		int q_type = 0;
		int q_min, q_max = -1;
		float q_cur = -1.0;
		
		//! item health
		health = GetItemHealth();
		SetItemHealth(health, "item", "item_health_mark", m_HealthEnabled);	
		//! quantity
		GetItemQuantity(q_type, q_cur, q_min, q_max);
		//! cargo in item
		GetItemCargoCount(cargoCount);
		//! fill the widget with data
		SetItemQuantity(q_type, q_cur, q_min, q_max, "item", "item_quantity_pb", "item_quantity_text", m_QuantityEnabled);
		SetInteractActionIcon("interact", "interact_icon_frame", "interact_btn_inner_icon", "interact_btn_text");
		SetItemDesc(GetItemDesc(m_Interact), cargoCount, "item", "item_desc");
		SetActionWidget(m_Interact, GetActionDesc(m_Interact), "interact", "interact_action_name");
		SetActionWidget(m_Single, GetActionDesc(m_Single), "single", "single_action_name");
		SetActionWidget(m_Continuous, GetActionDesc(m_Continuous), "continuous", "continuous_action_name");
		SetMultipleInteractAction("interact_mlt_wrapper");
	}
	
	protected void BuildFixedCursor()
	{
		int w, h, x, y;

		PrepareCursorContent();
		GetScreenSize(w, h);
		x = w/2 + 32;
		y = h/2 + 32;

		m_Root.SetPos(x, y);

		m_MainSpacer.Update();
		m_HealthQuantitySpacer.Update();	
	}
	
	protected void BuildFloatingCursor(bool forceRebuild)
	{
		float pos_x, pos_y = 0.0;

		PrepareCursorContent();
		
		if (forceRebuild)
		{
			GetOnScreenPosition(pos_x, pos_y);
		}
		//! in case of cached item, all above is reused except the position
		else
		{
			vector screen_pos = GetGame().GetScreenPos( m_CachedObject.GetCursorWorldPos() );
			pos_x = screen_pos[0];
			pos_y = screen_pos[1];
		}
		
		pos_x = Math.Ceil(pos_x);
		pos_y = Math.Ceil(pos_y);

		m_Root.SetPos(pos_x, pos_y);
		
		m_MainSpacer.Update();
		m_HealthQuantitySpacer.Update();
	}
	
	override protected void Update()
	{
		// TODO: if we choose to have it configurable throught options or from server settings
		// we need to change setting of these methods;
		SetHealthVisibility(true);
		SetQuantityVisibility(true);
		
		if(m_Player && !m_Player.IsAlive()) // handle respawn
		{
			m_Player = null;
			m_AM = null;
		}
		if(!m_Player) GetPlayer();
		if(!m_AM) GetActionManager();
		GetTarget();
		GetActions();

		if(m_Target && !GetGame().IsInventoryOpen())
		{
			if(IsActionRunning(m_Interact) || IsActionRunning(m_Single) || IsActionRunning(m_Continuous))
			{
				//Print(">> action already running");
				m_Root.Show(false);
				m_CachedObject.Invalidate(true);
				m_IsCached = false;
				m_FixedOnPosition = false;
			}
			else
			{
				//! cursor with fixed position
				if ( m_FixedOnPosition || m_Target.GetObject() == null )
				{
					//Print(">> fixed widget");
					m_CachedObject.Invalidate(true);
					BuildFixedCursor();
					m_Root.Show(true);
					m_FixedOnPosition = false;
					return;
				}
				else
				{
					//! build cursor for new target
					if ( m_Target.GetObject() != m_CachedObject.Get() )
					{
						//Print(">> non-cached widget");
						m_CachedObject.Invalidate(true);
						BuildFloatingCursor(true);
						m_Root.Show(true);
						return;
					}
					//! use cached version for known target - recalculate onscreen pos only
					else
					{
						//Print(">> cached widget");
						BuildFloatingCursor(false);
						m_Root.Show(true);
						return;
					}
				}
			}
		}
		else
		{
			m_CachedObject.Invalidate(true);
			m_Root.Show(false);
			m_IsCached = false;
			m_FixedOnPosition = false;
		}
	}
	
	protected void ShowXboxHidePCIcons( string widget, bool show_xbox_icon )
	{
		m_Root.FindAnyWidget( widget + "_btn_icon_xbox" ).Show( show_xbox_icon );
		m_Root.FindAnyWidget( widget + "_btn_icon" ).Show( !show_xbox_icon );
	}
	
	override protected void GetOnScreenPosition(out float x, out float y)
	{
		const float 	DEFAULT_HANDLE_OFFSET 	= 0.2;
		const string 	CE_CENTER_COMP_NAME 	= "ce_center";
		const string 	MEM_LOD_NAME 			= "memory";
		
		int compIdx;
		float pivotOffset = 0.0;
		float memOffset   = 0.0;
		string compName;

		bool isTargetForced = false;
		
		vector worldPos;
		vector modelPos;

		LOD lod;

		ref array<Selection> memSelections	= new array<Selection>();
		ref array<string> components = new array<string>; // for components with multiple selection
		
		Object object;

		if(m_Target)
		{
			object = m_Target.GetObject();
			compIdx  = m_Target.GetComponentIndex();

			if(m_Target.GetCursorHitPos() == vector.Zero)
				isTargetForced = true;
		}
		else
		{
			return;
		}

		if(object)
		{
			if(!isTargetForced)
			{
				compName = object.GetActionComponentName( compIdx );
				object.GetActionComponentNameList( compIdx, components );	
				pivotOffset	= object.ConfigGetFloat( "actionTargetPivotOffsetY" );
				memOffset	= object.ConfigGetFloat( "actionTargetMemOffsetY" );
	
				//! Get memory LOD from p3d
				lod = object.GetLODByName(MEM_LOD_NAME);
				if(lod != null)
				{
					//! save selection from memory lod
					lod.GetSelections(memSelections);

					// items with CE_Center mem point
					if( IsComponentInSelection( memSelections, CE_CENTER_COMP_NAME ) )
					{
						for( int i2 = 0; i2 < memSelections.Count(); ++i2 )
						{
							if( memSelections[i2].GetName() == CE_CENTER_COMP_NAME && memSelections[i2].GetVertexCount() == 1 )
							{
								m_FixedOnPosition = false;
								modelPos = object.GetSelectionPosition( CE_CENTER_COMP_NAME );
								worldPos = object.ModelToWorld( modelPos );
								if ( memOffset != 0.0 )
								{
									worldPos[1] = worldPos[1] + memOffset;
								}
								else
								{
									worldPos[1] = worldPos[1] + DEFAULT_HANDLE_OFFSET;
								}
							}
						}

						//! cache current object and the widget world pos
						m_CachedObject.Store(object, worldPos);
						m_IsCached = true;	
					}
					//! doors/handles
					else if( !compName.Contains("ladder") && IsComponentInSelection( memSelections, compName ) )
					{
						for( int i1 = 0; i1 < memSelections.Count(); ++i1 )
						{
							//! single vertex in selection
							if( memSelections[i1].GetName() == compName && memSelections[i1].GetVertexCount() == 1 )
							{
								modelPos = object.GetSelectionPosition( compName );
								worldPos = object.ModelToWorld( modelPos );

								m_FixedOnPosition = false;
								if ( memOffset != 0.0 )
								{
									worldPos[1] = worldPos[1] + memOffset;
								}
								else
								{
									worldPos[1] = worldPos[1] + DEFAULT_HANDLE_OFFSET;
								}
							}

							//! multiple vertices in selection
							if ( memSelections[i1].GetName() == compName && memSelections[i1].GetVertexCount() > 1 )
							{
								for( int j = 0; j < components.Count(); ++j )
								{
									if( IsComponentInSelection(memSelections, components[j]) )
									{
										modelPos = object.GetSelectionPosition( components[j] );
										worldPos = object.ModelToWorld( modelPos );

										m_FixedOnPosition = false;
										if ( memOffset != 0.0 )
										{
											worldPos[1] = worldPos[1] + memOffset;
										}
										else
										{
											worldPos[1] = worldPos[1] + DEFAULT_HANDLE_OFFSET;
										}
									}
								}
							}
						}
						
						//! cache current object and the widget world pos
						m_CachedObject.Store(object, worldPos);
						m_IsCached = true;	
					}
					//! ladders handling
					else if( compName.Contains("ladder") && IsComponentInSelection( memSelections, compName))
					{
						vector ladderHandlePointLS, ladderHandlePointWS;
						vector closestHandlePos;
						float lastDistance = 0;
	
						for( int i3 = 0; i3 < memSelections.Count(); ++i3 )
						{
							if ( memSelections[i3].GetName() == compName && memSelections[i3].GetVertexCount() > 1 )
							{
								ladderHandlePointLS = memSelections[i3].GetVertexPosition(lod, 0);
								ladderHandlePointWS = object.ModelToWorld( ladderHandlePointLS );
								closestHandlePos = ladderHandlePointWS;
								lastDistance = Math.AbsFloat(vector.DistanceSq(ladderHandlePointWS, m_Player.GetPosition()));
	
								for( int k = 1; k < memSelections[i3].GetVertexCount(); ++k )
								{
									ladderHandlePointLS = memSelections[i3].GetVertexPosition(lod, k);
									ladderHandlePointWS = object.ModelToWorld( ladderHandlePointLS );
									
									if ( lastDistance > Math.AbsFloat(vector.DistanceSq(ladderHandlePointWS, m_Player.GetPosition())) )
									{
										lastDistance = Math.AbsFloat(vector.DistanceSq(ladderHandlePointWS, m_Player.GetPosition()));
										closestHandlePos = ladderHandlePointWS;
									}
								}

								m_FixedOnPosition = false;
								worldPos = closestHandlePos;						
								if ( memOffset != 0.0 )
								{
									worldPos[1] = worldPos[1] + memOffset;
								}
								else
								{
									worldPos[1] = worldPos[1] + DEFAULT_HANDLE_OFFSET;
								}
							}
						}

						//! cache current object and the widget world pos
						m_CachedObject.Store(object, worldPos);
						m_IsCached = true;					
					}
					else
					{
						m_FixedOnPosition = true;
					}
				}
				else
				{
					m_FixedOnPosition = true;
				}
			}
			else
			{
				m_FixedOnPosition = true;
			}

			vector screen_pos = GetGame().GetScreenPos( worldPos );
			x = screen_pos[0];
			y = screen_pos[1];
		}

		worldPos = vector.Zero;
		isTargetForced = false;
	}

	private bool IsComponentInSelection( array<Selection> selection, string compName )
	{
		if (selection.Count() == 0 || compName.Length() == 0) return false;

		for ( int i = 0; i < selection.Count(); ++i )
		{
			compName.ToLower();
			if ( selection[i] && selection[i].GetName() == compName )
			{
				return true;
			}
		}
		return false;
	}

	// getters

	// selects Action Group like in OptionsMenu
	private void GetActionGroup(int group_index)
	{
		g_Game.GetInput().GetActionGroupItems(group_index, m_ActionIndices);
		string desc;

		for (int i = 0; i < m_ActionIndices.Count(); i++)
		{
			int action_index = m_ActionIndices.Get(i);
			g_Game.GetInput().GetActionDesc(action_index, desc);
			m_Actions.Insert(desc);
		}
	}

 	private void GetPlayer()
	{
		Class.CastTo(m_Player, GetGame().GetPlayer());
	}

	private void GetActionManager()
	{
		if( m_Player && m_Player.IsPlayerSelected() )
		{
			Class.CastTo(m_AM, m_Player.GetActionManager());
		}
		else
		{
			m_AM = null;
		}
	}

	//! get actions from Action Manager
  	private void GetActions()
	{
		m_Interact = null;
		m_Single = null;
		m_Continuous = null;

		if(!m_AM) return;
		if(!m_Target) return;
		if(m_Player.IsSprinting()) return;

		TSelectableActionInfoArray selectableActions = m_AM.GetSelectableActions();
		int selectedActionIndex = m_AM.GetSelectedActionIndex();
		m_InteractActionsNum = selectableActions.Count();
		if ( m_InteractActionsNum > 0 )
		{
			m_Interact = m_AM.GetAction(selectableActions.Get(selectedActionIndex).param2);
		}
				
		m_Single = m_AM.GetSingleUseAction();
		m_Continuous = m_AM.GetContinuousAction();
	}

	private bool IsActionRunning(ActionBase action)
	{
		if ( !action ) return false;

		int actionState = m_AM.GetActionState(action);
		if(actionState != UA_NONE)
			return true;
		
		return false;
	}

	private void GetTarget()
	{
		if(!m_AM) return;

		m_Target = m_AM.FindActionTarget();
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
	
	private string GetItemDesc(ActionBase action)
	{
		string desc = "";
		if(m_Target && m_Target.GetObject() && m_Target.GetObject().IsItemBase())
		{
			desc = m_Target.GetObject().GetDisplayName();
			return desc;
		}
		return desc;
	}
	
	private int GetItemHealth()
	{
		int health = -1;

		if(m_Interact && !m_Interact.HasTarget())
		{
			return health;
		}
		if(m_Target && m_Target.GetObject() && m_Target.GetObject().IsHealthVisible())
		{
			health = m_Target.GetObject().GetHealthLabel();
			return health;
		}
		
		return health;
	}

	private void GetItemQuantity(out int q_type, out float q_cur, out int q_min, out int q_max)
	{
		EntityAI entity = null;
		InventoryItem item = null;

		if(m_Interact && !m_Interact.HasTarget())
		{
			return;
		}	
		if( Class.CastTo(entity, m_Target.GetObject()) )
		{
			Class.CastTo(item, entity);
			q_type = QuantityConversions.HasItemQuantity(entity);
			if (q_type > 0)
				QuantityConversions.GetItemQuantity(item, q_cur, q_min, q_max);
		}
	}

	private void GetItemCargoCount(out int cargoCount)
	{
		EntityAI entity = null;
		
		if( Class.CastTo(entity, m_Target.GetObject()) )
		{
			if(entity.GetInventory())
			{
				Cargo cargo = entity.GetInventory().GetCargo();
				if (cargo)
				{
					cargoCount = cargo.GetItemCount();
					return;
				}
			}

			cargoCount = 0;
		}
	}

	// setters
	private void SetItemDesc(string descText, int cargoCount, string itemWidget, string descWidget)
	{
		Widget widget;
		
		widget = m_Root.FindAnyWidget(itemWidget);
		
		if(descText.Length() == 0)
		{
			widget.Show(false);
			return;
		}
		
		descText.ToUpper();
		TextWidget itemName;
		Class.CastTo(itemName, widget.FindAnyWidget(descWidget));

		//! when cargo in container
		if (cargoCount > 0)
		{
			descText = string.Format("[+] %1 ", descText, cargoCount);
			itemName.SetText(descText);		
		}
		else
			itemName.SetText(descText);

		widget.Show(true);
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
					float qty_num = Math.Round( ( current / max ) * 100 );
	
					textWidget.Show(false);
					progressBar.SetCurrent(qty_num);
					progressBar.Show(true);
					textWidget.GetParent().Show(true);
					break;
			}
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
		
		if(action)
		{
			if(action.HasTarget() && action.GetActionState() < 1) // targeted & action not performing
			{
				TextWidget actionName;
				Class.CastTo(actionName, widget.FindAnyWidget(descWidget));
				if(action.GetActionCategory() == AC_CONTINUOUS)
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
		else
		{
			widget.Show(false);
		}
	}

	//! shows arrows near the interact action if there are more than one available
	private void SetMultipleInteractAction(string multiActionsWidget)
	{
		Widget widget;

		widget = m_Root.FindAnyWidget(multiActionsWidget);

		if(m_InteractActionsNum > 1)
			widget.Show(true);
		else
			widget.Show(false);
	}
	
	private void SetInteractActionIcon(string actionWidget, string actionIconFrameWidget, string actionIconWidget, string actionIconTextWidget)
	{
		Param2<string, bool> key_data;
		string group_name;
		ref TIntArray group_items;

		Widget widget, frameWidget;
		ImageWidget iconWidget;
		TextWidget textWidget;
		
		widget = m_Root.FindAnyWidget(actionWidget);
		Class.CastTo(frameWidget, widget.FindAnyWidget(actionIconFrameWidget));
		Class.CastTo(iconWidget, widget.FindAnyWidget(actionIconWidget));
		Class.CastTo(textWidget, widget.FindAnyWidget(actionIconTextWidget));

		g_Game.GetInput().GetActionGroupName( 4, group_name );
		if ( group_name == "Interact" )
		{
			g_Game.GetInput().GetActionKeys(m_ActionIndices.Get(0), m_Keys);
			string default_action = m_Actions.Get(0);
			// get only the Default action which is first item in Interact Action Group
			if ( default_action.Contains("Use default action") )
			{
				// get data about action key (1st from selection)
					key_data = KeysToUIElements.GetKeyToUIElement( m_Keys.Get(0) );
			}
		}
#ifdef DEVELOPER
		else
			Print( "ActionTargetsCursor.c | SetInteractActionIcon | Bad options for group_name" );
#endif

		if ( key_data )
		{
			if ( key_data.param2 )
			{
				// uses image in floating widget
				frameWidget.Show(false);
				textWidget.Show(false);
				iconWidget.LoadImageFile( 0, key_data.param1 );
				iconWidget.Show(true);
			}
			else
			{
				// uses text in floating widget
				iconWidget.Show(false);

				textWidget.SetText( key_data.param1 );
#ifdef X1_TODO_TEMP_GUI
				textWidget.SetText("X");
#endif
				//frameWidget.Show(true);
				textWidget.Show(true);
			}
		}	
	}
}