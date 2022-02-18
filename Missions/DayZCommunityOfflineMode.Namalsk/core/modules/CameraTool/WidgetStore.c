class WidgetStore 
{
	private Widget layoutRoot;

	private ref array< TextWidget > textWidgets;
	private ref array< ButtonWidget > buttonWidgets;
	private ref array< EditBoxWidget > editBoxWidgets;
	private ref array< SliderWidget > sliderWidgets;
	private ref array< TextListboxWidget > textListBoxWidgets;

	private ref array< Widget > allWidgets;

	void WidgetStore( Widget root ) 
	{
		layoutRoot = root;

		textWidgets = new array< TextWidget >;
		buttonWidgets = new array < ButtonWidget >;
		editBoxWidgets = new array < EditBoxWidget >;
		sliderWidgets = new array < SliderWidget >;
		textListBoxWidgets = new array < TextListboxWidget >;
		allWidgets = new array < Widget >;

		Init();
	} 

	void ~WidgetStore() 
	{
	}

	void Init() 
	{
		SearchChildren( layoutRoot ); //recursion

		Print( "WidgetStore Init - Testing widget loading count: " + allWidgets.Count() );
	}

	void SearchChildren( Widget widget ) 
	{
		Widget child = widget.GetChildren();
		while ( child ) 
		{
			//Print( child.GetName() );
			allWidgets.Insert( child );
			AddWidget( child );
			SearchSibling( child );
			child = child.GetChildren();
		}
	}

	void SearchSibling( Widget widget )
	{
		Widget sibling = widget.GetSibling();
		while ( sibling ) 
		{
			//Print( sibling.GetName() );
			allWidgets.Insert( sibling );
			AddWidget( sibling );
			SearchChildren( sibling );
			sibling = sibling.GetSibling();
		}
	}

	void FindDuplicates() //debugging
	{
		int duplicates = 0;
		foreach( Widget widget : allWidgets ) 
		{
			string name = widget.GetName();

			int count = 0;
			foreach ( Widget check : allWidgets ) 
			{
				if ( name == check.GetName() ) 
				{
					count++;
				}
				if ( count > 1 ) 
				{
					Print( "Found duplicate: " + name );
					duplicates++;
					break;
				}
			}
		}
		Print( "Duplicates: " + duplicates );
	}

	void AddWidget( Widget widget ) 
	{
		if ( widget.IsInherited( TextWidget ) ) 
		{
			textWidgets.Insert( TextWidget.Cast(widget) );
		}
		if ( widget.IsInherited( ButtonWidget ) ) 
		{
			buttonWidgets.Insert( ButtonWidget.Cast(widget) );
		}
		if ( widget.IsInherited( EditBoxWidget ) ) 
		{
			editBoxWidgets.Insert( EditBoxWidget.Cast(widget) );
		}
		if ( widget.IsInherited( SliderWidget ) ) 
		{
			sliderWidgets.Insert( SliderWidget.Cast(widget) );
		}
		if ( widget.IsInherited( TextListboxWidget )) 
		{
			textListBoxWidgets.Insert( TextListboxWidget.Cast(widget) );
		}
	}

	Widget GetWidget( string name ) 
	{
		foreach( Widget widget : allWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return widget;
			}
		}
		return null;
	}

	TextWidget GetTextWidget( string name ) 
	{
		foreach( Widget widget : textWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return TextWidget.Cast(widget);
			}
		}
		return null;
	}

	ButtonWidget GetButtonWidget( string name ) 
	{
		foreach( Widget widget : buttonWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return ButtonWidget.Cast(widget);
			}
		}
		return null;
	}

	EditBoxWidget GetEditBoxWidget( string name ) 
	{
		foreach( Widget widget : editBoxWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return EditBoxWidget.Cast(widget);
			}
		}
		return null;
	}

	SliderWidget GetSliderWidget( string name ) 
	{
		foreach( Widget widget : sliderWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return SliderWidget.Cast(widget);
			}
		}
		return null;
	}

	TextListboxWidget GetListboxWidget( string name ) 
	{
		foreach( Widget widget : textListBoxWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return TextListboxWidget.Cast(widget);
			}
		}
		return null;
	}
}