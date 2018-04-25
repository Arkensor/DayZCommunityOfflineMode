//--------------------------------------------------------------------------
class InspectMenuNew extends UIScriptedMenu
{
	private ItemPreviewWidget m_item_widget;
	private int m_characterRotationX;
	private int m_characterRotationY;
	private int m_characterScaleDelta;
	private vector m_characterOrientation;

	
	void InspectMenuNew()
	{
		
	}
	
	//--------------------------------------------------------------------------
	void ~InspectMenuNew()
	{
		GetGame().GetDragQueue().RemoveCalls(this);
	
		delete m_item_widget;
	}
	
	//--------------------------------------------------------------------------
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory_new/day_z_inventory_new_inspect.layout");
		
		
		return layoutRoot;
	}
	
	//--------------------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		switch (w.GetUserID())
		{
			case IDC_CANCEL:
				Close();
				return true;
		}
	
		return false;
	}
	
	//--------------------------------------------------------------------------
	void SetItem(EntityAI item)
	{
		if (item)
		{
			UpdateItemInfo(layoutRoot, item);
	
			if (!m_item_widget)
			{
				Widget preview_frame = layoutRoot.FindAnyWidget("ItemFrameWidget");
				if (preview_frame)
				{
					float w;
					float h;
					preview_frame.GetSize(w, h);
					m_item_widget = ItemPreviewWidget.Cast( GetGame().GetWorkspace().CreateWidget(ItemPreviewWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, ARGB(255, 255, 255, 255), 10, preview_frame) );
				}
			}
			
			m_item_widget.SetItem(item);
			m_item_widget.SetModelPosition(Vector(0,0,1));
			
			float x, y;		
			m_item_widget.GetPos(x, y);

			m_item_widget.SetSize( 1.75, 1.75 );
		
			// align to center 
			m_item_widget.SetPos( -0.375, -0.375 );
			
			//m_item_widget.SetModelOrientation
			PPEffects.SetBlurInventory(1);
		}
	}
	
	//--------------------------------------------------------------------------
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonDown(w, x, y, button);
		
		if (w == m_item_widget)
		{
			GetGame().GetDragQueue().Call(this, "UpdateRotation");
			g_Game.GetMousePos(m_characterRotationX, m_characterRotationY);
			return true;
		}
		return false;
	}
	
	//--------------------------------------------------------------------------
	void UpdateRotation(int mouse_x, int mouse_y, bool is_dragging)
	{
		vector o = m_characterOrientation;
		o[0] = o[0] + (m_characterRotationY - mouse_y);
		o[1] = o[1] - (m_characterRotationX - mouse_x);
		
		m_item_widget.SetModelOrientation( o );
		
		if (!is_dragging)
		{
			m_characterOrientation = o;
		}
	}
	
	//--------------------------------------------------------------------------
	override bool OnMouseWheel(Widget  w, int  x, int  y, int wheel)
	{
		super.OnMouseWheel(w, x, y, wheel);
		
		if ( w == m_item_widget )
		{
			GetGame().GetDragQueue().Call(this, "UpdateScale");
			m_characterScaleDelta = wheel ;
		}
		return false;
	}
	
	//--------------------------------------------------------------------------
	void UpdateScale(int mouse_x, int mouse_y, int wheel, bool is_dragging)
	{
		float w, h, x, y;		
		m_item_widget.GetPos(x, y);
		m_item_widget.GetSize(w,h);
		w = w + ( m_characterScaleDelta / 4);
		h = h + ( m_characterScaleDelta / 4 );
		if ( w > 0.5 && w < 3 )
		{
			m_item_widget.SetSize( w, h );
	
			// align to center 
			int screen_w, screen_h;
			GetScreenSize(screen_w, screen_h);
			float new_x = x - ( m_characterScaleDelta / 8 );
			float new_y = y - ( m_characterScaleDelta / 8 );
			m_item_widget.SetPos( new_x, new_y );
		}
	}
};

const int COLOR_RUINED			= 0xFF0000;
const int COLOR_BADLY_DAMAGED	= 0xFFBF00;
const int COLOR_DAMAGED			= 0xFFFF00;
const int COLOR_WORN			= 0xBFFF00;
const int COLOR_PRISTINE		= 0x40FF00;

