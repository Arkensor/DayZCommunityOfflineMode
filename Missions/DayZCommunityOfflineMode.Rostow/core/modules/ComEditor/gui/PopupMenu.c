class PopupMenu extends ScriptedWidgetEventHandler 
{
	protected Widget layoutRoot;

	void PopupMenu() 
	{

	}

	void ~PopupMenu() 
	{
	}

	void OnWidgetScriptInit( Widget w )
	{
		layoutRoot = w;
		layoutRoot.SetHandler( this );

		Init();
	}

	void Init() 
	{

	}

	void OnShow()
	{
	}

	void OnHide() 
	{
	}

	void Update() 
	{
		
	}

	Widget GetLayoutRoot() 
	{
		return layoutRoot;
	}

}