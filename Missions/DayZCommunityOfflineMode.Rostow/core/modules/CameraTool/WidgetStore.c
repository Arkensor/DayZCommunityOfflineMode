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

		textWidgets = new ref array< TextWidget >;
		buttonWidgets = new ref array < ButtonWidget >;
		editBoxWidgets = new ref array < EditBoxWidget >;
		sliderWidgets = new ref array < SliderWidget >;
		textListBoxWidgets = new ref array < TextListboxWidget >;
		allWidgets = new ref array < Widget >;

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
			textWidgets.Insert( widget );
		}
		if ( widget.IsInherited( ButtonWidget ) ) 
		{
			buttonWidgets.Insert( widget );
		}
		if ( widget.IsInherited( EditBoxWidget ) ) 
		{
			editBoxWidgets.Insert( widget );
		}
		if ( widget.IsInherited( SliderWidget ) ) 
		{
			sliderWidgets.Insert( widget );
		}
		if ( widget.IsInherited( TextListboxWidget )) 
		{
			textListBoxWidgets.Insert( widget );
		}
	}

	ref Widget GetWidget( string name ) 
	{
		foreach( ref Widget widget : allWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return widget;
			}
		}
		return null;
	}

	ref TextWidget GetTextWidget( string name ) 
	{
		foreach( ref Widget widget : textWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return widget;
			}
		}
		return null;
	}

	ref ButtonWidget GetButtonWidget( string name ) 
	{
		foreach( ref Widget widget : buttonWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return widget;
			}
		}
		return null;
	}

	ref EditBoxWidget GetEditBoxWidget( string name ) 
	{
		foreach( ref Widget widget : editBoxWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return widget;
			}
		}
		return null;
	}

	ref SliderWidget GetSliderWidget( string name ) 
	{
		foreach( ref Widget widget : sliderWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return widget;
			}
		}
		return null;
	}

	ref TextListboxWidget GetListboxWidget( string name ) 
	{
		foreach( ref Widget widget : textListBoxWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return widget;
			}
		}
		return null;
	}
}