const int COLOR_DRENCHED		= 0xFF0000FF;
const int COLOR_SOAKING_WET 	= 0xFF3030FF;
const int COLOR_WET 			= 0xFF6060FF;
const int COLOR_DAMP 			= 0xFF9090FF;;

const int COLOR_LIQUID 			= 0x00EEFF;

const int STATE_RUINED 		 	= 4;
const int STATE_BADLY_DAMAGED 	= 3;
const int STATE_DAMAGED 	  	= 2;
const int STATE_WORN 		  	= 1;
const int STATE_PRISTINE 	  	= 0;

const float STATE_DRENCHED		= 0.8;
const float	STATE_SOAKING_WET	= 0.5;
const float	STATE_WET			= 0.25;
const float	STATE_DAMP			= 0.05;

//--------------------------------------------------------------------------
void UpdateItemInfo(Widget root_widget, EntityAI item)
{
	if (!root_widget || !item) return;

	Widget panelInfo = root_widget.FindAnyWidget("InventoryInfoPanelWidget");
	if ( panelInfo )
	{
		if ( item.IsInherited( ZombieBase ) || item.IsInherited( Car ) )
		{
			panelInfo.Show( false );
		}
		else
		{
			panelInfo.Show( true );
		}
	}

	if ( !item.IsInherited( ZombieBase ) && !item.IsInherited( Car ) )
	{
		InventoryItem iItem = InventoryItem.Cast( item );
		WidgetTrySetText(root_widget, "ItemDescWidget", iItem.GetTooltip());
	}

	WidgetTrySetText(root_widget, "ItemNameWidget", item.GetDisplayName());
	UpdateItemInfoDamage(root_widget, item);
	UpdateItemInfoLiquidType(root_widget, item);
	UpdateItemInfoTemperature(root_widget, item);
	UpdateItemInfoWetness(root_widget, item);
	UpdateItemInfoQuantity(root_widget, item);
	UpdateItemInfoWeight(root_widget, item);
}

//--------------------------------------------------------------------------
void UpdateItemInfoDamage(Widget root_widget, EntityAI item)
{
	if ( item.IsInherited( ZombieBase ) || item.IsInherited( Car ) ) return;
	
	int damageLevel = item.GetHealthLabel();
	
	switch(damageLevel)
	{
		case STATE_RUINED:
		{
			WidgetTrySetText(root_widget, "ItemDamageWidget", "RUINED", COLOR_RUINED);
			break;
		}
		case STATE_BADLY_DAMAGED:
		{
			WidgetTrySetText(root_widget, "ItemDamageWidget", "BADLY DAMAGED", COLOR_BADLY_DAMAGED);
			break;
		}
		
		case STATE_DAMAGED:
		{
			WidgetTrySetText(root_widget, "ItemDamageWidget", "DAMAGED", COLOR_DAMAGED);
			break;
		}
		
		case STATE_WORN:
		{
			WidgetTrySetText(root_widget, "ItemDamageWidget", "WORN", COLOR_WORN);
			break;
		}
		
		case STATE_PRISTINE:
		{
			WidgetTrySetText(root_widget, "ItemDamageWidget", "PRISTINE", COLOR_PRISTINE);
			break;
		}
				
		default:
		{
			WidgetTrySetText(root_widget, "ItemDamageWidget", "ERROR", COLOR_PRISTINE);
			break;
		}
	}

}

