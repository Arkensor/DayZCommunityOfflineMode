// -----------------------------------------------------------
class ItemsCounter : ScriptedWidgetEventHandler
{
	bool NumberOfItems;
	
	void OnWidgetScriptInit(Widget w)
	{
    Widget child = w.GetChildren();
  
    while (child)
    {
        child = child.GetSibling();
        NumberOfItems++;
    }
	}
};