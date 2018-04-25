//--------------------------------------------------------------------------
class InspectMenu extends UIScriptedMenu
{
	private ItemPreviewWidget m_item_widget;
	private int m_characterRotationX;
	private int m_characterRotationY;
	private int m_characterScaleDelta;
	private vector m_characterOrientation;

	
	void InspectMenu()
	{
	}
	
	//--------------------------------------------------------------------------
	void ~InspectMenu()
	{
		GetGame().GetDragQueue().RemoveCalls(this);
	
		delete m_item_widget;
	}
	
	//--------------------------------------------------------------------------
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_inventory_inspect.layout");
		
		
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
		
		m_item_widget.SetModelOrientation(o);
		
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
	
	override bool UseKeyboard()
	{
		return false;
	}
};
/* InspectMenu global functions and variables below commented out. use InspectMenuNew instead
const int COLOR_RUINED = 0xFF0000;
const int COLOR_BADLY_DAMAGED = 0xFFBF00;
const int COLOR_DAMAGED = 0xFFFF00;
const int COLOR_WORN = 0xBFFF00;
const int COLOR_PRISTINE = 0x40FF00;

const int COLOR_SOAKING_WET = 0x0051FF;
const int COLOR_WET = 0x009DFF;
const int COLOR_DAMP = 0x00EEFF;

const int STATE_RUINED 		  = 4;
const int STATE_BADLY_DAMAGED = 3;
const int STATE_DAMAGED 	  = 2;
const int STATE_WORN 		  = 1;
const int STATE_PRISTINE 	  = 0;

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
	UpdateItemInfoWetness(root_widget, item);
	UpdateItemInfoWeight(root_widget, item);
	UpdateItemInfoQuantity(root_widget, item);
	UpdateItemInfoDebug(root_widget, item);
}

//--------------------------------------------------------------------------
void UpdateItemInfoDamage(Widget root_widget, EntityAI item)
{
	if ( item.IsInherited( ZombieBase ) || item.IsInherited( Car ) ) return;
	
	int damageLevel = item.GetHealthLabel();
	
	string widgetText;

	// New damage system
	switch(damageLevel)
	{
		case STATE_RUINED:
			{
				widgetText = "Ruined";
				WidgetTrySetText(root_widget, "ItemDamageWidget", widgetText, COLOR_RUINED);
			}
			break;
		
		case STATE_BADLY_DAMAGED:
			{
				widgetText = "Badly Damaged";
				WidgetTrySetText(root_widget, "ItemDamageWidget", widgetText, COLOR_BADLY_DAMAGED);
			}
			break;
		
		case STATE_DAMAGED:
			{
				widgetText = "Damaged";
				WidgetTrySetText(root_widget, "ItemDamageWidget", widgetText, COLOR_DAMAGED);
			}
			break;
		
		case STATE_WORN:
			{
				widgetText = "Worn";
				WidgetTrySetText(root_widget, "ItemDamageWidget", widgetText, COLOR_WORN);
			}
			break;
		
		case STATE_PRISTINE:
			{
				widgetText = "Pristine";
				WidgetTrySetText(root_widget, "ItemDamageWidget", widgetText, COLOR_PRISTINE);
			}
			break;
	}

}

//--------------------------------------------------------------------------
void UpdateItemInfoWetness(Widget root_widget, EntityAI item)
{
	if ( item.IsInherited( ZombieBase ) || item.IsInherited( Car ) ) return;
	
//	_bagwet = 1;
//	if(!isNull (itemParent _item) && (itemParent _item) isKindOf "ClothingBase")then{
//	_bagwet = getNumber(configFile >> "cfgVehicles" >> typeOf (itemParent _item) >> "absorbency");
//	};

	//wetness setting
//	if(!isNull (itemParent _item) && _bagwet > 0 && _pwetness > 0)then{
//	_wetness=getNumber(_config >> "absorbency") min _pwetness;
//	};

	float wetness = 0;
	
	if ( item.IsInherited(ItemBase) )
	{
		ItemBase item_IB = ItemBase.Cast( item );
		wetness = item_IB.GetWet();
	}
	
	float bagwet = 1;

	EntityAI parent = item.GetOwner();
	if (parent && parent.IsClothing())
	{
		bagwet = parent.ConfigGetFloat("absorbency");
	}
	
	if (wetness >= 0.9) // TO DO: This number must be accessible from elsewhere!
	{
		WidgetTrySetText(root_widget, "ItemWetnessWidget", "Soaking Wet", COLOR_SOAKING_WET);
	}
	else if (wetness >= 0.5) // TO DO: This number must be accessible from elsewhere!
	{
		WidgetTrySetText(root_widget, "ItemWetnessWidget", "Wet", COLOR_WET);
	}
	else if (wetness > 0)
	{
		WidgetTrySetText(root_widget, "ItemWetnessWidget", "Damp", COLOR_DAMP);
	}
	else
	{
		WidgetTrySetText(root_widget, "ItemWetnessWidget", "");
	}
}

//--------------------------------------------------------------------------
void UpdateItemInfoWeight(Widget root_widget, EntityAI item)
{
	if ( item.IsInherited( ZombieBase ) || item.IsInherited( Car ) ) return;
	
	float quantity = 0;
	float wetness = 0;

//	if ( item.IsInherited(ItemBase) )
//	{
		ItemBase item_IB = ItemBase.Cast( item );
		quantity = item_IB.GetQuantity();
		wetness = item_IB.GetWet();
//	}

	if (item.ConfigGetString("stackedUnit") != "ml")
	{
		int confweight = item.ConfigGetInt("weight");
		// old calculation
		
//		float weight = 0;

//		if (quantity > 0 && confweight != 0)
//		{
//			weight = Math.Round( (wetness + 1) * confweight * quantity );
//		}
//		else if (quantity > 0)
//		{
//			weight = Math.Round( (wetness + 1) * quantity );
//		}
//		else
//		{
//			weight=Math.Round( (wetness + 1) * confweight );
//		}

		int weight = item_IB.GetTotalWeight();
		
		if (root_widget.GetName() != "BackPanelWidget")
		{
			weight = item_IB.GetInventoryItemWeight();
		}
		
		if (weight >= 1000)
		{
			int kilos = Math.Round(weight / 1000.0);
			WidgetTrySetText(root_widget, "ItemWeightWidget", "around " + kilos.ToString() + " kg");
		}
		else if (weight >= 500)
		{
			WidgetTrySetText(root_widget, "ItemWeightWidget", "under 1 kg");
		} 
		else if (weight >= 250)
		{
			WidgetTrySetText(root_widget, "ItemWeightWidget", "under 0.5 kg");
		}
		else 
		{
			WidgetTrySetText(root_widget, "ItemWeightWidget", "under 0.25 kg");
		}
	}
	else
	{
		WidgetTrySetText(root_widget, "ItemWeightWidget", "");
	}
}

//--------------------------------------------------------------------------
void UpdateItemInfoQuantity(Widget root_widget, EntityAI item)
{
	if ( item.IsInherited( ZombieBase ) || item.IsInherited( Car ) ) return;
	string quantity_text = "";
	ItemBase item_base = ItemBase.Cast( item );
	float quantity = item_base.GetQuantity();
	int max = item.ConfigGetInt("varQuantityMax");
	string unit = item.ConfigGetString("stackedUnit");
	int confweight = item.ConfigGetInt("weight");
	
	string item_quantity_text = "";
	if ( item.IsInherited( ItemBook) )
	{
		
	}
	else if (max > 0)
	{
		if (max == 1)
		{
			float tmp = Math.Round( ( quantity / max ) * 100 );
			item_quantity_text = tmp.ToString() + "% remaining";
		}
		else
		{
			if (unit == "ml")
			{
				if (quantity >= 1000)
				{
					int liters = Math.Round(quantity / 1000.0);
					item_quantity_text = "around " + liters.ToString() + " l";
				}
				else if (quantity >= 500)
				{
					item_quantity_text = "under 1 l";
				}
				else if (quantity >= 250)
				{
					item_quantity_text = "under 0.5 l";
				}
				else
				{
					item_quantity_text = "under 0.25 l";
				}
			}
			else if (confweight != 0)
			{
				tmp = Math.Round( ( quantity / max ) * 100 );
				item_quantity_text = tmp.ToString() + "% remaining";
			}
			else
			{
				if ( unit != "" ) unit = " " + unit;
				item_quantity_text = quantity.ToString() + unit;
			}
		}
	}
	WidgetTrySetText(root_widget, "ItemQuantityWidget", item_quantity_text);
}

//--------------------------------------------------------------------------
void UpdateItemInfoDebug(Widget root_widget, EntityAI item)
{
	if ( item.IsInherited( ZombieBase ) || item.IsInherited( Car ) ) return;
	WidgetTrySetText(root_widget, "DebugTextWidget", "");
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
*/