//--------------------------------------------------------------------------
void UpdateItemInfoLiquidType(Widget root_widget, EntityAI item)
{
	if ( item.IsInherited( ZombieBase ) || item.IsInherited( Car ) ) return;
	
	ItemBase item_base = ItemBase.Cast( item );
	
	if( item_base && item_base.GetQuantity() > 0 && item_base.IsBloodContainer() )
	{
		BloodContainerBase blood_container = BloodContainerBase.Cast( item_base );
		
		if( blood_container.GetBloodTypeVisible() )
		{
			string blood_type_name = BloodTypes.GetBloodTypeName(blood_container.GetLiquidType());
			WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "BLOOD: " + blood_type_name, COLOR_LIQUID);
		}
		else
		{
			WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "BLOOD", COLOR_LIQUID);
		}
	}
	else if( item_base && item_base.GetQuantity() > 0 && item_base.IsLiquidContainer() )
	{
		int liquid_type = item_base.GetLiquidType();
		
		switch(liquid_type)
		{
			case LIQUID_WATER:
			{
				WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "WATER", COLOR_LIQUID);
				break;
			}
				
			case LIQUID_RIVERWATER:
			{
				WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "RIVER WATER", COLOR_LIQUID);
				break;
			}
				
			case LIQUID_VODKA:
			{
				WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "VODKA", COLOR_LIQUID);
				break;
			}
			
			case LIQUID_BEER:
			{
				WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "BEER", COLOR_LIQUID);
				break;
			}
			
			case LIQUID_GASOLINE:
			{
				WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "GASOLINE", COLOR_LIQUID);
				break;
			}
			
			case LIQUID_DIESEL:
			{
				WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "DIESEL", COLOR_LIQUID);
				break;
			}
			
			case LIQUID_DISINFECTANT:
			{
				WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "DISINFECTANT", COLOR_LIQUID);
				break;
			}

			case LIQUID_SALINE:
			{
				WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "SALINE", COLOR_LIQUID);
				break;
			}
			
			default:
			{
				WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "ERROR", COLOR_LIQUID);
				break;
			}
		}
	}
	else
	{
		WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "");
	}
}

//--------------------------------------------------------------------------
void UpdateItemInfoTemperature(Widget root_widget, EntityAI item)
{
	if ( item.IsInherited( ZombieBase ) || item.IsInherited( Car ) ) return;
	float temperature;
	ItemBase item_base = ItemBase.Cast( item );
	if( item_base )
	{
		temperature = item_base.GetTemperature();
	}
	
	if( temperature != 0)
	{
		WidgetTrySetText(root_widget, "ItemTemperatureWidget",  "~ " + temperature.ToString() +  " CELSIUS", GetTemperatureColor( temperature ) );
	}	
	else
	{
		WidgetTrySetText(root_widget, "ItemTemperatureWidget", "");
	}
}

//--------------------------------------------------------------------------
void UpdateItemInfoWetness(Widget root_widget, EntityAI item)
{
	if ( item.IsInherited( ZombieBase ) || item.IsInherited( Car ) ) return;
	/*		
	_bagwet = 1;
	if(!isNull (itemParent _item) && (itemParent _item) isKindOf "ClothingBase")then{
	_bagwet = getNumber(configFile >> "cfgVehicles" >> typeOf (itemParent _item) >> "absorbency");
	};

	//wetness setting
	if(!isNull (itemParent _item) && _bagwet > 0 && _pwetness > 0)then{
	_wetness=getNumber(_config >> "absorbency") min _pwetness;
	};
	*/
	float wetness = 0;
	
	if ( item.IsInherited(ItemBase) )
	{
		ItemBase item_IB = ItemBase.Cast( item );
		wetness = item_IB.GetWet();
	}
	
	float bagwet = 1;

	EntityAI parent = item.GetHierarchyParent();
	if (parent && parent.IsClothing())
	{
		bagwet = parent.ConfigGetFloat("absorbency");
	}
	
	if( wetness < STATE_DAMP )
	{
		WidgetTrySetText(root_widget, "ItemWetnessWidget", "");
	}
	else if( wetness >= STATE_DAMP && wetness < STATE_WET )
	{
		WidgetTrySetText(root_widget, "ItemWetnessWidget", "DAMP", COLOR_DAMP);
	}
	else if( wetness >= STATE_WET && wetness < STATE_SOAKING_WET )
	{
		WidgetTrySetText( root_widget, "ItemWetnessWidget", "WET", COLOR_WET );
	}
	else if( wetness >= STATE_SOAKING_WET && wetness < STATE_DRENCHED )
	{
		WidgetTrySetText( root_widget, "ItemWetnessWidget", "SOAKING WET", COLOR_SOAKING_WET );
	}
	else
	{
		WidgetTrySetText( root_widget, "ItemWetnessWidget", "DRENCHED", COLOR_DRENCHED );
	}
}

//--------------------------------------------------------------------------
void UpdateItemInfoQuantity(Widget root_widget, EntityAI item)
{
	if ( item.IsInherited( ZombieBase ) || item.IsInherited( Car ) ) return;
	
	ItemBase item_base = ItemBase.Cast( item );
	if( item_base )
	{
		float item_quantity = item_base.GetQuantity();
		int max_quantity = item.ConfigGetInt("varQuantityMax");
		float quantity_ratio;
		
		if( max_quantity > 0 ) // Some items, like books, have max_quantity set to 0 => division by ZERO error in quantity_ratio
		{
			if( item.ConfigGetString("stackedUnit") == "pc." )
			{
				if( item_quantity == 1 )
				{
					WidgetTrySetText( root_widget, "ItemQuantityWidget", item_quantity.ToString() + " PIECE" );
				}
				else
				{
					WidgetTrySetText( root_widget, "ItemQuantityWidget", item_quantity.ToString() + " PIECES" );
				}		
			}
			else if( item.ConfigGetString("stackedUnit") == "g" )
			{
				quantity_ratio = Math.Round( ( item_quantity / max_quantity ) * 100 );
				
				WidgetTrySetText( root_widget, "ItemQuantityWidget", quantity_ratio.ToString() + " % REMAINING" );			
			}
			else if( item.ConfigGetString("stackedUnit") == "ml" )
			{
				quantity_ratio = Math.Round( ( item_quantity / max_quantity ) * 100 );
				
				WidgetTrySetText( root_widget, "ItemQuantityWidget", quantity_ratio.ToString() + "% REMAINING" );
			}
			else if( item.IsInherited( Magazine ) )
			{
				Magazine magazine_item;
				Class.CastTo(magazine_item, item);
				
				if( magazine_item.GetAmmoCount() == 1 )
				{
					WidgetTrySetText( root_widget, "ItemQuantityWidget",  magazine_item.GetAmmoCount().ToString() + " PIECE" );
				}
				else
				{
					WidgetTrySetText( root_widget, "ItemQuantityWidget",  magazine_item.GetAmmoCount().ToString() + " PIECES" );
				}
			}
			else
			{
				WidgetTrySetText( root_widget, "ItemQuantityWidget", "" );
			}
		}
		else
		{
			WidgetTrySetText( root_widget, "ItemQuantityWidget", "" );
		}
	}
}

//--------------------------------------------------------------------------
void UpdateItemInfoWeight(Widget root_widget, EntityAI item)
{
	if ( item.IsInherited( ZombieBase ) || item.IsInherited( Car ) ) return;
	
	float quantity = 0;
	float wetness = 0;

	ItemBase item_IB = ItemBase.Cast( item );
	if( item_IB )
	{
		quantity = item_IB.GetQuantity();
		wetness = item_IB.GetWet();
		
		int confweight = item.ConfigGetInt("weight");
		// old calculation
		/*
		float weight = 0;
	
		if (quantity > 0 && confweight != 0)
		{
			weight = Math.Round( (wetness + 1) * confweight * quantity );
		}
		else if (quantity > 0)
		{
			weight = Math.Round( (wetness + 1) * quantity );
		}
		else
		{
			weight=Math.Round( (wetness + 1) * confweight );
		}
		*/
		int weight = item_IB.GetItemWeight();
		
		if (root_widget.GetName() != "BackPanelWidget")
		{
			weight = item_IB.GetSingleInventoryItemWeight();
		}
		
		if (weight >= 1000)
		{
			int kilos = Math.Round(weight / 1000.0);
			WidgetTrySetText(root_widget, "ItemWeightWidget", "ABOUT " + kilos.ToString() + " KG");
		}
		else if (weight >= 500)
		{
			WidgetTrySetText(root_widget, "ItemWeightWidget", "UNDER 1 KG");
		} 
		else if (weight >= 250)
		{
			WidgetTrySetText(root_widget, "ItemWeightWidget", "UNDER 0.5 KG");
		}
		else 
		{
			WidgetTrySetText(root_widget, "ItemWeightWidget", "UNDER 0.25 KG");
		}
	}
}

//--------------------------------------------------------------------------
void WidgetTrySetText(Widget root_widget, string widget_name, string text, int color = 0)
{
	TextWidget widget = TextWidget.Cast( root_widget.FindAnyWidget(widget_name) );
	if (widget)
	{
		widget.SetText(text);

		Widget widget_background = root_widget.FindAnyWidget(widget_name+"Background");
		if (widget_background)
		{
			if (color != 0)
			{
				widget_background.Show( true );
				widget_background.SetColor(color | 0x7F000000);
			}
			else
			{
				widget_background.Show( false );
			}
		}
	}
}
